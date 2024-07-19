#include <stdio.h>
#include "nfc.h"

#include <phApp_Init.h>
#include <ph_Status.h>
#include <phhalHw.h>
#include <ph_RefDefs.h>

#include "phhalHw_Pn5180.h"
#include "NfcrdlibEx1_DiscoveryLoop.h"

#include "nfc.h"

void NFC_Run(void)
{
    printf("NFC Run\n");
    NfcrdlibEx1_DiscoveryLoop_main();
    NFC_GetVersion();
}

void NFC_GetVersion(void)
{
    printf("wFirmwareVer: 0x%x\n", pHal->wFirmwareVer);
}

void NFC_ReadE2Prom_Version(void)
{
    phStatus_t wStatus;
    uint16_t bFirmwareVer;

    /* Read Set Listen Parameters data from EEPROM */
    wStatus = phhalHw_Pn5180_Instr_ReadE2Prom(
        pHal,
        PHHAL_HW_PN5180_FIRMWARE_VERSION_ADDR,
        &bFirmwareVer,
        2U
        );
    CHECK_SUCCESS(wStatus);

    printf("(%d)wFirmwareVer: 0x%x\n", wStatus, bFirmwareVer);
}