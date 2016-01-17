// platform_config.h
// Автор: Ezhik
// Создано: 12.09.2011

#ifndef __PLATFORM_CONFIG_H
#define __PLATFORM_CONFIG_H

#include "stm32f10x.h"

// $BOARDSPECIFIC - тут необходимо задать конфигурацию платы
// сделано 3 конфига под различные платы
// Последний конфиг - дефолтный, его можно изменять, или на его примере сделать свой.
// Число светодиодов должно быть не больше 4 и они должны идти по порядку начиная с 1


// -------------------
// - Топовый уровень -
// Включаем-отключаем нужное

// Использовать светодиоды
//#define USE_LEDS

// Использовать COM-порты
//#define USE_COM

// Использовать COM-порт в качестве stdout
//#define USE_COM_STDOUT

#define USE_USB
// Также необходимо подключить/отключить компиляцию группы USB-FS_Device_Driver

#ifdef USE_USB
	// Если не Connectivity Line
	#ifndef STM32F10X_CL
		// Использовать пин для программного подключения/отключения USB
		#define USE_USB_DISCONNECT_PIN
	#endif
#endif


// Делитель: SysTick будет настроен на Clock / 1000 / SYSTICK_PRESCAL
// Или: 1мс / SYSTICK_PRESCAL
#define SYSTICK_PRESCALE	10

// - Конец -
// ---------


// Варианты конфигурации COM1 COM2

// COMx_USE_USART1_DEFAULT_PA9_PA10
// COMx_USE_USART1_REMAP_PB6_PB7

// COMx_USE_USART2_DEFAULT_PA2_PA3
// COMx_USE_USART2_REMAP_PD5_PD6

// COMx_USE_USART3_DEFAULT_PB10_PB11
// COMx_USE_USART3_REMAP1_PC10_PC11
// COMx_USE_USART3_REMAP2_PD8_PD9


// STM32 Development Board MINI (512K Flash 64K SRAM) 2.4-inch QVGA TFT module
// http://www.ebay.com/itm/Mini-STM32-Board-STM32F103VET6-2-4-LCD-Touch-Panel-JTAG-/200575289157
// http://www.mcueasy.com/product_detail.php?product_id=55
#if defined STRIVE_BOARD

	#ifdef USE_USB_DISCONNECT_PIN
		#define USB_DISCONNECT						GPIOC  
		#define USB_DISCONNECT_PIN					GPIO_Pin_13
		#define USB_DISCONNECT_LOG1					DISABLE
		#define RCC_APB2Periph_GPIO_DISCONNECT		RCC_APB2Periph_GPIOC
	#endif

	#ifdef USE_LEDS
		#define LED1								0
		#define LED1_GPIO_PORT						GPIOB
		#define LED1_GPIO_CLK						RCC_APB2Periph_GPIOB  
		#define LED1_GPIO_PIN						GPIO_Pin_5
	#endif

	#ifdef USE_COM
		#define COM1_USE_USART1_DEFAULT_PA9_PA10

		#ifdef USE_COM_STDOUT
			#define COM1_AS_STDOUT
		#endif
	#endif

// EMBEST EM-STM32C
// http://www.terraelectronica.ru/catalog_info.php?ID=1001&CODE=557292
#elif defined EM_STM3210C_BOARD

	#ifdef USE_LEDS
		#define LED1								0
		#define LED1_GPIO_PORT						GPIOE
		#define LED1_GPIO_CLK						RCC_APB2Periph_GPIOE  
		#define LED1_GPIO_PIN						GPIO_Pin_2
	
		#define LED2								1
		#define LED2_GPIO_PORT						GPIOE
		#define LED2_GPIO_CLK						RCC_APB2Periph_GPIOE  
		#define LED2_GPIO_PIN						GPIO_Pin_3
	
		#define LED3								2
		#define LED3_GPIO_PORT						GPIOE
		#define LED3_GPIO_CLK						RCC_APB2Periph_GPIOE  
		#define LED3_GPIO_PIN						GPIO_Pin_4
	
		#define LED4								3
		#define LED4_GPIO_PORT						GPIOE
		#define LED4_GPIO_CLK						RCC_APB2Periph_GPIOE  
		#define LED4_GPIO_PIN						GPIO_Pin_5
	#endif

	#ifdef USE_COM
		#define COM1_USE_USART2_REMAP_PD5_PD6

		#ifdef USE_COM_STDOUT
			#define COM1_AS_STDOUT
		#endif
	#endif

// TE-STM32F103 - «Махаон»
// http://www.terraelectronica.ru/catalog_info.php?ID=1001&CODE=333824
#elif defined TE_STM32F103_BOARD

	#ifdef USE_USB_DISCONNECT_PIN
		#define USB_DISCONNECT						GPIOB  
		#define USB_DISCONNECT_PIN					GPIO_Pin_5
		#define USB_DISCONNECT_LOG1					DISABLE
		#define RCC_APB2Periph_GPIO_DISCONNECT		RCC_APB2Periph_GPIOB
	#endif

	#ifdef USE_LEDS
		#define LED1								0
		#define LED1_GPIO_PORT						GPIOA
		#define LED1_GPIO_CLK						RCC_APB2Periph_GPIOA  
		#define LED1_GPIO_PIN						GPIO_Pin_4
	
		#define LED2								1
		#define LED2_GPIO_PORT						GPIOA
		#define LED2_GPIO_CLK						RCC_APB2Periph_GPIOA  
		#define LED2_GPIO_PIN						GPIO_Pin_5
	
		#define LED3								2
		#define LED3_GPIO_PORT						GPIOA
		#define LED3_GPIO_CLK						RCC_APB2Periph_GPIOA  
		#define LED3_GPIO_PIN						GPIO_Pin_6
	#endif

	#ifdef USE_COM
		#define COM1_USE_USART1_DEFAULT_PA9_PA10

		#ifdef USE_COM_STDOUT
			#define COM1_AS_STDOUT
		#endif
		#define COM1_AS_STDOUT
	#endif

#else // Дефолтная конфигурация - сделано под TE-STM32F103_BOARD

	#ifdef USE_USB_DISCONNECT_PIN
		#define USB_DISCONNECT						GPIOB  
		#define USB_DISCONNECT_PIN					GPIO_Pin_5
		#define USB_DISCONNECT_LOG1					DISABLE
		#define RCC_APB2Periph_GPIO_DISCONNECT		RCC_APB2Periph_GPIOB
	#endif

	#ifdef USE_LEDS
		#define LED1								0
		#define LED1_GPIO_PORT						GPIOA
		#define LED1_GPIO_CLK						RCC_APB2Periph_GPIOA  
		#define LED1_GPIO_PIN						GPIO_Pin_8
	
		#define LED2								1
		#define LED2_GPIO_PORT						GPIOA
		#define LED2_GPIO_CLK						RCC_APB2Periph_GPIOA  
		#define LED2_GPIO_PIN						GPIO_Pin_9
	
		#define LED3								2
		#define LED3_GPIO_PORT						GPIOB
		#define LED3_GPIO_CLK						RCC_APB2Periph_GPIOA  
		#define LED3_GPIO_PIN						GPIO_Pin_15
	#endif

	#ifdef USE_COM
		#define COM1_USE_USART1_DEFAULT_PA9_PA10

		#ifdef USE_COM_STDOUT
			#define COM1_AS_STDOUT
		#endif
	#endif

#endif

// ------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------

// Дальше ничего редактировать не нужно

// Собранные в одном месте все возможные конфигурации для USART

// ------------------
// COM1
// USART 1

#ifdef COM1_USE_USART1_DEFAULT_PA9_PA10
	#define COM1_USART1_USE_INTERRUPT_HANDLER
	#define COM1       	    	      		    USART1
	#define COM1_IRQ  	    	      		    USART1_IRQn
	#define COM1_GPIO   	          		    GPIOA
	#define COM1_CLK_APB              		    2
	#define COM1_CLK               	 		    RCC_APB2Periph_USART1
	#define COM1_GPIO_CLK               	    RCC_APB2Periph_GPIOA
	#define COM1_GPIO_REMAP						0
	#define COM1_TxPin                  	    GPIO_Pin_9
	#define COM1_RxPin                  	    GPIO_Pin_10  
#endif

#ifdef COM1_USE_USART1_REMAP_PB6_PB7
	#define COM1_USART1_USE_INTERRUPT_HANDLER
	#define COM1       	    	      		    USART1
	#define COM1_IRQ  	    	      		    USART1_IRQn
	#define COM1_GPIO   	          		    GPIOB
	#define COM1_CLK_APB              		    2
	#define COM1_CLK               	 		    RCC_APB2Periph_USART1
	#define COM1_GPIO_CLK               	    RCC_APB2Periph_GPIOB
	#define COM1_GPIO_REMAP						GPIO_Remap_USART1
	#define COM1_TxPin                  	    GPIO_Pin_6
	#define COM1_RxPin                  	    GPIO_Pin_7  
#endif

// COM1
// USART 2


#ifdef COM1_USE_USART2_DEFAULT_PA2_PA3
	#define COM1_USART2_USE_INTERRUPT_HANDLER
	#define COM1       	    	        	    USART2
	#define COM1_IRQ  	    	       		    USART2_IRQn
	#define COM1_GPIO   	            	  	GPIOA
	#define COM1_CLK_APB               		    1
	#define COM1_CLK               	   		    RCC_APB1Periph_USART2
	#define COM1_GPIO_CLK               	    RCC_APB2Periph_GPIOA
	#define COM1_GPIO_REMAP						0
	#define COM1_TxPin		                    GPIO_Pin_2
	#define COM1_RxPin      		            GPIO_Pin_3
#endif

#ifdef COM1_USE_USART2_REMAP_PD5_PD6
	#define COM1_USART2_USE_INTERRUPT_HANDLER
	#define COM1       	    	        	    USART2
	#define COM1_IRQ  	    	       		    USART2_IRQn
	#define COM1_GPIO   	            	  	GPIOD
	#define COM1_CLK_APB               		    1
	#define COM1_CLK               	   		    RCC_APB1Periph_USART2
	#define COM1_GPIO_CLK               	    RCC_APB2Periph_GPIOD
	#define COM1_GPIO_REMAP						GPIO_Remap_USART2
	#define COM1_TxPin		                    GPIO_Pin_5
	#define COM1_RxPin      		            GPIO_Pin_6
#endif

// COM1
// USART 3
 
#ifdef COM1_USE_USART3_DEFAULT_PB10_PB11
	#define COM1_USART3_USE_INTERRUPT_HANDLER
	#define COM1       	    	        	    USART3
	#define COM1_IRQ  	    	       		    USART3_IRQn
	#define COM1_GPIO   	            	  	GPIOB
	#define COM1_CLK_APB               		    1
	#define COM1_CLK               	   		    RCC_APB1Periph_USART3
	#define COM1_GPIO_CLK               	    RCC_APB2Periph_GPIOB
	#define COM1_GPIO_REMAP						0
	#define COM1_TxPin		                    GPIO_Pin_10
	#define COM1_RxPin      		            GPIO_Pin_11
#endif

#ifdef COM1_USE_USART3_REMAP1_PC10_PC11
	#define COM1_USART3_USE_INTERRUPT_HANDLER
	#define COM1       	    	        	    USART3
	#define COM1_IRQ  	    	       		    USART3_IRQn
	#define COM1_GPIO   	            	  	GPIOC
	#define COM1_CLK_APB               		    1
	#define COM1_CLK               	   		    RCC_APB1Periph_USART3
	#define COM1_GPIO_CLK               	    RCC_APB2Periph_GPIOC
	#define COM1_GPIO_REMAP						GPIO_PartialRemap_USART3
	#define COM1_TxPin		                    GPIO_Pin_10
	#define COM1_RxPin      		            GPIO_Pin_11
#endif

#ifdef COM1_USE_USART3_REMAP2_PD8_PD9
	#define COM1_USART3_USE_INTERRUPT_HANDLER										
	#define COM1       	    	        	    USART3
	#define COM1_IRQ  	    	       		    USART3_IRQn
	#define COM1_GPIO   	            	  	GPIOC
	#define COM1_CLK_APB               		    1
	#define COM1_CLK               	   		    RCC_APB1Periph_USART3
	#define COM1_GPIO_CLK               	    RCC_APB2Periph_GPIOC
	#define COM1_GPIO_REMAP						GPIO_FullRemap_USART3
	#define COM1_TxPin		                    GPIO_Pin_8
	#define COM1_RxPin      		            GPIO_Pin_9
#endif


#ifdef USE_COM_STDOUT
	#ifdef COM1_AS_STDOUT
		#define STDOUT_COM							COM1
	#endif
#endif

// ------------------
// COM2
// USART 1

#ifdef COM2_USE_USART1_DEFAULT_PA9_PA10
	#ifdef COM1_USE_USART1_DEFAULT_PA9_PA10
	#error "COM2 and COM1 are same!"
	#endif

	#define COM2_USART1_USE_INTERRUPT_HANDLER
	#define COM2       	    	      		    USART1
	#define COM2_IRQ  	    	      		    USART1_IRQn
	#define COM2_GPIO   	          		    GPIOA
	#define COM2_CLK_APB              		    2
	#define COM2_CLK               	 		    RCC_APB2Periph_USART1
	#define COM2_GPIO_CLK               	    RCC_APB2Periph_GPIOA
	#define COM2_GPIO_REMAP						0
	#define COM2_TxPin                  	    GPIO_Pin_9
	#define COM2_RxPin                  	    GPIO_Pin_10  
#endif

#ifdef COM2_USE_USART1_REMAP_PB6_PB7
	#ifdef COM1_USE_USART1_REMAP_PB6_PB7
	#error "COM2 and COM1 are same!"
	#endif

	#define COM2_USART1_USE_INTERRUPT_HANDLER
	#define COM2       	    	      		    USART1
	#define COM2_IRQ  	    	      		    USART1_IRQn
	#define COM2_GPIO   	          		    GPIOB
	#define COM2_CLK_APB              		    2
	#define COM2_CLK               	 		    RCC_APB2Periph_USART1
	#define COM2_GPIO_CLK               	    RCC_APB2Periph_GPIOB
	#define COM2_GPIO_REMAP						GPIO_Remap_USART1
	#define COM2_TxPin                  	    GPIO_Pin_6
	#define COM2_RxPin                  	    GPIO_Pin_7  
#endif

// COM2
// USART 2

#ifdef COM2_USE_USART2_DEFAULT_PA2_PA3
	#ifdef COM1_USE_USART2_DEFAULT_PA2_PA3
	#error "COM2 and COM1 are same!"
	#endif

	#define COM2_USART2_USE_INTERRUPT_HANDLER
	#define COM2       	    	        	    USART2
	#define COM2_IRQ  	    	       		    USART2_IRQn
	#define COM2_GPIO   	            	  	GPIOA
	#define COM2_CLK_APB               		    1
	#define COM2_CLK               	   		    RCC_APB1Periph_USART2
	#define COM2_GPIO_CLK               	    RCC_APB2Periph_GPIOA
	#define COM2_GPIO_REMAP						0
	#define COM2_TxPin		                    GPIO_Pin_2
	#define COM2_RxPin      		            GPIO_Pin_3
#endif

#ifdef COM2_USE_USART2_REMAP_PD5_PD6
	#ifdef COM1_USE_USART2_REMAP_PD5_PD6
	#error "COM2 and COM1 are same!"
	#endif

	#define COM2_USART2_USE_INTERRUPT_HANDLER
	#define COM2       	    	        	    USART2
	#define COM2_IRQ  	    	       		    USART2_IRQn
	#define COM2_GPIO   	            	  	GPIOD
	#define COM2_CLK_APB               		    1
	#define COM2_CLK               	   		    RCC_APB1Periph_USART2
	#define COM2_GPIO_CLK               	    RCC_APB2Periph_GPIOD
	#define COM2_GPIO_REMAP						GPIO_Remap_USART2
	#define COM2_TxPin		                    GPIO_Pin_5
	#define COM2_RxPin      		            GPIO_Pin_6
#endif

// COM2
// USART 3
 
#ifdef COM2_USE_USART3_DEFAULT_PB10_PB11
	#ifdef COM1_USE_USART3_DEFAULT_PB10_PB11
	#error "COM2 and COM1 are same!"
	#endif

	#define COM2_USART3_USE_INTERRUPT_HANDLER
	#define COM2       	    	        	    USART3
	#define COM2_IRQ  	    	       		    USART3_IRQn
	#define COM2_GPIO   	            	  	GPIOB
	#define COM2_CLK_APB               		    1
	#define COM2_CLK               	   		    RCC_APB1Periph_USART3
	#define COM2_GPIO_CLK               	    RCC_APB2Periph_GPIOB
	#define COM2_GPIO_REMAP						0
	#define COM2_TxPin		                    GPIO_Pin_10
	#define COM2_RxPin      		            GPIO_Pin_11
#endif

#ifdef COM2_USE_USART3_REMAP1_PC10_PC11
	#ifdef COM1_USE_USART3_REMAP1_PC10_PC11
	#error "COM2 and COM1 are same!"
	#endif

	#define COM2_USART3_USE_INTERRUPT_HANDLER
	#define COM2       	    	        	    USART3
	#define COM2_IRQ  	    	       		    USART3_IRQn
	#define COM2_GPIO   	            	  	GPIOC
	#define COM2_CLK_APB               		    1
	#define COM2_CLK               	   		    RCC_APB1Periph_USART3
	#define COM2_GPIO_CLK               	    RCC_APB2Periph_GPIOC
	#define COM2_GPIO_REMAP						GPIO_PartialRemap_USART3
	#define COM2_TxPin		                    GPIO_Pin_10
	#define COM2_RxPin      		            GPIO_Pin_11
#endif

#ifdef COM2_USE_USART3_REMAP2_PD8_PD9
	#ifdef COM1_USE_USART3_REMAP2_PD8_PD9
	#error "COM2 and COM1 are same!"
	#endif
								  
	#define COM2_USART3_USE_INTERRUPT_HANDLER										
	#define COM2       	    	        	    USART3
	#define COM2_IRQ  	    	       		    USART3_IRQn
	#define COM2_GPIO   	            	  	GPIOC
	#define COM2_CLK_APB               		    1
	#define COM2_CLK               	   		    RCC_APB1Periph_USART3
	#define COM2_GPIO_CLK               	    RCC_APB2Periph_GPIOC
	#define COM2_GPIO_REMAP						GPIO_FullRemap_USART3
	#define COM2_TxPin		                    GPIO_Pin_8
	#define COM2_RxPin      		            GPIO_Pin_9
#endif

#ifdef USE_COM_STDOUT
	#ifdef COM2_AS_STDOUT
		#ifdef COM1_AS_STDOUT
		#error "COM1 already defined as STDOUT"
		#endif
	
		#define STDOUT_COM							COM2
	#endif
#endif


// Подсчет количества светодиодов

#if (defined LED1) && (defined LED2) && (defined LED3) && (defined LED4)
	#define LEDS_NUM							4 
#elif (defined LED1) && (defined LED2) && (defined LED3)
	#define LEDS_NUM							3
#elif (defined LED1) && (defined LED2)
	#define LEDS_NUM							2
#elif (defined LED1)
	#define LEDS_NUM							1
#else
	#define LEDS_NUM							0
#endif

#endif /* __PLATFORM_CONFIG_H */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
