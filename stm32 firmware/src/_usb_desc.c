// usb_desc.c
// Автор: Ezhik
// Создано: 12.09.2011
// Использован шаблон от MCD Application Team (thx very much!)
// Дескрипторы USB-девайса
// Вариант для TE


#include "platform_config.h"

#ifdef USE_USB

#include "usb_desc.h"

// $USBCONFIG - тут надо правильно расписать все дескрипторы
// Текущая конфигурация: 
// 1 конфигурация, 1 интерфейс
// 4 bulk-ендпойнта: 0x81 in, 0x02 out, 0x83 in, 0x04 out
// при изменении любого дескриптора также необходимо поправить его размер в файле usb_desc.h

const uint8_t SomeDev_DeviceDescriptor[SomeDev_SIZ_DEVICE_DESC] =
  {
    0x12,   /* bLength  */
    0x01,   /* bDescriptorType */
    0x00,   /* bcdUSB, version 2.00 */
    0x02,
    0xff,   /* bDeviceClass : each interface define the device class */
    0xff,   /* bDeviceSubClass */
    0xff,   /* bDeviceProtocol */
    0x40,   /* bMaxPacketSize0 0x40 = 64 */
    
	// $USBCONFIG - сюда вставить vendor ID (VID). Его можно купить у usb.org за несколько тысяч долларов :)
	// В данном примере используется VID от ST Microelectrinics
	// У них на каких-то условияъ можно получить PID для своего устройства
	0x83,   /* idVendor     (0483) */
    0x04,
	// $USBCONFIG - сюда вставить product ID (PID)
	// В данном примере _ТОЛЬКО С ЦЕЛЬЮ ОЗНАКОМЛЕНИЯ И ТЕСТИРОВАНИЯ_ используется PID "от балды".
    0xF0,   /* idProduct	(FFF0) */
    0xFF,
    0x00,   /* bcdDevice 2.00*/
    0x02,
    1,              /* index of string Manufacturer  */
    /**/
    2,              /* index of string descriptor of product*/
    /* */
    3,              /* */
    /* */
    /* */
    0x01    /*bNumConfigurations */
  };
const uint8_t SomeDev_ConfigDescriptor[SomeDev_SIZ_CONFIG_DESC] =
  {

    0x09,   /* bLength: Configuation Descriptor size */
    0x02,   /* bDescriptorType: Configuration */
    SomeDev_SIZ_CONFIG_DESC,

    0x00,
    0x01,   /* bNumInterfaces: 1 interface */
    0x01,   /* bConfigurationValue: */
    /*      Configuration value */
    0x00,   /* iConfiguration: */
    /*      Index of string descriptor */
    /*      describing the configuration */
    0xC0,   /* bmAttributes: */
    /*      bus powered */
    0x32,   /* MaxPower 100 mA */

    /******************** Descriptor of Mass Storage interface ********************/
    /* 09 */
    0x09,   /* bLength: Interface Descriptor size */
    0x04,   /* bDescriptorType: */
    /*      Interface descriptor type */
    0x00,   /* bInterfaceNumber: Number of Interface */
    0x00,   /* bAlternateSetting: Alternate setting */
    0x02,   /* bNumEndpoints*/
    0xff,   /* bInterfaceClass */
    0xff,   /* bInterfaceSubClass */
    0xff,   /* nInterfaceProtocol */
    4,          /* iInterface: */
    /* 18 */
    0x07,   /*Endpoint descriptor length = 7*/
    0x05,   /*Endpoint descriptor type */
    0x81,   /*Endpoint address (IN, address 1) */
    0x02,   /*Bulk endpoint type */
    0x40,   /*Maximum packet size (64 bytes) */
    0x00,
    0x00,   /*Polling interval in milliseconds */
    /* 25 */
    0x07,   /*Endpoint descriptor length = 7 */
    0x05,   /*Endpoint descriptor type */
    0x02,   /*Endpoint address (OUT, address 2) */
    0x02,   /*Bulk endpoint type */
    0x40,   /*Maximum packet size (64 bytes) */
    0x00,
    0x00,     /*Polling interval in milliseconds*/
    /*32*/
	// */
  };


// Строковые идентификаторы

// Язык
const uint8_t SomeDev_StringLangID[SomeDev_SIZ_STRING_LANGID] =
  {
    SomeDev_SIZ_STRING_LANGID,
    0x03,
    0x09,
    0x04
  };      /* LangID = 0x0409: U.S. English */

// Vendor
const uint8_t SomeDev_StringVendor[SomeDev_SIZ_STRING_VENDOR] =
  {
    SomeDev_SIZ_STRING_VENDOR, /* Size of manufaturer string */
    0x03,           /* bDescriptorType = String descriptor */
    /* Manufacturer */
    'O', 0, 
	'A', 0, 
	'O', 0, 
	' ', 0, 
	'V', 0, 
	'E', 0, 
	'N', 0, 
	'D', 0,
	'O', 0,
	'R', 0
  };

// Продукт
const uint8_t SomeDev_StringProduct[SomeDev_SIZ_STRING_PRODUCT] =
  {
    SomeDev_SIZ_STRING_PRODUCT,
    0x03,
    /* Product name */
    'S', 0,
	'a', 0,
	'm', 0,
	'p', 0,
	'l', 0, 
	'e', 0, 
	' ', 0, 
	's', 0,
    'o', 0, 
	'm', 0, 
	'e', 0, 
	' ', 0, 
	'd', 0, 
	'e', 0, 
	'v', 0, 
	'i', 0, 
	'c', 0, 
	'e', 0

  };

// $BOARDSPECIFIC - Серийный номер
uint8_t SomeDev_StringSerial[SomeDev_SIZ_STRING_SERIAL] =
  {
    SomeDev_SIZ_STRING_SERIAL,
    0x03,
    /* Serial number */
#if defined STRIVE_BOARD
    'S', 0,
	'T', 0,
	'R', 0,
	'V', 0,
	'0', 0,
	'0', 0,
	'2', 0
#elif defined EM_STM3210C_BOARD
    'E', 0,
	'M', 0,
	'B', 0,
	'C', 0,
	'0', 0,
	'0', 0,
	'2', 0
#elif defined TE_STM32F103_BOARD
    'T', 0,
	'E', 0,
	'R', 0,
	'3', 0,
	'0', 0,
	'0', 0,
	'2', 0
#else
    'D', 0,
	'E', 0,
	'F', 0,
	'T', 0,
	'0', 0,
	'0', 0,
	'2', 0
#endif
  };

// Интерфейс
const uint8_t SomeDev_StringInterface[SomeDev_SIZ_STRING_INTERFACE] =
  {
    SomeDev_SIZ_STRING_INTERFACE,
    0x03,
    /* Interface 0 */
    'I', 0,
	'n', 0,
	't', 0,
	'e', 0,
	'r', 0,
	'f', 0,
	'a', 0,
	'c', 0,
	'e', 0
  };


#endif USE_USB

