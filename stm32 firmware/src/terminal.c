// terminal.c
// Автор: Ezhik
// Создано: 12.09.2011
// Вариант для TE.

#include <stdio.h>

#include "platform_config.h"


// ---------------------------------------------------------------------
// Пользовательская секция
// подменяем fputc для вывода stdout на USART1

int fputc(int ch, FILE *f)
{
#ifdef STDOUT_COM

	USART_SendData(STDOUT_COM, (uint8_t) ch);

	while (USART_GetFlagStatus(STDOUT_COM, USART_FLAG_TXE) == RESET)
	{}
	
#endif
	
	return ch;
}

