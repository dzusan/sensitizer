/*
 * modbus.c
 *
 *  Created on: 25.12.2012
 *      Author: dzusan
 */

/* Includes -----------------------------------------------------------------*/
#include "utils.h"
#include "modbus.h"

uint8_t Write(uint8_t node, uint16_t reg, uint16_t length, uint16_t *data)
{
	int size = 1 + 1 + 2 + 2 + 1 + length * 2 + 2; /* Size of transmit PDU in bytes*/
	uint8_t *pduT = (uint8_t *)malloc(size * sizeof(uint8_t)); /* Transmit PDU */
	uint8_t pduR[8]; /* Response PDU */
	uint8_t pduR_expected[8]; /* Known response PDU for comparation with original */
	int count = 0;
	uint16_t tmp;
	
	pduT[0] = node;
	pduR_expected[0] = node;
	if(length == 1)
	{
		size -= 3;
		pduT[1] = 0x06; /* Preset Single Register */
		pduT[2] = reg >> 8;
		pduT[3] = reg;
		pduT[4] = *data >> 8;
		pduT[5] = *data;
		
		pduR_expected[1] = 0x06;
		pduR_expected[2] = reg >> 8;
		pduR_expected[2] = reg;
		pduR_expected[4] = *data >> 8;
		pduR_expected[5] = *data;
	}
	else
	{
		pduT[1] = 0x10; /* Preset Multiple Registers */
		pduT[2] = reg >> 8;
		pduT[3] = reg;
		pduT[4] = length >> 8;
		pduT[5] = length;
		pduT[6] = length * 2;
		for(count = 0; count < length * 2; count += 2)
		{
			tmp = data[count / 2];
			pduT[7 + count] = tmp >> 8;
			pduT[7 + count + 1] = tmp;
		}
		
		pduR_expected[1] = 0x10;
		pduR_expected[2] = reg >> 8;
		pduR_expected[3] = reg;
		pduR_expected[4] = length >> 8;
		pduR_expected[5] = length;
		pduR_expected[6] = length * 2;
	}
	
	tmp = CRC16(pduT, size - 2, 0xFFFF);
	pduT[size - 2] = tmp;
	pduT[size - 1] = tmp >> 8;
	
	tmp = CRC16(&pduR_expected[0], size - 2, 0xFFFF);
	pduR_expected[6] = tmp;
	pduR_expected[7] = tmp >> 8;
	
	request(pduT, size);
	response(&pduR[0], 8);
	
	for(count = 0; count < 8; count++)
		if(pduT[count] != pduR_expected[count]) return 0;
	
	return 1;
}
	
uint8_t Read(uint8_t node, uint16_t reg, uint16_t length, uint16_t *data)
{
	int size = 1 + 1 + 1 + length * 2 + 2; /* Size of response PDU in bytes*/
	uint8_t *pduR = (uint8_t *)malloc(size * sizeof(uint8_t)); /* Response PDU */
	uint8_t pduT[8]; /* Transmit PDU */
	int count = 0;
	uint16_t tmp;
	
	pduT[0] = node;
	
	pduT[1] = 0x03; /* Read Holding Registers */
	pduT[2] = reg >> 8;
	pduT[3] = reg;
	pduT[4] = length >> 8;
	pduT[5] = length;
	
	tmp = CRC16(pduT, 6, 0xFFFF);
	pduT[6] = tmp;
	pduT[7] = tmp >> 8;
	
	request(&pduT[0], 8);
	response(pduR, size);
	
	for(count = 0; count < length * 2; count += 2)
		data[count / 2] = ((uint16_t)pduR[count + 3] << 8) | (uint16_t)pduR[count + 4];
	
	tmp = CRC16(pduT, size - 2, 0xFFFF);
	
	if(tmp == ((uint16_t)(pduR[size - 1] << 8) | (uint16_t)pduR[size - 2])) return 0;
	
	return 1;
}

uint8_t Com(uint8_t node, uint8_t com, uint16_t data)
{
	uint8_t pduR[5]; /* Transmit PDU */
	uint8_t pduT[5]; /* Response PDU */
	uint16_t tmp;
	
	pduT[0] = node;
	pduT[1] = com; /* Custom command */
	pduT[2] = data; /* Enclosed data byte */

	tmp = CRC16(pduT, 3, 0xFFFF);
	pduT[3] = tmp;
	pduT[4] = tmp >> 8;
	
	request(pduT, 5);
	response(pduR, 5);
	
	tmp = CRC16(pduR, 3, 0xFFFF);
	
	if(tmp == ((uint16_t)(pduR[4] << 8) | (uint16_t)pduR[3])) return 0;
	
	return 1;
}

uint16_t CRC16 (const uint8_t *puchMsg, int usDataLen, uint16_t crc_initialize )
{
	uint8_t uchCRCHi = (uint8_t)(crc_initialize >> 8); /* high byte of CRC initialized */
	uint8_t uchCRCLo = (uint8_t)crc_initialize; /* low byte of CRC initialized */
	unsigned uIndex ; /* will index into CRC lookup table */
	while (usDataLen--) /* pass through message buffer */
	{
		uIndex = uchCRCLo ^ *puchMsg++ ; /* calculate the CRC */
		uchCRCLo = uchCRCHi ^ auchCRCHi[uIndex] ;
		uchCRCHi = auchCRCLo[uIndex] ;
	}
	return (uchCRCHi << 8 | uchCRCLo) ;
}

void request(uint8_t *startByte, int num)
{
	int count = 0;
	 
	GPIO_WriteBit(GPIOA, GPIO_Pin_1, Bit_SET); /* Transmitting enable
																							  for RS485 tranceiver */
	while(count < num)
	{
		TxFlag = 0;
		USART_SendData(USART2, (uint16_t)(*startByte));
		while(!TxFlag);
		//TxFlag = 0;
		count++;
		startByte++;
	}
	
	GPIO_WriteBit(GPIOA, GPIO_Pin_1, Bit_RESET); /* Transmitting disable
																								  for RS485 tranceiver */
}

void response(uint8_t *startByte, int num)
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
