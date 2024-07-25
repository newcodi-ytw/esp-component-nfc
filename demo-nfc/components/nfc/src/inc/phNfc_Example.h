#if !defined(__PH_NFC_EXAMPLE_H__)
#define __PH_NFC_EXAMPLE_H__

#include <ph_Status.h>
#include <ph_TypeDefs.h>

// #define DEBUG_LOG_CORE_ENABLE
// #define DEBUG_LOG_RTOS_ENABLE
// #define DEBUG_LOG_TIMER_ENABLE
// #define DEBUG_LOG_HW_ENABLE

#define DEBUG_LOG_UI_ENABLE

extern void PCD_HelpShowByte(const char *prefix, uint8_t *data, uint32_t len);

#ifdef DEBUG_LOG_CORE_ENABLE
#define DEBUG_LOG_CORE(format, ...) \
    do{\
        esp_rom_printf("\n[CORE]::%s(%d): "format"\n", __FUNCTION__, __LINE__, ##__VA_ARGS__); \
    }while(0);
#else
#define DEBUG_LOG_CORE(format, ...)
#endif

#ifdef DEBUG_LOG_RTOS_ENABLE
#define DEBUG_LOG_RTOS(format, ...) \
    do{\
        esp_rom_printf("\n[RTOS]::%s(%d): "format"\n", __FUNCTION__, __LINE__, ##__VA_ARGS__); \
    }while(0);
#else
#define DEBUG_LOG_RTOS(format, ...)
#endif

#ifdef DEBUG_LOG_TIMER_ENABLE
#define DEBUG_LOG_TIMER(format, ...) \
    do{\
        esp_rom_printf("\n[TIMER]::%s(%d): "format"\n", __FUNCTION__, __LINE__, ##__VA_ARGS__); \
    }while(0);
#else
#define DEBUG_LOG_TIMER(format, ...)
#endif

#ifdef DEBUG_LOG_HW_ENABLE
#define DEBUG_LOG_HW(format, ...) \
    do{\
        esp_rom_printf("\n[HW]::%s(%d): "format"\n", __FUNCTION__, __LINE__, ##__VA_ARGS__); \
    }while(0);
#else
#define DEBUG_LOG_HW(format, ...)
#endif

#ifdef DEBUG_LOG_UI_ENABLE
#define DEBUG_LOG_UI(format, ...) \
    do{\
        esp_rom_printf("\n[UI]:: "format"\n", ##__VA_ARGS__); \
    }while(0);
#else
#define DEBUG_LOG_UI(format, ...)
#endif

#define LISTEN_PHASE_TIME_MS              1000       /* Listen Phase TIME */

/* Enables configuring of Discovery loop */
// #define ENABLE_DISC_CONFIG

#ifdef PH_OSAL_FREERTOS
    #define DISC_DEMO_TASK_STACK         (1024*2)
    #define DISC_DEMO_TASK_PRIO          2
#endif /* PH_OSAL_FREERTOS */

void phNfc_Example_Init(void);
void phNfc_Example_Main(void* pDataParams);

#endif // __PH_NFC_EXAMPLE_H__
