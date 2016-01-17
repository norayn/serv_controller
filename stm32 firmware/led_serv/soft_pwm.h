#pragma once

#include "stm32f10x.h"

#define INVALID_INDEX -1

#define PWM_COUNT 12
#define PWM_PERIOD_IS_END -2
#define PWM_PERIOD_IS_STARTED -1

typedef  unsigned short int uint8;
typedef unsigned short  int uint16;
typedef unsigned        int uint32;


void PwmInit( void );
void AdcInit( void );
void SortDataIndex( void );
void PwmStep( uint8 IsResCNT );
void PwmSetLed( uint8 Index, uint8 Data );
void PwmSetFan( uint8 Index, uint8 Data );
uint8	PwmGetLed( uint8 Index );
uint32*	GetAbc( void );

