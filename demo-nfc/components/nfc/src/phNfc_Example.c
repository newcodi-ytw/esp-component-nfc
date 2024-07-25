#include <stdio.h>
#include "phApp_Init.h"

#include "phNfc_Example.h"

phacDiscLoop_Sw_DataParams_t       * pDiscLoop;       /* Discovery loop component */
/*The below variables needs to be initialized according to example requirements by a customer */
uint8_t  sens_res[2]     = {0x04, 0x00};              /* ATQ bytes - needed for anti-collision */
uint8_t  nfc_id1[3]      = {0xA1, 0xA2, 0xA3};        /* user defined bytes of the UID (one is hardcoded) - needed for anti-collision */
uint8_t  sel_res         = 0x40;
uint8_t  nfc_id3         = 0xFA;                      /* NFC3 byte - required for anti-collision */
uint8_t  poll_res[18]    = {0x01, 0xFE, 0xB2, 0xB3, 0xB4, 0xB5,
                                   0xB6, 0xB7, 0xC0, 0xC1, 0xC2, 0xC3,
                                   0xC4, 0xC5, 0xC6, 0xC7, 0x23, 0x45 };
static uint16_t bSavePollTechCfg;

uint16_t NFCForumProcess(uint16_t wEntryPoint, phStatus_t DiscLoopStatus);

#ifdef PH_OSAL_FREERTOS
const uint8_t bTaskName[configMAX_TASK_NAME_LEN] = {"DiscLoop"};
#endif

#ifdef ENABLE_DISC_CONFIG
static phStatus_t LoadProfile(phacDiscLoop_Profile_t bProfile);
#endif

void phNfc_Example_Init(void)
{
    phDriver_GpioDebugToggle();

    phStatus_t status = PH_ERR_INTERNAL_ERROR;
    phNfcLib_Status_t     dwStatus;
#ifdef PH_PLATFORM_HAS_ICFRONTEND
    phNfcLib_AppContext_t AppContext = {0};
#endif /* PH_PLATFORM_HAS_ICFRONTEND */

    /* Perform Controller specific initialization. */
    phApp_CPU_Init();

    /* Perform OSAL Initialization. */
    (void)phOsal_Init();

    DEBUG_LOG_UI("DiscoveryLoop Example:");
#ifdef PH_PLATFORM_HAS_ICFRONTEND
    status = phbalReg_Init(&sBalParams, sizeof(phbalReg_Type_t));
    CHECK_STATUS(status);

    AppContext.pBalDataparams = &sBalParams;
    dwStatus = phNfcLib_SetContext(&AppContext);
    CHECK_NFCLIB_STATUS(dwStatus);
#endif

    /* Initialize library */
    dwStatus = phNfcLib_Init();
    CHECK_NFCLIB_STATUS(dwStatus);
    if(dwStatus != PH_NFCLIB_STATUS_SUCCESS) return;

    /* Set the generic pointer */
    pHal = phNfcLib_GetDataParams(PH_COMP_HAL);
    pDiscLoop = phNfcLib_GetDataParams(PH_COMP_AC_DISCLOOP);

    DEBUG_LOG_CORE("FirmwareVer: 0x%x\n", pHal->wFirmwareVer);

    /* Initialize other components that are not initialized by NFCLIB and configure Discovery Loop. */
    status = phApp_Comp_Init(pDiscLoop);
    CHECK_STATUS(status);
    if(status != PH_ERR_SUCCESS) return;

    /* Perform Platform Init */
    status = phApp_Configure_IRQ();
    CHECK_STATUS(status);
    if(status != PH_ERR_SUCCESS) return;

#ifndef PH_OSAL_NULLOS

        phOsal_ThreadObj_t DiscLoop;

        DiscLoop.pTaskName = (uint8_t *)bTaskName;
        DiscLoop.pStackBuffer = NULL/* aDiscTaskBuffer */;
        DiscLoop.priority = DISC_DEMO_TASK_PRIO;
        DiscLoop.stackSizeInNum = DISC_DEMO_TASK_STACK;
        phOsal_ThreadCreate(&DiscLoop.ThreadHandle, &DiscLoop, &phNfc_Example_Main, pDiscLoop);

        // phOsal_StartScheduler();
#else
        phNfc_Example_Main(pDiscLoop);
#endif
}

void phNfc_Example_Main(void  *pDataParams)
{
    phStatus_t    status, statustmp;
    uint16_t      wEntryPoint;
    phacDiscLoop_Profile_t bProfile = PHAC_DISCLOOP_PROFILE_UNKNOWN;

#ifdef ENABLE_DISC_CONFIG
#ifndef ENABLE_EMVCO_PROF
    bProfile = PHAC_DISCLOOP_PROFILE_NFC;
#else
    bProfile = PHAC_DISCLOOP_PROFILE_EMVCO;
#endif
    /* Load selected profile for Discovery loop */
    LoadProfile(bProfile);
#endif /* ENABLE_DISC_CONFIG */

#ifdef NXPBUILD__PHHAL_HW_TARGET
    /* Initialize the setting for Listen Mode */
    status = phApp_HALConfigAutoColl();
    CHECK_STATUS(status);
#endif /* NXPBUILD__PHHAL_HW_TARGET */

    /* Get Poll Configuration */
    status = phacDiscLoop_GetConfig(pDataParams, PHAC_DISCLOOP_CONFIG_PAS_POLL_TECH_CFG, &bSavePollTechCfg);
    CHECK_STATUS(status);

    /* Start in poll mode */
    wEntryPoint = PHAC_DISCLOOP_ENTRY_POINT_POLL;
    status = PHAC_DISCLOOP_LPCD_NO_TECH_DETECTED;

    /* Switch off RF field */
    statustmp = phhalHw_FieldOff(pHal);
    CHECK_STATUS(statustmp);

    DEBUG_LOG_UI("start");

    static uint32_t cnt = 0;
    while (1)
    {
        DEBUG_LOG_UI("Detect LOOP: %d", cnt++);
        /* Before polling set Discovery Poll State to Detection , as later in the code it can be changed to e.g. PHAC_DISCLOOP_POLL_STATE_REMOVAL*/
        statustmp = phacDiscLoop_SetConfig(pDataParams, PHAC_DISCLOOP_CONFIG_NEXT_POLL_STATE, PHAC_DISCLOOP_POLL_STATE_DETECTION);
        CHECK_STATUS(statustmp);

    #if !defined(ENABLE_EMVCO_PROF) && defined(PH_EXAMPLE1_LPCD_ENABLE)

    #ifdef NXPBUILD__PHHAL_HW_RC663
        if (wEntryPoint == PHAC_DISCLOOP_ENTRY_POINT_POLL)
    #else
        /* Configure LPCD */
        if ((status & PH_ERR_MASK) == PHAC_DISCLOOP_LPCD_NO_TECH_DETECTED)
    #endif
        {
            status = phApp_ConfigureLPCD();
            CHECK_STATUS(status);
        }

        /* Bool to enable LPCD feature. */
        status = phacDiscLoop_SetConfig(pDataParams, PHAC_DISCLOOP_CONFIG_ENABLE_LPCD, PH_ON);
        CHECK_STATUS(status);
    #endif /* PH_EXAMPLE1_LPCD_ENABLE*/

        /* Start discovery loop */
        DEBUG_LOG_CORE("%d %d - s", wEntryPoint, status);
        status = phacDiscLoop_Run(pDataParams, wEntryPoint);
        DEBUG_LOG_CORE("%d %d - e", wEntryPoint, status);

        if(bProfile == PHAC_DISCLOOP_PROFILE_EMVCO)
        {
    #if defined(ENABLE_EMVCO_PROF)

            EmvcoProfileProcess(pDataParams, status);

    #endif /* ENABLE_EMVCO_PROF */
        }
        else
        {
            DEBUG_LOG_CORE("wEntryPoint:%d DiscLoopStatus:%d s", wEntryPoint, status);
            wEntryPoint = NFCForumProcess(wEntryPoint, status);
            DEBUG_LOG_CORE("wEntryPoint:%d DiscLoopStatus:%d s", wEntryPoint, status);

            /* Set Poll Configuration */
            statustmp = phacDiscLoop_SetConfig(pDataParams, PHAC_DISCLOOP_CONFIG_PAS_POLL_TECH_CFG, bSavePollTechCfg);
            CHECK_STATUS(statustmp);

            /* Switch off RF field */
            statustmp = phhalHw_FieldOff(pHal);
            CHECK_STATUS(statustmp);

            phacDiscLoop_Sw_DataParams_t *temp = (phacDiscLoop_Sw_DataParams_t *)pDataParams;
            PCD_HelpShowByte("UUID:", 
                            temp->sTypeATargetInfo.aTypeA_I3P3[0].aUid,
                            temp->sTypeATargetInfo.aTypeA_I3P3[0].bUidSize);

            /* Wait for field-off time-out */
            statustmp = phhalHw_Wait(pHal, PHHAL_HW_TIME_MICROSECONDS, 1000*30);
            CHECK_STATUS(statustmp);
        }
    }

    DEBUG_LOG_UI("end");

    vTaskDelete(NULL);
}

uint16_t NFCForumProcess(uint16_t wEntryPoint, phStatus_t DiscLoopStatus)
{
    phStatus_t    status;
    uint16_t      wTechDetected = 0;
    uint16_t      wNumberOfTags = 0;
    uint16_t      wValue;
    uint8_t       bIndex;
    uint16_t      wReturnEntryPoint;

    DEBUG_LOG_UI("wEntryPoint: %d DiscLoopStatus: %d", wEntryPoint, DiscLoopStatus);
    if(wEntryPoint == PHAC_DISCLOOP_ENTRY_POINT_POLL)
    {
        if((DiscLoopStatus & PH_ERR_MASK) == PHAC_DISCLOOP_MULTI_TECH_DETECTED)
        {
            DEBUG_LOG_UI (" \n Multiple technology detected: \n");

            status = phacDiscLoop_GetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_TECH_DETECTED, &wTechDetected);
            CHECK_STATUS(status);

            if(PHAC_DISCLOOP_CHECK_ANDMASK(wTechDetected, PHAC_DISCLOOP_POS_BIT_MASK_A))
            {
                DEBUG_LOG_UI (" \tType A detected... \n");
            }
            if(PHAC_DISCLOOP_CHECK_ANDMASK(wTechDetected, PHAC_DISCLOOP_POS_BIT_MASK_B))
            {
                DEBUG_LOG_UI (" \tType B detected... \n");
            }
            if(PHAC_DISCLOOP_CHECK_ANDMASK(wTechDetected, PHAC_DISCLOOP_POS_BIT_MASK_F212))
            {
                DEBUG_LOG_UI (" \tType F detected with baud rate 212... \n");
            }
            if(PHAC_DISCLOOP_CHECK_ANDMASK(wTechDetected, PHAC_DISCLOOP_POS_BIT_MASK_F424))
            {
                DEBUG_LOG_UI (" \tType F detected with baud rate 424... \n");
            }
            if(PHAC_DISCLOOP_CHECK_ANDMASK(wTechDetected, PHAC_DISCLOOP_POS_BIT_MASK_V))
            {
                DEBUG_LOG_UI(" \tType V / ISO 15693 / T5T detected... \n");
            }

            /* Select 1st Detected Technology to Resolve*/
            for(bIndex = 0; bIndex < PHAC_DISCLOOP_PASS_POLL_MAX_TECHS_SUPPORTED; bIndex++)
            {
                if(PHAC_DISCLOOP_CHECK_ANDMASK(wTechDetected, (1 << bIndex)))
                {
                    /* Configure for one of the detected technology */
                    status = phacDiscLoop_SetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_PAS_POLL_TECH_CFG, (1 << bIndex));
                    CHECK_STATUS(status);
                    break;
                }
            }

            /* Print the technology resolved */
            phApp_PrintTech((1 << bIndex));

            /* Set Discovery Poll State to collision resolution */
            status = phacDiscLoop_SetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_NEXT_POLL_STATE, PHAC_DISCLOOP_POLL_STATE_COLLISION_RESOLUTION);
            CHECK_STATUS(status);

            /* Restart discovery loop in poll mode from collision resolution phase */
            DiscLoopStatus = phacDiscLoop_Run(pDiscLoop, wEntryPoint);
        }

        if((DiscLoopStatus & PH_ERR_MASK) == PHAC_DISCLOOP_MULTI_DEVICES_RESOLVED)
        {
            DEBUG_LOG_UI();
            /* Get Detected Technology Type */
            status = phacDiscLoop_GetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_TECH_DETECTED, &wTechDetected);
            CHECK_STATUS(status);

            /* Get number of tags detected */
            status = phacDiscLoop_GetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_NR_TAGS_FOUND, &wNumberOfTags);
            CHECK_STATUS(status);

            DEBUG_LOG_UI (" \n Multiple cards resolved: %d cards \n",wNumberOfTags);
            phApp_PrintTagInfo(pDiscLoop, wNumberOfTags, wTechDetected);

            if(wNumberOfTags > 1)
            {
                /* Get 1st Detected Technology and Activate device at index 0 */
                for(bIndex = 0; bIndex < PHAC_DISCLOOP_PASS_POLL_MAX_TECHS_SUPPORTED; bIndex++)
                {
                    if(PHAC_DISCLOOP_CHECK_ANDMASK(wTechDetected, (1 << bIndex)))
                    {
                        DEBUG_LOG_UI("\t Activating one card...\n");
                        status = phacDiscLoop_ActivateCard(pDiscLoop, bIndex, 0);
                        break;
                    }
                }

                if(((status & PH_ERR_MASK) == PHAC_DISCLOOP_DEVICE_ACTIVATED) ||
                        ((status & PH_ERR_MASK) == PHAC_DISCLOOP_PASSIVE_TARGET_ACTIVATED) ||
                        ((status & PH_ERR_MASK) == PHAC_DISCLOOP_MERGED_SEL_RES_FOUND))
                {
                    /* Get Detected Technology Type */
                    status = phacDiscLoop_GetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_TECH_DETECTED, &wTechDetected);
                    CHECK_STATUS(status);

                    phApp_PrintTagInfo(pDiscLoop, 0x01, wTechDetected);
                }
                else
                {
                    DEBUG_LOG_UI("\t\tCard activation failed...\n");
                }
            }
            /* Switch to LISTEN mode after POLL mode */
        }
        else if (((DiscLoopStatus & PH_ERR_MASK) == PHAC_DISCLOOP_NO_TECH_DETECTED) ||
                ((DiscLoopStatus & PH_ERR_MASK) == PHAC_DISCLOOP_NO_DEVICE_RESOLVED))
        {
            DEBUG_LOG_UI("Card detected, but no technologies found");
            /* Switch to LISTEN mode after POLL mode */
        }
        else if((DiscLoopStatus & PH_ERR_MASK) == PHAC_DISCLOOP_EXTERNAL_RFON)
        {
            DEBUG_LOG_UI("can restart the loop in LISTEN mode");
            /*
             * If external RF is detected during POLL, return back so that the application
             * can restart the loop in LISTEN mode
             */
        }
        else if((DiscLoopStatus & PH_ERR_MASK) == PHAC_DISCLOOP_MERGED_SEL_RES_FOUND)
        {
            DEBUG_LOG_UI (" \n Device having T4T and NFC-DEP support detected... \n");

            /* Get Detected Technology Type */
            status = phacDiscLoop_GetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_TECH_DETECTED, &wTechDetected);
            CHECK_STATUS(status);

            phApp_PrintTagInfo(pDiscLoop, 1, wTechDetected);

        /* Switch to LISTEN mode after POLL mode */
        }
        else if((DiscLoopStatus & PH_ERR_MASK) == PHAC_DISCLOOP_DEVICE_ACTIVATED)
        {
            DEBUG_LOG_UI (" \n Card detected and activated successfully... \n");
            status = phacDiscLoop_GetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_NR_TAGS_FOUND, &wNumberOfTags);
            CHECK_STATUS(status);

            /* Get Detected Technology Type */
            status = phacDiscLoop_GetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_TECH_DETECTED, &wTechDetected);
            CHECK_STATUS(status);

            phApp_PrintTagInfo(pDiscLoop, wNumberOfTags, wTechDetected);

            /* Switch to LISTEN mode after POLL mode */
        }
        else if((DiscLoopStatus & PH_ERR_MASK) == PHAC_DISCLOOP_ACTIVE_TARGET_ACTIVATED)
        {
            DEBUG_LOG_UI (" \n Active target detected... \n");

            /* Switch to LISTEN mode after POLL mode */
        }
        else if((DiscLoopStatus & PH_ERR_MASK) == PHAC_DISCLOOP_PASSIVE_TARGET_ACTIVATED)
        {
            DEBUG_LOG_UI (" \n Passive target detected... \n");

            /* Get Detected Technology Type */
            status = phacDiscLoop_GetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_TECH_DETECTED, &wTechDetected);
            CHECK_STATUS(status);

            phApp_PrintTagInfo(pDiscLoop, 1, wTechDetected);

            /* Switch to LISTEN mode after POLL mode */
        }
        else if ((DiscLoopStatus & PH_ERR_MASK) == PHAC_DISCLOOP_LPCD_NO_TECH_DETECTED)
        {
            DEBUG_LOG_UI();
            /* LPCD is succeed but no tag is detected. */
        }
        else
        {
            if((DiscLoopStatus & PH_ERR_MASK) == PHAC_DISCLOOP_FAILURE)
            {
                status = phacDiscLoop_GetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_ADDITIONAL_INFO, &wValue);
                CHECK_STATUS(status);
                DEBUG_LOG_UI("wValue: %d", wValue);
                DEBUG_ERROR_PRINT(PrintErrorInfo(wValue));
            }
            else
            {
                DEBUG_ERROR_PRINT(PrintErrorInfo(status));
            }
        }

        /* Update the Entry point to LISTEN mode. */
        wReturnEntryPoint = PHAC_DISCLOOP_ENTRY_POINT_LISTEN;
        DEBUG_LOG_UI("pdate the Entry point to LISTEN mode");
    }
    else
    {
        if((DiscLoopStatus & PH_ERR_MASK) == PHAC_DISCLOOP_EXTERNAL_RFOFF)
        {
            DEBUG_LOG_UI(">>PHAC_DISCLOOP_EXTERNAL_RFOFF<<");
            /*
             * Enters here if in the target/card mode and external RF is not available
             * Wait for LISTEN timeout till an external RF is detected.
             * Application may choose to go into standby at this point.
             */
            status = phhalHw_EventConsume(pHal);
            CHECK_STATUS(status);

            status = phhalHw_SetConfig(pHal, PHHAL_HW_CONFIG_RFON_INTERRUPT, PH_ON);
            CHECK_STATUS(status);

            DEBUG_LOG_UI("LISTEN_PHASE_TIME_MS: %d", LISTEN_PHASE_TIME_MS);
            status = phhalHw_EventWait(pHal, LISTEN_PHASE_TIME_MS);
            DEBUG_LOG_UI("phhalHw_EventWait: %d", status);
            if((status & PH_ERR_MASK) == PH_ERR_IO_TIMEOUT)
            {
                wReturnEntryPoint = PHAC_DISCLOOP_ENTRY_POINT_POLL;
            }
            else
            {
                wReturnEntryPoint = PHAC_DISCLOOP_ENTRY_POINT_LISTEN;
            }
        }
        else
        {
            if((DiscLoopStatus & PH_ERR_MASK) == PHAC_DISCLOOP_ACTIVATED_BY_PEER)
            {
                DEBUG_LOG_UI (" \n Device activated in listen mode... \n");
            }
            else if ((DiscLoopStatus & PH_ERR_MASK) == PH_ERR_INVALID_PARAMETER)
            {
                /* In case of Front end used is RC663, then listen mode is not supported.
                 * Switch from listen mode to poll mode. */
            }
            else
            {
                if((DiscLoopStatus & PH_ERR_MASK) == PHAC_DISCLOOP_FAILURE)
                {
                    status = phacDiscLoop_GetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_ADDITIONAL_INFO, &wValue);
                    CHECK_STATUS(status);
                    DEBUG_ERROR_PRINT(PrintErrorInfo(wValue));
                }
                else
                {
                    DEBUG_ERROR_PRINT(PrintErrorInfo(status));
                }
            }

            /* On successful activated by Peer, switch to LISTEN mode */
            wReturnEntryPoint = PHAC_DISCLOOP_ENTRY_POINT_POLL;
        }
    }

    return wReturnEntryPoint;
}

#ifdef ENABLE_DISC_CONFIG
/**
* This function will load/configure Discovery loop with default values based on interested profile
 * Application can read these values from EEPROM area and load/configure Discovery loop via SetConfig
* \param   bProfile      Reader Library Profile
* \note    Values used below are default and is for demonstration purpose.
*/
static phStatus_t LoadProfile(phacDiscLoop_Profile_t bProfile)
{
    phStatus_t status = PH_ERR_SUCCESS;
    uint16_t   wPasPollConfig = 0;
    uint16_t   wActPollConfig = 0;
    uint16_t   wPasLisConfig = 0;
    uint16_t   wActLisConfig = 0;

#ifdef NXPBUILD__PHAC_DISCLOOP_TYPEA_TAGS
    wPasPollConfig |= PHAC_DISCLOOP_POS_BIT_MASK_A;
#endif
#ifdef NXPBUILD__PHAC_DISCLOOP_TYPEB_TAGS
    wPasPollConfig |= PHAC_DISCLOOP_POS_BIT_MASK_B;
#endif
#ifdef NXPBUILD__PHAC_DISCLOOP_TYPEF_TAGS
    wPasPollConfig |= (PHAC_DISCLOOP_POS_BIT_MASK_F212 | PHAC_DISCLOOP_POS_BIT_MASK_F424);
#endif
#ifdef NXPBUILD__PHAC_DISCLOOP_TYPEV_TAGS
    wPasPollConfig |= PHAC_DISCLOOP_POS_BIT_MASK_V;
#endif
#ifdef NXPBUILD__PHAC_DISCLOOP_I18000P3M3_TAGS
    wPasPollConfig |= PHAC_DISCLOOP_POS_BIT_MASK_18000P3M3;
#endif

#ifdef NXPBUILD__PHAC_DISCLOOP_TYPEA_P2P_ACTIVE
    wActPollConfig |= PHAC_DISCLOOP_ACT_POS_BIT_MASK_106;
#endif
#ifdef NXPBUILD__PHAC_DISCLOOP_TYPEF212_P2P_ACTIVE
    wActPollConfig |= PHAC_DISCLOOP_ACT_POS_BIT_MASK_212;
#endif
#ifdef NXPBUILD__PHAC_DISCLOOP_TYPEF424_P2P_ACTIVE
    wActPollConfig |= PHAC_DISCLOOP_ACT_POS_BIT_MASK_424;
#endif

#ifdef NXPBUILD__PHAC_DISCLOOP_TYPEA_TARGET_PASSIVE
    wPasLisConfig |= PHAC_DISCLOOP_POS_BIT_MASK_A;
#endif
#ifdef NXPBUILD__PHAC_DISCLOOP_TYPEF212_TARGET_PASSIVE
    wPasLisConfig |= PHAC_DISCLOOP_POS_BIT_MASK_F212;
#endif
#ifdef NXPBUILD__PHAC_DISCLOOP_TYPEF424_TARGET_PASSIVE
    wPasLisConfig |= PHAC_DISCLOOP_POS_BIT_MASK_F424;
#endif

#ifdef NXPBUILD__PHAC_DISCLOOP_TYPEA_TARGET_ACTIVE
    wActLisConfig |= PHAC_DISCLOOP_POS_BIT_MASK_A;
#endif
#ifdef NXPBUILD__PHAC_DISCLOOP_TYPEF212_TARGET_ACTIVE
    wActLisConfig |= PHAC_DISCLOOP_POS_BIT_MASK_F212;
#endif
#ifdef NXPBUILD__PHAC_DISCLOOP_TYPEF424_TARGET_ACTIVE
    wActLisConfig |= PHAC_DISCLOOP_POS_BIT_MASK_F424;
#endif

    if(bProfile == PHAC_DISCLOOP_PROFILE_NFC)
    {
        /* passive Bailout bitmap config. */
        status = phacDiscLoop_SetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_BAIL_OUT, 0x00);
        CHECK_STATUS(status);

        /* Set Passive poll bitmap config. */
        status = phacDiscLoop_SetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_PAS_POLL_TECH_CFG, wPasPollConfig);
        CHECK_STATUS(status);

        /* Set Active poll bitmap config. */
        status = phacDiscLoop_SetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_ACT_POLL_TECH_CFG, wActPollConfig);
        CHECK_STATUS(status);

        /* Set Passive listen bitmap config. */
        status = phacDiscLoop_SetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_PAS_LIS_TECH_CFG, wPasLisConfig);
        CHECK_STATUS(status);

        /* Set Active listen bitmap config. */
        status = phacDiscLoop_SetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_ACT_LIS_TECH_CFG, wActLisConfig);
        CHECK_STATUS(status);

        /* reset collision Pending */
        status = phacDiscLoop_SetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_COLLISION_PENDING, PH_OFF);
        CHECK_STATUS(status);

        /* whether anti-collision is supported or not. */
        status = phacDiscLoop_SetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_ANTI_COLL, PH_ON);
        CHECK_STATUS(status);

        /* Poll Mode default state*/
        status = phacDiscLoop_SetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_NEXT_POLL_STATE, PHAC_DISCLOOP_POLL_STATE_DETECTION);
        CHECK_STATUS(status);

#ifdef  NXPBUILD__PHAC_DISCLOOP_TYPEA_TAGS
        /* Device limit for Type A */
        status = phacDiscLoop_SetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_TYPEA_DEVICE_LIMIT, 1);
        CHECK_STATUS(status);

        /* Passive polling Tx Guard times in micro seconds. */
        status = phacDiscLoop_SetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_GTA_VALUE_US, 5100);
        CHECK_STATUS(status);
#endif

#ifdef NXPBUILD__PHAC_DISCLOOP_TYPEB_TAGS
        /* Device limit for Type B */
        status = phacDiscLoop_SetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_TYPEB_DEVICE_LIMIT, 1);
        CHECK_STATUS(status);

        status = phacDiscLoop_SetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_GTB_VALUE_US, 5100);
        CHECK_STATUS(status);
#endif

#ifdef NXPBUILD__PHAC_DISCLOOP_TYPEF_TAGS
        /* Device limit for Type F */
        status = phacDiscLoop_SetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_TYPEF_DEVICE_LIMIT, 1);
        CHECK_STATUS(status);

        /* Guard time for Type F. This guard time is applied when Type F poll before Type B */
        status = phacDiscLoop_SetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_GTFB_VALUE_US, 20400);
        CHECK_STATUS(status);

        /* Guard time for Type F. This guard time is applied when Type B poll before Type F */
        status = phacDiscLoop_SetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_GTBF_VALUE_US, 15300);
        CHECK_STATUS(status);
#endif

#ifdef NXPBUILD__PHAC_DISCLOOP_TYPEV_TAGS
        /* Device limit for Type V (ISO 15693) */
        status = phacDiscLoop_SetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_TYPEV_DEVICE_LIMIT, 1);
        CHECK_STATUS(status);

        status = phacDiscLoop_SetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_GTV_VALUE_US, 5200);
        CHECK_STATUS(status);
#endif

#ifdef NXPBUILD__PHAC_DISCLOOP_I18000P3M3_TAGS
        /* Device limit for 18000P3M3 */
        status = phacDiscLoop_SetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_18000P3M3_DEVICE_LIMIT, 1);
        CHECK_STATUS(status);

        status = phacDiscLoop_SetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_GT18000P3M3_VALUE_US, 10000);
        CHECK_STATUS(status);
#endif

        /* Discovery loop Operation mode */
        status = phacDiscLoop_SetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_OPE_MODE, RD_LIB_MODE_NFC);
        CHECK_STATUS(status);
    }
    else if(bProfile == PHAC_DISCLOOP_PROFILE_EMVCO)
    {
        /* EMVCO */
        /* passive Bailout bitmap config. */
        status = phacDiscLoop_SetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_BAIL_OUT, 0x00);
        CHECK_STATUS(status);

        /* passive poll bitmap config. */
        status = phacDiscLoop_SetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_PAS_POLL_TECH_CFG, (PHAC_DISCLOOP_POS_BIT_MASK_A | PHAC_DISCLOOP_POS_BIT_MASK_B));
        CHECK_STATUS(status);

        /* Active Listen bitmap config. */
        status = phacDiscLoop_SetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_PAS_LIS_TECH_CFG, 0x00);
        CHECK_STATUS(status);

        /* Active Listen bitmap config. */
        status = phacDiscLoop_SetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_ACT_LIS_TECH_CFG, 0x00);
        CHECK_STATUS(status);

        /* Active Poll bitmap config. */
        status = phacDiscLoop_SetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_ACT_POLL_TECH_CFG, 0x00);
        CHECK_STATUS(status);

        /* Bool to enable LPCD feature. */
        status = phacDiscLoop_SetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_ENABLE_LPCD, PH_OFF);
        CHECK_STATUS(status);

        /* reset collision Pending */
        status = phacDiscLoop_SetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_COLLISION_PENDING, PH_OFF);
        CHECK_STATUS(status);

        /* whether anti-collision is supported or not. */
        status = phacDiscLoop_SetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_ANTI_COLL, PH_ON);
        CHECK_STATUS(status);

        /* Poll Mode default state*/
        status = phacDiscLoop_SetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_NEXT_POLL_STATE, PHAC_DISCLOOP_POLL_STATE_DETECTION);
        CHECK_STATUS(status);

#ifdef NXPBUILD__PHAC_DISCLOOP_TYPEA_TAGS
        /* Device limit for Type A */
        status = phacDiscLoop_SetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_TYPEA_DEVICE_LIMIT, 1);
        CHECK_STATUS(status);

        /* Passive polling Tx Guard times in micro seconds. */
        status = phacDiscLoop_SetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_GTA_VALUE_US, 5100);
        CHECK_STATUS(status);

        /* Configure FSDI for the 14443P4A tags */
        status = phacDiscLoop_SetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_TYPEA_I3P4_FSDI, 0x08);
        CHECK_STATUS(status);

        /* Configure CID for the 14443P4A tags */
        status = phacDiscLoop_SetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_TYPEA_I3P4_CID, 0x00);
        CHECK_STATUS(status);

        /* Configure DRI for the 14443P4A tags */
        status = phacDiscLoop_SetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_TYPEA_I3P4_DRI, 0x00);
        CHECK_STATUS(status);

        /* Configure DSI for the 14443P4A tags */
        status = phacDiscLoop_SetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_TYPEA_I3P4_DSI, 0x00);
        CHECK_STATUS(status);
#endif

#ifdef NXPBUILD__PHAC_DISCLOOP_TYPEB_TAGS
        /* Device limit for Type B */
        status = phacDiscLoop_SetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_TYPEB_DEVICE_LIMIT, 1);
        CHECK_STATUS(status);

        status = phacDiscLoop_SetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_GTB_VALUE_US, 5100);
        CHECK_STATUS(status);

        /* Configure AFI for the type B tags */
        status = phacDiscLoop_SetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_TYPEB_AFI_REQ, 0x00);
        CHECK_STATUS(status);

        /* Configure FSDI for the type B tags */
        status = phacDiscLoop_SetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_TYPEB_FSDI, 0x08);
        CHECK_STATUS(status);

        /* Configure CID for the type B tags */
        status = phacDiscLoop_SetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_TYPEB_CID, 0x00);
        CHECK_STATUS(status);

        /* Configure DRI for the type B tags */
        status = phacDiscLoop_SetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_TYPEB_DRI, 0x00);
        CHECK_STATUS(status);

        /* Configure DSI for the type B tags */
        status = phacDiscLoop_SetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_TYPEB_DSI, 0x00);
        CHECK_STATUS(status);

        /* Configure Extended ATQB support for the type B tags */
        status = phacDiscLoop_SetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_TYPEB_EXTATQB, 0x00);
        CHECK_STATUS(status);
#endif
        /* Configure reader library mode */
        status = phacDiscLoop_SetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_OPE_MODE, RD_LIB_MODE_EMVCO);
        CHECK_STATUS(status);
    }
    else
    {
        /* Do Nothing */
    }
    return status;
}
#endif /* ENABLE_DISC_CONFIG */
