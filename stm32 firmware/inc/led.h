// led.h
// Автор: Ezhik
// Создано: 12.09.2011
// Вариант для TE

#ifndef _LED_H
#define _LED_H

#include "stm32f10x.h"

#include "platform_config.h"


#define LEDSTATE_UNKNOWN		0x00

#ifdef USE_LEDS

#define LEDSTATE_OFF         	0x01
#define LEDSTATE_ON         	0x10
#define LEDSTATE_BLINK       	0x20

void Led_Init();

uint8_t LED_GetState(int led);
uint8_t LED_GetBlinkState(int led, uint16_t *blink_on, uint16_t *blink_off);
void LED_On(int led);
void LED_Off(int led);
void LED_OnBlink(int led, uint16_t blink_on, uint16_t blink_off);

void LED_SysTick_Handler();

#endif // USE_LEDS

#endif 