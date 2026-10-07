#include "FILTER.h"



#define WINDOW_SIZE 8

// 滑动平均滤波
uint32_t Moving_Average_Filter(uint32_t new_sample) 
{
    static uint32_t buffer[WINDOW_SIZE] = {0};
    static uint8_t write_index = 0;
    static uint32_t sum = 0;
    
    sum -= buffer[write_index];      // 减去要覆盖的值
    sum += new_sample;               // 加上新值
    buffer[write_index] = new_sample; // 存储新值
    
    write_index = (write_index + 1) % WINDOW_SIZE;
    
    return (uint32_t)(sum / WINDOW_SIZE);
}



#define COMPOSITE_SIZE 5

// 复合滤波：先中值后均值
uint32_t Composite_Filter(uint32_t new_sample) {
    static uint32_t median_samples[COMPOSITE_SIZE] = {0};
    static uint8_t median_index = 0;
    uint32_t temp[COMPOSITE_SIZE];
    
    // 中值滤波部分
    median_samples[median_index] = new_sample;
    median_index = (median_index + 1) % COMPOSITE_SIZE;
    
    // 复制并排序
    for(uint8_t i = 0; i < COMPOSITE_SIZE; i++) {
        temp[i] = median_samples[i];
    }
    
    // 排序
    for(uint8_t i = 0; i < COMPOSITE_SIZE - 1; i++) {
        for(uint8_t j = 0; j < COMPOSITE_SIZE - 1 - i; j++) {
            if(temp[j] > temp[j + 1]) {
                uint16_t swap = temp[j];
                temp[j] = temp[j + 1];
                temp[j + 1] = swap;
            }
        }
    }
    
    // 去掉最大最小值后求平均
    uint32_t sum = 0;
    for(uint8_t i = 1; i < COMPOSITE_SIZE - 1; i++) {
        sum += temp[i];
    }
    
    return (uint32_t)(sum / (COMPOSITE_SIZE - 2));
}








