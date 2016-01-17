#pragma once
#include "hw_config.h"
#include "soft_pwm.h"



#define MAX_DISCONNECTED_TIME 25000	//mSec
#define MAX_LOAD_TIME 3 * 60	//Sec


typedef enum
{
    false = ( 1 == 0 ),
    true = ( ! false )
} boolean;

typedef enum 
{
	LED_LEFT,
	LED_RIGHT,
	LED_UP,
	LED_DOWN,
	LED_ALL
} led_segment;

typedef enum 
{
	FAN_1,
	FAN_2,
	FAN_3,
	FAN_4,
	FAN_ALL
} fan_unit;

typedef enum 
{
	MODE_START,
	MODE_WAIT_USB_CONNECT,	
	MODE_WAIT_START_HOST,		
	MODE_RUN,
	MODE_PWR_OFF,
	MODE_OFF,
	MODE_WAIT_OFF,			
	MODE_COUNT
} logic_mode;

typedef enum 
{
	TC_R0,
	TC_R1,
	TC_R2,	
	TC_IS_RUN,		
	TC_PWR_OFF,
	TC_DEBUG_PAUSE,
	TC_SET_LED_SEGMENT_RGB,		
	TC_SET_FAN_POWER,
	TC_TO_OFF	
} tc_comand;

void InitServLedPeripherals();
void ParsePcComand( uint8_t* Buff );
void GetDataToPc( uint8_t* Buff );

void SetLedSegmentRGB(  led_segment const Segment, uint8 const R, uint8 const G, uint8 const B, boolean const NeedSort );
void SetFanPower( fan_unit const Fan, uint8 const Power );

void LogicInit();
void UpdateLogic( boolean IsConfiguredUSB );
void UpdateTimings();

void ToModePwrOff();
void ToModeWaitUsbConnect();
void ToModeWaitStartHost();
void ToModeRun();
void ToModeOff();
void ToModeWaitOff();




