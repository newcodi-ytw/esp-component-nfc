#include <stdio.h>

#ifdef EXAMPLE_DISC_LOOP
#include "NfcrdlibEx1_DiscoveryLoop.h"
#include "phNfc_Example.h"
#endif

#ifdef EXAMPLE_SIMPLE_ISO
#include "Nfcrdlib_SimplifiedApi_ISO.h"
#endif

#ifdef EXAMPLE_MIFARE_C
#include "NfcrdlibEx4_MIFAREClassic.h"
#endif

#include "nfc.h"
void NFC_Run(void)
{
    printf("NFC Run\n");
    
#ifdef EXAMPLE_DISC_LOOP
    // phNfc_Example_Init();
    DiscoveryLoop_Demo_Main();
#endif  

#ifdef EXAMPLE_SIMPLE_ISO
    SimplifiedApiDemo_Main();
#endif

#ifdef EXAMPLE_MIFARE_C
    NfcrdlibEx4_MIFAREClassic_Main();
#endif

}