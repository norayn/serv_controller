// main.c
// �����: Ezhik ( bi_zet ������ mail ����� �� )
// �������: 12.09.2011
// ������� ��� TE

#include "platform_config.h"

#include "hw_config.h"
#include "helpers.h"

#include "soft_pwm.h"
#include "serv_led.h"

#ifdef USE_USB
#include "usb_pwr.h"
#endif

#ifdef USE_LEDS
#include "led.h"
#endif


#include <stdio.h>

static void Set_System(void);
static void ShowSystemInfo();

__IO uint8_t PrevXferComplete = 1;

// Main
int main(void)
{
	//uint32_t  usbLastState;	

	// ��������� �������. ���������� ���� � hw_config.c, ������ ��� ��� ��������� 
	// ����� �� �������� ���� ������ ���������, ��������� �� � main
    Set_System();

	#ifdef USE_LEDS
    LED_OnBlink(LED1, 200, 200);
	#endif

	PwmInit();
	AdcInit();
	
	SetLedSegmentRGB( LED_ALL, 0, 0, 111, true );
	//SortDataIndex();	
	LogicInit();
	
	while (1)
	{		
		if ( bDeviceState == CONFIGURED )
		{
			UpdateLogic( true );
			
			//GPIO_SetBits(GPIOB,GPIO_Pin_15);
			if (PrevXferComplete)
			{
					//GPIO_SetBits(GPIOA,GPIO_Pin_9);
			  RHIDCheckState();
			}
		}
		else
			UpdateLogic( false );	

		// ��� ���������, �� ����������� ������ �� �������
		//if(bDeviceState != usbLastState)
		//{
		//	usbLastState = bDeviceState;
		//	
		//	if (bDeviceState == CONFIGURED)
		//		SetLedSegmentRGB( LED_ALL, 0, 111, 0, true );
		//}
		
		// �������� 1��
		Delay(1);		
	
	}
}

// ��������� �������.
// ������� ��� ���������� ��������� � startup-�����.
// ��� ������������ �������������
static void Set_System(void)
{
	// ��������� SysTick - 1 �� / SYSTICK_PRESCALE .��������, ��� ������� ���������� (�� led.c)
	SysTick_Config(SystemCoreClock / SYSTICK_PRESCALE / 1000);	

  	// RCC configuration 
 	RCC_Config();

	// ����� �� ����������� COM, ����� ����� ���� �������� �� ��������

#ifdef USE_COM
	InitCOM();
#endif

	// ���������� ������ � ���������� � �������
	ShowSystemInfo();
	
#ifdef USE_LEDS
	// ������������ ����������
    Led_Config();

    LED_OnBlink(LED1, 2000, 2000);
#endif

	printf("LED Count: %d", LEDS_NUM);
	NextLine();

#ifdef USB_DISCONNECT
	// ������������ ����, ������������ USB
	USB_Disconnect_Config();
#endif

#ifdef USE_USB	
	// ������������ USB
    Set_USBClock();
    USB_Interrupts_Config();
    USB_Init();

	printf("USB startup."); 
	NextLine();
#endif // USE_USB

}


static void ShowSystemInfo()
{
	printf("Starting system");
	NextLine();
	printf("Compiled: %s %s", __DATE__, __TIME__);
	NextLine();

	printboard();
	NextLine();
}



#ifdef USE_FULL_ASSERT
/*******************************************************************************
* Function Name  : assert_failed
* Description    : Reports the name of the source file and the source line number
*                  where the assert_param error has occurred.
* Input          : - file: pointer to the source file name
*                  - line: assert_param error line source number
* Output         : None
* Return         : None
*******************************************************************************/
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {}
}
#endif

/************* (C) COPYRIGHT 2009 Wuhan R&D Center, Embest *****END OF FILE****/
