// stm32f10x_it.c
// �����: Ezhik
// �������: 12.09.2011
// ������� ��� TE

#include "stm32f10x_it.h"
#include "led.h"
#include "helpers.h"

#include "platform_config.h"

#include "serv_led.h"

// ---------------------------------
// ����������� ���������� ������

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
// ���������� ���������� SysTick
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
	
	// ���������� SysTick ��� ������� ���������� (led.c)
#ifdef USE_LEDS
	LED_SysTick_Handler();
#endif
	Delay_SysTickHandler();
}

// ---------------------------------
// ����������� ���������� ���������


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
// ���������� USB High Priority ��� CAN TX
void USB_HP_CAN1_TX_IRQHandler(void)
{
#ifdef USE_USB
	CTR_HP();
#endif
}

// ���������� USB Low Priority ��� CAN RX0
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

// ���������� USB-On-The-Go FS
void OTG_FS_IRQHandler(void)
{
#ifdef USE_USB
	STM32_PCD_OTG_ISR_Handler(); 
#endif
}

#endif //


// ����� ����� �������� ������ ����������� ���������� (�� startup_stm32f10x_xx.s)


// ����� �����