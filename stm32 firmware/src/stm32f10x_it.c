// stm32f10x_it.c
// Автор: Ezhik
// Создано: 12.09.2011
// Вариант для TE

#include "stm32f10x_it.h"
#include "led.h"
#include "helpers.h"

#include "platform_config.h"

#include "serv_led.h"

// ---------------------------------
// Обработчики прерываний разные

void NMI_Handler(void)
{

}
void HardFault_Handler(void)
{		  
  /* Go to infinite loop when Hard Fault exception occurs */
  printf("Hard fault!\r\n");
  while (1)
  {
  }
}

void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  printf("MemManage fault!\r\n");
  while (1)
  {
  }
}

void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  printf("Bus fault!\r\n");
  while (1)
  {
  }
}

void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  printf("Usage fault!\r\n");
  while (1)
  {
  }
}

void SVC_Handler(void)
{
}

void DebugMon_Handler(void)
{
}

void PendSV_Handler(void)
{
}

// ---------------------------------
// Обработчик прерывания SysTick
uint16_t systick_prescale_count = 0;
uint16_t Logic_prescale_count = 0;


void SysTick_Handler(void)
{
	systick_prescale_count++;
	if(systick_prescale_count < SYSTICK_PRESCALE)
		return;
	systick_prescale_count = 0;

	Logic_prescale_count++;
	if( Logic_prescale_count > 10 ) /// 10 mS period
	{
		UpdateTimings();
		Logic_prescale_count = 0;
	}
	
	// Обработчик SysTick для мигания светодиода (led.c)
#ifdef USE_LEDS
	LED_SysTick_Handler();
#endif
	Delay_SysTickHandler();
}

// ---------------------------------
// Обработчики прерывания периферии


void TIM3_IRQHandler(void)
{
  if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
  {    
    TIM_ClearITPendingBit(TIM3, TIM_IT_Update);    
    //PwmFanStep( 1 );
  }
}


void TIM4_IRQHandler(void)
{
  if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
  {    
    TIM_ClearITPendingBit(TIM4, TIM_IT_Update);    
    //PwmLedStep( 1 );
    PwmStep( 1 );		
  }
}


#ifndef STM32F10X_CL
// Прерывание USB High Priority или CAN TX
void USB_HP_CAN1_TX_IRQHandler(void)
{
#ifdef USE_USB
	CTR_HP();
#endif
}

// Прерывание USB Low Priority или CAN RX0
void USB_LP_CAN1_RX0_IRQHandler(void)
{
#ifdef USE_USB
	USB_Istr();
#endif
}

#endif // STM32F10X_CL


#ifdef STM32F10X_HD

void SDIO_IRQHandler(void)
{ 
  
 
}
#endif // STM32F10X_HD 

#ifdef STM32F10X_CL

// Прерывание USB-On-The-Go FS
void OTG_FS_IRQHandler(void)
{
#ifdef USE_USB
	STM32_PCD_OTG_ISR_Handler(); 
#endif
}

#endif //


// Далее можно добавить другие обработчики прерываний (см startup_stm32f10x_xx.s)


// Конец файла