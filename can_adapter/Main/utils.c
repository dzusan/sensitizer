/*
 * utils.c
 *
 *  Created on: 25.12.2012
 *      Author: dzusan
 */

/* Includes -----------------------------------------------------------------*/
#include "utils.h"

volatile uint8_t TxFlag;
//extern volatile uint8_t RxFlag;
volatile uint8_t LoadFlag;

volatile uint16_t response[5];
volatile int responseCount;

volatile uint16_t gauge[13];
volatile int gaugeCount;

volatile BitAction TmpBitVal;

volatile uint8_t CANRXFlag = 0;

volatile CanRxMsg CanRxMsgStructure;
