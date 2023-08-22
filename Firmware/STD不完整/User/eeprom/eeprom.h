#ifndef __EEPROM_H_
#define __EEPROM_H_
#include "stm32f10x.h"
#define EEPROM_DEV_ADDR			0xA0		/* 24xx02的设备地址 */
#define EEPROM_PAGE_SIZE		  8			  /* 24xx02的页面大小 */
#define EEPROM_SIZE				  256			  /* 24xx02总容量 */

#define EEPROM_GPIO_PORT GPIOA
#define EEPROM_GPIO_SDA_PIN GPIO_Pin_6
#define EEPROM_GPIO_SCL_PIN GPIO_Pin_7
#define EEPROM_GPIO_RCC_PORT RCC_APB2Periph_GPIOA
#define EEPROM_GPIO_RCC_CMD RCC_APB2PeriphClockCmd
void EEPROM_Config(void);
void EEPROMReload(void);
uint8_t EEPROM_ReadBytes(uint8_t *_pReadBuf, uint16_t _usAddress, uint16_t _usSize);
uint8_t EEPROM_WriteBytes(uint8_t *_pWriteBuf, uint16_t _usAddress, uint16_t _usSize);
void EE_Delay(void);



#endif
