// usb_endp.c
// �����: Ezhik
// �������: 12.09.2011
// ����������� ������ �� MCD Application Team (thx very much!)
// Callback-� Endpoint-��
// ������� ��� TE

#include "platform_config.h"

#ifdef USE_USB

#include "usb_lib.h"
#include "usb_istr.h"

#include "user_usb.h"

#include <stdio.h>

// $USBCONFIG - ��� ����� ��� ������������� ��������/������� Callback-� �����������
// ��. ����� usb_conf.h

// �������� 1. 
void EP1_IN_Callback(void)
{
#ifdef _DEBUG_
    printf("EP: 01 in \r\n");
#endif
}

// �������� 2. 
void EP2_OUT_Callback(void)
{
    #ifndef STM32F10X_CL
		// ��������� ��������� �������� ��������� �������
        SetEPRxStatus(ENDP2, EP_RX_VALID);
    #else
        if (GetEPRxStatus(EP2_OUT) == EP_RX_STALL)
        {
			// ��������� ��������� �������� ��������� �������
            SetEPRxStatus(EP2_OUT, EP_RX_VALID);
        }
    #endif /* STM32F10X_CL */

#ifdef _DEBUG_
	printf("EP: 02 out \r\n");
#endif

	// ������ � ��������� ��������� ������
	usb_handle_command();
}

#endif