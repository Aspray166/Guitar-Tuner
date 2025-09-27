#include "data.h"
#include "math.h"
#include "arm_math.h"



extern arm_cfft_radix4_instance_f32 scfft;
/** FFT测试函数，生成正弦信号测试准确度
  */ 
void FFT_test()
{	
	u16 i;
	float t;

	for(i=0;i<ADC1_BUFFER_SIZE+3;i++)//生成信号序列
	{
		 t=(float)i/(108000000/arr/psc);
		 fft_test[i]=2*arm_sin_f32(2*3.14*2000*t)+arm_sin_f32(2*3.14*1000*t);
	}
	for(i=0;i<ADC1_BUFFER_SIZE;i++)
	{
		 fft_inputbuf[2*i]=(float)(fft_test[i]+fft_test[i+1]+fft_test[i+2]+fft_test[i+3])/4;
		 fft_inputbuf[2*i+1]=0;//虚部全部为0
	}
	arm_cfft_radix4_f32(&scfft,fft_inputbuf);	//FFT计算（基4）
	arm_cmplx_mag_f32(fft_inputbuf,fft_outputbuf,ADC1_BUFFER_SIZE);	//把运算结果复数求模得幅值
	arm_std_f32	(&fft_outputbuf[0]+1,ADC1_BUFFER_SIZE-1,&std);
	arm_max_f32(&fft_outputbuf[0]+1,ADC1_BUFFER_SIZE-1,&max_result,&max_index);
	f=max_index*(float)(108000000/arr/psc)/ADC1_BUFFER_SIZE;
}

/** 对adc_trans[]数组的采样值进行FFT及后续处理
  */ 
void FFT_ADsampling()
{
	u16 i=0;
	float a=0.5;
	fft_inputbuf[2*i]=(float)(adc_trans[i]+adc_trans[i+1]+adc_trans[i+2]+adc_trans[i+3])/4;
	fft_inputbuf[2*i+1]=0;
	for(i=1;i<ADC1_BUFFER_SIZE;i++)
	{
		 fft_inputbuf[2*i]=(float)a*(adc_trans[i]+adc_trans[i+1]+adc_trans[i+2]+adc_trans[i+3])/4+(1-a)*fft_inputbuf[2*i-2];
		 fft_inputbuf[2*i+1]=0;//虚部全部为0
	}
	arm_cfft_radix4_f32(&scfft,fft_inputbuf);	//FFT计算（基4）
	arm_cmplx_mag_f32(fft_inputbuf,fft_outputbuf,ADC1_BUFFER_SIZE);	//把运算结果复数求模得幅值
	f=(float)(108000000/arr/psc)/ADC1_BUFFER_SIZE;
	arm_std_f32	(&fft_outputbuf[0]+1,(u32)350/f,&std);
	arm_max_f32(&fft_outputbuf[0]+1,(u32)350/f,&max_result,&max_index);
	f=max_index*f;
	fn=12*log(440/f)/log(2);

}
/**
 @float fn 当前的频率值
float 和 int（即枚举）运算精度要求不高不影响，会都转换为double类型后比较
*/
String string_identify(double fn,float std)
{
if(std<=300) //标准差太小，认为没有有效声音
	return NONE;
if((fn>=3.5)&&(fn<=6.5))
	return E1;
else if((fn>=8.5)&&(fn<=11.5))
	return B;
else if((fn>=12.5)&&(fn<=15.5))
	return G;
else if((fn>=17.5)&&(fn<=20.5))
	return D;
else if((fn>=22.5)&&(fn<=25.5))
	return A;
else if((fn>=27.5)&&(fn<=30.5))
	return E;
else
	return NONE;
}
String string_select(u8 mode)
{
if(mode==1)
	return E1;
else if(mode==2)
	return B;
else if(mode==3)
	return G;
else if(mode==4)
	return D;
else if(mode==5)
	return A;
else if(mode==6)
	return E;
else
	return ERR;
}


void reset_pid(inc_pid *pid,float kp,float ki,float kd)
{
	pid->kp = kp;
	pid->ki = ki;
	pid->kd = kd;
	pid->error = 0;
	pid->lastError = 0;
	pid->dError = 0 ;
	pid->ddError = 0;
	pid->dError_last = 0;
	pid->output = 0 ;
	pid->output_last = 0;
}

float increment_pid(float error,inc_pid *pid)//增量式PID
{
	pid->error = error;
	pid->dError = error - pid->lastError;
	pid->ddError = pid->dError - pid->dError_last;
	pid->lastError = error;
	pid->dError_last = pid->dError;
//	pid->output =(pid->kp*pid->dError)+(pid->ki*error)+(pid->kd*pid->ddError) + pid->output_last; 
	pid->output =(pid->kp*pid->dError)+(pid->ki*error)+(pid->kd*pid->ddError);
	pid->output_last = pid->output;	
   // int32_t pidoutput= (int32_t)pid->output;
	return pid->output;
}
/**
 @String s 目标琴弦
 @float f 当前的频率值
*/
float angle_output(inc_pid *pid,String S,float f)
{
	float error=0,angle=0;
	switch(S){
		case NONE:
			error=0;break;
		case E1:
			error=E1-f;break;
		case B:
			error=B-f;break;
		case G:
			error=G-f;break;
		case D:
			error=D-f;break;
		case A:
			error=A-f;break;
		case E:
			error=E-f;break;
		default:
			error=0;break;
	}
	angle=-increment_pid(error,pid);
	return angle;
}




