#ifndef __MAIN_H__
#define __MAIN_H__


#include "stm32f0xx.h"
#include "hardware.h"
#include "PioneerInterface.h"
#include "BMWbus.h"


#define IWDG_REFRESH      (uint32_t)(0x0000AAAA)
#define IWDG_WRITE_ACCESS (uint32_t)(0x00005555)
#define IWDG_START        (uint32_t)(0x0000CCCC)
#define IWDG_RELOAD       (469)				// max 2047, RLR has 11-bits -> (1/(40000/256))*469 = 3s


#define rxBuffSize	64


#define TIME	80
#define TIME2	500


#define RING_ON() 	GPIOA->BSRR = GPIO_ODR_1	// PA1
#define RING_OFF()	GPIOA->BRR  = GPIO_ODR_1	// PA1

#define R24k	GPIO_ODR_2	// PA2
#define R16k	GPIO_ODR_3	// PA3
#define R12k	GPIO_ODR_7	// PA7
#define R8k		GPIO_ODR_5	// PA5
#define R5k5	GPIO_ODR_6	// PA6				16k || 8k = 5.33k
#define R1k2	GPIO_ODR_1	// PB1

#define R24k_ON()		GPIOA->BSRR = R24k
#define R24k_OFF()	GPIOA->BRR  = R24k
#define R16k_ON()		GPIOA->BSRR = R16k
#define R16k_OFF()	GPIOA->BRR  = R16k
#define R12k_ON()		GPIOA->BSRR = R12k
#define R12k_OFF()	GPIOA->BRR  = R12k
#define R8k_ON()		GPIOA->BSRR = R8k
#define R8k_OFF()		GPIOA->BRR  = R8k
#define R5k5_ON()		GPIOA->BSRR = R5k5
#define R5k5_OFF()	GPIOA->BRR  = R5k5
#define R1k2_ON()		GPIOB->BSRR = R1k2
#define R1k2_OFF()	GPIOB->BRR  = R1k2


enum Button2F_t
{
	Btn2F_NOTHING,
	Pressed,
	Pressed1s
};

enum Ignition_t
{
	Ign_NOTHING,
	Pos1Acc,
	Pos3Start
};


void Delay(uint32_t);

extern volatile uint8_t rxBuffer[rxBuffSize];


#endif
