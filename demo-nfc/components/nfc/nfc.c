#include <stdio.h>

#ifdef EXAMPLE_DISC_LOOP
#include "phNfc_Example.h"
#endif

#ifdef EXAMPLE_SIMPLE_ISO
#include "Nfcrdlib_SimplifiedApi_ISO.h"
#endif

#include "nfc.h"
void NFC_Run(void)
{
    printf("NFC Run\n");
    
#ifdef EXAMPLE_DISC_LOOP
    phNfc_Example_Init();
#endif  

#ifdef EXAMPLE_SIMPLE_ISO
    SimplifiedApiDemo_Main();
#endif

}