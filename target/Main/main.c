#include "stm32f10x.h"
#include "periph_setup.h"
#include "utils.h"
#include "calc.h"
#include "modbus.h"


int main()
{
	/*Checking*/ int i,k,j; uint32_t a;
	int dataCount;
	int simpleCount;

	SetupLED();

	Setup_USART_NVIC();
	SetupUSART();
	SetupCAN();
	SetupTIM();

	for(simpleCount = 0; simpleCount < 3000; simpleCount++)
	{
	TIM_Cmd(TIM2, ENABLE);
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	while(!TIM_GetFlagStatus(TIM2, TIM_FLAG_Update));
	TIM_Cmd(TIM2, DISABLE);
	}
	
	CAN_Transmit(CAN1, &CanTxMsgStructure);
		
		if(SensorSetup())
		{
			
			gaugeCount = 0;
			TmpBitVal = Bit_RESET;
			cbInit(&cb, 5);
			while(1)
			{
				while(!CAN_GetFlagStatus(CAN1, CAN_FLAG_FMP0));
				
				USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);
				
				CAN_ClearFlag(CAN1, CAN_FLAG_FMP0);
				CAN_Receive(CAN1, 0, &CanRxMsgStructure);
				
				if(cbIsFull(&cb))
					GaugeVectorConversion();
				USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
			}
		}
		
}
