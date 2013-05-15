/*
 * utils.c
 *
 *  Created on: 25.12.2012
 *      Author: dzusan
 */

/* Includes -----------------------------------------------------------------*/
#include "utils.h"

volatile CanTxMsg CanTxMsgStructure;
volatile CanRxMsg CanRxMsgStructure;

volatile uint8_t TxFlag = 0;

volatile uint16_t gauge[6];
volatile int gaugeCount = 0;
volatile ElemType gaugeByte;
volatile int gaugeByteCount = 0;
//volatile uint8_t GaugeFlag = 0;
volatile CircularBuffer cb;

volatile float basicMatrix[6][6];

volatile BitAction TmpBitVal;

volatile uint16_t previousTimerCounter;
