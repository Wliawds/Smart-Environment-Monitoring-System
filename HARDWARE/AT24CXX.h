#ifndef __AT24CXX_H
#define __AT24CXX_H


#include "stm32f4xx_hal.h"

#define EEPROM_I2C_ADDR    0xA0  
#define EEPROM_TIMEOUT     1000  

#pragma pack(push,1)

typedef struct
{
	uint8_t   sta;		//1字节
	int8_t 		temp;  	//1字节
	int8_t 		humi;		//1字节
	int16_t 	pm25;		//2字节
	int32_t  	ch2o;		//4字节
	int32_t		co2;		//4字节
	int32_t  	ch4;		//4字节

}set_sensordata;		//1+1+1+2+4+4+4=17字节



#pragma pack(pop)

uint8_t Eeprom_WriteData(uint16_t pos,set_sensordata *senddata);
uint8_t Eeprom_ReadData(uint16_t pos,set_sensordata *readdata);
#endif






