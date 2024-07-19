#include <stdint.h>

#include "freertos/FreeRTOS.h"
#include "driver/spi_master.h"

#include "BoardSelection.h"

#include "phDriver.h"

esp_err_t init_spi(spi_device_handle_t *dev) {
    esp_err_t rv;

    spi_bus_config_t bus_cfg = {
        .miso_io_num = PIN_MISO,
        .mosi_io_num = PIN_MOSI,
        .sclk_io_num = PIN_SCLK,

        .quadwp_io_num = -1,
        .quadhd_io_num = -1,

        .max_transfer_sz = 0,
    };

    spi_device_interface_config_t dev_cfg = {
        .clock_speed_hz = 4000000,
        .mode = 0,
        .spics_io_num = PIN_SSEL,
        .queue_size = 4,
        .pre_cb = NULL,
    };

    rv = spi_bus_initialize(SPI3_HOST, &bus_cfg, 2);
    ESP_ERROR_CHECK(rv);

    rv = spi_bus_add_device(SPI3_HOST, &dev_cfg, dev);
    ESP_ERROR_CHECK(rv);

    return rv;
}

