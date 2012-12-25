#include "stm32f10x.h"
#include "stm32f10x_conf.h"

#include "periph_setup.h"
#include "globals.h"
#include "calculations.h"

volatile BitAction TmpBitVal;

int main()
{

	TmpBitVal = Bit_RESET;

	SetupLED();

	Setup_USART_NVIC();
	SetupUSART();

	while(1)
	{
		if(rxCount == END_RX_SEQUENCE)
		{
			USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);
			Calculations();
			USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
		}

		USART_SendData(USART3, *txSeq[txCount]);
	}
}



/**************** Interrupts *******************/

void RTC_IRQHandler(void)
{
	if(RTC_GetITStatus(RTC_IT_SEC))
	{
		RTC_ClearITPendingBit(RTC_IT_SEC);
		if(TmpBitVal == Bit_RESET) TmpBitVal = Bit_SET;
		else TmpBitVal = Bit_RESET;
	}

	GPIO_WriteBit(GPIOC, GPIO_Pin_12, TmpBitVal);

}

void USART3_IRQHandler(void)
{
	if(USART_GetITStatus(USART3, USART_IT_RXNE))
	{
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
		uint16_t tmp;
		tmp = USART_ReceiveData(USART3);
		rxCount++;

		if(rxCount > END_RX_SEQUENCE)
		{
			if(tmp == START_RX_BYTE) rxCount = 0;
		}
		else *rxSeq[rxCount] = tmp;
	}
	else if(USART_GetITStatus(USART3, USART_IT_TC))
	{
		USART_ClearITPendingBit(USART3, USART_IT_TC);
		if(txCount > END_TX_SEQUENCE) txCount = 0;
		else txCount++;
	}

	if(TmpBitVal == Bit_RESET) TmpBitVal = Bit_SET;
	else TmpBitVal = Bit_RESET;
	GPIO_WriteBit(GPIOC, GPIO_Pin_12, TmpBitVal);
}

