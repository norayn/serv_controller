// usb_prop.c
// Автор: Ezhik
// Создано: 12.09.2011
// Использован шаблон от MCD Application Team (thx very much!)
// Обработчики управляющих запросов USB
// Вариант для TE

#include "platform_config.h"

#ifdef USE_USB

#include "usb_lib.h"
#include "usb_desc.h"
#include "usb_pwr.h"
#include "hw_config.h"
#include "usb_prop.h"


DEVICE Device_Table =
{
	EP_NUM,
	1
};

DEVICE_PROP Device_Property =
{
	SomeDev_init,
	SomeDev_Reset,
	SomeDev_Status_In,
	SomeDev_Status_Out,
	SomeDev_Data_Setup,
	SomeDev_NoData_Setup,
	SomeDev_Get_Interface_Setting,
	SomeDev_GetDeviceDescriptor,
	SomeDev_GetConfigDescriptor,
	SomeDev_GetStringDescriptor,
	0,
	0x40 // Максимальный размер пакета  
};

USER_STANDARD_REQUESTS User_Standard_Requests =
{
	SomeDev_GetConfiguration,
	SomeDev_SetConfiguration,
	SomeDev_GetInterface,
	SomeDev_SetInterface,
	SomeDev_GetStatus,
	SomeDev_ClearFeature,
	SomeDev_SetEndPointFeature,
	SomeDev_SetDeviceFeature,
	SomeDev_SetDeviceAddress
};

ONE_DESCRIPTOR Device_Descriptor =
{
	(uint8_t*)SomeDev_DeviceDescriptor,
	SomeDev_SIZ_DEVICE_DESC
};

ONE_DESCRIPTOR Config_Descriptor =
{
	(uint8_t*)SomeDev_ConfigDescriptor,
	SomeDev_SIZ_CONFIG_DESC
};

ONE_DESCRIPTOR String_Descriptor[5] =
{
	{(uint8_t*)SomeDev_StringLangID, SomeDev_SIZ_STRING_LANGID},
	{(uint8_t*)SomeDev_StringVendor, SomeDev_SIZ_STRING_VENDOR},
	{(uint8_t*)SomeDev_StringProduct, SomeDev_SIZ_STRING_PRODUCT},
	{(uint8_t*)SomeDev_StringSerial, SomeDev_SIZ_STRING_SERIAL},
	{(uint8_t*)SomeDev_StringInterface, SomeDev_SIZ_STRING_INTERFACE},
};

// Инициализация USB-девайса
void SomeDev_init()
{
	// Получаем серийный номер
	Get_SerialNum();
	
	// Задаем текущую конфигурацию
	pInformation->Current_Configuration = 0;
	
	// Подключение 
	PowerOn();
	
	// Основные инициализации
	USB_SIL_Init();
	
	bDeviceState = UNCONNECTED;
}


// Ресет USB
void SomeDev_Reset()
{
	// Установка девайса в несконфигурированное состояние
	Device_Info.Current_Configuration = 0;
	
	// Инициализация CurrentFeature
	pInformation->Current_Feature = SomeDev_ConfigDescriptor[7];
	
#ifdef STM32F10X_CL   
	
	// EP0 уже сконфигурирован в USB_SIL_Init()
	
	// $USBCONFIG - инициализация ендпойнтов
	
	// EP1 IN - Bulk endpoint 
	OTG_DEV_EP_Init(EP1_IN, OTG_DEV_EP_TYPE_BULK, BULK_MAX_PACKET_SIZE);
	
	// EP2 OUT - Bulk endpoint 
	OTG_DEV_EP_Init(EP2_OUT, OTG_DEV_EP_TYPE_BULK, BULK_MAX_PACKET_SIZE); 
	
	
#else 
	
	// $USBCONFIG - инициализация ендпойнтов
	
	SetBTABLE(BTABLE_ADDRESS);
	
	// Инициализация Endpoint 0
	SetEPType(ENDP0, EP_CONTROL);
	SetEPTxStatus(ENDP0, EP_TX_NAK);
	SetEPRxAddr(ENDP0, ENDP0_RXADDR);
	SetEPRxCount(ENDP0, Device_Property.MaxPacketSize);
	SetEPTxAddr(ENDP0, ENDP0_TXADDR);
	Clear_Status_Out(ENDP0);
	SetEPRxValid(ENDP0);
	
	// Инициализация Endpoint 1
	SetEPType(ENDP1, EP_BULK);
	SetEPTxAddr(ENDP1, ENDP1_TXADDR);
	SetEPTxStatus(ENDP1, EP_TX_NAK);
	SetEPRxStatus(ENDP1, EP_RX_DIS);
	
	// Инициализация Endpoint 2
	SetEPType(ENDP2, EP_BULK);
	SetEPRxAddr(ENDP2, ENDP2_RXADDR);
	SetEPRxCount(ENDP2, Device_Property.MaxPacketSize);
	SetEPRxStatus(ENDP2, EP_RX_VALID);
	SetEPTxStatus(ENDP2, EP_TX_DIS);
	
	
	SetEPRxCount(ENDP0, Device_Property.MaxPacketSize);
	SetEPRxValid(ENDP0);
	
	// Установка дефолтного адреса
	SetDeviceAddress(0);
#endif /}
	
	bDeviceState = ATTACHED;
}


// Обработка запрса SetConfiguration
void SomeDev_SetConfiguration(void)
{
	if (pInformation->Current_Configuration != 0)
	{
		// Сконфигурирован
		bDeviceState = CONFIGURED;
		
	#ifdef STM32F10X_CL 
		
		// $USBCONFIG - инициализация ендпойнтов
		
		// EP1 IN - Bulk endpoint 
		OTG_DEV_EP_Init(EP1_IN, OTG_DEV_EP_TYPE_BULK, BULK_MAX_PACKET_SIZE);
		
		// EP2 OUT - Bulk endpoint 
		OTG_DEV_EP_Init(EP2_OUT, OTG_DEV_EP_TYPE_BULK, BULK_MAX_PACKET_SIZE);     
	#else    
		// $USBCONFIG - очистка DTOG
		
		ClearDTOG_TX(ENDP1);
		ClearDTOG_RX(ENDP2);
	#endif //

	}
}


// Обработка запроса ClearFeature
void SomeDev_ClearFeature(void)
{

}


// Обновление девайса в состояние ADDRESSED
void SomeDev_SetDeviceAddress (void)
{
	bDeviceState = ADDRESSED;
}


// Обработка Status_In
void SomeDev_Status_In(void)
{
	return;
}


// Обработка Status_Out
void SomeDev_Status_Out(void)
{
	return;
}


// Обработка Data_Setup ( для class-specific запросов ) 
RESULT SomeDev_Data_Setup(uint8_t RequestNo)
{
	return USB_SUCCESS;
}


// Обработка NoData_Setup ( для class-specific запросов ) 
RESULT SomeDev_NoData_Setup(uint8_t RequestNo)
{
	return USB_SUCCESS;
}

// Тестирование интерфейса и его альтернативной установки
RESULT SomeDev_Get_Interface_Setting(uint8_t Interface, uint8_t AlternateSetting)
{
	if (AlternateSetting > 0)
	{
		return USB_UNSUPPORT;/* in this application we don't have AlternateSetting*/
	}
	else if (Interface > 0)
	{
		return USB_UNSUPPORT;/*in this application we have only 1 interfaces*/
	}
	return USB_SUCCESS;
}


// Получает дескриптор девайса
uint8_t *SomeDev_GetDeviceDescriptor(uint16_t Length)
{
	return Standard_GetDescriptorData(Length, &Device_Descriptor );
}


// Получает дескриптор конфигурации
uint8_t *SomeDev_GetConfigDescriptor(uint16_t Length)
{
	return Standard_GetDescriptorData(Length, &Config_Descriptor );
}


// Получает строковой дескриптор
uint8_t *SomeDev_GetStringDescriptor(uint16_t Length)
{
  uint8_t wValue0 = pInformation->USBwValue0;

  if (wValue0 > 5)
  {
    return NULL;
  }
  else
  {
    return Standard_GetDescriptorData(Length, &String_Descriptor[wValue0]);
  }
}

#endif // USE_USB