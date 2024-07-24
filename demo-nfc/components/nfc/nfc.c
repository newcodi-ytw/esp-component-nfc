#include <stdio.h>

#include "phhalHw.h"
#ifdef NXPBUILD__PHHAL_HW_PN5180
#include "nxp/NxpNfcRdLib/comps/phhalHw/src/Pn5180/phhalHw_Pn5180.h"
#endif /* NXPBUILD__PHHAL_HW_PN5180 */

#include "phApp_Init.h"
#include "phNfc_Example.h"
#include "ph_Status.h"

#include "nfc.h"

void NFC_Run(void)
{
    MY_DEBUG_PRINT("NFC Run\n");
    phNfc_Example_Init();
}
void NFC_ReadE2Prom_Version(void)
{
    phStatus_t wStatus = 0;
    uint16_t bFirmwareVer = 0;

    /* Read Set Listen Parameters data from EEPROM */
    wStatus = phhalHw_Pn5180_Instr_ReadE2Prom(
        pHal,
        PHHAL_HW_PN5180_FIRMWARE_VERSION_ADDR,
        &bFirmwareVer,
        2U
        );
    CHECK_SUCCESS(wStatus);

    MY_DEBUG_PRINT("(%d)wFirmwareVer: 0x%x\n", wStatus, bFirmwareVer);
}

#include "phhalHw_Pn5180_Reg.h"
void NFC_UpdateConfig(uint16_t config, uint16_t value)
{
    // MY_DEBUG_PRINT("Set config: %d, value: %d", config, value);
    // phhalHw_Pn5180_SetConfig(pHal, config, value);

    // value = 0xA123;
    // phhalHw_Pn5180_GetConfig(pHal, config, &value);
    // MY_DEBUG_PRINT("Get config: %d, value: %d", config, value);
    if(value == PH_ON)
    {
        /* Enable RF-ON Interrupt*/
        phhalHw_Pn5180_Instr_WriteRegister(pHal, IRQ_ENABLE, IRQ_ENABLE_RFON_DET_IRQ_SET_ENABLE_MASK);

        MY_DEBUG_PRINT2("enable IRQ Reg");
    }
    else
    {
        /* Disable RF-ON Interrupt*/
        phhalHw_Pn5180_Instr_WriteRegisterAndMask(pHal, IRQ_ENABLE,  ~(uint32_t)IRQ_ENABLE_RFON_DET_IRQ_SET_ENABLE_MASK);

        MY_DEBUG_PRINT2("disable IRQ Reg");
    }
}

void NFC_SendISR(void)
{
    phDriver_GpioSendFakeISR();
}