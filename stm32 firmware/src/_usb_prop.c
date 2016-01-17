// usb_prop.c
// �����: Ezhik
// �������: 12.09.2011
// ����������� ������ �� MCD Application Team (thx very much!)
// ����������� ����������� �������� USB
// ������� ��� TE

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
	0x40 // ������������ ������ ������  
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

// ������������� USB-�������
void SomeDev_init()
{
	// �������� �������� �����
	Get_SerialNum();
	
	// ������ ������� ������������
	pInformation->Current_Configuration = 0;
	
	// ����������� 
	PowerOn();
	
	// �������� �������������
	USB_SIL_Init();
	
	bDeviceState = UNCONNECTED;
}


// ����� USB
void SomeDev_Reset()
{
	// ��������� ������� � �������������������� ���������
	Device_Info.Current_Configuration = 0;
	
	// ������������� CurrentFeature
	pInformation->Current_Feature = SomeDev_ConfigDescriptor[7];
	
#ifdef STM32F10X_CL   
	
	// EP0 ��� ��������������� � USB_SIL_Init()
	
	// $USBCONFIG - ������������� ����������
	
	// EP1 IN - Bulk endpoint 
	OTG_DEV_EP_Init(EP1_IN, OTG_DEV_EP_TYPE_BULK, BULK_MAX_PACKET_SIZE);
	
	// EP2 OUT - Bulk endpoint 
	OTG_DEV_EP_Init(EP2_OUT, OTG_DEV_EP_TYPE_BULK, BULK_MAX_PACKET_SIZE); 
	
	
#else 
	
	// $USBCONFIG - ������������� ����������
	
	SetBTABLE(BTABLE_ADDRESS);
	
	// ������������� Endpoint 0
	SetEPType(ENDP0, EP_CONTROL);
	SetEPTxStatus(ENDP0, EP_TX_NAK);
	SetEPRxAddr(ENDP0, ENDP0_RXADDR);
	SetEPRxCount(ENDP0, Device_Property.MaxPacketSize);
	SetEPTxAddr(ENDP0, ENDP0_TXADDR);
	Clear_Status_Out(ENDP0);
	SetEPRxValid(ENDP0);
	
	// ������������� Endpoint 1
	SetEPType(ENDP1, EP_BULK);
	SetEPTxAddr(ENDP1, ENDP1_TXADDR);
	SetEPTxStatus(ENDP1, EP_TX_NAK);
	SetEPRxStatus(ENDP1, EP_RX_DIS);
	
	// ������������� Endpoint 2
	SetEPType(ENDP2, EP_BULK);
	SetEPRxAddr(ENDP2, ENDP2_RXADDR);
	SetEPRxCount(ENDP2, Device_Property.MaxPacketSize);
	SetEPRxStatus(ENDP2, EP_RX_VALID);
	SetEPTxStatus(ENDP2, EP_TX_DIS);
	
	
	SetEPRxCount(ENDP0, Device_Property.MaxPacketSize);
	SetEPRxValid(ENDP0);
	
	// ��������� ���������� ������
	SetDeviceAddress(0);
#endif /}
	
	bDeviceState = ATTACHED;
}


// ��������� ������ SetConfiguration
void SomeDev_SetConfiguration(void)
{
	if (pInformation->Current_Configuration != 0)
	{
		// ���������������
		bDeviceState = CONFIGURED;
		
	#ifdef STM32F10X_CL 
		
		// $USBCONFIG - ������������� ����������
		
		// EP1 IN - Bulk endpoint 
		OTG_DEV_EP_Init(EP1_IN, OTG_DEV_EP_TYPE_BULK, BULK_MAX_PACKET_SIZE);
		
		// EP2 OUT - Bulk endpoint 
		OTG_DEV_EP_Init(EP2_OUT, OTG_DEV_EP_TYPE_BULK, BULK_MAX_PACKET_SIZE);     
	#else    
		// $USBCONFIG - ������� DTOG
		
		ClearDTOG_TX(ENDP1);
		ClearDTOG_RX(ENDP2);
	#endif //

	}
}


// ��������� ������� ClearFeature
void SomeDev_ClearFeature(void)
{

}


// ���������� ������� � ��������� ADDRESSED
void SomeDev_SetDeviceAddress (void)
{
	bDeviceState = ADDRESSED;
}


// ��������� Status_In
void SomeDev_Status_In(void)
{
	return;
}


// ��������� Status_Out
void SomeDev_Status_Out(void)
{
	return;
}


// ��������� Data_Setup ( ��� class-specific �������� ) 
RESULT SomeDev_Data_Setup(uint8_t RequestNo)
{
	return USB_SUCCESS;
}


// ��������� NoData_Setup ( ��� class-specific �������� ) 
RESULT SomeDev_NoData_Setup(uint8_t RequestNo)
{
	return USB_SUCCESS;
}

// ������������ ���������� � ��� �������������� ���������
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


// �������� ���������� �������
uint8_t *SomeDev_GetDeviceDescriptor(uint16_t Length)
{
	return Standard_GetDescriptorData(Length, &Device_Descriptor );
}


// �������� ���������� ������������
uint8_t *SomeDev_GetConfigDescriptor(uint16_t Length)
{
	return Standard_GetDescriptorData(Length, &Config_Descriptor );
}


// �������� ��������� ����������
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