// usb_conf.h
// јвтор: Ezhik
// —оздано: 12.09.2011
// »спользован шаблон от MCD Application Team (thx very much!)

#ifndef __USB_DESC_H
#define __USB_DESC_H

#include "stm32f10x.h"

// $USBCONFIG - тут надо подправить размеры всех дескрипторов, в случае их изменени€
#define SomeDev_SIZ_DEVICE_DESC         	     18
#define SomeDev_SIZ_CONFIG_DESC          	     32

#define SomeDev_SIZ_STRING_LANGID       	     4
#define SomeDev_SIZ_STRING_VENDOR       	     22
#define SomeDev_SIZ_STRING_PRODUCT       		 38

#if defined STRIVE_BOARD
	#define SomeDev_SIZ_STRING_SERIAL            16
#elif defined EM_STM3210C_BOARD
	#define SomeDev_SIZ_STRING_SERIAL            16
#elif defined TE_STM32F103_BOARD
	#define SomeDev_SIZ_STRING_SERIAL            16
#else
	#define SomeDev_SIZ_STRING_SERIAL            16
#endif

#define SomeDev_SIZ_STRING_INTERFACE        	 20

// Ёкспортируемые функции
extern const uint8_t SomeDev_DeviceDescriptor[SomeDev_SIZ_DEVICE_DESC];
extern const uint8_t SomeDev_ConfigDescriptor[SomeDev_SIZ_CONFIG_DESC];

extern const uint8_t SomeDev_StringLangID[SomeDev_SIZ_STRING_LANGID];
extern const uint8_t SomeDev_StringVendor[SomeDev_SIZ_STRING_VENDOR];
extern const uint8_t SomeDev_StringProduct[SomeDev_SIZ_STRING_PRODUCT];
extern uint8_t SomeDev_StringSerial[SomeDev_SIZ_STRING_SERIAL];
extern const uint8_t SomeDev_StringInterface[SomeDev_SIZ_STRING_INTERFACE];

#endif 
