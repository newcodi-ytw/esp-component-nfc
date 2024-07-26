#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "sdkconfig.h"
#include "nvs_flash.h"
#include "esp_log.h"

#include "nfc.h"
#include "console_cmd.hpp"

const char *TAG = "demo-nfc";
void app_main(void)
{
    ESP_LOGI(TAG, "start");
    ESP_ERROR_CHECK(nvs_flash_init());

    ConsoleStart();

    while(1)
    {
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}
