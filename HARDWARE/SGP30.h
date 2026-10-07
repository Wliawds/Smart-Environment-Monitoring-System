#ifndef __SGP30_H
#define __SGP30_H


#include "stm32f4xx_hal.h"
#define SGP30_ADDR 	0X58 //

void SGP30_Init(void);
void SGP20_Read_data(uint32_t *dat);
void SGP30_START_MEASURE(void);



#endif






