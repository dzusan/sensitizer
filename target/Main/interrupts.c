/*
 * interrupts.c
 *
 *  Created on: 23.12.2012
 *      Author: dzusan
 */

/* Includes -----------------------------------------------------------------*/
#include "utils.h"
#include "calc.h"
#include "modbus.h"
#include "handlers.h"

void USART2_IRQHandler(void)
{
	TIM_Cmd(TIM4, DISABLE);
	
	if(USART_GetITStatus(USART2, USART_IT_RXNE))
	{
		USART_ClearITPendingBit(USART2, USART_IT_RXNE); 
		if(MODBUS)
		{
			*modbusByte = USART_ReceiveData(USART2);
			RxFlag = 1;
		}
		else
		{
			uint16_t currentTimerCounter;
			currentTimerCounter = TIM_GetCounter(TIM4);
			
			if(currentTimerCounter > previousTimerCounter + 20) /* Timeout detected then     */
			{																										/* read first byte in sample */		
				gaugeByteCount = 0;
				gaugeByte.bytes[gaugeByteCount] = (uint8_t)(0x00FF & USART_ReceiveData(USART2));
			}
			else																								/* Timeout not detected then  */
			{																										/* read byte in the middle    */
				gaugeByteCount++;
				gaugeByte.bytes[gaugeByteCount] = (uint8_t)(0x00FF & USART_ReceiveData(USART2));
				if(gaugeByteCount == 12)													/* Last byte of the sample    */
				{
					if(StreamingSample()) cbWrite(&cb, &gaugeByte); /* Check of the CRC           */
					gaugeByteCount = 0;
				}
			}
			
			previousTimerCounter = currentTimerCounter;
		}
	}
	else if(USART_GetITStatus(USART2, USART_IT_ORE)) /* Dirty hack for         */
	{																								 /* overrun error handling */
		uint8_t tmp;
		USART_ClearITPendingBit(USART2, USART_IT_ORE);
		tmp = (uint8_t)(0x00FF & USART_ReceiveData(USART2));
	}
	else if(USART_GetITStatus(USART2, USART_IT_TC))
	{
		USART_ClearITPendingBit(USART2, USART_IT_TC);
		TxFlag = 1;
	}
	
	TIM_Cmd(TIM4, ENABLE);
}


void USB_LP_CAN1_RX0_IRQHandler(void)
{
	if(CAN_GetITStatus(CAN1, CAN_IT_FMP0))
	{		
		CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);

		CAN_Receive(CAN1, 0, &CanRxMsgStructure);	
		
		GPIO_WriteBit(GPIOB, GPIO_Pin_14, Bit_SET);
		//if(cbIsFull(&cb)) GaugeVectorConversion();
		CAN_requestHandler();
		GPIO_WriteBit(GPIOB, GPIO_Pin_14, Bit_RESET);
	}
}
