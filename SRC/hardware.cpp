#include "hardware.h"


void HardwareInit(void)
{
	SysTick_Config(SystemCoreClock/1000);	// 1ms config
	Configure_IWDG();
	Configure_GPIO_OUT();
	Configure_GPIO_USART1();
	Configure_DMA1();
	Configure_USART1();
}


void IWDGRefresh(void)
{
	IWDG->KR = IWDG_REFRESH;
}


bool ResetByIWDG(void)
{
	// check if IWDG caused CPU reset
	if((RCC->CSR & RCC_CSR_IWDGRSTF) == RCC_CSR_IWDGRSTF)
	{
		RCC->CSR |= RCC_CSR_RMVF;		// clear reset flag
		return true;
	}
	else
	{
		return false;
	}
}


void Configure_IWDG(void)
{
	RCC->CSR |= RCC_CSR_LSION;

	int timeout=0xFFF;
	while((RCC->CSR & RCC_CSR_LSIRDY) != RCC_CSR_LSIRDY) // wait until it's ready
	{
		if(!(timeout--))
			break;
	}

	IWDG->KR = IWDG_START;				// activate IWDG (not needed if done in option bytes)
	IWDG->KR = IWDG_WRITE_ACCESS;	// enable write access to IWDG registers
	IWDG->PR = IWDG_PR_PR_1 | IWDG_PR_PR_2;	// set prescaler by 256		(40khz/256 = 156,25Hz = 6,4ms)
	IWDG->RLR = IWDG_RELOAD;			// set reload value to have a rollover each 100ms

	timeout=0xFFF;
	while(IWDG->SR)
	{ 
	if(!(timeout--))
			break;
	}

	IWDGRefresh();
}


void Configure_GPIO_OUT(void)
{
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	RCC->AHBENR |= RCC_AHBENR_GPIOBEN;

	GPIOB->MODER = (GPIOB->MODER & ~(GPIO_MODER_MODER1)) | (GPIO_MODER_MODER1_0);

	GPIOA->MODER = (GPIOA->MODER & ~(GPIO_MODER_MODER1 | GPIO_MODER_MODER2)) | (GPIO_MODER_MODER1_0 | GPIO_MODER_MODER2_0);
	GPIOA->MODER = (GPIOA->MODER & ~(GPIO_MODER_MODER3 | GPIO_MODER_MODER4)) | (GPIO_MODER_MODER3_0 | GPIO_MODER_MODER4_0);	
	GPIOA->MODER = (GPIOA->MODER & ~(GPIO_MODER_MODER5 | GPIO_MODER_MODER6)) | (GPIO_MODER_MODER5_0 | GPIO_MODER_MODER6_0);
	GPIOA->MODER = (GPIOA->MODER & ~(GPIO_MODER_MODER7 | GPIO_MODER_MODER8)) | (GPIO_MODER_MODER7_0 | GPIO_MODER_MODER8_0);
}


void Configure_GPIO_USART1(void)
{
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;

	GPIOA->MODER = (GPIOA->MODER & ~(GPIO_MODER_MODER10)) | (GPIO_MODER_MODER10_1);
	GPIOA->AFR[1] = (GPIOA->AFR[1] &~ (GPIO_AFRH_AFRH2))  | (1 << (2 * 4));
}


void Configure_DMA1(void)
{
	RCC->AHBENR |= RCC_AHBENR_DMA1EN;

	DMA1_Channel3->CPAR = (uint32_t)&(USART1->RDR);
	DMA1_Channel3->CMAR = (uint32_t)rxBuffer;
	DMA1_Channel3->CNDTR = rxBuffSize;
	DMA1_Channel3->CCR  = DMA_CCR_CIRC | DMA_CCR_MINC | DMA_CCR_EN;
}


void Configure_USART1(void)
{
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

	USART1->BRR = SystemCoreClock / 9600;
	USART1->CR3 = USART_CR3_DMAR;
	USART1->CR1 = USART_CR1_M0 | USART_CR1_PCE | USART_CR1_RE | USART_CR1_UE;

	int timeout=0xFFF;
	while((USART1->ISR & USART_ISR_TC) != USART_ISR_TC) // polling idle frame Transmission
	{
		if(!(timeout--))
			break;
	}
	USART1->ICR |= USART_ICR_TCCF;	// clear TC flag
	USART1->CR1 |= USART_CR1_TCIE;	// enable TC interrupt
}
