#ifndef __DMA_H
#define __DMA_H
#include "sys.h"



extern DMA_HandleTypeDef  hdma_adc1;      //DMA¾ä±ú


void DMA_CLK(void);
void DMA_Init(void);
void DMA2_Stream0_IRQHandler(void);
void send(void);
 
#endif
