#ifndef __DATA_H
#define __DATA_H
#include "sys.h"


typedef enum{
	NONE=0, //没有距离很近的空弦音
	E1=330,
	B=247,
	G=197,
	D=147,
	A=110,
	E=82,
	ERR=-1 //手动时补充string_select的if-else用，不会出现
}String;
typedef struct{
	float kp ;		//比例系数
	float ki ;		//积分系数
	float kd ;	    //微分系数
	float error;    //误差值
	float lastError;	//上一个误差值
	float dError;        //上次误差和这次误差的变化值
	float ddError;       //上次误差变化值和这次误差的变化值的变化值
	float dError_last;   //上上次误差和上次次误差的变化值
	float output;         //输出值
	float output_last;    //上次的输出值
	float output_new;
}inc_pid;

#define ADC1_BUFFER_SIZE		4096 		//FFT长度
extern u8 mode,time;

extern uint16_t adc_trans[ADC1_BUFFER_SIZE+3];
extern float fft_test[ADC1_BUFFER_SIZE+3];

extern u16 arr,psc;
extern u32 max_index;
extern float f,std,max_result;
extern float fft_inputbuf[ADC1_BUFFER_SIZE*2];
extern float fft_outputbuf[ADC1_BUFFER_SIZE];

extern float angle;
extern double fn;
extern float kp,ki,kd;
extern String S_last,S;
extern inc_pid pid_i;

void FFT_test(void);
void FFT_ADsampling(void);
String string_identify(double fn,float std);
String string_select(u8 mode);
void reset_pid(inc_pid *pid,float kp,float ki,float kd);
float increment_pid(float error,inc_pid *pid);
float angle_output(inc_pid *pid,String S,float f);
#endif

