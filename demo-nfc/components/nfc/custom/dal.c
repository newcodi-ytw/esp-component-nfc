#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#include "driver/gpio.h"
#include "driver/spi_master.h"

#include <phCustomHelper.h>
#include "BoardSelection.h"
#include "dal.h"

#ifdef SPI_SEMA_ENABLE
static SemaphoreHandle_t    _spiSemaHandle = NULL;
#define SPI_SEMA_LOCK       xSemaphoreTake(_spiSemaHandle, portMAX_DELAY);
#define SPI_SEMA_UNLOCK     xSemaphoreGive(_spiSemaHandle);
#else
#define SPI_SEMA_LOCK
#define SPI_SEMA_UNLOCK
#endif

#ifdef SPI_SEMA_ENABLE
esp_err_t dal_spi_SemaInit(void)
{
    if (_spiSemaHandle == NULL)
        _spiSemaHandle = xSemaphoreCreateMutex();

    if (_spiSemaHandle)
    {
        DEBUG_LOG_HW("SPI lock inited");
        return ESP_OK;
    }

    DEBUG_LOG_HW("SPI lock Failed!");
    return ESP_FAIL;
}
#else
esp_err_t dal_spi_SemaInit(void){}
#endif

esp_err_t dal_spi_transact(spi_device_handle_t dev, const void *tx, void *rx, int n)
{
    SPI_SEMA_LOCK;

    spi_transaction_t txn = {
        .length = n * 8,
        .tx_buffer = tx,
        .rx_buffer = rx,
    };
    esp_err_t ret = spi_device_transmit(dev, &txn);
    // DEBUG_LOG_HW("SPI data transmitted\n");

    SPI_SEMA_UNLOCK;
    return ret;
}
