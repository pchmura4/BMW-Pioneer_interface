/**
  ******************************************************************************
  * @file    main.c 
  * @author  Patryk Chmura
  * @version V1.0.0
  * @date    08-December-2015
  * @brief   Pioneer radio interface to BMW OEM steering wheel controls
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
volatile static uint32_t uwTick;
volatile uint16_t mode_count=0, source_count=12000;
volatile bool source_deactivate=false, mode_hold=false;
volatile uint8_t rxBuffer[rxBuffSize];


/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
	Button2F_t DialButton = Btn2F_NOTHING;
	Ignition_t Ignition = Ign_NOTHING;
	uint32_t rxIndex = 0;
	PioneerInterface Radio;
	BMWbus SWheel;
	
	HardwareInit();
	
	if(ResetByIWDG())		source_count = 0;
	else								source_count = 12000;

	// Infinite loop
	for(;;)
	{
		IWDGRefresh();
		
		uint32_t rxWaiting = rxBuffSize - DMA1_Channel3->CNDTR;
		uint32_t bufCnt = rxWaiting - (rxIndex % rxBuffSize);
		bufCnt = bufCnt % rxBuffSize;
		while(bufCnt)
		{
			uint8_t rxByte = rxBuffer[rxIndex % rxBuffSize]; rxIndex++; bufCnt--;
			SWheel.parse(rxByte);
		}
		
		if((SWheel.isAvailable()))
		{
			switch(SWheel.interpret())
			{
				case Next_release:
					if(!mode_count)
					{
						Radio.TuneRt_release();
					}
					break;
					
				case Prev_release:
					if(!mode_count)
					{
						Radio.TuneLt_release();
					}	
					break;
					
				case VolUp_press:
					if(!mode_count)
					{
 						Radio.VolUp_push();
					}
					else
					{
						Radio.BTAnswer();
						Delay(TIME2);
						mode_count = 10;
					}
					break;
					
				case VolDn_press:
					if(!mode_count)
					{
						Radio.VolDn_push();
					}
					else
					{
						Radio.BTReject();
						Delay(TIME2);
						mode_count = 10;
					}
					break;
					
				case Next_press:
					if(!mode_count)
					{
						Radio.TuneRt_press();
					}
					else
					{
						Radio.FolderUp();
						Delay(TIME2);
						mode_count = 10;
					}
					break;
					
				case Prev_press:
					if(!mode_count)
					{
						Radio.TuneLt_press();
					}
					else
					{
						Radio.FolderDn();
						Delay(TIME2);
						mode_count = 10;
					}
					break;
					
				case RT_pushed:
					if(source_deactivate == false)
					{
						if(!mode_count)
						{
							Radio.Source();
						}
						else
						{
							Radio.Mute();
							Delay(TIME2);
							mode_count = 10;
						}
					}
					break;
					
				case Dial_press:
					DialButton = Pressed;
					break;
				
				case Dial_1s:
					DialButton = Pressed1s;
					break;
					
				case Dial_release:
					if(DialButton == Pressed)					mode_hold = false;
					else if(DialButton == Pressed1s)	mode_hold = true;
					DialButton = Btn2F_NOTHING;
					mode_count = (mode_count>0) ? 0 : 3000;	// set alternative mode for 5 seconds, if it's set already - clear it
					break;
				
				case Pos1_Acc:
					Ignition = Pos1Acc;
					source_count = 12000;
					break;
					
				case Pos3_Start:
					Ignition  = Pos3Start;
					break;
					
				case Pos2_On:
					if(Ignition == Pos1Acc)					__NOP();
					else if(Ignition == Pos3Start)	source_count = 12000;
					Ignition = Ign_NOTHING;
					break;
				
				default:
					break;
			}
		}
	}
}


void Delay(uint32_t delay)
{
	uint32_t tickstart = 0;
	tickstart = uwTick;
	while((uwTick - tickstart) < delay);
}


/******************************************************************************/
/*            Cortex-M0 Processor Exceptions Handlers                         */
/******************************************************************************/
extern "C" {
/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
	/* Go to infinite loop when Hard Fault exception occurs */
	while (1)
	{
	}
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */

void SysTick_Handler(void)
{
	uwTick++;
	
	if((mode_count > 0) && (mode_hold == false))	mode_count--;
	
	if(source_count > 0)
	{
		source_count--;		// BMW module sends source command 3 times in exact time windows everytime ignition is turned on, ignoring so it's not triggered
		if((source_count < 11000) && (source_count > 10000))		source_deactivate = true;
		else if((source_count < 7000) && (source_count > 6000))	source_deactivate = true;
		else if((source_count < 2000) && (source_count > 1000)) source_deactivate = true;
		else	source_deactivate = false;
	}
}
} // extern "C"

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
