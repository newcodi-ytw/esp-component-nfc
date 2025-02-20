/*----------------------------------------------------------------------------*/
/* Copyright 2016-2023 NXP                                                    */
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
* $Author: NXP $
* $Revision:  $ (v07.10.00)
* $Date:  $
*
*/

#include "phApp_Init.h"

#ifdef PHDRIVER_KINETIS_K82
#include <fsl_port.h>
#include <fsl_pit.h>
#ifdef DEBUG
#include <fsl_clock.h>
#endif
#endif /* PHDRIVER_KINETIS_K82 */

#ifdef PHDRIVER_KINETIS_K82
#ifdef DEBUG

#define KINETIS_K82_DEBUG_UART_CLK_FREQ         CLOCK_GetOsc0ErClkFreq()
#define KINETIS_K82_DEBUG_UART_BASEADDR         ((uint32_t)(LPUART4))
#define KINETIS_K82_DEBUG_UART_INSTANCE         (4U)
#define KINETIS_K82_DEBUG_UART_BAUDRATE         (115200U)
#define KINETIS_K82_DEBUG_UART_TYPE             DEBUG_CONSOLE_DEVICE_TYPE_LPUART

#endif

/*! @brief Clock configuration structure. */
typedef struct _clock_config
{
    mcg_config_t mcgConfig;       /*!< MCG configuration.      */
    sim_clock_config_t simConfig; /*!< SIM configuration.      */
    osc_config_t oscConfig;       /*!< OSC configuration.      */
    uint32_t coreClock;           /*!< core clock frequency.   */
} clock_config_t;
#endif /* PHDRIVER_KINETIS_K82 */

#if defined(PHDRIVER_LPC1769) && defined(__CC_ARM)
uint32_t SystemCoreClock;
#endif
/*******************************************************************************
**   Function Declarations
*******************************************************************************/
#ifdef PHDRIVER_KINETIS_K82
static void phApp_K82_Init(void);
#endif /* PHDRIVER_KINETIS_K82 */

/*******************************************************************************
**   Clock configuration of K82 Platform
*******************************************************************************/

#ifdef PHDRIVER_KINETIS_K82
/* Configuration for enter RUN mode. Core clock = 50MHz. */
const clock_config_t g_defaultClockConfigRun = {
    .mcgConfig =
        {
            .mcgMode = kMCG_ModePEE,             /* Work in PEE mode. */
            .irclkEnableMode = kMCG_IrclkEnable, /* MCGIRCLK enable. */
            .ircs = kMCG_IrcSlow,                /* Select IRC32k. */
            .fcrdiv = 0U,                        /* FCRDIV is 0. */

            .frdiv = 4U,
            .drs = kMCG_DrsLow,         /* Low frequency range. */
            .dmx32 = kMCG_Dmx32Default, /* DCO has a default range of 25%. */
            .oscsel = kMCG_OscselOsc,   /* Select OSC. */

            .pll0Config =
                {
                    .enableMode = 0U, .prdiv = 0x01U, .vdiv = 0x01U,
                },
        },
    .simConfig =
        {
            .pllFllSel = 1U,        /* PLLFLLSEL select PLL. */
            .pllFllDiv = 0U,        /* PLLFLLSEL clock divider divisor. */
            .pllFllFrac = 0U,       /* PLLFLLSEL clock divider fraction. */
            .er32kSrc = 5U,         /* ERCLK32K selection, use RTC. */
            .clkdiv1 = 0x01140000U, /* SIM_CLKDIV1. */
        },
    .oscConfig = {.freq = CPU_XTAL_CLK_HZ,
                  .capLoad = 0,
                  .workMode = kOSC_ModeOscLowPower,
                  .oscerConfig =
                      {
                          .enableMode = kOSC_ErClkEnable,
#if (defined(FSL_FEATURE_OSC_HAS_EXT_REF_CLOCK_DIVIDER) && FSL_FEATURE_OSC_HAS_EXT_REF_CLOCK_DIVIDER)
                          .erclkDiv = 0U,
#endif
                      }},

};
#endif /* PHDRIVER_KINETIS_K82 */

/*******************************************************************************
**   Function Definitions
*******************************************************************************/
#ifdef PHDRIVER_KINETIS_K82
static void phApp_K82_Init(void)
{
#ifdef DEBUG
    uint32_t uartClkSrcFreq;
#endif /* DEBUG */

    pit_config_t pitConfig;         /* Structure of initialize PIT */

    CLOCK_SetSimSafeDivs();

    CLOCK_InitOsc0(&g_defaultClockConfigRun.oscConfig);
    CLOCK_SetXtal0Freq(CPU_XTAL_CLK_HZ);

    CLOCK_BootToPeeMode(g_defaultClockConfigRun.mcgConfig.oscsel, kMCG_PllClkSelPll0,
        &g_defaultClockConfigRun.mcgConfig.pll0Config);

    CLOCK_SetInternalRefClkConfig(g_defaultClockConfigRun.mcgConfig.irclkEnableMode,
        g_defaultClockConfigRun.mcgConfig.ircs, g_defaultClockConfigRun.mcgConfig.fcrdiv);

    CLOCK_SetSimConfig(&g_defaultClockConfigRun.simConfig);

    SystemCoreClockUpdate();

    /*
     * pitConfig.enableRunInDebug = false;
     */
    PIT_GetDefaultConfig(&pitConfig);

    /* Init pit module */
    PIT_Init(PIT, &pitConfig);

#ifdef DEBUG

    /* Initialize LPUART4 pins below used to Print */
    /* Ungate the port clock */
    CLOCK_EnableClock(kCLOCK_PortC);
    /* Affects PORTC_PCR14 register */
    PORT_SetPinMux(PORTC, 14U, kPORT_MuxAlt3);
    /* Affects PORTC_PCR15 register */
    PORT_SetPinMux(PORTC, 15U, kPORT_MuxAlt3);

    /* SIM_SOPT2[27:26]:
     *  00: Clock Disabled
     *  01: MCGFLLCLK, or MCGPLLCLK, or IRC48M
     *  10: OSCERCLK
     *  11: MCGIRCCLK
     */
    CLOCK_SetLpuartClock(2);

    uartClkSrcFreq = KINETIS_K82_DEBUG_UART_CLK_FREQ;

    DbgConsole_Init(KINETIS_K82_DEBUG_UART_INSTANCE, KINETIS_K82_DEBUG_UART_BAUDRATE, KINETIS_K82_DEBUG_UART_TYPE, uartClkSrcFreq);
#endif /* DEBUG */
}
#endif /* NXPBUILD__PH_KINETIS_K82 */

#ifdef PH_PLATFORM_HAS_ICFRONTEND
/**
* This function will initialize Host Controller interfaced with NXP Reader IC's.
* Any initialization which is not generic across Platforms, should be done here.
* Note: For NXP NFC Controllers HOST initialization is not required.
*/
void phApp_CPU_Init(void)
{
#if defined PHDRIVER_KINETIS_K82
    phApp_K82_Init();
#elif defined(PHDRIVER_LPC1769) && defined(__CC_ARM)
    SystemCoreClock =  (( unsigned long ) 96000000);
#else
    /* In case of LPC series, startup file takes care of initializing clock and ports.
     * No initialization is required in Linux environment. */
#endif
}
#endif /* PH_PLATFORM_HAS_ICFRONTEND */

phStatus_t phApp_Configure_IRQ(void)
{
#ifdef PH_OSAL_LINUX
    phStatus_t  wStatus;
#endif /* PH_OSAL_LINUX */

#ifdef PH_PLATFORM_HAS_ICFRONTEND
    phDriver_Pin_Config_t pinCfg;

    pinCfg.bOutputLogic = PH_DRIVER_SET_LOW;
    pinCfg.bPullSelect = PH_DRIVER_PULL_UP;

    pinCfg.eInterruptConfig = PIN_IRQ_TRIGGER_TYPE;
    phDriver_PinConfig(PHDRIVER_PIN_IRQ, PH_DRIVER_PINFUNC_INTERRUPT, &pinCfg);

#ifdef PHDRIVER_LPC1769
    NVIC_SetPriority(EINT_IRQn, EINT_PRIORITY);
    /* Enable interrupt in the NVIC */
    NVIC_ClearPendingIRQ(EINT_IRQn);
    NVIC_EnableIRQ(EINT_IRQn);
#endif /* PHDRIVER_LPC1769 */

#ifdef PH_OSAL_LINUX

    gphPiThreadObj.pTaskName = (uint8_t *) "IrqPolling";
    gphPiThreadObj.pStackBuffer = NULL;
    gphPiThreadObj.priority = PI_IRQ_POLLING_TASK_PRIO;
    gphPiThreadObj.stackSizeInNum = PI_IRQ_POLLING_TASK_STACK;
    PH_CHECK_SUCCESS_FCT(wStatus, phOsal_ThreadCreate(&gphPiThreadObj.ThreadHandle, &gphPiThreadObj,
        &phExample_IrqPolling, NULL));

#endif /* PH_OSAL_LINUX */

#ifdef PHDRIVER_KINETIS_K82
    NVIC_SetPriority(EINT_IRQn, EINT_PRIORITY);
    NVIC_ClearPendingIRQ(EINT_IRQn);
    EnableIRQ(EINT_IRQn);
#endif /* PHDRIVER_KINETIS_K82 */

#endif /* #ifdef PH_PLATFORM_HAS_ICFRONTEND */

    return PH_ERR_SUCCESS;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
