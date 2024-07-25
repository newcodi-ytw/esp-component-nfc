#include <stdio.h>

#ifdef EXAMPLE_DISC_LOOP
#include "phNfc_Example.h"
#endif

#include "nfc.h"
void NFC_Run(void)
{
    printf("NFC Run\n");
    
#ifdef EXAMPLE_DISC_LOOP
    phNfc_Example_Init();
#endif

}