#include <stdint.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

#include "driver/gpio.h"

#include "BoardSelection.h"

#include "phDriver.h"

// #undef portYIELD_FROM_ISR
// #define portYIELD_FROM_ISR(...)        {traceISR_EXIT_TO_SCHEDULER(); _frxt_setup_switch();}

extern void CLIF_IRQHandler();

typedef struct {
    gpio_pullup_t   pup;
    gpio_pulldown_t pdn;
} pull_map_t;

static const pull_map_t pull_map[] = {
    [PH_DRIVER_PULL_DOWN] = {GPIO_PULLUP_DISABLE, GPIO_PULLDOWN_ENABLE},
    [PH_DRIVER_PULL_UP]   = {GPIO_PULLUP_ENABLE,  GPIO_PULLDOWN_DISABLE},
};

static const gpio_mode_t mode_map[] = {
    [PH_DRIVER_PINFUNC_INPUT]     = GPIO_MODE_INPUT,
    [PH_DRIVER_PINFUNC_OUTPUT]    = GPIO_MODE_OUTPUT,
    [PH_DRIVER_PINFUNC_BIDIR]     = GPIO_MODE_INPUT_OUTPUT,
    [PH_DRIVER_PINFUNC_INTERRUPT] = GPIO_MODE_INPUT,
};

static const gpio_int_type_t intr_map[] = {
    [PH_DRIVER_INTERRUPT_LEVELZERO]   = GPIO_INTR_LOW_LEVEL,
    [PH_DRIVER_INTERRUPT_LEVELONE]    = GPIO_INTR_HIGH_LEVEL,
    [PH_DRIVER_INTERRUPT_RISINGEDGE]  = GPIO_INTR_POSEDGE,
    [PH_DRIVER_INTERRUPT_FALLINGEDGE] = GPIO_INTR_NEGEDGE,
    [PH_DRIVER_INTERRUPT_EITHEREDGE]  = GPIO_INTR_ANYEDGE,
};

static EventGroupHandle_t isr_events = NULL;

static void gpio_isr(void *param) {
    EventBits_t mask = (EventBits_t)param;
    BaseType_t do_switch = pdFALSE;
    BaseType_t xResult = pdFALSE;

    // EventBits_t test = (1 << (PHDRIVER_PIN_IRQ & 0x1f));
    // uint8_t result = (test & mask) ? 1 : 0;
    // gpio_set_level(PIN_GPO1_GREEN, result);
    // gpio_set_level(PIN_GPO1_RED, !result);

    xResult = xEventGroupSetBitsFromISR(isr_events, mask, &do_switch);

    CLIF_IRQHandler();

    if(xResult == pdPASS) 
    {
        portYIELD_FROM_ISR(do_switch);
    }
    else
    {
        // gpio_set_level(PIN_GPO1_GREEN, 0);
        // gpio_set_level(PIN_GPO1_BLUE, 1);
    }
}

phStatus_t phDriver_GPIOInit() {
    if(isr_events != NULL) return PH_DRIVER_ERROR;

    isr_events = xEventGroupCreate();
    if(ESP_OK != gpio_install_isr_service(0)) 
    {
        printf("GPIO init PH_DRIVER_ERROR\n");
        return PH_DRIVER_ERROR;
    }

    PIN_FUNC_SELECT(PIN_BUSY_REG, PIN_FUNC_GPIO);
	gpio_set_direction(PIN_BUSY, GPIO_MODE_INPUT);

    PIN_FUNC_SELECT(PIN_RESET_REG, PIN_FUNC_GPIO);
	gpio_set_direction(PIN_RESET, GPIO_MODE_OUTPUT);

	gpio_set_direction(PIN_GPO1_GREEN, GPIO_MODE_OUTPUT);	
	gpio_set_direction(PIN_GPO1_BLUE, GPIO_MODE_OUTPUT);	
	gpio_set_direction(PIN_GPO1_RED, GPIO_MODE_OUTPUT);	

    vTaskDelay(pdMS_TO_TICKS(10));

    printf("GPIO init!\n");
    return PH_DRIVER_SUCCESS;
}

phStatus_t phDriver_PinConfig(uint32_t               dwPinNumber,
                              phDriver_Pin_Func_t    ePinFunc,
                              phDriver_Pin_Config_t *pPinConfig) {
    printf("GPIO pin %d config (int 0x%08x pull %d)!\n",
           dwPinNumber, pPinConfig->eInterruptConfig, pPinConfig->bPullSelect);

    // Pre-map the pull config.
    const pull_map_t *pull_cfg = &pull_map[pPinConfig->bPullSelect];
    const gpio_int_type_t intr = (ePinFunc == PH_DRIVER_PINFUNC_INTERRUPT) ?
                                    intr_map[pPinConfig->eInterruptConfig] :
                                    GPIO_INTR_DISABLE;

    // Fill out the config struct.
    const gpio_config_t gpio_cfg = {
        .pin_bit_mask = ((uint64_t)1 << dwPinNumber),
        .mode         = mode_map[ePinFunc],
        .pull_up_en   = pull_cfg->pup,
        .pull_down_en = pull_cfg->pdn,
        .intr_type    = intr,
    };

    // Execute the config.
    esp_err_t err = gpio_config(&gpio_cfg);

    if(err != ESP_OK) return PH_DRIVER_ERROR;

    // if(intr == GPIO_INTR_DISABLE) return PH_DRIVER_SUCCESS;
    err = gpio_set_intr_type(dwPinNumber, intr);

    // Set the interrupt handler.
    EventBits_t mask = (1 << (dwPinNumber & 0x1f));
    err = gpio_isr_handler_add(dwPinNumber, gpio_isr, (void *)mask);

    return err == ESP_OK ? PH_DRIVER_SUCCESS : PH_DRIVER_ERROR;
}

uint8_t phDriver_PinRead(uint32_t            dwPinNumber,
                         phDriver_Pin_Func_t ePinFunc) {
    // Is it an interrupt pin? I guess we need to get the interrupt status.
    if(ePinFunc == PH_DRIVER_PINFUNC_INTERRUPT) {
        EventBits_t mask = (1 << (dwPinNumber & 0x1f));

        EventBits_t retBit = 0;
        uint8_t ret = 0;
        if (xPortInIsrContext())
        {
            retBit = xEventGroupGetBitsFromISR(isr_events);
            ret = (retBit & mask) ? 1 : 0;

            gpio_set_level(PIN_GPO1_GREEN, ret);
            gpio_set_level(PIN_GPO1_RED, !ret);

            // portYIELD_FROM_ISR();

            return ret;
        }
        else
        {
            retBit = xEventGroupGetBits(isr_events);
            ret = (retBit & mask) ? 1 : 0;
            return ret;
        }

        return ret;
    }

    return gpio_get_level(dwPinNumber) ? 1 : 0;
}

void phDriver_PinWrite(uint32_t dwPinNumber,
                       uint8_t  bValue) {
    gpio_set_level(dwPinNumber, bValue);
}

void phDriver_PinClearIntStatus(uint32_t dwPinNumber) {
    EventBits_t mask = (1 << (dwPinNumber & 0x1f));
    // gpio_set_level(PIN_GPO1_GREEN, 1);

    if(xPortInIsrContext()) {
        xEventGroupClearBitsFromISR(isr_events, mask);
        // portYIELD_FROM_ISR();
    } else {
        xEventGroupClearBits(isr_events, mask);
    }
}
