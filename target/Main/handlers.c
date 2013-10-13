/*
 * handlers.c
 *
 *  Created on: 16.09.2013
 *      Author: dzusan
 */

/* Includes -----------------------------------------------------------------*/
#include "utils.h"
#include "calc.h"
#include "handlers.h"

void CAN_requestHandler(void)
{
	int count = 0;
	int16_t norm;
	//float F_T_units[6];
	uint8_t forceIssue[6];
	uint8_t torqueIssue[6];
	
	if(cbIsFull(&cb)) GaugeVectorConversion(/*&F_T_units[0]*/);
	
	/* Debug section BEGIN */
	/*if(DEBUG_count < 250)
		DEBUG_count++;
	else
		errFlag = 3;*/
	/* Debug section END */
	
	for(count = 0; count < 6; count += 2)
	{
		norm = (int16_t)(F_T_units[count / 2] / 10000);
		forceIssue[count]     = (uint8_t)(norm >> 8);
		forceIssue[count + 1] = (uint8_t)(norm);
	
		norm = (int16_t)(F_T_units[3 + count / 2] / 10000);
		torqueIssue[count]     = (uint8_t)(norm >> 8);
		torqueIssue[count + 1] = (uint8_t)(norm);
	}
	
	CanTxMsgStructure.ExtId = (CanTxMsgStructure.ExtId & 0x1FFF80FF) | \
														(CanRxMsgStructure.ExtId & 0x7F00);
	
	switch (CanRxMsgStructure.ExtId & 0xFF)
	{
		case 1:
			for(count = 0; count < 6; count++)
				CanTxMsgStructure.Data[count] = forceIssue[count];
			CAN_Transmit(CAN1, &CanTxMsgStructure);
			break;
		
		case 2:
			for(count = 0; count < 6; count++)
				CanTxMsgStructure.Data[count] = torqueIssue[count];
			CAN_Transmit(CAN1, &CanTxMsgStructure);
			break;
		
		case 3:
			for(count = 0; count < 6; count++)
				CanTxMsgStructure.Data[count] = forceIssue[count];
			CAN_Transmit(CAN1, &CanTxMsgStructure);
			for(count = 0; count < 6; count++)
				CanTxMsgStructure.Data[count] = torqueIssue[count];
			CAN_Transmit(CAN1, &CanTxMsgStructure);
			break;
		
		default:
			for(count = 0; count < 6; count++)
				CanTxMsgStructure.Data[count] = 0;
			CAN_Transmit(CAN1, &CanTxMsgStructure);
	}
}

void ErrHandler(void)
{
	uint8_t steadyLight = 0;
	BitAction TmpBitVal = Bit_RESET;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseStructInit(&TIM_TimeBaseInitStructure);
	TIM_TimeBaseInitStructure.TIM_Prescaler = 65535;
	
	switch(errFlag)
	{
		case 0: /* Nothing happened */
			break;
		
		case 1: /* MODBUS error */
			TIM_TimeBaseInitStructure.TIM_Period = 600;
			break;
		
		case 2: /* Open in the sensor wire */
			TIM_TimeBaseInitStructure.TIM_Period = 300;
			break;
		
		default: /* Devilry... */
			steadyLight = 1;
	}
	
	//CanTxMsgStructure.Data[7] = errFlag;
	
	if(!steadyLight) /* Error light blinking */
	{
		TIM_DeInit(TIM2);
		TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
		
		while(errFlag)
		{
			if(TmpBitVal == Bit_RESET) TmpBitVal = Bit_SET;
			else TmpBitVal = Bit_RESET;
			GPIO_WriteBit(GPIOB, GPIO_Pin_15, TmpBitVal);
		
			TIM_Cmd(TIM2, ENABLE);
			TIM_ClearFlag(TIM2, TIM_FLAG_Update);
			while(!TIM_GetFlagStatus(TIM2, TIM_FLAG_Update));
			TIM_Cmd(TIM2, DISABLE);
		}
	}
	else /* Error light steady */
	{
		GPIO_WriteBit(GPIOB, GPIO_Pin_15, Bit_SET);
		while(errFlag);
	}
	
	GPIO_WriteBit(GPIOB, GPIO_Pin_15, Bit_RESET);
}