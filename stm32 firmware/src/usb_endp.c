
/* Includes ------------------------------------------------------------------*/
#include "hw_config.h"
#include "usb_desc.h"
#include "platform_config.h"
#include "usb_lib.h"
#include "usb_istr.h"
#include "serv_led.h"

uint8_t Receive_Buffer[9];
extern __IO uint8_t PrevXferComplete;
extern uint8_t Buffer[RPT4_COUNT+1];


void EP1_OUT_Callback(void)
{
  BitAction Led_State;

  /* Read received data (2 bytes) */
  USB_SIL_Read(EP1_OUT, Receive_Buffer);

  if (Receive_Buffer[1] == 0)
  {
    Led_State = Bit_RESET;
  }
  else
  {
    Led_State = Bit_SET;
  }


  switch (Receive_Buffer[0])
  {
    case 1: /* Led 1 */
     if (Led_State != Bit_RESET)
     {
       ;//GPIO_SetBits(LED_PORT,LED1_PIN);
     }
     else
     {
       ;//GPIO_ResetBits(LED_PORT,LED1_PIN);
     }
     break;
    case 2: /* Led 2 */
     if (Led_State != Bit_RESET)
     {
       ;//GPIO_SetBits(LED_PORT,LED2_PIN);
     }
     else
     {
       ;//GPIO_ResetBits(LED_PORT,LED2_PIN);
     }
      break;
    case 3: /* Led 1&2 */
        Buffer[4]=Receive_Buffer[1];
     break;
  }

	ParsePcComand( Receive_Buffer );
	
  SetEPRxStatus(ENDP1, EP_RX_VALID);
}


void EP1_IN_Callback(void)
{
  /* Set the transfer complete token to inform upper layer that the current
  transfer has been complete */
  PrevXferComplete = 1;
}

