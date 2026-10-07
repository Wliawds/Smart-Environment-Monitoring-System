#ifndef __LCD_INTERFACE_H
#define __LCD_INTERFACE_H		
#include "stm32f4xx_hal.h"

#include "main.h"
#include "AT24CXX.h"

#define  SENSOR_COUNT  6

typedef union{
	struct
	{
		uint8_t warn_temp : 1;
		uint8_t warn_humi : 1;
		uint8_t warn_pm25 : 1;
		uint8_t warn_ch2o : 1;
		uint8_t warn_co2  : 1;
		uint8_t warn_ch4  : 1;
		uint8_t reserved  : 2;
	}bits;
	uint8_t byte_value;
}warn_dev;

typedef struct{
		uint16_t x1,y1;
		uint16_t x2,y2;
		const unsigned char *img;//第一页图像
}UI_img;
typedef struct {
    const UI_img* img_array;
    int img_count;
} Page_imgInfo;


typedef struct{
		uint16_t x,y;
		uint16_t width;
		uint16_t height;
		uint8_t  font_size;
		const char *text;//第一页图像
}UI_Text;

typedef struct {
    const UI_Text* text_array;
    int text_count;
} Page_textInfo;

// 传感器阀值结构体
typedef struct {
    int32_t min_value;    // 最大
    int32_t max_value;    // 最小
    uint8_t level;        // 对应等级
} ThresholdRange;

// 传感器类型
typedef enum {
    SENSOR_TEMP = 1,
    SENSOR_HUMI,
    SENSOR_PM25,
    SENSOR_CH2O,
    SENSOR_CO2,
    SENSOR_CH4
} SensorType;

extern Page_textInfo T_pages[] ;
extern Page_imgInfo  I_pages[] ;

void Page_draw_all_img	(const Page_imgInfo* page) ;
void Page_draw_all_text(const Page_textInfo* page);

void LCD_DisplayValue(int sensor_idx, int is_interface, int32_t val);
// 保持原函数接口（调用统一函数）
#define LCD_TempSet(val)    	LCD_DisplayValue(0, 0, val)
#define LCD_HumiSet(val)   		LCD_DisplayValue(1, 0, val)
#define LCD_PM25Set(val)     	LCD_DisplayValue(2, 0, val)
#define LCD_CH2OSet(val)      	LCD_DisplayValue(3, 0, val)
#define LCD_CO2Set(val)     	LCD_DisplayValue(4, 0, val)
#define LCD_CH4Set(val)      	LCD_DisplayValue(5, 0, val)

#define LCD_TempInterface(val)  LCD_DisplayValue(0, 1, val)
#define LCD_HumiInterface(val)	LCD_DisplayValue(1, 1, val)
#define LCD_PM25Interface(val)	LCD_DisplayValue(2, 1, val)
#define LCD_CH2OInterface(val)	LCD_DisplayValue(3, 1, val)
#define LCD_CO2Interface(val) 	LCD_DisplayValue(4, 1, val)
#define LCD_CH4Interface(val)  	LCD_DisplayValue(5, 1, val)
void Sensor_DegreeCalibrate(uint8_t item, RealTime_Data *senddata,uint8_t force_refresh);















void LCD_Splash_Screen(void);

void LCD_LevelSwitch(uint8_t level,uint8_t force_refresh);
void LCD_Select_Category(uint8_t select, uint8_t force_refresh);
void Page2_ParChoose(uint8_t item);


void Alarm_Pic(uint8_t val);
uint8_t Data_Compare(RealTime_Data* redat,set_sensordata* sedat);

#endif  
	 
	 



