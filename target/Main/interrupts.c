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

void USART2_IRQHandler(void)
{
	if(USART_GetITStatus(USART2, USART_IT_RXNE))
	{
		USART_ClearITPendingBit(USART2, USART_IT_RXNE); 
		if(MODBUS)
		{
			*modbusByte = USART_ReceiveData(USART2);
			RxFlag = 1;
		}
		else if(gaugeByteCount < 13)
		{
			gaugeByte.bytes[gaugeByteCount] = (uint8_t)(0x00FF & USART_ReceiveData(USART2));
			gaugeByteCount++;
		}
		else
		{
			gaugeByteCount = 0;
			if(StreamingSample()) cbWrite(&cb, &gaugeByte);
			
			if(TmpBitVal == Bit_RESET) TmpBitVal = Bit_SET;
			else TmpBitVal = Bit_RESET;
			GPIO_WriteBit(GPIOB, GPIO_Pin_14, TmpBitVal);
		}
	}
	else if(USART_GetITStatus(USART2, USART_IT_TC))
	{
		USART_ClearITPendingBit(USART2, USART_IT_TC);
		TxFlag = 1;
	}
}
