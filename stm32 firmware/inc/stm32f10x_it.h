// stm32f10x_it.h
// Автор: Ezhik
// Создано: 12.09.2011
// Вариант для TE

#ifndef _STM32F10X_IT_H
#define _STM32F10X_IT_H

#include "stm32f10x.h"
#include "platform_config.h"


void NMI_Handler(void);

void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);

void SVC_Handler(void);
void DebugMon_Handler(void);
void PendSV_Handler(void);

void SysTick_Handler(void);
void TIM3_IRQHandler(void);
void TIM4_IRQHandler(void);

#ifdef USART1_USE_INTERRUPT_HANDLER
void USART1_IRQHandler(void);
#endif 

#ifdef USART2_USE_INTERRUPT_HANDLER
void USART2_IRQHandler(void);
#endif 

#ifdef USART3_USE_INTERRUPT_HANDLER
void USART3_IRQHandler(void);
#endif 
					 
#endif //	_STM32F10X_IT_H


