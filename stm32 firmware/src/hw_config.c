// hw_config.c
// Автор: Ezhik
// Создано: 12.09.2011
// Использован шаблон от MCD Application Team (thx very much!)
// Функции для конфигурации периферии
// Вариант для TE

#include "platform_config.h"

#include "stm32f10x_it.h"
#include "usb_desc.h"
#include "hw_config.h"

#ifdef USE_USB
#include "usb_lib.h"
#include "usb_pwr.h"
#endif

#include <stdio.h>

#include "serv_led.h"

extern __IO uint8_t PrevXferComplete;

// Локальные переменные
ErrorStatus HSEStartUpStatus;

uint8_t Buffer[RPT4_COUNT+1];
 
#ifdef USE_COM
static void InitCOM1();
static void InitCOM2();

static void COM1_RcvHandler(char c);
static void COM2_RcvHandler(char c);
#endif


// -------------------------------------------------
// COM

#ifdef USE_COM
void InitCOM(void)
{
	InitCOM1();
	InitCOM2();
}

void InitCOM1()
{
	#ifdef COM1

    NVIC_InitTypeDef NVIC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

	RCC_APB2PeriphClockCmd(COM1_GPIO_CLK | RCC_APB2Periph_AFIO, ENABLE);

	if(COM1_GPIO_REMAP != 0)
	{
		GPIO_PinRemapConfig(COM1_GPIO_REMAP, ENABLE);
	}

	switch(COM1_CLK_APB)
	{
		case 1:
			RCC_APB1PeriphClockCmd(COM1_CLK, ENABLE);
			break;
		case 2:
		default:
			RCC_APB2PeriphClockCmd(COM1_CLK, ENABLE);
			break;
	}
	

	// TX как Alternate Function Push-Pull
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = COM1_TxPin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(COM1_GPIO, &GPIO_InitStructure);

	// RX как Floating Input
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = COM1_RxPin;
	GPIO_Init(COM1_GPIO, &GPIO_InitStructure);

	// 115200 бод, 8 бит, 1 стоп, N четность, без контроля, разрешены прием и передача
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(COM1, &USART_InitStructure);

	// Конфигурация прерывания
	NVIC_InitStructure.NVIC_IRQChannel = COM1_IRQ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	// Включаем прерывание на получение
	USART_ITConfig(COM1, USART_IT_RXNE, ENABLE);

	
	USART_Cmd(COM1, ENABLE);

	#endif

}

void InitCOM2()
{
	#ifdef COM2

    NVIC_InitTypeDef NVIC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

	RCC_APB2PeriphClockCmd(COM2_GPIO_CLK | RCC_APB2Periph_AFIO, ENABLE);

	if(COM2_GPIO_REMAP != 0)
	{
		GPIO_PinRemapConfig(COM2_GPIO_REMAP, ENABLE);
	}

	switch(COM2_CLK_APB)
	{
		case 1:
			RCC_APB1PeriphClockCmd(COM2_CLK, ENABLE);
			break;
		case 2:
		default:
			RCC_APB2PeriphClockCmd(COM2_CLK, ENABLE);
			break;
	}
	

	// TX как Alternate Function Push-Pull
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = COM2_TxPin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(COM2_GPIO, &GPIO_InitStructure);

	// RX как Floating Input
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = COM2_RxPin;
	GPIO_Init(COM2_GPIO, &GPIO_InitStructure);

	// 115200 бод, 8 бит, 1 стоп, N четность, без контроля, разрешены прием и передача
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(COM2, &USART_InitStructure);

	// Конфигурация прерывания
	NVIC_InitStructure.NVIC_IRQChannel = COM2_IRQ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	// Включаем прерывание на получение
	USART_ITConfig(COM2, USART_IT_RXNE, ENABLE);

	USART_Cmd(COM2, ENABLE);

	#endif
}

// Обработчик прерывания USART, в данном случае - получения байта
#if (defined COM1_USART1_USE_INTERRUPT_HANDLER) || (defined COM2_USART1_USE_INTERRUPT_HANDLER)
void USART1_IRQHandler()
{
	char c = USART_ReceiveData(USART1);
	#ifdef COM1_USART1_USE_INTERRUPT_HANDLER
			COM1_RcvHandler(c);
	#endif
	#ifdef COM2_USART1_USE_INTERRUPT_HANDLER
			COM2_RcvHandler(c);
	#endif
}
#endif

#if (defined COM1_USART2_USE_INTERRUPT_HANDLER) || (defined COM2_USART2_USE_INTERRUPT_HANDLER)
void USART2_IRQHandler()
{
	char c = USART_ReceiveData(USART2);
	#ifdef COM1_USART2_USE_INTERRUPT_HANDLER
			COM1_RcvHandler(c);
	#endif
	#ifdef COM2_USART2_USE_INTERRUPT_HANDLER
			COM2_RcvHandler(c);
	#endif
}
#endif

#if (defined COM1_USART3_USE_INTERRUPT_HANDLER) || (defined COM2_USART3_USE_INTERRUPT_HANDLER)
void USART3_IRQHandler()
{
	char c = USART_ReceiveData(USART3);
	#ifdef COM1_USART3_USE_INTERRUPT_HANDLER
			COM1_RcvHandler(c);
	#endif
	#ifdef COM2_USART3_USE_INTERRUPT_HANDLER
			COM2_RcvHandler(c);
	#endif
}
#endif
		 
// Обработка пришедшего байта в COM1
void COM1_RcvHandler(char c)
{
	#ifdef COM1_AS_STDOUT
		printf("%c", c);
	#else
	// $COM - Обработка
	#endif
}

// Обработка пришедшего байта в COM2
void COM2_RcvHandler(char c)
{
	#ifdef COM2_AS_STDOUT
		printf("%c", c);
	#else
	// $COM - Обработка
	#endif
}

// Обычная посылка данных по COM
void COM_SendData(USART_TypeDef* USARTx, uint8_t *data, int length)
{
	int i;

	for(i=0; i < length;i++)
	{
		USART_SendData(USARTx, (uint8_t) data[i]);
	
		while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET)
		{}
	}
}

#else	// USE_COM
#define USART1_IRQHandler Dummy
#define USART2_IRQHandler Dummy
#define USART3_IRQHandler Dummy

void Dummy() {}

#endif	// USE_COM


// -------------------------------------------------
// LED

#ifdef USE_LEDS
// Конфигурация индикаторов 
void Led_Config(void)
{
	// отсылаем к led.c, так как индикаторов может быть больше одного, а там все разрулено
	Led_Init();
}
#endif

// -------------------------------------------------
// USB

#ifdef USE_USB
// Задаем клоки USB (48 МГц)
void Set_USBClock(void)
{
#if defined(STM32F10X_CL)
  /* Select USBCLK source */
  RCC_OTGFSCLKConfig(RCC_OTGFSCLKSource_PLLVCO_Div3);

  /* Enable the USB clock */ 
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_OTG_FS, ENABLE) ;
#else
  /* Select USBCLK source */
  RCC_USBCLKConfig(RCC_USBCLKSource_PLLCLK_1Div5);
  
  /* Enable the USB clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USB, ENABLE);
#endif 
  
}

// Конфигурация USB-прерываний
void USB_Interrupts_Config(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

#if defined(STM32F10X_CL) 
  NVIC_InitStructure.NVIC_IRQChannel = OTG_FS_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
#else
  NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  NVIC_InitStructure.NVIC_IRQChannel = USB_HP_CAN1_TX_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
#endif /* STM32F10X_CL */
  
}


// Программный коннект-дисконнект USB-кабеля
void USB_Cable_Config (FunctionalState NewState)
{
#if defined(STM32F10X_CL) 
  if (NewState != DISABLE)
  {
    USB_DevConnect();
  }
  else
  {
    USB_DevDisconnect();
  }
  
#else 
#ifdef USB_DISCONNECT
  if (NewState != USB_DISCONNECT_LOG1)
  {
    GPIO_ResetBits(USB_DISCONNECT, USB_DISCONNECT_PIN);
  }
  else
  {
    GPIO_SetBits(USB_DISCONNECT, USB_DISCONNECT_PIN);
  }
#endif
#endif
}

// Конфигурация пина, отвечающего за программный коннект-дисконнект USB
// RCC_APB2Periph_GPIO_DISCONNECT, USB_DISCONNECT, USB_DISCONNECT_PIN необходимо править в platform_config.h
void USB_Disconnect_Config(void)
{
#ifdef USB_DISCONNECT
  GPIO_InitTypeDef GPIO_InitStructure;

  // Включаем нужный клок
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIO_DISCONNECT, ENABLE);


  // Пин конфигурируется как Out Push-Pull
  GPIO_SetBits(USB_DISCONNECT, USB_DISCONNECT_PIN);
  GPIO_InitStructure.GPIO_Pin = USB_DISCONNECT_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(USB_DISCONNECT, &GPIO_InitStructure);

  // Начальное значение - лог. 1. Соответствует дисконнекту USB.
  GPIO_SetBits(USB_DISCONNECT, USB_DISCONNECT_PIN);
#endif
}

#ifdef STM32F10X_CL
/*******************************************************************************
* Function Name  : USB_OTG_BSP_uDelay.
* Description    : provide delay.
* Input          : Delay in micro seconds.
* Output         : None.
* Return         : None.
*******************************************************************************/
void USB_OTG_BSP_uDelay (const uint32_t usec)
{
/*
  RCC_ClocksTypeDef  RCC_Clocks;  

  /* Configure HCLK clock as SysTick clock source 
  SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
  
  RCC_GetClocksFreq(&RCC_Clocks);
  
  SysTick_Config(usec * (RCC_Clocks.HCLK_Frequency / 1000000));  
  
  SysTick->CTRL  &= ~SysTick_CTRL_TICKINT_Msk ;
  
  while (!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk));
*/
	
	int i;
	for(i=0;i<SystemCoreClock / 1000000;i++);
	// да ну нафиг эти микрозадеожки, только жить мешают. да еще и мой systick трогают
}
#endif

// Режим низкого энергопотребления. Лучше всего тут отключить все лишние клоки.
void Enter_LowPowerMode(void)
{
  // Переводим состояние девайса в Suspended
  bDeviceState = SUSPENDED;
}

// Выход из режима низкого энергопотребления. Включить клоки обратно.
void Leave_LowPowerMode(void)
{
  DEVICE_INFO *pInfo = &Device_Info;

  // Задаем правильное состояние девайса
  if (pInfo->Current_Configuration != 0)
  {
    bDeviceState = CONFIGURED;
  }
  else
  {
    bDeviceState = ATTACHED;
  }

}

// Создание серийного номера устройства.
// Не обязательно. Только при использовании с VID, PID и Windows-драйвером от STM
void Get_SerialNum(void)
{
  /* 
  uint32_t Device_Serial0, Device_Serial1, Device_Serial2;

  Device_Serial0 = *(__IO uint32_t*)(0x1FFFF7E8);
  Device_Serial1 = *(__IO uint32_t*)(0x1FFFF7EC);
  Device_Serial2 = *(__IO uint32_t*)(0x1FFFF7F0);

  if (Device_Serial0 != 0)
  {
    SomeDev_StringSerial[2] = (uint8_t)(Device_Serial0 & 0x000000FF);
    SomeDev_StringSerial[4] = (uint8_t)((Device_Serial0 & 0x0000FF00) >> 8);
    SomeDev_StringSerial[6] = (uint8_t)((Device_Serial0 & 0x00FF0000) >> 16);
    SomeDev_StringSerial[8] = (uint8_t)((Device_Serial0 & 0xFF000000) >> 24);

    SomeDev_StringSerial[10] = (uint8_t)(Device_Serial1 & 0x000000FF);
    SomeDev_StringSerial[12] = (uint8_t)((Device_Serial1 & 0x0000FF00) >> 8);
    SomeDev_StringSerial[14] = (uint8_t)((Device_Serial1 & 0x00FF0000) >> 16);
    SomeDev_StringSerial[16] = (uint8_t)((Device_Serial1 & 0xFF000000) >> 24);

    SomeDev_StringSerial[18] = (uint8_t)(Device_Serial2 & 0x000000FF);
    SomeDev_StringSerial[20] = (uint8_t)((Device_Serial2 & 0x0000FF00) >> 8);
    SomeDev_StringSerial[22] = (uint8_t)((Device_Serial2 & 0x00FF0000) >> 16);
    SomeDev_StringSerial[24] = (uint8_t)((Device_Serial2 & 0xFF000000) >> 24);
  }
  */
}

#endif // USE_USB


// ---------------------------------------------------------

// Конфигурация клока и питания
void RCC_Config(void)
{ 
  // Конфигурация SYSCLK, HCLK, PCLK2 и PCLK1 
  RCC_DeInit();
  RCC_HSEConfig(RCC_HSE_ON);
  HSEStartUpStatus = RCC_WaitForHSEStartUp();

  if (HSEStartUpStatus == SUCCESS)
  {
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
    FLASH_SetLatency(FLASH_Latency_2);
    RCC_HCLKConfig(RCC_SYSCLK_Div1); 
    RCC_PCLK2Config(RCC_HCLK_Div1); 
    RCC_PCLK1Config(RCC_HCLK_Div2);

#ifdef STM32F10X_CL	// обычно к 105-107 сериям подключают кварц в 25 МГц, в то время как к остальным - 8 МГц
    RCC_PREDIV2Config(RCC_PREDIV2_Div5);
    RCC_PLL2Config(RCC_PLL2Mul_8);
    RCC_PLL2Cmd(ENABLE);
    while (RCC_GetFlagStatus(RCC_FLAG_PLL2RDY) == RESET)
    {}
    RCC_PREDIV1Config(RCC_PREDIV1_Source_PLL2, RCC_PREDIV1_Div5);
    RCC_PLLConfig(RCC_PLLSource_PREDIV1, RCC_PLLMul_9);
#else
    // PLLCLK = 12MHz * 6 = 72 MHz
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_6);
#endif

    RCC_PLLCmd(ENABLE);

    while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
    {
    }

    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    while(RCC_GetSYSCLKSource() != 0x08)
    {
    }
  }
  else
  {
  	// Если ошибка старта HSE, будет неверная конфигурация клока 
	// Можно эту ситуацию тут обработать

    while (1)
    {
    }
  }
}			

// Конец файла

uint16_t btn1_prev, btn2_prev;
uint8_t Buffer[RPT4_COUNT+1];
uint8_t RHIDCheckState(void)
{
    uint16_t btn1=0, btn2=0;
    //btn1 = 0;//GPIO_ReadInputDataBit(BTN1_PORT, BTN1_PIN);
    //btn2 = 1;//GPIO_ReadInputDataBit(BTN2_PORT, BTN2_PIN);
    Buffer[0] = 4;
   // Buffer[1] = btn1;
   // Buffer[2] = btn2;
    //Buffer[3] = (GPIO_ReadInputDataBit(LED_PORT, LED1_PIN) | GPIO_ReadInputDataBit(LED_PORT, LED2_PIN)<<1);
		GetDataToPc( Buffer );
    /* Reset the control token to inform upper layer that a transfer is ongoing */
    PrevXferComplete = 0;

    /* Copy mouse position info in ENDP1 Tx Packet Memory Area*/
    USB_SIL_Write(EP1_IN, Buffer, RPT4_COUNT+1);
    /* Enable endpoint for transmission */
    SetEPTxValid(ENDP1);

    return (btn1 | btn2<<1);
}

/*******************************************************************************
* Function Name : RHID_Send.
* Description   : prepares buffer to be sent containing event infos.
* Input         : Keys: keys received from terminal.
* Output        : None.
* Return value  : None.
*******************************************************************************/
void RHID_Send(uint8_t report, uint8_t state)
{
  uint8_t Buffer[2] = {0, 0};

  Buffer[0] = report;
  Buffer[1] = state;

  /* Reset the control token to inform upper layer that a transfer is ongoing */
  PrevXferComplete = 0;

  /* Copy buttons data in ENDP1 Tx Packet Memory Area*/
  USB_SIL_Write(EP1_IN, Buffer, 2);
  /* Enable endpoint for transmission */
  SetEPTxValid(ENDP1);
}
