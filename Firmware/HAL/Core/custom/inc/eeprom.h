#ifndef __EEPROM_H_
#define __EEPROM_H_
#include "stm32f1xx_hal.h"
#define EEPROM_DEV_ADDR			0xA0		/* 24xx02的设备地址 */
#define EEPROM_PAGE_SIZE		  8			  /* 24xx02的页面大小 */
#define EEPROM_SIZE				  256			  /* 24xx02总容量 */

#define EEPROM_GPIO_PORT GPIOA
#define EEPROM_GPIO_SDA_PIN GPIO_PIN_6
#define EEPROM_GPIO_SCL_PIN GPIO_PIN_7

void EEPROM_Config(void);
void EEPROMReload(void);
void EEPROMOutload(void);
uint8_t EEPROMWriteFloat(float f,uint16_t *index);
float EEPROMReadFloat(uint16_t *index);
uint8_t EEPROM_ReadBytes(uint8_t *_pReadBuf, uint16_t _usAddress, uint16_t _usSize);
uint8_t EEPROM_WriteBytes(uint8_t *_pWriteBuf, uint16_t _usAddress, uint16_t _usSize);
void EE_Delay(void);



#endif
