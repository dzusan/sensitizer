/*
 * sensor_setup.c
 *
 *  Created on: 06.10.2013
 *      Author: dzusan
 */
 
 /* Includes -----------------------------------------------------------------*/
#include "sensor_setup.h"

uint8_t SensorSetup(void)
{
	uint16_t sessionID = 0x0001;
	uint16_t statusWord[3];
	uint16_t gain[6];
	uint16_t offset[6];
	uint16_t basicMatrixSeq[72];
	
	MODBUS = 1; /*Enable modbus*/
	
	Read(0x0A, 0x001D, 0x0003, statusWord);
	Write(0x0A, 0x000C, 0x0001, &sessionID);
	Read(0x0A, 0x0103, 0x0048, basicMatrixSeq);
	Read(0x0A, 0x015C, 0x0006, gain);
	Read(0x0A, 0x0162, 0x0006, offset);
	Com(0x0A, 0x6A, 0xAA); /* Unlock storage */
	Write(0x0A, 0x0000, 0x0006, gain);
	Write(0x0A, 0x0006, 0x0006, offset);
	Com(0x0A, 0x6A, 0x18); /* Lock storage */
	Com(0x0A, 0x46, 0x55); /* StartStreaming */
	
	MODBUS = 0; /*Disable modbus*/
	
	BasicMatrixConversion(basicMatrixSeq);
	
	return 1;	
}

void BasicMatrixConversion(uint16_t *matrixSeq)
{
	float *basicMatrixPt = &basicMatrix[0][0];
	int count;
	uint32_t bin;
	
	for(count = 0; count <= 72; count += 2)
	{
		bin = (uint32_t)((matrixSeq[count] << 16) | matrixSeq[count + 1]);
		
		*basicMatrixPt = *(float *) &bin;
		basicMatrixPt++;
	}
}
