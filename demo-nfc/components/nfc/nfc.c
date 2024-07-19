#include <stdio.h>
#include "nfc.h"

#include "Board_Pn5180_CustomDev.h"

#include <phApp_Init.h>
#include <NfcrdlibEx1_DiscoveryLoop.h>

#include "nfc.h"

void NFC_Run(void)
{
    printf("NFC Run\n");
    NfcrdlibEx1_DiscoveryLoop_main();
}

void NFC_GetVersion(void)
{
    printf("wFirmwareVer: 0x%x\n", pHal->wFirmwareVer);
}
