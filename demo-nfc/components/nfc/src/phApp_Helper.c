/*----------------------------------------------------------------------------*/
/* Copyright 2020 NXP                                                         */
/*                                                                            */
/* NXP Confidential. This software is owned or controlled by NXP and may only */
/* be used strictly in accordance with the applicable license terms.          */
/* By expressly accepting such terms or by downloading, installing,           */
/* activating and/or otherwise using the software, you are agreeing that you  */
/* have read, and that you agree to comply with and are bound by, such        */
/* license terms. If you do not agree to be bound by the applicable license   */
/* terms, then you may not retain, install, activate or otherwise use the     */
/* software.                                                                  */
/*----------------------------------------------------------------------------*/

/** \file
* Example Source abstracting component data structure and code initialization and code specific to HW used in the examples
* This file shall be present in all examples. A customer does not need to touch/modify this file. This file
* purely depends on the phNxpBuild_Lpc.h or phNxpBuild_App.h
* The phAppInit.h externs the component data structures initialized here that is in turn included by the core examples.
* The core example shall not use any other variable defined here except the RdLib component data structures(as explained above)
* The RdLib component initialization requires some user defined data and function pointers.
* These are defined in the respective examples and externed here.
*
* Keystore and Crypto initialization needs to be handled by application.
*
* $Author$
* $Revision$ (v07.10.00)
* $Date$
*
*/

#include "phApp_Init.h"

#include <phOsal.h>

/*******************************************************************************
**   Function Declarations
*******************************************************************************/

/*******************************************************************************
**   Function Definitions
*******************************************************************************/
/* Print technology being resolved */
void phApp_PrintTech(uint8_t TechType)
{
    switch(TechType)
    {
    case PHAC_DISCLOOP_POS_BIT_MASK_A:
        MY_DEBUG_PRINT ("\tResolving Type A... \n");
        break;

    case PHAC_DISCLOOP_POS_BIT_MASK_B:
        MY_DEBUG_PRINT ("\tResolving Type B... \n");
        break;

    case PHAC_DISCLOOP_POS_BIT_MASK_F212:
        MY_DEBUG_PRINT ("\tResolving Type F with baud rate 212... \n");
        break;

    case PHAC_DISCLOOP_POS_BIT_MASK_F424:
        MY_DEBUG_PRINT ("\tResolving Type F with baud rate 424... \n");
        break;

    case PHAC_DISCLOOP_POS_BIT_MASK_V:
        MY_DEBUG_PRINT ("\tResolving Type V... \n");
        break;

    default:
        break;
    }
}

/**
* This function will print buffer content
* \param   *pBuff   Buffer Reference
* \param   num      data size to be print
*/
void phApp_Print_Buff(uint8_t *pBuff, uint8_t num)
{
    uint32_t    i;

    for(i = 0; i < num; i++)
    {
        MY_DEBUG_PRINT(" %02X",pBuff[i]);
    }
}

/**
* This function will print Tag information
* \param   pDataParams      The discovery loop data parameters
* \param   wNumberOfTags    Total number of tags detected
* \param   wTagsDetected    Technology Detected
*/
void phApp_PrintTagInfo(phacDiscLoop_Sw_DataParams_t *pDataParams, uint16_t wNumberOfTags, uint16_t wTagsDetected)
{
#if defined(NXPBUILD__PHAC_DISCLOOP_TYPEA_TAGS) || \
    defined(NXPBUILD__PHAC_DISCLOOP_TYPEA_P2P_ACTIVE) || \
    defined(NXPBUILD__PHAC_DISCLOOP_TYPEB_TAGS) || \
    defined(NXPBUILD__PHAC_DISCLOOP_TYPEF_TAGS) || \
    defined(NXPBUILD__PHAC_DISCLOOP_TYPEV_TAGS) || \
    defined(NXPBUILD__PHAC_DISCLOOP_I18000P3M3_TAGS)
    uint8_t bIndex;
#endif
#if defined(NXPBUILD__PHAC_DISCLOOP_TYPEA_TAGS) || defined(NXPBUILD__PHAC_DISCLOOP_TYPEA_P2P_ACTIVE)
    uint8_t bTagType;
#endif

#if defined(NXPBUILD__PHAC_DISCLOOP_TYPEA_TAGS) || defined(NXPBUILD__PHAC_DISCLOOP_TYPEA_P2P_ACTIVE)
    if (PHAC_DISCLOOP_CHECK_ANDMASK(wTagsDetected, PHAC_DISCLOOP_POS_BIT_MASK_A))
    {
        if(pDataParams->sTypeATargetInfo.bT1TFlag)
        {
            MY_DEBUG_PRINT("\tTechnology  : Type A");
            MY_DEBUG_PRINT ("\n\t\tUID :");
            phApp_Print_Buff( pDataParams->sTypeATargetInfo.aTypeA_I3P3[0].aUid,
                        pDataParams->sTypeATargetInfo.aTypeA_I3P3[0].bUidSize);
            MY_DEBUG_PRINT ("\n\t\tSAK : 0x%02x",pDataParams->sTypeATargetInfo.aTypeA_I3P3[0].aSak);
            MY_DEBUG_PRINT ("\n\t\tType: Type 1 Tag\n");
        }
        else
        {
            MY_DEBUG_PRINT("\tTechnology  : Type A");
            for(bIndex = 0; bIndex < wNumberOfTags; bIndex++)
            {
                MY_DEBUG_PRINT ("\n\t\tCard: %d",bIndex + 1);
                MY_DEBUG_PRINT ("\n\t\tUID :");
                phApp_Print_Buff( pDataParams->sTypeATargetInfo.aTypeA_I3P3[bIndex].aUid,
                            pDataParams->sTypeATargetInfo.aTypeA_I3P3[bIndex].bUidSize);
                MY_DEBUG_PRINT ("\n\t\tSAK : 0x%02x",pDataParams->sTypeATargetInfo.aTypeA_I3P3[bIndex].aSak);

                if ((pDataParams->sTypeATargetInfo.aTypeA_I3P3[bIndex].aSak & (uint8_t) ~0xFB) == 0)
                {
                    /* Bit b3 is set to zero, [Digital] 4.8.2 */
                    /* Mask out all other bits except for b7 and b6 */
                    bTagType = (pDataParams->sTypeATargetInfo.aTypeA_I3P3[bIndex].aSak & 0x60);
                    bTagType = bTagType >> 5;

                    switch(bTagType)
                    {
                    case PHAC_DISCLOOP_TYPEA_TYPE2_TAG_CONFIG_MASK:
                        MY_DEBUG_PRINT ("\n\t\tType: Type 2 Tag\n");
                        break;
                    case PHAC_DISCLOOP_TYPEA_TYPE4A_TAG_CONFIG_MASK:
                        MY_DEBUG_PRINT ("\n\t\tType: Type 4A Tag\n");
                        break;
                    case PHAC_DISCLOOP_TYPEA_TYPE_NFC_DEP_TAG_CONFIG_MASK:
                        MY_DEBUG_PRINT ("\n\t\tType: P2P\n");
                        break;
                    case PHAC_DISCLOOP_TYPEA_TYPE_NFC_DEP_TYPE4A_TAG_CONFIG_MASK:
                        MY_DEBUG_PRINT ("\n\t\tType: Type NFC_DEP and  4A Tag\n");
                        break;
                    default:
                        break;
                    }
                }
            }
        }
    }
#endif

#ifdef NXPBUILD__PHAC_DISCLOOP_TYPEB_TAGS
    if (PHAC_DISCLOOP_CHECK_ANDMASK(wTagsDetected, PHAC_DISCLOOP_POS_BIT_MASK_B))
    {
        MY_DEBUG_PRINT("\tTechnology  : Type B");
        /* Loop through all the Type B tags detected and print the Pupi */
        for (bIndex = 0; bIndex < wNumberOfTags; bIndex++)
        {
            MY_DEBUG_PRINT ("\n\t\tCard: %d",bIndex + 1);
            MY_DEBUG_PRINT ("\n\t\tUID :");
            /* PUPI Length is always 4 bytes */
            phApp_Print_Buff( pDataParams->sTypeBTargetInfo.aTypeB_I3P3[bIndex].aPupi, 0x04);
        }
        MY_DEBUG_PRINT("\n");
    }
#endif /* NXPBUILD__PHAC_DISCLOOP_TYPEB_TAGS */

#ifdef NXPBUILD__PHAC_DISCLOOP_TYPEF_TAGS
    if( PHAC_DISCLOOP_CHECK_ANDMASK(wTagsDetected, PHAC_DISCLOOP_POS_BIT_MASK_F212) ||
        PHAC_DISCLOOP_CHECK_ANDMASK(wTagsDetected, PHAC_DISCLOOP_POS_BIT_MASK_F424))
    {
        MY_DEBUG_PRINT("\tTechnology  : Type F");

        /* Loop through all the type F tags and print the IDm */
        for (bIndex = 0; bIndex < wNumberOfTags; bIndex++)
        {
            MY_DEBUG_PRINT ("\n\t\tCard: %d",bIndex + 1);
            MY_DEBUG_PRINT ("\n\t\tUID :");
            phApp_Print_Buff( pDataParams->sTypeFTargetInfo.aTypeFTag[bIndex].aIDmPMm,
                        PHAC_DISCLOOP_FELICA_IDM_LENGTH );
            if ((pDataParams->sTypeFTargetInfo.aTypeFTag[bIndex].aIDmPMm[0] == 0x01) &&
                (pDataParams->sTypeFTargetInfo.aTypeFTag[bIndex].aIDmPMm[1] == 0xFE))
            {
                /* This is Type F tag with P2P capabilities */
                MY_DEBUG_PRINT ("\n\t\tType: P2P");
            }
            else
            {
                /* This is Type F T3T tag */
                MY_DEBUG_PRINT ("\n\t\tType: Type 3 Tag");
            }

            if(pDataParams->sTypeFTargetInfo.aTypeFTag[bIndex].bBaud != PHAC_DISCLOOP_CON_BITR_212)
            {
                MY_DEBUG_PRINT ("\n\t\tBit Rate: 424\n");
            }
            else
            {
                MY_DEBUG_PRINT ("\n\t\tBit Rate: 212\n");
            }
        }
    }
#endif /* NXPBUILD__PHAC_DISCLOOP_TYPEF_TAGS */

#ifdef NXPBUILD__PHAC_DISCLOOP_TYPEV_TAGS
    if (PHAC_DISCLOOP_CHECK_ANDMASK(wTagsDetected, PHAC_DISCLOOP_POS_BIT_MASK_V))
    {
        MY_DEBUG_PRINT("\tTechnology  : Type V / ISO 15693 / T5T");
        /* Loop through all the Type V tags detected and print the UIDs */
        for (bIndex = 0; bIndex < wNumberOfTags; bIndex++)
        {
            MY_DEBUG_PRINT ("\n\t\tCard: %d",bIndex + 1);
            MY_DEBUG_PRINT ("\n\t\tUID :");
            phApp_Print_Buff( pDataParams->sTypeVTargetInfo.aTypeV[bIndex].aUid, 0x08);
        }
        MY_DEBUG_PRINT("\n");
    }
#endif /* NXPBUILD__PHAC_DISCLOOP_TYPEV_TAGS */

#ifdef NXPBUILD__PHAC_DISCLOOP_I18000P3M3_TAGS
    if (PHAC_DISCLOOP_CHECK_ANDMASK(wTagsDetected, PHAC_DISCLOOP_POS_BIT_MASK_18000P3M3))
    {
        MY_DEBUG_PRINT("\tTechnology  : ISO 18000p3m3 / EPC Gen2");
        /* Loop through all the 18000p3m3 tags detected and print the UII */
        for (bIndex = 0; bIndex < wNumberOfTags; bIndex++)
        {
            MY_DEBUG_PRINT("\n\t\tCard: %d",bIndex + 1);
            MY_DEBUG_PRINT("\n\t\tUII :");
            phApp_Print_Buff(
                pDataParams->sI18000p3m3TargetInfo.aI18000p3m3[bIndex].aUii,
                (pDataParams->sI18000p3m3TargetInfo.aI18000p3m3[bIndex].wUiiLength / 8));
        }
        MY_DEBUG_PRINT("\n");
    }
#endif /* NXPBUILD__PHAC_DISCLOOP_I18000P3M3_TAGS */
}

/**
* This function will print Error information received from Reader Lib
* \param   wStatus      Error status
*/
void PrintErrorInfo(phStatus_t wStatus)
{
    MY_DEBUG_PRINT("\n ErrorInfo Comp:");

    switch(wStatus & 0xFF00)
    {
    case PH_COMP_BAL:
        MY_DEBUG_PRINT("\t PH_COMP_BAL");
        break;
    case PH_COMP_HAL:
        MY_DEBUG_PRINT("\t PH_COMP_HAL");
        break;
    case PH_COMP_PAL_ISO14443P3A:
        MY_DEBUG_PRINT("\t PH_COMP_PAL_ISO14443P3A");
        break;
    case PH_COMP_PAL_ISO14443P3B:
        MY_DEBUG_PRINT("\t PH_COMP_PAL_ISO14443P3B");
        break;
    case PH_COMP_PAL_ISO14443P4A:
        MY_DEBUG_PRINT("\t PH_COMP_PAL_ISO14443P4A");
        break;
    case PH_COMP_PAL_ISO14443P4:
        MY_DEBUG_PRINT("\t PH_COMP_PAL_ISO14443P4");
        break;
    case PH_COMP_PAL_FELICA:
        MY_DEBUG_PRINT("\t PH_COMP_PAL_FELICA");
        break;
    case PH_COMP_PAL_EPCUID:
        MY_DEBUG_PRINT("\t PH_COMP_PAL_EPCUID");
        break;
    case PH_COMP_PAL_SLI15693:
        MY_DEBUG_PRINT("\t PH_COMP_PAL_SLI15693");
        break;
    case PH_COMP_PAL_I18000P3M3:
        MY_DEBUG_PRINT("\t PH_COMP_PAL_I18000P3M3");
        break;
    case PH_COMP_PAL_I18092MPI:
        MY_DEBUG_PRINT("\t PH_COMP_PAL_I18092MPI");
        break;
    case PH_COMP_PAL_I18092MT:
        MY_DEBUG_PRINT("\t PH_COMP_PAL_I18092MT");
        break;
    case PH_COMP_PAL_I14443P4MC:
        MY_DEBUG_PRINT("\t PH_COMP_PAL_I14443P4MC");
        break;
    case PH_COMP_AC_DISCLOOP:
        MY_DEBUG_PRINT("\t PH_COMP_AC_DISCLOOP");
        break;
    case PH_COMP_OSAL:
        MY_DEBUG_PRINT("\t PH_COMP_OSAL");
        break;
    default:
        MY_DEBUG_PRINT("\t 0x%x",(wStatus & PH_COMPID_MASK));
        break;
    }

    MY_DEBUG_PRINT("\t type:");

    switch(wStatus & PH_ERR_MASK)
    {
    case PH_ERR_SUCCESS_INCOMPLETE_BYTE:
        MY_DEBUG_PRINT("\t PH_ERR_SUCCESS_INCOMPLETE_BYTE");
        break;
    case PH_ERR_IO_TIMEOUT:
        MY_DEBUG_PRINT("\t PH_ERR_IO_TIMEOUT");
        break;
    case PH_ERR_INTEGRITY_ERROR:
        MY_DEBUG_PRINT("\t PH_ERR_INTEGRITY_ERROR");
        break;
    case PH_ERR_COLLISION_ERROR:
        MY_DEBUG_PRINT("\t PH_ERR_COLLISION_ERROR");
        break;
    case PH_ERR_BUFFER_OVERFLOW:
        MY_DEBUG_PRINT("\t PH_ERR_BUFFER_OVERFLOW");
        break;
    case PH_ERR_FRAMING_ERROR:
        MY_DEBUG_PRINT("\t PH_ERR_FRAMING_ERROR");
        break;
    case PH_ERR_PROTOCOL_ERROR:
        MY_DEBUG_PRINT("\t PH_ERR_PROTOCOL_ERROR");
        break;
    case PH_ERR_RF_ERROR:
        MY_DEBUG_PRINT("\t PH_ERR_RF_ERROR");
        break;
    case PH_ERR_EXT_RF_ERROR:
        MY_DEBUG_PRINT("\t PH_ERR_EXT_RF_ERROR");
        break;
    case PH_ERR_NOISE_ERROR:
        MY_DEBUG_PRINT("\t PH_ERR_NOISE_ERROR");
        break;
    case PH_ERR_ABORTED:
        MY_DEBUG_PRINT("\t PH_ERR_ABORTED");
        break;
    case PH_ERR_INTERNAL_ERROR:
        MY_DEBUG_PRINT("\t PH_ERR_INTERNAL_ERROR");
        break;
    case PH_ERR_INVALID_DATA_PARAMS:
        MY_DEBUG_PRINT("\t PH_ERR_INVALID_DATA_PARAMS");
        break;
    case PH_ERR_INVALID_PARAMETER:
        MY_DEBUG_PRINT("\t PH_ERR_INVALID_PARAMETER");
        break;
    case PH_ERR_PARAMETER_OVERFLOW:
        MY_DEBUG_PRINT("\t PH_ERR_PARAMETER_OVERFLOW");
        break;
    case PH_ERR_UNSUPPORTED_PARAMETER:
        MY_DEBUG_PRINT("\t PH_ERR_UNSUPPORTED_PARAMETER");
        break;
    case PH_ERR_OSAL_ERROR:
        MY_DEBUG_PRINT("\t PH_ERR_OSAL_ERROR");
        break;
    case PHAC_DISCLOOP_LPCD_NO_TECH_DETECTED:
        MY_DEBUG_PRINT("\t PHAC_DISCLOOP_LPCD_NO_TECH_DETECTED");
        break;
    case PHAC_DISCLOOP_COLLISION_PENDING:
        MY_DEBUG_PRINT("\t PHAC_DISCLOOP_COLLISION_PENDING");
        break;
    default:
        MY_DEBUG_PRINT("\t 0x%x",(wStatus & PH_ERR_MASK));
        break;
    }
}

/******************************************************************************
**                            End Of File
******************************************************************************/
