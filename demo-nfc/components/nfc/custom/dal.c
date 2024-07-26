#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#include "driver/gpio.h"
#include "driver/spi_master.h"

#include <phCustomHelper.h>
#include "BoardSelection.h"
#include "dal.h"

static SemaphoreHandle_t _spiLockHandle = NULL;
#define SPI_LOCK xSemaphoreTake(_spiLockHandle, portMAX_DELAY);
#define SPI_UNLOCK xSemaphoreGive(_spiLockHandle);

esp_err_t dal_spi_lockInit(void)
{
    if (_spiLockHandle == NULL)
        _spiLockHandle = xSemaphoreCreateMutex();

    if (_spiLockHandle)
    {
        DEBUG_LOG_HW("SPI lock inited");
        return ESP_OK;
    }

    DEBUG_LOG_HW("SPI lock Failed!");
    return ESP_FAIL;
}

esp_err_t dal_spi_transact(spi_device_handle_t dev, const void *tx, void *rx, int n)
{
    // SPI_LOCK;

    spi_transaction_t txn = {
        .length = n * 8,
        .tx_buffer = tx,
        .rx_buffer = rx,
    };

    esp_err_t ret = spi_device_transmit(dev, &txn);
    // esp_err_t ret = spi_device_polling_transmit(dev, &txn);
    // printf("SPI data transmitted\n");

    // SPI_UNLOCK;
    return ret;
}
