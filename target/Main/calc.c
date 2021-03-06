/*
 * calc.c
 *
 *  Created on: 25.12.2012
 *      Author: dzusan
 */

/* Includes -----------------------------------------------------------------*/
#include "utils.h"
#include "calc.h"

uint8_t StreamingSample(void)
{
	int count;
	uint8_t checkSum = 0;
	uint8_t tmpStat = 0;
	
	for(count = 0; count < 12; count++)
		checkSum += gaugeByte.bytes[count];
	
	/************** Error checking: "Open in the sensor wire" ******************/
	for(count = 0; count < 12; count += 2)
	  if(gaugeByte.bytes[count] == 0x7F && gaugeByte.bytes[count + 1] == 0xFF) 
			tmpStat++;
	if(tmpStat != 0) errFlag = 2;
	else errFlag = 0;
	/***************************************************************************/
	
	if((checkSum & 0x7f) != (gaugeByte.bytes[12] & 0x7f))
		return 0;
	else
		return 1;	
}

void cbInit(CircularBuffer *cb, int size) {
    cb->size  = size + 1; /* include empty elem */
    cb->start = 0;
    cb->end   = 0;
    cb->elems = (ElemType *)calloc(cb->size, sizeof(ElemType));
}
 
void cbFree(CircularBuffer *cb) {
    free(cb->elems); /* OK if null */ }
 
int cbIsFull(CircularBuffer *cb) {
    return (cb->end + 1) % cb->size == cb->start; }
 
int cbIsEmpty(CircularBuffer *cb) {
    return cb->end == cb->start; }
 
/* Write an element, overwriting oldest element if buffer is full. App can
   choose to avoid the overwrite by checking cbIsFull(). */
void cbWrite(CircularBuffer *cb, ElemType *elem) {
    cb->elems[cb->end] = *elem;
    cb->end = (cb->end + 1) % cb->size;
    if (cb->end == cb->start)
        cb->start = (cb->start + 1) % cb->size; /* full, overwrite */
}
 
/* Read oldest element. App must ensure !cbIsEmpty() first. */
void cbRead(CircularBuffer *cb, ElemType *elem) {
    *elem = cb->elems[cb->start];
    cb->start = (cb->start + 1) % cb->size;
}

void GaugeVectorConversion(void/*float *F_T_units*/)
{
	int count = 0;
	int countEx, countIn;
	ElemType seq;
	int16_t sum;
	float average[6];
	float orderedAverage[6];
	
	/* average zeroing */
	for(count = 0; count < 6; count++) average[count] = 0;

	while (!cbIsEmpty(&cb))
	{
		cbRead(&cb, &seq);
		
		for(count = 0; count < 12; count += 2)
		{
			sum = (int16_t)(((uint16_t)seq.bytes[count] << 8) | (uint16_t)seq.bytes[count + 1]);
			average[count / 2] = (float)sum; //Write last value from buffer without Arithmetic mean
		}
		
		//for(count = 0; count <= 12; count += 2)
		//{
		//	sum = (int16_t)(((uint16_t)seq.bytes[count] << 8) | (uint16_t)seq.bytes[count + 1]);
		//	average[count / 2] += (float)sum / 12; //Arithmetic mean of all the values from buffer
		//}
	}
	
	/* Gauge vector regularizing */
	orderedAverage[0] = average[0];
	orderedAverage[1] = average[3];
	orderedAverage[2] = average[1];
	orderedAverage[3] = average[4];
	orderedAverage[4] = average[2];
	orderedAverage[5] = average[5];
	
	/* F_T_units zeroing */
	for(count = 0; count < 6; count++) F_T_units[count] = 0;
	
	/* Conversion into units */
	for(countEx = 0; countEx < 6; countEx++)
		for(countIn = 0; countIn < 6; countIn++)
			F_T_units[countEx] += orderedAverage[countIn] * basicMatrix[countEx][countIn];
}
