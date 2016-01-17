// user_usb.c
// Автор: Ezhik
// Создано: 12.09.2011
// Пользовательская работа с USB (см Doc\protocol.txt)
// Endpoint 83 In, Endpoint 02 Out (направление со стороны хоста)
// Вариант для TE

#include "platform_config.h"

#ifdef USE_USB

#include "usb_lib.h"

#include "user_usb.h"
#include "led.h"
#include "hw_config.h"

#include <stdio.h>
					
bool led_state_ = FALSE;

// Чтение и обработка пришедшего запроса
int usb_handle_command()
{
	int nRead;

	uint8_t request[64];
	uint8_t answer[64];

	uint8_t command_type;
	uint16_t command_id;
	uint16_t *command_id_ptr;

	int i;

	for(i=0;i<64;i++)
	{
		request[i] = 0;
	}

	nRead = USB_SIL_Read(EP2_OUT, request);
	request[nRead] = 0; 
	command_type = request[0];
	command_id = *(uint16_t *)(request + 1);

	#ifdef _DEBUG_
	printf("Command type %d id %d received\r\n", command_type, command_id);
	#endif

	switch(command_type)
	{
		case SOMEDEVCMD_SIMPLE:
			#ifdef _DEBUG_
			printf("Simple command\r\n");
			#endif

			if(nRead != SOMEDEVCMD_SIMPLE_LENGTH)
			{
				#ifdef _DEBUG_
				printf("Bad length = %d\r\n", nRead);
				#endif
				for(i=1;i<64;i++)
					answer[i] = 0;
				answer[0] = 0xff; // bad command
				USB_SIL_Write(EP1_IN, answer, nRead);
				break;
			}
			switch (command_id)
			{
				case SOMEDEVCMD_SIMPLE_PING:
					#ifdef _DEBUG_
					printf("Ping command %d %d %d %d\r\n", request[3], request[4], request[5], request[6]);
					#endif
					// else просто шлем пинг со всеми данными обратно
					USB_SIL_Write(EP1_IN, request, nRead);
					break;
				case SOMEDEVCMD_SIMPLE_LED:
					handle_led_request(request, nRead);
					break;
				default:
					#ifdef _DEBUG_
					printf("Unknown command id\r\n");
					#endif
					for(i=1;i<64;i++)
						answer[i] = 0;
					answer[0] = 0xff; // bad command
					answer[1] = 0xff; // bad command
					answer[2] = 0xff; // bad command
					USB_SIL_Write(EP1_IN, answer, nRead);
					break;
			}
			break;
		default:
				#ifdef _DEBUG_
				printf("Unknown command type\r\n");
				#endif
				for(i=1;i<64;i++)
					answer[i] = 0;
				answer[0] = 0xff; // bad command
				USB_SIL_Write(EP1_IN, answer, nRead);
				break;
			break;
	}

	// отправляем пакет
	SetEPTxValid(ENDP1);
}

void handle_led_request(uint8_t *request, int nRead)
{
	uint8_t led_command = request[3];
	uint8_t led_number = request[4];
	uint16_t blink_on = *(uint16_t *)(request + 5);
	uint16_t blink_off = *(uint16_t *)(request + 7);

	#ifdef _DEBUG_
	printf("LED Command: %d %d %d %d\r\n", led_command, led_number, blink_on, blink_off);
	#endif
	switch(led_command)
	{
		case SOMEDEVCMD_SIMPLE_LED_STATE:
		#ifdef USE_LEDS
			request[3] = LED_GetBlinkState(led_number, (uint16_t *)(request + 5), (uint16_t *)(request + 7));
		#else
			request[3] = LEDSTATE_UNKNOWN; // UNKNOWN
		#endif
			if(led_number == 0xff)
			{
			#ifdef USE_LEDS
				request[4] = LEDS_NUM;
			#else
				request[4] = 0;
			#endif
				#ifdef _DEBUG_
				printf("Requested LED_NUM. Answer - %d\r\n", request[4]);
				#endif
			}
			USB_SIL_Write(EP1_IN, request, nRead);
			break;
		case SOMEDEVCMD_SIMPLE_LED_OFF:
		#ifdef USE_LEDS
			LED_Off(led_number);
			request[3] = LED_GetState(led_number);
		#else
			request[3] = LEDSTATE_UNKNOWN;
		#endif
		    USB_SIL_Write(EP1_IN, request, nRead);
			break;
		case SOMEDEVCMD_SIMPLE_LED_ON:
		#ifdef USE_LEDS
			LED_On(led_number);
			request[3] = LED_GetState(led_number);
		#else
			request[3] = LEDSTATE_UNKNOWN;
		#endif
			USB_SIL_Write(EP1_IN, request, nRead);
			break;
		case SOMEDEVCMD_SIMPLE_LED_BLINK:
		#ifdef USE_LEDS
			LED_OnBlink(led_number, blink_on, blink_off);
			request[3] = LED_GetBlinkState(led_number, (uint16_t *)(request + 5), (uint16_t *)(request + 7));
		#else
			request[3] = LEDSTATE_UNKNOWN;
		#endif
			USB_SIL_Write(EP1_IN, request, nRead);
			break;
	}
}

#endif // USE_USB