#ifndef __MYDELAY_H
#define __MYDELAY_H

#include "stm32f4xx_hal.h"


void c(void);
void USR_Delay_us(uint32_t nus);
void USR_Delay_ms(uint16_t nms);

void T_Delay_us(uint32_t nus);

void T_Delay_ms(uint16_t nms);











#endif
