/*
 * modbus.h
 *
 *  Created on: 25.12.2012
 *      Author: dzusan
 */


#ifndef MODBUS_H_
#define MODBUS_H_

/* Includes -----------------------------------------------------------------*/
#include "stm32f10x.h"
#include "utils.h"
#include <stdlib.h>

extern volatile uint8_t MODBUS;

extern volatile uint16_t *modbusByte;
extern volatile uint8_t RxFlag;

uint8_t SensorSetup(void);
void request(uint16_t *startByte, int num);
void response(uint16_t *startByte, int num);
void BasicMatrixConversion(uint16_t *startByte);


#endif /* MODBUS_H_ */
