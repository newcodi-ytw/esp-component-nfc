#include <stdint.h>

#include "esp_log.h"
#include "dal.h"

#include "phDriver.h"
#include "phDriver_gpio.h"
#include "phDriver_spi.h"

#include "phCustomHelper.h"

static const char *TAG = "NFC";

static spi_device_handle_t g_spi_dev;

phStatus_t phbalReg_Init(void     *pDataParams,
                         uint16_t  wSizeOfDataParams) {
    phbalReg_Type_t *params = (phbalReg_Type_t *)pDataParams;
    params->wId = PH_COMP_DRIVER;
    params->bBalType = PHBAL_REG_TYPE_SPI;

    ESP_ERROR_CHECK( init_spi(&g_spi_dev) );

    phDriver_GPIOInit();
    return PH_DRIVER_SUCCESS;
}

phStatus_t phbalReg_SetConfig(void     *pDataParams,
                              uint16_t  wConfig,
                              uint32_t  wValue) {
    return PH_DRIVER_SUCCESS;
}

phStatus_t phbalReg_GetConfig(void     *pDataParams,
                              uint16_t  wConfig,
                              uint32_t *pValue) {
    return PH_DRIVER_SUCCESS;
}

#if 0
void PCD_HelpShowByte(const char *prefix, uint8_t *data, uint32_t len){}
#else
void PCD_HelpShowByte(const char *prefix, uint8_t *data, uint32_t len)
{
    if(prefix != NULL)
        esp_rom_printf("%s(%d):\n", prefix, len);
        
    if (len == 0) return;

    uint32_t i = 0;
    uint32_t consecCnt = 0;
    uint8_t current = data[0];
    bool showDots = false;
    esp_rom_printf("%02x ", current);

    i = 1;
    while (i < len)
    {
        if(i > 100) {
            esp_rom_printf("Too long .... (size= %d)", len);
            break;
        }

        if (current == data[i])
        {
            consecCnt++;
            if (consecCnt >= 10)
            {
                if (!showDots)
                {
                    showDots = true;
                    esp_rom_printf("... ");
                }
                else
                {
                    if (i == len - 1)
                    {
                        esp_rom_printf("%02x ", data[i]);
                    }
                }
            }
            else
            {
                esp_rom_printf("%02x ", data[i]);
            }
        }
        else
        {
            current = data[i];
            consecCnt = 0;
            showDots = false;
            esp_rom_printf("%02x ", data[i]);
        }

        if (i % 8 == 0)
            esp_rom_printf("\n");

        i++;
    }
    esp_rom_printf("\n");
}
#endif

phStatus_t phbalReg_Exchange(void     *pDataParams,
                             uint16_t  wOption,
                             uint8_t  *pTxBuffer,
                             uint16_t  wTxLength,
                             uint16_t  wRxBufSize,
                             uint8_t  *pRxBuffer,
                             uint16_t *pRxLength) {
    uint16_t len = wTxLength ? wTxLength : wRxBufSize; 
    ESP_LOGD(TAG, "SPI transaction: write %d read %d options 0x%08x",
             wTxLength, wRxBufSize, wOption);

    if(wTxLength && pTxBuffer != NULL) {
        ESP_LOGD(TAG, "Write data:");
        ESP_LOG_BUFFER_HEX_LEVEL(TAG, pTxBuffer, wTxLength, ESP_LOG_DEBUG);
    #ifdef DEBUG_LOG_HW_ENABLE
        PCD_HelpShowByte(">>", pTxBuffer, wTxLength);
    #endif

        // Do the write transaction.
        ESP_ERROR_CHECK( dal_spi_transact(g_spi_dev,
                                          pTxBuffer,
                                          NULL,
                                          wTxLength) );
        DEBUG_LOG_HW(">>>>");
    }
    if(wRxBufSize && pRxBuffer != NULL) {
        ESP_LOGD(TAG, "Initing %d bytes of read buffer %p", wRxBufSize, pRxBuffer);
        // Fill the read buffer, which will be outgoing, with FF.
        //memset(pRxBuffer, 0xff, wRxBufSize);

        ESP_LOGD(TAG, "Reading %d bytes", wRxBufSize);
        // Do the read transaction.
        ESP_ERROR_CHECK( dal_spi_transact(g_spi_dev,
                                          pRxBuffer,
                                          pRxBuffer,
                                          wRxBufSize) );

        ESP_LOGD(TAG, "Read data:\n");
        ESP_LOG_BUFFER_HEX_LEVEL(TAG, pRxBuffer, wTxLength, ESP_LOG_DEBUG);
    #ifdef DEBUG_LOG_HW_ENABLE
        PCD_HelpShowByte("--", pRxBuffer, wTxLength);
    #endif
        *pRxLength = len;
    }
    DEBUG_LOG_HW("----");

    return PH_DRIVER_SUCCESS;
}
