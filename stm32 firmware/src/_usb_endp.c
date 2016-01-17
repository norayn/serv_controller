// usb_endp.c
// Автор: Ezhik
// Создано: 12.09.2011
// Использован шаблон от MCD Application Team (thx very much!)
// Callback-и Endpoint-ов
// Вариант для TE

#include "platform_config.h"

#ifdef USE_USB

#include "usb_lib.h"
#include "usb_istr.h"

#include "user_usb.h"

#include <stdio.h>

// $USBCONFIG - Тут нужно при необходимости добавить/удалить Callback-и Ендпойинтов
// См. также usb_conf.h

// Ендпойнт 1. 
void EP1_IN_Callback(void)
{
#ifdef _DEBUG_
    printf("EP: 01 in \r\n");
#endif
}

// Ендпойнт 2. 
void EP2_OUT_Callback(void)
{
    #ifndef STM32F10X_CL
		// Разрешаем ендпойнту получить следующую команду
        SetEPRxStatus(ENDP2, EP_RX_VALID);
    #else
        if (GetEPRxStatus(EP2_OUT) == EP_RX_STALL)
        {
			// Разрешаем ендпойнту получить следующую команду
            SetEPRxStatus(EP2_OUT, EP_RX_VALID);
        }
    #endif /* STM32F10X_CL */

#ifdef _DEBUG_
	printf("EP: 02 out \r\n");
#endif

	// Чтение и обработка пришедших данных
	usb_handle_command();
}

#endif