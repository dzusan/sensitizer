/*
 * periph_setup.c
 *
 *  Created on: 23.12.2012
 *      Author: dzusan
 */

/* Includes -----------------------------------------------------------------*/
#include "periph_setup.h"

void SetupLED(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_DeInit(GPIOB);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB , &GPIO_InitStructure); 
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
		NVIC_InitTypeDef NVIC_InitStructure;
	
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

		NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

		NVIC_Init(&NVIC_InitStructure);
}

void SetupUSART(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
		/* Enable GPIOA clock                                                   */
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

		GPIO_DeInit(GPIOA);		

		/* Configure USART3 Rx (PA3) as input floating                         */
		GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_3;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
		GPIO_Init(GPIOA, &GPIO_InitStructure);

		/* Configure USART3 Tx (PA2) as alternate function push-pull            */
		GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		
		/* Configure USART2 Transceive enable (PA1) push-pull out*/
		GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_1;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
		GPIO_Init(GPIOA, &GPIO_InitStructure); 

		/* Enable USART2 clock                                                   */
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

		USART_InitStructure.USART_BaudRate            = 1250000;
		USART_InitStructure.USART_WordLength          = USART_WordLength_9b;
		USART_InitStructure.USART_StopBits            = USART_StopBits_1;
		USART_InitStructure.USART_Parity              = USART_Parity_Even;
		USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;

		USART_Init(USART2, &USART_InitStructure);
		USART_Cmd(USART2, ENABLE);
		//USART_ITConfig(USART2, USART_IT_RXNE | USART_IT_TC, ENABLE);
		USART_ITConfig(USART2, USART_IT_TC, ENABLE);
		USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

}

void Setup_USART_NVIC(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_Init(&NVIC_InitStructure);
}

void SetupCAN(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	CAN_InitTypeDef CAN_InitStructure;
	CAN_FilterInitTypeDef CAN_FilterInitStructure;
	uint8_t defaultDataSample[8] = {6,5,4,1,2,3,0,1};
	int i;
	
	/* Configure CAN Rx (PA11) as input floating                         */
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Configure CAN Tx (PB12) as alternate function push-pull            */
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);

	CAN_InitStructure.CAN_Prescaler = 9;
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
	CAN_InitStructure.CAN_BS1 = CAN_BS1_2tq;
	CAN_InitStructure.CAN_BS2 = CAN_BS2_1tq;
	CAN_InitStructure.CAN_TTCM = DISABLE;
	CAN_InitStructure.CAN_ABOM = DISABLE;
	CAN_InitStructure.CAN_AWUM = DISABLE;
	CAN_InitStructure.CAN_NART = DISABLE;
	CAN_InitStructure.CAN_RFLM = DISABLE;
	CAN_InitStructure.CAN_TXFP = DISABLE;

	CAN_DeInit(CAN1);
	
	CAN_Init(CAN1, &CAN_InitStructure);
	
	CanTxMsgStructure.ExtId = 0x10210346;
	CanTxMsgStructure.IDE = CAN_Id_Extended;
	CanTxMsgStructure.RTR = CAN_RTR_Data;
	CanTxMsgStructure.DLC = 8;
	for(i = 0; i < 8; i++) CanTxMsgStructure.Data[i] = defaultDataSample[i];
	
	CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000; //0x0040; /* 8-bit adress is 0b00001000 */
	CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000; //0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000; //0x07E0;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000; //0x0000;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;
	CAN_FilterInitStructure.CAN_FilterNumber = 0;
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
	
	CAN_FilterInit(&CAN_FilterInitStructure);
}

void SetupTIM(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	TIM_TimeBaseInitStructure.TIM_Prescaler = 41840;
	TIM_TimeBaseInitStructure.TIM_Period = 2;
	
	TIM_DeInit(TIM2);
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
}
