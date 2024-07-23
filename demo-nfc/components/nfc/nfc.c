#include <stdio.h>
#include "nfc.h"

#include "phApp_Init.h"
#include "phNfc_Example.h"

void NFC_Run(void)
{
    printf("NFC Run\n");
    phNfc_Example_Init();
}
void NFC_ReadE2Prom_Version(void)
{
    phStatus_t wStatus = 0;
    uint16_t bFirmwareVer = 0;

    /* Read Set Listen Parameters data from EEPROM */
    // wStatus = phhalHw_Pn5180_Instr_ReadE2Prom(
    //     pHal,
    //     PHHAL_HW_PN5180_FIRMWARE_VERSION_ADDR,
    //     &bFirmwareVer,
    //     2U
    //     );
    // CHECK_SUCCESS(wStatus);

    printf("(%d)wFirmwareVer: 0x%x\n", wStatus, bFirmwareVer);
}

void NFC_PollMode(void)
{
    
}