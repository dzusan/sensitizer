#include "stm32f10x.h"
#include "periph_setup.h"

volatile CanRxMsg CanRxMsgStructure;
volatile int countUSART = 0;
volatile int countCAN = 0;

int main()
{
	uint16_t dataSeq[13] = {'L','E','T','S',' ','S','T','A','R','T','!','\n','\n'};
	int count;

	Setup_USART_NVIC();
	SetupUSART();
	Setup_CAN_NVIC();
	SetupCAN();

	SetupLED();

	//CAN_FIFORelease(CAN1, CAN_FIFO0);

	//CAN_Transmit(CAN1, &CanTxMsgStructure);
		
		/* === Action === */
		/*for(dataCount = 0; dataCount < 13; dataCount++)
		{
			USART_SendData(USART2, dataSeq[dataCount]);
			while(!TxFlag);
			TxFlag = 0;
		}
		
		
			while(1)
			{
				while(!CANRXFlag);
				
				for(dataCount = 0; dataCount < 8; dataCount++)
				{
					USART_SendData(USART2, (uint16_t)CanRxMsgStructure.Data[dataCount]);
					while(!TxFlag);
					TxFlag = 0;
				}
				USART_SendData(USART2, '\n');
				CANRXFlag = 0;
			}*/
			
			for(count = 0; count < 13; count++)
			{
				while(!USART_GetFlagStatus(USART2, USART_FLAG_TC));
				USART_SendData(USART2, dataSeq[count]);
				USART_ClearFlag(USART2, USART_FLAG_TC);
			}
			
			count = 0;
			
			CAN_Transmit(CAN1, &CanTxMsgStructure);
			
			while(1);
}

void USART2_IRQHandler(void)
{
	if(USART_GetITStatus(USART2, USART_IT_RXNE))
	{
		USART_ClearITPendingBit(USART2, USART_IT_RXNE); 
		if(countUSART < 7)
		{
			CanTxMsgStructure.Data[countUSART] = (uint8_t)(0x00FF & USART_ReceiveData(USART2));
			countUSART++;
		}
		else 
		{
			CanTxMsgStructure.Data[7] = (uint8_t)(0x00FF & USART_ReceiveData(USART2));
			CAN_Transmit(CAN1, &CanTxMsgStructure);
			countUSART = 0;
		}
	}
}

void USB_LP_CAN1_RX0_IRQHandler(void)
{
	if(CAN_GetITStatus(CAN1, CAN_IT_FMP0))
	{
		CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);
		
		CAN_Receive(CAN1, 0, &CanRxMsgStructure);
		
		for(countCAN = 0; countCAN < 8; countCAN++)
		{
			USART_SendData(USART2, CanRxMsgStructure.Data[countCAN]);
			while(!USART_GetFlagStatus(USART2, USART_FLAG_TC));
			USART_ClearFlag(USART2, USART_FLAG_TC);
		}
		
		USART_SendData(USART2, '\n');
		while(!USART_GetFlagStatus(USART2, USART_FLAG_TC));
		USART_ClearFlag(USART2, USART_FLAG_TC);
	}
}
