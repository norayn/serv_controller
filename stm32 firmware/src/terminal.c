// terminal.c
// �����: Ezhik
// �������: 12.09.2011
// ������� ��� TE.

#include <stdio.h>

#include "platform_config.h"


// ---------------------------------------------------------------------
// ���������������� ������
// ��������� fputc ��� ������ stdout �� USART1

int fputc(int ch, FILE *f)
{
#ifdef STDOUT_COM

	USART_SendData(STDOUT_COM, (uint8_t) ch);

	while (USART_GetFlagStatus(STDOUT_COM, USART_FLAG_TXE) == RESET)
	{}
	
#endif
	
	return ch;
}

