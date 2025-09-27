#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "stepper.h"
#include "key.h"
#include "adc.h"
#include "timer.h"
#include "dma.h"
#include "exti.h"
#include "data.h"
#include "math.h"
#include "arm_math.h"
/************************************************
吉他自动调弦装置
天津大学2019级自动化四班
李家丞 郭天烁 
微机原理课程设计自拟题目
************************************************/
/***********************************************
吉他六根弦的标准音分别是：⑥弦一E，⑤弦一A，④弦一d，③弦一g，②弦一b，①弦一E，也就是简谱C调的3（Mi）、6（La）、2（Re）、5（Sol）、7（Si）、3（Mi）。

在左手不按任何弦的情况下，直接用右手拨动琴弦，发出的音叫做“空弦音”。所以在C调的情况下，如果使用音名来表示每一个琴弦的空弦音高，那么一把吉他的标准的音高从粗弦到细弦，分别是E、A、D、G、B、E。

分别是：

1弦空弦，音高e1，频率答f＝440．0000／2＾（5／12）＝329．6276Hz

2弦空弦，音高b，频率f＝440．0000／2＾（10／12）＝246．9417Hz

3弦空弦，音高g，频率f＝440．0000／2＾（14／12）＝195．9977Hz

4弦空弦，音高d，频率f＝440．0000／2＾（19／12）＝146．8324Hz

5弦空弦，音高A，频率f＝440．0000／2＾（24／12）＝110．0000Hz

6弦空弦，音高E，频率f＝440．0000／2＾（29／12）＝82．4069Hz

在data.h中定义
********************************************************/

u8 mode=0;//当前状态
u8 time=0;//有效识别次数

u16 arr=200,psc=108;//定时器计数值和分频
u32 max_index;//fft_output中最大幅值所在的索引
//频率值，fft_output的标准差和最大幅值
float f=0,std,max_result;

uint16_t adc_trans[ADC1_BUFFER_SIZE+3];//dma存储的数组
float fft_test[ADC1_BUFFER_SIZE+3];//测试FFT的数组
float fft_inputbuf[ADC1_BUFFER_SIZE*2];	//FFT输入数组
float fft_outputbuf[ADC1_BUFFER_SIZE];	//FFT输出数组

float angle;//步进电机旋转角度
double fn;//f转为用十二平均律表示的结果
float kp=10,ki=1,kd=0.1;//PID参数值
String S_last,S;//琴弦
inc_pid pid_i;//pid相关参数的结构体
arm_cfft_radix4_instance_f32 scfft;
int main(void)
{ 
    HAL_Init();				        //初始化HAL库
    Stm32_Clock_Init(432,25,2,9);   //设置时钟,216Mhz 
    delay_init(216);                //延时初始化
	uart_init(115200);		        //串口初始化
    stepper_Init();   
//	KEY_Init();
	EXTI_Init();                    //外部中断初始化
	DMA_CLK();	
	TIM3_PWM_Init(arr-1,psc-1);     //PWM频率为108M/arr/psc
//	TIM4_PWM_Init(399,5399);     //驱动舵机用的，不需要可以注释
//	TIM_SetTIM4Compare2(380);	//修改比较值，修改占空比	
	DMA_Init();
	MY_ADC_Init();                  //初始化ADC1通道5
	
	reset_pid(&pid_i,kp,ki,kd);
	
	arm_cfft_radix4_init_f32(&scfft,ADC1_BUFFER_SIZE,0,1);//初始化scfft结构体，设定FFT相关参数
	HAL_ADC_Start(&hadc1);
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adc_trans, ADC1_BUFFER_SIZE+3);

	while(1)
	{
		stepper_angle(angle,1);
				
	}
 }	




