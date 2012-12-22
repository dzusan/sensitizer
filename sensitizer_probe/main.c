#include "stm32f10x.h"
#include "stm32f10x_conf.h"

/*Blink frequency*/
#define RTC_PRESCALER 8000

void SetupLED(void);
void SetupRTC(void);
void Setup_RTC_NVIC(void);
void SetupUSART(void);
void Setup_USART_NVIC(void);


volatile BitAction TmpBitVal;

int main()
{

	TmpBitVal = Bit_RESET;

	SetupLED();

	//Setup_RTC_NVIC();

	//SetupRTC();

	Setup_USART_NVIC();
	SetupUSART();

	//Action

	/*uint16_t sym;
	int count;*/

	while(1)
	{
		//USART_SendData(USART3,'E');
		__NOP();
		/*sym = USART_ReceiveData(USART3);
		for(count = 0; count < 10000; count++) __NOP();
		if(sym)
		{
			USART_SendData(USART3, sym);
			sym = 0;
		}*/
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
		if(TmpBitVal == Bit_RESET) TmpBitVal = Bit_SET;
		else TmpBitVal = Bit_RESET;
	}

	uint16_t sym;
	sym = USART_ReceiveData(USART3);
	USART_SendData(USART3, sym);

	GPIO_WriteBit(GPIOC, GPIO_Pin_12, TmpBitVal);

}

/************ Peripherial functions **************/

void SetupLED(void)
{
	/* ==GPIO== */

	//Clocking

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

	//Configure

	GPIO_DeInit(GPIOC);

	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	//Init

	GPIO_Init(GPIOC , &GPIO_InitStructure);

	/* =======*/
}

void SetupRTC(void)
{
	/* CK_RTC clock selection */
		RCC_APB1PeriphClockCmd( RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE );

		/* Allow access to BKP Domain */
		PWR_BackupAccessCmd( ENABLE );

		/* Enable the LSE OSC */
		RCC_LSEConfig( RCC_LSE_ON );

		/* Wait till LSE is ready */
		while ( RCC_GetFlagStatus( RCC_FLAG_LSERDY ) == RESET ){}

		/* Select the RTC Clock Source */
		RCC_RTCCLKConfig( RCC_RTCCLKSource_LSE );

		/* Enable the RTC Clock */
		RCC_RTCCLKCmd( ENABLE );

		/* Wait for RTC registers synchronization */
		RTC_WaitForSynchro();

		/* Wait until last write operation on RTC registers has finished */
		RTC_WaitForLastTask();

		/* Enable the RTC Second interrupt */
		RTC_ITConfig(RTC_IT_SEC, ENABLE);

		/* Wait until last write operation on RTC registers has finished */
		RTC_WaitForLastTask();

		/* Set RTC prescaler: set RTC period to 1sec */
		/* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) */
		RTC_SetPrescaler( RTC_PRESCALER );

		/* Wait until last write operation on RTC registers has finished */
		RTC_WaitForLastTask();

		RTC_EnterConfigMode();

		RTC_ExitConfigMode();
}

void Setup_RTC_NVIC(void)
{
	/* Interrupt structure */
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

		NVIC_InitTypeDef NVIC_InitStructure;
		NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

		NVIC_Init(&NVIC_InitStructure);
}

void SetupUSART(void)
{
	/* Enable GPIOB clock                                                   */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	GPIO_DeInit(GPIOB);

	GPIO_InitTypeDef  GPIO_InitStructure;

	/* Configure USART3 Rx (PB11) as input floating                         */
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* Configure USART3 Tx (PB10) as alternate function push-pull            */
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* Enable USART3 clock                                                   */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	USART_InitTypeDef USART_InitStructure;

	USART_InitStructure.USART_BaudRate            = 9600;
	USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits            = USART_StopBits_1;
	USART_InitStructure.USART_Parity              = USART_Parity_No ;
	USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;

	USART_Init(USART3, &USART_InitStructure);
	USART_Cmd(USART3, ENABLE);
	/*Interrupt after receive*/
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);

}

void Setup_USART_NVIC(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

	NVIC_Init(&NVIC_InitStructure);
}

