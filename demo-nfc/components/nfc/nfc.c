#include <stdio.h>

#ifdef EXAMPLE_DISC_LOOP
#include "NfcrdlibEx1_DiscoveryLoop.h"
#ifdef EXAMPLE_TEST_LOOP
#include "phNfc_Example.h"
#endif
#elif EXAMPLE_SIMPLE_ISO
#include "Nfcrdlib_SimplifiedApi_ISO.h"
#elif EXAMPLE_MIFARE_C
#include "NfcrdlibEx4_MIFAREClassic.h"
#elif EXAMPLE_NFCFORUM
#include "NfcrdlibEx3_NFCForum.h"
#endif

#include "nfc.h"
void NFC_Run(void)
{
    printf("NFC Run\n");
    
#ifdef EXAMPLE_DISC_LOOP
#ifdef EXAMPLE_TEST_LOOP
    phNfc_Example_Init();
#else
    DiscoveryLoop_Demo_Main();
#endif
#elif EXAMPLE_SIMPLE_ISO
    SimplifiedApiDemo_Main();
#elif EXAMPLE_MIFARE_C
    NfcrdlibEx4_MIFAREClassic_Main();
#elif EXAMPLE_NFCFORUM
    NfcrdlibEx3_NFCForum_Main();
#endif

}