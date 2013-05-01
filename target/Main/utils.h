/*
 * utils.h
 *
 *  Created on: 25.12.2012
 *      Author: dzusan
 */


#ifndef UTILS_H_
#define UTILS_H_

/* Includes -----------------------------------------------------------------*/
#include "stm32f10x.h"

extern volatile CanTxMsg CanTxMsgStructure;
extern volatile CanRxMsg CanRxMsgStructure;

extern volatile uint8_t TxFlag;

typedef struct {uint8_t bytes[13];} ElemType;
/* Circular buffer object */
typedef struct {
    int         size;   /* maximum number of elements           */
    int         start;  /* index of oldest element              */
    int         end;    /* index at which to write new element  */
    ElemType   *elems;  /* vector of elements                   */
} CircularBuffer;

extern volatile uint16_t gauge[6];
extern volatile int gaugeCount;
extern volatile ElemType gaugeByte;
extern volatile int gaugeByteCount;
extern volatile CircularBuffer cb;

extern volatile float basicMatrix[6][6];

extern volatile BitAction TmpBitVal;

#endif /* UTILS_H_ */
