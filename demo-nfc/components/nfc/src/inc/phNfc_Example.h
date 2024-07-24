#if !defined(__PH_NFC_EXAMPLE_H__)
#define __PH_NFC_EXAMPLE_H__

#include <ph_Status.h>

#define LISTEN_PHASE_TIME_MS              1000       /* Listen Phase TIME */

/* Enables configuring of Discovery loop */
#define ENABLE_DISC_CONFIG

#ifdef PH_OSAL_FREERTOS
    #define DISC_DEMO_TASK_STACK         (2048)
    #define DISC_DEMO_TASK_PRIO          2
#endif /* PH_OSAL_FREERTOS */

void phNfc_Example_Init(void);
void phNfc_Example_Main(void* pDataParams);

#endif // __PH_NFC_EXAMPLE_H__
