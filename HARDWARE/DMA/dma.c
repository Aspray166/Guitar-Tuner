#include "dma.h"
#include "adc.h"
#include "delay.h"
#include "data.h"
#include "math.h"
#include "arm_math.h"



DMA_HandleTypeDef  hdma_adc1;      //DMA句柄



void DMA_CLK(void)
{ 
	__HAL_RCC_DMA2_CLK_ENABLE();//DMA2时钟使能	
	HAL_NVIC_SetPriority(DMA2_Stream0_IRQn,0,0);//dma中断优先级
	HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);//使能中断
}

void DMA_Init(void)
{    
    
    //Tx DMA配置
    hdma_adc1.Instance=DMA2_Stream0;                            //数据流选择
    hdma_adc1.Init.Channel=DMA_CHANNEL_0;                      //通道选择
    hdma_adc1.Init.Direction=DMA_PERIPH_TO_MEMORY;             //外设到存储器
    hdma_adc1.Init.PeriphInc=DMA_PINC_DISABLE;                 //外设非增量模式
    hdma_adc1.Init.MemInc=DMA_MINC_ENABLE;                     //存储器增量模式
    hdma_adc1.Init.PeriphDataAlignment=DMA_PDATAALIGN_HALFWORD;    //外设数据长度:16位
    hdma_adc1.Init.MemDataAlignment=DMA_MDATAALIGN_HALFWORD;       //存储器数据长度:16位
    hdma_adc1.Init.Mode=DMA_CIRCULAR;                            //循环模式
    hdma_adc1.Init.Priority=DMA_PRIORITY_MEDIUM;               //中等优先级
    hdma_adc1.Init.FIFOMode=DMA_FIFOMODE_DISABLE;             //不使能FIFO
    hdma_adc1.Init.FIFOThreshold=DMA_FIFO_THRESHOLD_FULL;      
    hdma_adc1.Init.MemBurst=DMA_MBURST_SINGLE;                 //存储器突发单次传输
    hdma_adc1.Init.PeriphBurst=DMA_PBURST_SINGLE;              //外设突发单次传输
    
    HAL_DMA_DeInit(&hdma_adc1);   
    HAL_DMA_Init(&hdma_adc1);
	__HAL_LINKDMA(&hadc1,DMA_Handle,hdma_adc1);    //将DMA与ADC1联系起来(发送DMA)
} 


void DMA2_Stream0_IRQHandler(void)
{	
if(__HAL_DMA_GET_FLAG(&hdma_adc1,DMA_FLAG_TCIF0_4))
{
	FFT_ADsampling();//见data.c
	if(mode==0)
		S=string_identify(fn,std);
	else
		S=string_select(mode);
	
	if((mode==0)&&(S!=NONE)&&(S==S_last))
		time++;
	else if((mode!=0)&&(S==S_last)&&(S!=string_identify(fn,std)))
		time++;
	else
		time=0;

		S_last=S;
	
	angle=angle_output(&pid_i,S,f);
	send();
	
}
	HAL_DMA_IRQHandler(&hdma_adc1);//dma中断处理函数
}

void send(void)
{
//		printf("频率值%f\r\n",f);
//		printf("标准差为：%f\r\n",std);
//		printf("%f\r\n",angle);
//		printf("%i\r\n",S);
//		printf("%i",time);
	if(mode==0){
		printf("当前模式为：自动 ");
		if(time>=1)
			printf("识别出的琴弦为%i弦\r\n",(S==E1)?1:(S==B)?2:(S==G)?3:(S==D)?4:(S==A)?5:6);
		else
			printf("\r\n");
	}
	
	else{
		printf("当前模式为：手动 调%i弦\r\n",mode);
		if(time>=15)
			printf("多次调整仍未达到目标空弦音，请检查设定是否正确\r\n");
	}
	printf("需要调整的角度为%f\r\n",angle);
}

 
 
