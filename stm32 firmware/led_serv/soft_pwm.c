#pragma once
#include "soft_pwm.h"


GPIO_TypeDef		*PwmPortPrt[ PWM_COUNT ];
uint16 				PwmPortPin[ PWM_COUNT ];
uint8 				PwmData[ PWM_COUNT ];
uint8 				PwmSortedIndex[ PWM_COUNT ];
int   				CurrentIndexStep;
uint8				PwmIsOn = 0;

unsigned int adc_res[ 4 ];

static void PwmPeripheralsInit()
{
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

  TIM_TimeBaseInitTypeDef base_timer;
  TIM_TimeBaseStructInit(&base_timer);
  /* Делитель учитывается как TIM_Prescaler + 1, поэтому отнимаем 1 */
  base_timer.TIM_Prescaler = 500 - 1;
  base_timer.TIM_Period = 11;
  TIM_TimeBaseInit(TIM4, &base_timer);

  TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
  TIM_Cmd(TIM4, ENABLE);
	
  NVIC_EnableIRQ(TIM4_IRQn);
	
	//--GPIO------------------------------------------------
	GPIO_InitTypeDef GPIO_InitStructure;
		
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = /*GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 |*/ GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_15;		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
	GPIO_Init(GPIOA, &GPIO_InitStructure);	

	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
  AFIO->MAPR |= AFIO_MAPR_SWJ_CFG_JTAGDISABLE; // disable JTAG
}


static void HardPwmPeripheralsInit()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
		
	GPIO_InitTypeDef GPIO_InitStructure;

	/*GPIOA Configuration: TIM3 channel 1 and 2 as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/*GPIOB Configuration: TIM3 channel 3 and 4 as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_1;

	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	
	/* TIM3 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
		
	/* -----------------------------------------------------------------------
	  TIM3 Configuration: generate 4 PWM signals with 4 different duty cycles:
	  TIM3CLK = 36 MHz, Prescaler = 0x0, TIM3 counter clock = 36 MHz
	  TIM3 ARR Register = 999 => TIM3 Frequency = TIM3 counter clock/(ARR + 1)
	  TIM3 Frequency = 36 KHz.
	  TIM3 Channel1 duty cycle = (TIM3_CCR1/ TIM3_ARR)* 100 = 50%
	  TIM3 Channel2 duty cycle = (TIM3_CCR2/ TIM3_ARR)* 100 = 37.5%
	  TIM3 Channel3 duty cycle = (TIM3_CCR3/ TIM3_ARR)* 100 = 25%
	  TIM3 Channel4 duty cycle = (TIM3_CCR4/ TIM3_ARR)* 100 = 12.5%
	----------------------------------------------------------------------- */
	
	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Prescaler = 9 - 1;	
	TIM_TimeBaseStructure.TIM_Period = 256;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	
	/* PWM1 Mode configuration: Channel1 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 200;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;    // MATCH 시 HIGH
	 // TIM_OCPlarity_Low는 반대
	
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);
	
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
	
	/* PWM1 Mode configuration: Channel2 */
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 200;
	
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);
	
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
	
	/* PWM1 Mode configuration: Channel3 */
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 200;
	
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);
	
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
	
	/* PWM1 Mode configuration: Channel4 */
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 200;
	
	TIM_OC4Init(TIM3, &TIM_OCInitStructure);
	
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);
	
	TIM_ARRPreloadConfig(TIM3, ENABLE);
	
	/* TIM3 enable counter */
	TIM_Cmd(TIM3, ENABLE);	
}


void PwmInit( void )
{	
	PwmPeripheralsInit();
	HardPwmPeripheralsInit();
	
	for ( int I = 0; I < PWM_COUNT; I++ )
		{
			PwmData[ I ] = 55;
			PwmSortedIndex[ I ] = I;
		}
	CurrentIndexStep = PWM_PERIOD_IS_END;
					 
	PwmPortPrt[  0 ] = GPIOB;	PwmPortPin[  0 ] = GPIO_Pin_12;	//led up B
	PwmPortPrt[  1 ] = GPIOB;	PwmPortPin[  1 ] = GPIO_Pin_13;	//led up R
	PwmPortPrt[  2 ] = GPIOB;	PwmPortPin[  2 ] = GPIO_Pin_14;	//led up G				 							 
	PwmPortPrt[  3 ] = GPIOB;	PwmPortPin[  3 ] = GPIO_Pin_15;	//led left G
	PwmPortPrt[  4 ] = GPIOA;	PwmPortPin[  4 ] = GPIO_Pin_8;	//led left R
	PwmPortPrt[  5 ] = GPIOA;	PwmPortPin[  5 ] = GPIO_Pin_9;	//led left B								
	PwmPortPrt[  6 ] = GPIOA;	PwmPortPin[  6 ] = GPIO_Pin_15;	//led down B
	PwmPortPrt[  7 ] = GPIOB;	PwmPortPin[  7 ] = GPIO_Pin_3;	//led down R
	PwmPortPrt[  8 ] = GPIOB;	PwmPortPin[  8 ] = GPIO_Pin_4;	//led down G								
	PwmPortPrt[  9 ] = GPIOB;	PwmPortPin[  9 ] = GPIO_Pin_5;	//led right B
	PwmPortPrt[ 10 ] = GPIOB;	PwmPortPin[ 10 ] = GPIO_Pin_6;	//led right G
	PwmPortPrt[ 11 ] = GPIOB;	PwmPortPin[ 11 ] = GPIO_Pin_7;	//led right R


	for ( int I = 0; I < PWM_COUNT; I++ )
		{
			GPIO_ResetBits( PwmPortPrt[ I ], PwmPortPin[ I ] );			
		}

	PwmIsOn = 1;
}


void AdcInit( void )
{
	//--Enable ADC1 and GPIOA--
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure; //Variable used to setup the GPIO pins
	//==Configure ADC pins (PA0 -> Channel 0 to PA7 -> Channel 7) as analog inputs==
	GPIO_StructInit(&GPIO_InitStructure); // Reset init structure, if not it can cause issues...
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	ADC1->CR2 |= ADC_CR2_CAL; //Запуск калибровки АЦП
	while (!(ADC1->CR2 & ADC_CR2_CAL));
	ADC_InitTypeDef ADC_InitStructure;
	//ADC1 configuration
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	//We will convert multiple channels
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	//select continuous conversion mode
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;//!
	//select no external triggering
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	//right 12-bit data alignment in ADC data register
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	//8 channels conversion
	ADC_InitStructure.ADC_NbrOfChannel = 2;
	//load structure values to control and status registers
	ADC_Init(ADC1, &ADC_InitStructure);
	//wake up temperature sensor
	//ADC_TempSensorVrefintCmd(ENABLE);
	//configure each channel
	ADC_Cmd(ADC1, ENABLE);
}


uint32* GetAbc( void )
{
	
	//unsigned char index=0;
	//unsigned int adc[7],adc1[7];
	//for(index=0;index<6;index++)
	//{
		ADC_RegularChannelConfig(ADC1,ADC_Channel_0, 1,ADC_SampleTime_28Cycles5);
		ADC_ClearFlag (ADC1,ADC_FLAG_EOC);
		ADC_SoftwareStartConvCmd(ADC1, ENABLE);
		while (ADC_GetFlagStatus (ADC1,ADC_FLAG_EOC) == RESET);		// wait for conversion to complete
		adc_res[ 0 ]  = ADC_GetConversionValue (ADC1);
		
		ADC_RegularChannelConfig(ADC1,ADC_Channel_1, 1,ADC_SampleTime_28Cycles5);
		ADC_ClearFlag (ADC1,ADC_FLAG_EOC);
		ADC_SoftwareStartConvCmd(ADC1, ENABLE);		// start another
		while (ADC_GetFlagStatus (ADC1,ADC_FLAG_EOC) == RESET);
		adc_res[ 1 ]  = ADC_GetConversionValue (ADC1);
		
		ADC_RegularChannelConfig(ADC1,ADC_Channel_2, 1,ADC_SampleTime_28Cycles5);
		ADC_ClearFlag (ADC1,ADC_FLAG_EOC);
		ADC_SoftwareStartConvCmd(ADC1, ENABLE);
		while (ADC_GetFlagStatus (ADC1,ADC_FLAG_EOC) == RESET);		// wait for conversion to complete
		adc_res[ 2 ]  = ADC_GetConversionValue (ADC1);
		
		ADC_RegularChannelConfig(ADC1,ADC_Channel_3, 1,ADC_SampleTime_28Cycles5);
		ADC_ClearFlag (ADC1,ADC_FLAG_EOC);
		ADC_SoftwareStartConvCmd(ADC1, ENABLE);		// start another
		while (ADC_GetFlagStatus (ADC1,ADC_FLAG_EOC) == RESET);
		adc_res[ 3 ]  = ADC_GetConversionValue (ADC1);
			
		return adc_res;
	//}
}


void SortDataIndex( void )
{
	PwmIsOn = 0;
	for( int I = 0; I < PWM_COUNT - 1; I++ )
		for( int J = 0; J < PWM_COUNT - I - 1; J++ )
		    if( PwmData[ PwmSortedIndex[ J ] ] > PwmData[ PwmSortedIndex[ J + 1 ]] )
		        {
					uint8 const Tmp = PwmSortedIndex[ J ];
					PwmSortedIndex[ J ] = PwmSortedIndex[ J + 1 ];
					PwmSortedIndex[ J + 1 ] = Tmp;
				}
	PwmIsOn = 1;	
}


void PwmStep( uint8 IsResCNT )
{
	if ( !PwmIsOn ) 
		return;
	//reset timer
	if ( IsResCNT )
		TIM4->CNT  = 0x0000;

	if ( CurrentIndexStep == PWM_PERIOD_IS_END )//on all enable pins
	{
		for ( int I = 0; I < PWM_COUNT; I++ )		
			if ( PwmData[ I ] )
				GPIO_SetBits( PwmPortPrt[ I ], PwmPortPin[ I ] );			
		
		CurrentIndexStep = PWM_PERIOD_IS_STARTED;
		CurrentIndexStep++;	
		uint8 const CurrentPwmData = PwmData[ PwmSortedIndex[ CurrentIndexStep ] ];
		if ( CurrentPwmData )
			TIM4->ARR = CurrentPwmData;			
		else
			PwmStep( 0 );
	}
	else
	{
	//dp
		uint8 const PortIndex = PwmSortedIndex[ CurrentIndexStep ];
		GPIO_ResetBits( PwmPortPrt[ PortIndex ], PwmPortPin[ PortIndex ] );
		CurrentIndexStep++;		
	}	
	
	if ( CurrentIndexStep == PWM_COUNT )//go to end
	{
		CurrentIndexStep = PWM_PERIOD_IS_END;
		
		TIM4->ARR  = 255 - PwmData[ PwmSortedIndex[ PWM_COUNT - 1 ] ];
		if ( TIM4->ARR == 0 )//check if 255
			PwmStep( 1 );	 
		return;
	}	
	//set next	
	uint8 const CurrentPwmData = PwmData[ PwmSortedIndex[ CurrentIndexStep -1 ] ];
	uint8 const NextPwmData = PwmData[ PwmSortedIndex[ CurrentIndexStep ] ];
	if ( CurrentPwmData == NextPwmData )		
		PwmStep( 0 );	
	else
		TIM4->ARR  = NextPwmData - CurrentPwmData;
}


void PwmSetLed( uint8 Index, uint8 Data )
{
	PwmData[ Index ] = Data;
}


void PwmSetFan( uint8 Index, uint8 Data )
{
	switch( Index )
	{
		case 0:		TIM_SetCompare1( TIM3, Data ); break;
		case 1:		TIM_SetCompare2( TIM3, Data ); break;
		case 2:		TIM_SetCompare3( TIM3, Data ); break;
		case 3:		TIM_SetCompare4( TIM3, Data ); break;						
	}
}


uint8	PwmGetLed( uint8 Index )
{
	return PwmData[ Index ];
}

// if ind == -1: up all pin ind=0, set next: arr = next, ( no res tim cnt )
// 0	-	0	DP, ind++, set next:( if next == curr, call next ), ( no res tim cnt )
// 1	-	0	DP, ind++, set next: arr = next - current( =2-0 )
// 2	-	2	DP, ind++, set next: arr = next - current( =5-2 )
// 3	-	5	DP, ind++, set next:( if next == curr, call next ) ( no res tim cnt )
// 4	-	5	DP, ind++, set next: arr = next - current( =9-5 )
// 5	-	9	DP, ind++, if end: ind =-1  arr = fin - current( =255-9 )				
//-end, all 255






