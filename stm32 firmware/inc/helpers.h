// helpers.h
// Автор: Ezhik
// Создано: 12.09.2011
// Вариант для TE

#ifndef _HELPERS_H
#define _HELPERS_H

const char *get_usb_state_name(uint32_t state);

void Delay(uint16_t time);
void Delay_SysTickHandler();
void printboard();

#endif