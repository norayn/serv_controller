// user_usb.h
// Автор: Ezhik
// Создано: 12.09.2011
// Вариант для TE

#ifndef _USER_USB_H
#define _USER_USB_H

// Дефайны

#define SOMEDEVCMD_SIMPLE 				0x00
#define SOMEDEVCMD_SIMPLE_LENGTH 		0x0B

#define SOMEDEVCMD_SIMPLE_PING 			0x0000

#define SOMEDEVCMD_SIMPLE_LED 			0x0100
#define SOMEDEVCMD_SIMPLE_LED_STATE		0x00
#define SOMEDEVCMD_SIMPLE_LED_OFF		0x01
#define SOMEDEVCMD_SIMPLE_LED_ON		0x10
#define SOMEDEVCMD_SIMPLE_LED_BLINK		0x20 

// Функции

int usb_handle_command();

void handle_led_request(uint8_t *request, int nRead);

#endif

