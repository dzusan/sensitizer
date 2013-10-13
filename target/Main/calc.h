/*
 * calc.h
 *
 *  Created on: 25.12.2012
 *      Author: dzusan
 */


#ifndef CALC_H_
#define CALC_H_

/* Includes -----------------------------------------------------------------*/
#include "stm32f10x.h"
#include "utils.h"
#include <stdlib.h>

void cbInit(CircularBuffer *cb, int size);
void cbFree(CircularBuffer *cb);
int cbIsFull(CircularBuffer *cb);
int cbIsEmpty(CircularBuffer *cb);
void cbWrite(CircularBuffer *cb, ElemType *elem);
void cbRead(CircularBuffer *cb, ElemType *elem);

uint8_t StreamingSample(void);

void GaugeVectorConversion(void/*float *F_T_units*/);

#endif /* CALC_H_ */
