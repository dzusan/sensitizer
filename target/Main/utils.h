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

extern volatile uint16_t previousTimerCounter;

/* Modbus receive vars (for interrupts using) */
extern volatile uint8_t MODBUS;
extern volatile uint8_t *modbusByte;
extern volatile uint8_t RxFlag;

/* Tables of CRC values */
extern const uint8_t auchCRCHi[256];
extern const uint8_t auchCRCLo[256];

/* Debug vars */
extern volatile uint16_t DEBUG_count;

extern volatile float F_T_units[6];

/* Global error code */
extern volatile uint8_t errFlag;

#endif /* UTILS_H_ */
