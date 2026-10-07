#ifndef __MY_IIC_S_H
#define __MY_IIC_S_H

#include "stm32f4xx_hal.h"

#include "main.h"



#define S_IIC_SDA_0 		HAL_GPIO_WritePin(IIC_SDA_PIN_GPIO_Port, IIC_SDA_PIN_Pin, GPIO_PIN_RESET)
#define S_IIC_SDA_1 		HAL_GPIO_WritePin(IIC_SDA_PIN_GPIO_Port, IIC_SDA_PIN_Pin, GPIO_PIN_SET)
#define S_IIC_SCL_0 		HAL_GPIO_WritePin(IIC_SCL_PIN_GPIO_Port, IIC_SCL_PIN_Pin, GPIO_PIN_RESET)
#define S_IIC_SCL_1 		HAL_GPIO_WritePin(IIC_SCL_PIN_GPIO_Port, IIC_SCL_PIN_Pin, GPIO_PIN_SET)

#define S_IIC_SDA_R			HAL_GPIO_ReadPin(IIC_SDA_PIN_GPIO_Port,IIC_SDA_PIN_Pin)


void S_IIC_SDA_OUT(void);
void S_IIC_SDA_IN(void);
void S_IIC_Start(void);
void S_IIC_Stop(void);
void S_IIC_SendByte(uint8_t val);
void S_IIC_SendAck(uint8_t val);

uint8_t S_IIC_RecAck(void);
uint8_t S_IIC_ReadByte(uint8_t ack);









#endif
