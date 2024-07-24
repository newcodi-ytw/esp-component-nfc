#ifndef NFC_PHDRIVER_GPIO_H
#define NFC_PHDRIVER_GPIO_H

phStatus_t phDriver_GPIOInit();
void phDriver_GpioDebugToggle(void);
void phDriver_GpioSendFakeISR(void);

#endif /* NFC_PHDRIVER_GPIO_H */
