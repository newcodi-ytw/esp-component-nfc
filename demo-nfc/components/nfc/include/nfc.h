#if !defined(__NFC_H__)
#define __NFC_H__

#ifdef __cplusplus
extern "C" {
#endif

void NFC_Run(void);
void NFC_ReadE2Prom_Version(void);
void NFC_UpdateConfig(uint16_t config, uint16_t value);
void NFC_SendISR(void);

#ifdef __cplusplus
}
#endif

#endif // __NFC_H__