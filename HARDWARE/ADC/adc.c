#include "adc.h"
#include "delay.h"
	

ADC_HandleTypeDef hadc1;//ADC句柄

//初始化ADC
//ch: ADC_channels 
//通道值 0~16取值范围为：ADC_CHANNEL_0~ADC_CHANNEL_16
void MY_ADC_Init(void)
{   ADC_ChannelConfTypeDef ADC1_ChanConf;
	
    hadc1.Instance=ADC1;
    hadc1.Init.ClockPrescaler=ADC_CLOCK_SYNC_PCLK_DIV4;   //4分频，ADCCLK=PCLK2/4=108/4=27MHZ
    hadc1.Init.Resolution=ADC_RESOLUTION_12B;             //12位模式
    hadc1.Init.DataAlign=ADC_DATAALIGN_RIGHT;             //右对齐
    hadc1.Init.ScanConvMode=DISABLE;                      //非扫描模式
    hadc1.Init.EOCSelection=DISABLE;                      //关闭EOC中断
    hadc1.Init.ContinuousConvMode=DISABLE;                //关闭连续转换
    hadc1.Init.NbrOfConversion=1;                         //1个转换在规则序列中 也就是只转换规则序列1 
    hadc1.Init.DiscontinuousConvMode=DISABLE;             //禁止不连续采样模式
    hadc1.Init.NbrOfDiscConversion=0;                     //不连续采样通道数为0
    hadc1.Init.ExternalTrigConv=ADC_EXTERNALTRIGCONV_T3_CC4;       //定时器TIME3通道4
    hadc1.Init.ExternalTrigConvEdge=ADC_EXTERNALTRIGCONVEDGE_RISING;//上升沿触发
    hadc1.Init.DMAContinuousRequests=ENABLE;             //使能DMA请求
    HAL_ADC_Init(&hadc1);                                 //初始化 
	    
    
    ADC1_ChanConf.Channel=ADC_CHANNEL_5;                         //通道
    ADC1_ChanConf.Rank=1;                                       //1个序列
    ADC1_ChanConf.SamplingTime=ADC_SAMPLETIME_3CYCLES;        //采样时间
//    ADC1_ChanConf.Offset=0;                 
    HAL_ADC_ConfigChannel(&hadc1,&ADC1_ChanConf);        //通道配置
	
}

//ADC底层驱动，引脚配置，时钟使能
//此函数会被HAL_ADC_Init()调用
//hadc:ADC句柄
void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_ADC1_CLK_ENABLE();            //使能ADC1时钟
    __HAL_RCC_GPIOA_CLK_ENABLE();			//开启GPIOA时钟
	
    GPIO_Initure.Pin=GPIO_PIN_5;            //PA5
    GPIO_Initure.Mode=GPIO_MODE_ANALOG;     //模拟
    GPIO_Initure.Pull=GPIO_NOPULL;          //不带上下拉
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
	
}

