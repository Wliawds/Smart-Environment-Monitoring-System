#ifndef __AHT20_H
#define __AHT20_H

#include "stm32f4xx_hal.h"

#define AHT20_ADDR 	0X38

uint8_t AHT20_Read_Status(void);
void AHT20_SendAC(void);
void AHT20_Read_CTdata(int32_t *ct);
void AHT20_RST_REG(void);





#endif




