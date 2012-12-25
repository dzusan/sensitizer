/*
 * periph_setup.c
 *
 *  Created on: 23.12.2012
 *      Author: dzusan
 */

/* Includes ------------------------------------------------------------------*/
#include "periph_setup.h"

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
	/*Interrupt after reception or transfer*/
	USART_ITConfig(USART3, USART_IT_RXNE | USART_IT_TC, ENABLE);

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

