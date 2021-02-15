#ifndef __HARDWARE_H__
#define __HARDWARE_H__


#include "main.h"


void HardwareInit(void);
bool ResetByIWDG(void);
void IWDGRefresh(void);
void Configure_IWDG(void);
void Configure_GPIO_OUT(void);
void Configure_GPIO_LED(void);
void Configure_GPIO_USART1(void);
void Configure_GPIO_USART3(void);
void Configure_DMA1(void);
void Configure_USART1(void);
void Configure_USART3(void);


#endif
