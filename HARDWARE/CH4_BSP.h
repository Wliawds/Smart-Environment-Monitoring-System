#ifndef __CH4_BSP_H
#define __CH4_BSP_H

#include "stm32f4xx_hal.h"

#define RO_CONSTANT  85

uint32_t Get_PM25Val(uint32_t *buf);

uint32_t CH4_RoVal(uint32_t (*filter)(uint32_t),uint32_t *buf);
uint32_t CH4_RsVal(uint32_t (*filter)(uint32_t),uint32_t *buf);
uint32_t CH4_GetVol(uint32_t (*filter)(uint32_t),uint32_t *buf);
uint32_t CH4_CalculatePpmVal(uint32_t (*filter)(uint32_t),uint32_t *buf);
#endif
