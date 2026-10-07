#include "AT24CXX.h"
#include "i2c.h"
#include "MYDELAY.h"

extern I2C_HandleTypeDef hi2c1;

uint8_t Eeprom_WriteData(uint16_t pos,set_sensordata *senddata)
{
	HAL_StatusTypeDef status =  HAL_I2C_Mem_Write(		 &hi2c1, 
																						EEPROM_I2C_ADDR, 
																											  pos, 
																			I2C_MEMADD_SIZE_16BIT,
																				 (uint8_t*)senddata, 
																												 17, 
																					  EEPROM_TIMEOUT);
	HAL_Delay(5);
	return status;
}
uint8_t Eeprom_ReadData(uint16_t pos,set_sensordata *readdata)
{
	HAL_StatusTypeDef status =  HAL_I2C_Mem_Read (		 &hi2c1, 
																						EEPROM_I2C_ADDR, 
																											  pos, 
																			I2C_MEMADD_SIZE_16BIT,
																				 (uint8_t*)readdata, 
																												 17, 
																					  EEPROM_TIMEOUT);
	return status;
}
