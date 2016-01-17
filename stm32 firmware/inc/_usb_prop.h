// usb_istr.h
// Автор: MCD Application Team
// Создано: 21-March-2011

/******************** (C) COPYRIGHT 2011 STMicroelectronics ********************
* File Name          : usb_prop.h
* Author             : MCD Application Team
* Version            : V3.3.0
* Date               : 21-March-2011
* Description        : All processing related to Sample Some Device (Endpoint 0)
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __usb_prop_H
#define __usb_prop_H
/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

// $USBCONFIG - закомментировать используемое

#define SomeDev_GetConfiguration          NOP_Process
/* #define SomeDev_SetConfiguration          NOP_Process*/
#define SomeDev_GetInterface              NOP_Process
#define SomeDev_SetInterface              NOP_Process
#define SomeDev_GetStatus                 NOP_Process
/* #define SomeDev_ClearFeature              NOP_Process*/
#define SomeDev_SetEndPointFeature        NOP_Process
#define SomeDev_SetDeviceFeature          NOP_Process
/*#define SomeDev_SetDeviceAddress          NOP_Process*/


/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void SomeDev_init(void);
void SomeDev_Reset(void);
void SomeDev_SetConfiguration(void);
void SomeDev_ClearFeature(void);
void SomeDev_SetDeviceAddress (void);
void SomeDev_Status_In (void);
void SomeDev_Status_Out (void);
RESULT SomeDev_Data_Setup(uint8_t);
RESULT SomeDev_NoData_Setup(uint8_t);
RESULT SomeDev_Get_Interface_Setting(uint8_t Interface, uint8_t AlternateSetting);
uint8_t *SomeDev_GetDeviceDescriptor(uint16_t );
uint8_t *SomeDev_GetConfigDescriptor(uint16_t);
uint8_t *SomeDev_GetStringDescriptor(uint16_t);

#endif /* __usb_prop_H */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/

