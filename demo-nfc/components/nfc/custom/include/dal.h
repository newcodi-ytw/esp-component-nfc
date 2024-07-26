#ifndef NFC_DAL_H
#define NFC_DAL_H

#include "driver/spi_master.h"

#include "ph_Status.h"
#include "phDriver.h"
#include "phNfcLib.h"

esp_err_t dal_spi_lockInit(void);
esp_err_t dal_spi_transact(spi_device_handle_t dev, const void *tx, void *rx, int n);

#endif /* NFC_DAL_H */
