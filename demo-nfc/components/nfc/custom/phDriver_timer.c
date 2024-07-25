#include <stdint.h>

#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"

#include "ph_Status.h"
#include "phDriver.h"

// The time, in ticks, to block waiting for timer operations.
#define BLOCK_TIME 10

static TimerHandle_t g_timer = NULL;

typedef struct {
    TickType_t num, den;
} timer_div_t;

static void timer_expiry_cb(TimerHandle_t xTimer) {
    // Just call the callback.
    pphDriver_TimerCallBck_t cb = (pphDriver_TimerCallBck_t)pvTimerGetTimerID(xTimer);
    if(cb)
    {
        cb();
    }
}

phStatus_t phDriver_TimerStart(phDriver_Timer_Unit_t eTimerUnit, uint32_t dwTimePeriod, pphDriver_TimerCallBck_t pTimerCallback) {
    // TickType_t period = pdMS_TO_TICKS(dwTimePeriod);

    //convert to ms unit
    TickType_t period = dwTimePeriod;
    switch(eTimerUnit) {
        case PH_DRIVER_TIMER_SECS:
            period *= 1000;
            break;
        case PH_DRIVER_TIMER_MICRO_SECS:
            period /= 1000;
            break;
        default:
            // Do nothing if it's milliseconds.
            break;
    }
    MY_DEBUG_PRINT("timer start: %d MS\n", period);
    TickType_t tick = pdMS_TO_TICKS(period);
    if(tick == 0) tick = 1;

    // If there's no callback, we just need a delay.
    if(pTimerCallback == NULL) {
        vTaskDelay(tick);
    } else {
        // If it doesn't exist, create it.
        if(g_timer == NULL) {
            g_timer = xTimerCreate("phDriver_TimerStart", tick, pdFALSE, pTimerCallback, timer_expiry_cb);
        }

        // Set the period, then start/restart it.
        xTimerChangePeriod(g_timer, tick, BLOCK_TIME);
        xTimerReset(g_timer, BLOCK_TIME);
    }
    
    return PH_DRIVER_SUCCESS;
}

phStatus_t phDriver_TimerStop(void) {
    if(g_timer == NULL) {
        return PH_DRIVER_ERROR;
    }

    xTimerStop(g_timer, BLOCK_TIME);
    return PH_DRIVER_SUCCESS;
}
