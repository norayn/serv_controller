// hw_config.h
// Автор: Ezhik
// Создано: 12.09.2011
// Использован шаблон от MCD Application Team (thx very much!)
// Вариант для TE

#ifndef __HW_CONFIG_H
#define __HW_CONFIG_H


#include "stm32f10x.h"


// ----------
// RCC

void RCC_Config(void);


// ----------
// USB


#ifdef USE_USB
// $USBCONFIG - Максимальный размер USB Bulk-пакета
#define BULK_MAX_PACKET_SIZE  0x00000040

void Get_SerialNum(void);
void Enter_LowPowerMode(void);
void Leave_LowPowerMode(void);

void Set_USBClock(void);
void USB_Interrupts_Config(void);
void USB_Cable_Config (FunctionalState NewState);

void RHID_Send(uint8_t report, uint8_t state);
uint8_t RHIDCheckState(void);

#ifdef USB_DISCONNECT
void USB_Disconnect_Config(void);
#endif 

#endif // USE_USB


// ----------
// LED

#ifdef USE_LEDS
void Led_Config(void);
#endif


// ----------
// COM

#ifdef USE_COM 
void InitCOM();
void COM_SendData(USART_TypeDef* USARTx, uint8_t *data, int length);
#endif // USE_COM

#endif // __HW_CONFIG_H