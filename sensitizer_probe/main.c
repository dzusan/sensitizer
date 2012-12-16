#include "stm32f10x.h"
#include "stm32f10x_conf.h"


volatile BitAction TmpBitVal;

int main()
{

	TmpBitVal = Bit_RESET;

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

	/* ==RTC== */

	/* Interrupt structure */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

	NVIC_Init(&NVIC_InitStructure);

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
	RTC_SetPrescaler( 16000 );

	/* Wait until last write operation on RTC registers has finished */
	RTC_WaitForLastTask();

	RTC_EnterConfigMode();

	RTC_ExitConfigMode();

	//Action

	do __NOP(); while (1);
}

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
