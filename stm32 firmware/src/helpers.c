// helpers.c
// �����: Ezhik
// �������: 12.09.2011
// ��������������� �������
// ������� ��� TE

#include "stm32f10x.h"
#include "platform_config.h"
#include "helpers.h"

#include <stdio.h>

// ������� 

#define USB_STATES	6
const char *st[USB_STATES] = { 
 "UNCONNECTED",
 "ATTACHED",
 "POWERED",
 "SUSPENDED",
 "ADDRESSED",
 "CONFIGURED",
 };


const char *get_usb_state_name(uint32_t state)
{
	if(state > USB_STATES) return "UNKNOWN";
	return st[state];
}

// -----------------------------------------------------------
// ��������

volatile uint16_t delay_counter = 0;

// �������� � ������������
void Delay(uint16_t time)
{
		delay_counter = time;
        while(delay_counter != 0);
}
 
// ���������� SysTick. ���������� �� SysTick_Handler, stm32f10x_it.c
void Delay_SysTickHandler()
{
        if(delay_counter > 0)
        {
                delay_counter--;
        }
 
}

// -----------------------------------------------------------
// �����

// ������ ������� �� ��������� ������. ��� ��������.
void NextLine()
{
	printf("\r\n");
}


// -----------------------------------------------------------

// $BOARDSPECIFIC - ����� �������� �����
void printboard()
{
	#if defined STRIVE_BOARD
	printf("Strive Board");

	#elif defined EM_STM3210C_BOARD
	printf("EM-STM3210C Board");

	#elif defined TE_STM32F103_BOARD
	printf("TE-STM32F103 Board");

	#else	
	printf("Undefined Board");

	#endif
}