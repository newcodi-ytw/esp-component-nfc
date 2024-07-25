#if !defined(__PH_CUSTOM_HELPER_H__)
#define __PH_CUSTOM_HELPER_H__

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

#endif // __PH_CUSTOM_HELPER_H__
