#ifndef __MY_IIC_T_H
#define __MY_IIC_T_H
#include "stm32f4xx_hal.h"    
#include "main.h"  



#define T_IIC_SCL_1     HAL_GPIO_WritePin(GPIOG, GPIO_PIN_7, GPIO_PIN_SET) //SCL
#define T_IIC_SCL_0     HAL_GPIO_WritePin(GPIOG, GPIO_PIN_7, GPIO_PIN_RESET) //SCL
#define T_IIC_SDA_1     HAL_GPIO_WritePin(GPIOI, GPIO_PIN_3, GPIO_PIN_SET) //SCL
#define T_IIC_SDA_0     HAL_GPIO_WritePin(GPIOI, GPIO_PIN_3, GPIO_PIN_RESET) //SCL

#define T_IIC_SDA_R   		HAL_GPIO_ReadPin(GPIOI,GPIO_PIN_3)   //输入SDA 

//IIC所有操作函数
void T_IIC_GpioInit(void);  	//初始化IIC的IO口				 
void T_IIC_Start(void);				//发送IIC开始信号
void T_IIC_Stop(void);	  		//发送IIC停止信号
void T_IIC_SendByte(uint8_t txd);			//IIC发送一个字节
uint8_t T_IIC_ReadByte(unsigned char ack);	//IIC读取一个字节
uint8_t T_IIC_RecAck(void); 					//IIC等待ACK信号


#endif







