#pragma once
#include "serv_led.h"


boolean			m_PrevConfiguredUSB = false;
unsigned int	m_StartTemp[ 4 ] = { 0, 0, 0, 0 };
uint8			m_FanPower[ 4 ] = { 255, 255, 255, 255 };
boolean			m_MainPowerLineOnIsStart = false;
//boolean         m_PowerSw = false;
int				m_DisconnectedTime = INVALID_INDEX;
int				m_LoadTime = INVALID_INDEX;
uint8			m_TimeMsCounter = 0;
logic_mode		m_Mode = MODE_START;
boolean         m_DebugPause = false;
	
void InitServLedPeripherals()
{
	//--GPIO------------------------------------------------
	GPIOB->BRR  = GPIO_Pin_11; // power sw off
	
	GPIO_InitTypeDef GPIO_InitStructure;
		
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
	GPIO_Init(GPIOB, &GPIO_InitStructure);	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;	
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
	
	
	//--watch dog---------------------------------------------
	IWDG->KR  = 0x5555;                                           // enable write to PR, RLR
    IWDG->PR  = 255;										      // Init prescaler
    IWDG->RLR = 4000;											  // Init RLR max 0FFF
    IWDG->KR  = 0xAAAA;                                           // Reload the watchdog
    IWDG->KR  = 0xCCCC;                                           // Start the watchdog
	
	//if (RCC->CSR & (1<<29)) {                       // IWDG Reset Flag set
	//	RCC->CSR |= (1<<24);                          // Clear Reset Flags
}

	
void ParsePcComand( uint8_t* Buff )
{	
	switch( Buff[ 0 ] )
	{
	case TC_DEBUG_PAUSE:	
		m_DebugPause = true;
		break;	
			
	case  TC_SET_LED_SEGMENT_RGB:	
		SetLedSegmentRGB( ( led_segment )Buff[ 1 ], Buff[ 2 ], Buff[ 3 ], Buff[ 4 ], true );
		break;	
		
	case   TC_SET_FAN_POWER:	
		SetFanPower( ( fan_unit )Buff[ 1 ], Buff[ 2 ] );
		break;	
		
	case   TC_IS_RUN:	
		ToModeRun();
		break;	
		
	case   TC_PWR_OFF:	
		ToModePwrOff();
		break;		
					
	case   TC_TO_OFF:	
		ToModeWaitOff();
		break;		
			
		//if ( Buff[ 0 ] == 2 )
		//{
		//	m_PowerSw = true;
		//}	
		
	}
	//m_DisconnectedTime = 0;	
}


void GetDataToPc( uint8_t* Buff )
{
	//Buff[ 0 ] = 4;// report id 4
	
	unsigned int* Abc = GetAbc();
	
	Buff[ 1 ] = 1;	//type
	Buff[ 2 ] = Abc[ 0 ] / 16;
	Buff[ 3 ] = Abc[ 1 ] / 16;
	Buff[ 4 ] = Abc[ 2 ] / 16;
	Buff[ 5 ] = Abc[ 3 ] / 16;
	
	Buff[ 6 ] = m_FanPower[ 0 ];
	Buff[ 7 ] = m_FanPower[ 1 ];
	Buff[ 8 ] = m_FanPower[ 2 ];
	Buff[ 9 ] = m_FanPower[ 3 ];
			
	Buff[ 10 ] = m_StartTemp[ 0 ];
	Buff[ 11 ] = m_StartTemp[ 1 ];
	Buff[ 12 ] = m_StartTemp[ 2 ];
	Buff[ 13 ] = m_StartTemp[ 3 ];
	
	Buff[ 14 ] = m_MainPowerLineOnIsStart;
	Buff[ 15 ] = m_Mode;		
}


void SetLedSegmentRGB( led_segment const Segment, uint8 const R, uint8 const G, uint8 const B, boolean const NeedSort )
{
	switch( Segment )
	{
		case LED_LEFT:
		{
			PwmSetLed( 3, G );
			PwmSetLed( 4, R );
			PwmSetLed( 5, B );
		}
		break;
		
		case LED_RIGHT:
		{
			PwmSetLed( 9, B );
			PwmSetLed( 10, G );
			PwmSetLed( 11, R );			
		}
		break;
		
		case LED_UP:
		{
			PwmSetLed( 0, B );
			PwmSetLed( 1, R );
			PwmSetLed( 2, G );			
		}
		break;
		
		case LED_DOWN:
		{
			PwmSetLed( 6, B );
			PwmSetLed( 7, R );
			PwmSetLed( 8, G );			
		}
		break;
		
		case LED_ALL:
		{
			PwmSetLed( 3, G );
			PwmSetLed( 4, R );
			PwmSetLed( 5, B );
			PwmSetLed( 9, B );
			PwmSetLed( 10, G );
			PwmSetLed( 11, R );
			PwmSetLed( 0, B );
			PwmSetLed( 1, R );
			PwmSetLed( 2, G );	
			PwmSetLed( 6, B );
			PwmSetLed( 7, R );
			PwmSetLed( 8, G );									
		}
		break;								
	}
	
	if ( NeedSort )
		SortDataIndex();
}


void SetFanPower( fan_unit const Fan, uint8 const Power )
{
	switch( Fan )
	{
		case FAN_1:	
		{
			PwmSetFan( 0, Power );
			m_FanPower[ Fan ] = Power;
		}	
		break;

		case FAN_2:	
		{
			PwmSetFan( 1, Power );
			m_FanPower[ Fan ] = Power;
		}		
		break;

		case FAN_3:	
		{			
			PwmSetFan( 2, Power );
			m_FanPower[ Fan ] = Power;	
		}	
		break;

		case FAN_4:	
		{	
			PwmSetFan( 3, Power );
			m_FanPower[ Fan ] = Power;	
		}	
		break;

		case FAN_ALL:	
		{
			PwmSetFan( 0, Power );	
			PwmSetFan( 1, Power );	
			PwmSetFan( 2, Power );	
			PwmSetFan( 3, Power );
			m_FanPower[ 0 ] = m_FanPower[ 1 ] = m_FanPower[ 2 ] = m_FanPower[ 3 ] = Power;		
		}			
		break;							
	}
}


void LogicInit()
{		
	unsigned int* Abc = GetAbc();
	m_StartTemp[ 0 ] = Abc[ 0 ] / 16;
	m_StartTemp[ 1 ] = Abc[ 1 ] / 16;
	m_StartTemp[ 2 ] = Abc[ 2 ] / 16;
	m_StartTemp[ 3 ] = Abc[ 3 ] / 16;
	
	
	InitServLedPeripherals();	
	m_MainPowerLineOnIsStart = GPIO_ReadInputDataBit( GPIOB, GPIO_Pin_10 ) ? true : false;
}


void UpdateLogic( boolean IsConfiguredUSB )
{	
	IWDG->KR  = 0xAAAA;                           // reload the watchdog
	
	switch( m_Mode )
	{
		case MODE_START:
		{			
			ToModeWaitUsbConnect();
		}
		break;
		
		case MODE_WAIT_USB_CONNECT:
		{
			if ( IsConfiguredUSB )
				ToModeWaitStartHost();
		}
		break;
		
		case MODE_WAIT_START_HOST:
		{
			;
		}
		break;	
		
		case MODE_RUN:
		{
			if ( m_DebugPause )
				while( 1 )	{}
		}
		break;	
		
		case MODE_PWR_OFF:
		{			
			if ( !GPIO_ReadInputDataBit( GPIOB, GPIO_Pin_10 )  )
				ToModeOff();
		}
		break;
				
		case MODE_OFF:
		{			
			if ( GPIO_ReadInputDataBit( GPIOB, GPIO_Pin_10 )  )
				ToModeWaitUsbConnect();
		}
		break;							
	}
}


void UpdateTimings()
{
	/// 10 mS period	
	if ( m_DisconnectedTime != INVALID_INDEX )
	{
		m_DisconnectedTime += 10;	
			if ( m_DisconnectedTime > MAX_DISCONNECTED_TIME )
				ToModePwrOff();
	}
	
	if ( m_LoadTime != INVALID_INDEX )	
	{
		m_TimeMsCounter++;
		if ( m_TimeMsCounter > 100 ) // 1sec
		{
			m_TimeMsCounter = 0;
			m_LoadTime++;	
			if ( m_LoadTime > MAX_LOAD_TIME )
				ToModePwrOff();
		}
	}
}


void ToModePwrOff()
{
	m_DisconnectedTime = INVALID_INDEX;
	m_LoadTime = INVALID_INDEX;
	m_Mode = MODE_PWR_OFF;
	GPIOB->BSRR  = GPIO_Pin_11; // power sw on
	SetLedSegmentRGB( LED_ALL, 222, 222, 0, true );
	SetFanPower( FAN_ALL, 222 );
}


void ToModeWaitUsbConnect()
{
	m_LoadTime = 0;
	m_Mode = MODE_WAIT_USB_CONNECT;
	SetLedSegmentRGB( LED_ALL, 111, 0, 71, true );
}


void ToModeWaitStartHost()
{	
	m_Mode = MODE_WAIT_START_HOST;
	SetLedSegmentRGB( LED_ALL, 0, 0, 111, true );
}


void ToModeRun()
{
	m_DisconnectedTime = 0;	
	m_LoadTime = INVALID_INDEX;
	m_Mode = MODE_RUN;
}


void ToModeOff()
{
	GPIOB->BRR  = GPIO_Pin_11; // power sw off
	m_DisconnectedTime =INVALID_INDEX;
	m_LoadTime = INVALID_INDEX;
	m_Mode = MODE_OFF;
}


void ToModeWaitOff()
{	
	m_DisconnectedTime =INVALID_INDEX;
	m_LoadTime = 0;
	m_Mode = MODE_WAIT_OFF;
	SetFanPower( FAN_ALL, 222 );
	SetLedSegmentRGB( LED_ALL, 0, 111, 111, true );
}