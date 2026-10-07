#include "CH4_BSP.h"
#include "MYDELAY.h"
#include <math.h>
#include "stdio.h"

extern ADC_HandleTypeDef hadc1;

extern uint32_t  CH4_Ro;
uint32_t Get_CH4Val(uint32_t *buf)
{
	uint32_t ch4_code;
	HAL_ADC_Stop_DMA(&hadc1);//先关后开

	HAL_ADC_Start_DMA(&hadc1, buf, 2);
	ch4_code = buf[1];
	
	return  ch4_code;
}	


uint32_t CH4_RsVal(uint32_t (*filter)(uint32_t),uint32_t *buf)
{
	uint32_t adc_val,filter_val;
	uint32_t ch4_rs_val;	
	adc_val = Get_CH4Val(buf);
	filter_val = filter(adc_val) * 2;
	ch4_rs_val = (5000.f-((double)filter_val*3300.f/4096.f))/(((double)filter_val*3300.f/4096.f)/1000.f);

	return ch4_rs_val;
}
uint32_t CH4_RoVal(uint32_t (*filter)(uint32_t),uint32_t *buf)
{
	uint32_t adc_ro_code,ch4_ro_ca_val;
	uint32_t adc_val,filter_val;
	uint8_t s_count=0,f_count=0,t_count=0;
	while(1)
	{
		do{
				adc_val = Get_CH4Val(buf);
				filter_val = filter(adc_val);
				adc_ro_code = filter_val * 2;
				printf("adc_ro_code = %d\n",adc_ro_code);
				t_count ++;
				if(t_count > 80){t_count = 0;break;}
				USR_Delay_ms(100);
			}while(adc_ro_code < 50);
		if(adc_ro_code >=80 && adc_ro_code <= 100)
		{
			s_count ++;
			printf("success count = %d\n",s_count);
			if(s_count >= 10)
			{

				printf("Sampling completed.adc_ro_code = %d\n",adc_ro_code);
				
				ch4_ro_ca_val = (5000.f-((double)adc_ro_code*3300.f/4096.f))/(((double)adc_ro_code*3300.f/4096.f)/1000.f);
				return ch4_ro_ca_val;
			}
		}
		else
		{
			s_count = 0;
			f_count ++;
			if(f_count > 5){break;}
			printf("Counting failed, starting to resample.=%d\n",f_count);
		}
	}
	printf("Sampling failure,adc code using constant value.=%d\n",RO_CONSTANT);
	
	ch4_ro_ca_val = (5000.f-((double)RO_CONSTANT*3300.f/4096.f))/(((double)RO_CONSTANT*3300.f/4096.f)/1000.f);
	return ch4_ro_ca_val;
}
double calculate_ppm(double rs_ro_ratio)
{
    double a = 17.5183;
    double b = -1.75647;
    if (rs_ro_ratio <= 0) 
		{
        return 0.0;  
    }
    
    double ppm = a * pow(rs_ro_ratio, b);
    return ppm;
}
uint32_t CH4_CalculatePpmVal(uint32_t (*filter)(uint32_t),uint32_t *buf)
{
	uint32_t ch4_rs;
	double ch4_rsro;
	ch4_rs = CH4_RsVal(filter,buf);
	ch4_rsro = (double)ch4_rs/(double)CH4_Ro;
	printf("Ratio of CH4 RS to RO.=%f\n",ch4_rsro);
	
	
	return (uint32_t)calculate_ppm(ch4_rsro);
}





