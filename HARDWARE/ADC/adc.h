#ifndef __ADC_H
#define __ADC_H
#include "sys.h"



extern ADC_HandleTypeDef hadc1;//ADC句柄

void MY_ADC_Init(void); 				//ADC通道初始化
void adc_start(u32 ch);
u16  Get_Adc(void); 		        //获得某个通道值 
u16 Get_Adc_Average(u32 ch,u8 times);//得到某个通道给定次数采样的平均值
#endif 
