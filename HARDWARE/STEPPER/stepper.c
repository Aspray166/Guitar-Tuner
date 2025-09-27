#include "stepper.h"

u8 phasecw[8] = {0x08, 0x0c, 0x04, 0x06, 0x02, 0x03, 0x01, 0x09}; //正转 电机导通相序  D-DC-C-CB-B-BA-A-AD
u8 phaseccw[8] = {0x01, 0x03, 0x02, 0x06, 0x04, 0x0c, 0x08, 0x89}; //反转 电机导通相序  A-AB-B-BC-C-CD-D-DA

void stepper_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOB_CLK_ENABLE();			//开启GPIOB时钟
	
    GPIO_Initure.Pin=GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7; //PB0,1
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);     //初始化GPIOB.0和GPIOB.1
	//复位
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4,GPIO_PIN_RESET);	
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,GPIO_PIN_RESET);	
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_6,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_RESET);
}

void stepper(u8 phase[],u8 t)
{
		u8 i=0;
		u8 temp=phase[i];
		for(i=0;i<8;i++)
		{
			temp = phase[i];
			GPIOB->BSRR=(temp|(((~temp)&0x0F)<<16))<<4;
			delay_ms(t);               //延时，不加的话换相太快电机不转
		}
		GPIOB->BSRR=0x00<<16;
}

void stepper_angle(float angle,u8 t)
{
	int i;
	u8*phase;
	if(angle<0)
	{
		angle=-angle;phase=phasecw;
	}
	else
	{
		phase=phaseccw;
	}
	for(i=angle*512/360;i>0;i--)//除法自带取整，512周期步进电机转一周
	{
		stepper(phase,t);
	}
}
