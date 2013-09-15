/*
 * modbus.c
 *
 *  Created on: 25.12.2012
 *      Author: dzusan
 */

/* Includes -----------------------------------------------------------------*/
#include "utils.h"
#include "modbus.h"

volatile uint8_t MODBUS = 0;
volatile uint8_t RxFlag = 0;
volatile uint16_t *modbusByte;

uint8_t SensorSetup(void)
{
	int count;
	
	uint16_t request_UnlockStorage[5] = {0x0A, 0x6A, 0xAA, 0xFF, 0x1D};
	uint16_t request_Gain[21] = {0x0A, 0x10, 0x00, 0x00, 0x00, 0x06, 0x0C,\
															 0x00, 0xCD, 0x00, 0xC7, 0x00, 0xCD, 0x00,\
															 0xCB, 0x00, 0xC7, 0x00, 0xC7, 0x54, 0x1B};
	uint16_t request_Offset[21] = {0x0A, 0x10, 0x00, 0x06, 0x00, 0x06, 0x0C,\
																 0x76, 0xF3, 0x86, 0xC4, 0x7D, 0x92, 0x7B,\
																 0xEA, 0x7E, 0xFE, 0x7A, 0xEA, 0x52, 0x8D}; 
	uint16_t request_LockStorage[5] = {0x0A, 0x6A, 0x18, 0x7F, 0x68};
	uint16_t request_StartStreaming[5] = {0x0A, 0x46, 0x55, 0xA3, 0x9D};
	uint16_t request_Basic_Matrix[3][8] = {{0x0A, 0x03, 0x01, 0x03, 0x00, 0x20, 0xB4, 0x95}, \
																				 {0x0A, 0x03, 0x01, 0x23, 0x00, 0x20, 0xB5, 0x5F}, \
																				 {0x0A, 0x03, 0x01, 0x43, 0x00, 0x20, 0xB5, 0x41},};
	uint16_t request_StatusWord[8] = {0x0A, 0x03, 0x00, 0x1D, 0x00, 0x03, 0x94, 0xB6};
	uint16_t request_SessionID[8] = {0x0A, 0x06, 0x00, 0x0C, 0x00, 0x01, 0x89, 0x72};
	
	uint16_t response_Basic_Matrix[3][69];
	uint16_t response_StartStreaming[5];
	uint16_t response_UnlockStorage[5];
	uint16_t response_Gain[8];
	uint16_t response_Offset[8];
	uint16_t response_LockStorage[5];
	uint16_t response_StatusWord[11];
	uint16_t response_SessionID[8];
	
	/*Enable modbus*/
	MODBUS = 1;
	
	request(&request_StatusWord[0], 8);
	response(&response_StatusWord[0], 11);
	
	request(&request_SessionID[0], 8);
	response(&response_SessionID[0], 8);
																				 
	for(count = 0; count < 3; count++)
	{
		request(&request_Basic_Matrix[count][0], 8);
		response(&response_Basic_Matrix[count][0], 69);
	}
	BasicMatrixConversion(&response_Basic_Matrix[0][0]);
	
	request(&request_UnlockStorage[0], 5);
	response(&response_UnlockStorage[0], 5);
	
	request(&request_Gain[0], 21);
	response(&response_Gain[0], 8);
	
	request(&request_Offset[0], 21);
	response(&response_Offset[0], 8);
	
	request(&request_LockStorage[0], 5);
	response(&response_LockStorage[0], 5);
	
	request(&request_StartStreaming[0], 5);
	response(&response_StartStreaming[0], 5);
	
	MODBUS = 0;
	/*Disable modbus*/
	
	if((response_StartStreaming[0] & 0x00FF) == 0x0A && \
		 (response_StartStreaming[1] & 0x00FF) == 0x46 && \
		 (response_StartStreaming[2] & 0x00FF) == 0x01 && \
		 (response_StartStreaming[3] & 0x00FF) == 0xA2 && \
		 (response_StartStreaming[4] & 0x00FF) == 0x62)
	return 1;
	else return 0;
	
}

void request(uint16_t *startByte, int num)
{
	int count = 0;
	 
	GPIO_WriteBit(GPIOA, GPIO_Pin_1, Bit_SET); /*Transmitting enable*/
	
	while(count < num)
	{
		TxFlag = 0;
		USART_SendData(USART2, *startByte);
		while(!TxFlag);
		//TxFlag = 0;
		count++;
		startByte++;
	}
	
	GPIO_WriteBit(GPIOA, GPIO_Pin_1, Bit_RESET); /*Transmitting disable*/
}

void response(uint16_t *startByte, int num)
{
	int count = 0;
	
	modbusByte = startByte;
	
	for(count = 0; count < num; count++)
	{
		while(!RxFlag);
		modbusByte++;
		RxFlag = 0;
	}
	
	TIM_Cmd(TIM3, ENABLE);
	TIM_ClearFlag(TIM3, TIM_FLAG_Update);
	while(!TIM_GetFlagStatus(TIM3, TIM_FLAG_Update));
	TIM_Cmd(TIM3, DISABLE);
}

void BasicMatrixConversion(uint16_t *startByte)
{
	uint8_t matrixStream[144];
	float *basicMatrixPt = &basicMatrix[0][0];
	int countCopy = 0;
	int countStream;
	uint32_t bin;
	
	/*Copying matrix stream bytes*/
	startByte += 3;
	while(countCopy < 64)
	{
		matrixStream[countCopy] = *startByte & 0x00FF;
		countCopy++;
		startByte++;
	}
	startByte += 5;
	while(countCopy < 128)
	{
		matrixStream[countCopy] = *startByte & 0x00FF;
		countCopy++;
		startByte++;
	}startByte += 5;
	while(countCopy < 144)
	{
		matrixStream[countCopy] = *startByte & 0x00FF;
		countCopy++;
		startByte++;
	}

	
	for(countStream = 0; countStream <= 144; countStream += 4)
	{
		bin = (uint32_t)(matrixStream[countStream]     << 24 | \
										 matrixStream[countStream + 1] << 16 | \
										 matrixStream[countStream + 2] << 8  | \
										 matrixStream[countStream + 3]         );
		
		*basicMatrixPt = *(float *) &bin;
		basicMatrixPt++;
	}
}
