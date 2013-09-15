#include "stm32f10x.h"
#include "periph_setup.h"
#include "utils.h"
#include "calc.h"
#include "modbus.h"


int main()
{	
	SetupLED();
	Setup_USART_NVIC();
	SetupUSART();
	SetupCAN();
	Setup_CAN_NVIC();
	SetupTIM();
	
	TIM_Cmd(TIM2, ENABLE);
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	while(!TIM_GetFlagStatus(TIM2, TIM_FLAG_Update));
	TIM_Cmd(TIM2, DISABLE);
	
	GPIO_WriteBit(GPIOB, GPIO_Pin_13, Bit_SET);
		
	if(SensorSetup())
	{
		TmpBitVal = Bit_RESET;
		cbInit(&cb, 10);
		TIM_Cmd(TIM4, ENABLE);
		while(1);
	}
}
