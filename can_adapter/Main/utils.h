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


extern volatile uint8_t TxFlag;
//extern volatile uint8_t RxFlag;
extern volatile uint8_t LoadFlag;

extern volatile uint16_t response[5];
extern volatile int responseCount;

extern volatile uint16_t gauge[13];
extern volatile int gaugeCount;

extern volatile BitAction TmpBitVal;

extern volatile CanRxMsg CanRxMsgStructure;

extern volatile uint8_t CANRXFlag;

#endif /* UTILS_H_ */
