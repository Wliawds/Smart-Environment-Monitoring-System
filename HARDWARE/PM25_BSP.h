#ifndef __PM25_BSP_H
#define __PM25_BSP_H

#include "stm32f4xx_hal.h"

#define PM25_CONSTANT  70
uint32_t Get_PM25Val(uint32_t *buf);
uint32_t PM25_Getmg(uint32_t (*filter)(uint32_t),uint32_t *buf);
uint32_t PM25_Getmg_Init(uint32_t (*filter)(uint32_t),uint32_t *buf);
#endif
