/*
 * interrupts.c
 *
 *  Created on: 23.12.2012
 *      Author: dzusan
 */

/* Includes -----------------------------------------------------------------*/
#include "stm32f10x.h"
#include "utils.h"

void USART3_IRQHandler(void)
{
	if(USART_GetITStatus(USART3, USART_IT_RXNE))
	{
		USART_ClearITPendingBit(USART3, USART_IT_RXNE); 
		if(responseCount < 5)
		{
			response[responseCount] = USART_ReceiveData(USART3);
			responseCount++;
		}
		else if(gaugeCount < 13)
		{
			gauge[gaugeCount] = USART_ReceiveData(USART3);
			gaugeCount++;
		}
		else
			gaugeCount = 0;
			if(TmpBitVal == Bit_RESET) TmpBitVal = Bit_SET;
			else TmpBitVal = Bit_RESET;
			//GPIO_WriteBit(GPIOB, GPIO_Pin_14, TmpBitVal);
	}
	else if(USART_GetITStatus(USART3, USART_IT_TC))
	{
		USART_ClearITPendingBit(USART3, USART_IT_TC);
		TxFlag = 1;
	}
}


void USB_LP_CAN1_RX0_IRQHandler(void)
{
	GPIO_WriteBit(GPIOB, GPIO_Pin_14, Bit_SET);
	CANRXFlag = 1;
	if(CAN_GetITStatus(CAN1, CAN_IT_FMP0))
	{
		CAN_Receive(CAN1, 0, &CanRxMsgStructure);
	}
}
