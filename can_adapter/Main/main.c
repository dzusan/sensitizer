#include "stm32f10x.h"
#include "periph_setup.h"

volatile CanRxMsg CanRxMsgStructure;
volatile int countUSART = 0;
volatile int countCAN = 0;
volatile uint8_t countPkg = 0;
volatile uint8_t autoSendFlag = 0;
volatile uint8_t startFlag = 0;

int main()
{
	uint16_t dataSeq[13] = {'L','E','T','S',' ','S','T','A','R','T','!','\n','\n'};
	int count;

	Setup_USART_NVIC();
	SetupUSART();
	Setup_CAN_NVIC();
	SetupCAN();
	SetupLED();
	SetupTIM();
	

	while(!startFlag);

	TIM_Cmd(TIM2, ENABLE);
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	while(!TIM_GetFlagStatus(TIM2, TIM_FLAG_Update));
	TIM_Cmd(TIM2, DISABLE);

	GPIO_WriteBit(GPIOB, GPIO_Pin_14, Bit_SET);

	/*for(count = 0; count < 13; count++)
	{
		while(!USART_GetFlagStatus(USART2, USART_FLAG_TC));
		USART_SendData(USART2, dataSeq[count]);
		USART_ClearFlag(USART2, USART_FLAG_TC);
	}*/
			
	count = 0;
			
	while(1)
	{
		autoSendFlag = 0;
		
		CAN_Transmit(CAN1, &CanTxMsgStructure);
		
		//while(autoSendFlag < 2);
		
		TIM_Cmd(TIM3, ENABLE);
		TIM_ClearFlag(TIM3, TIM_FLAG_Update);
		while(!TIM_GetFlagStatus(TIM3, TIM_FLAG_Update));
		TIM_Cmd(TIM3, DISABLE);
	}
				
}

void USART2_IRQHandler(void)
{
	if(USART_GetITStatus(USART2, USART_IT_RXNE))
	{
		USART_ClearITPendingBit(USART2, USART_IT_RXNE); 
		if(countUSART < 7)
		{
			autoSendFlag = 0;
			CanTxMsgStructure.Data[countUSART] = (uint8_t)(0x00FF & USART_ReceiveData(USART2));
			countUSART++;
		}
		else 
		{
			CanTxMsgStructure.Data[7] = (uint8_t)(0x00FF & USART_ReceiveData(USART2));
			CanTxMsgStructure.ExtId |= (0x7F & countPkg) << 8;
			CAN_Transmit(CAN1, &CanTxMsgStructure);
			countUSART = 0;
			countPkg++;
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
		
		autoSendFlag++;
		startFlag = 1;
	}
}
