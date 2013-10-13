/*
 * sensor_setup.h
 *
 *  Created on: 06.10.2013
 *      Author: dzusan
 */


#ifndef SENSOR_SETUP_H_
#define SENSOR_SETUP_H_

/* Includes -----------------------------------------------------------------*/
#include "stm32f10x.h"
#include "utils.h"
#include "modbus.h"
#include <stdlib.h>

uint8_t SensorSetup(void);
void BasicMatrixConversion(uint16_t *matrixSeq);

#endif /* SENSOR_SETUP_H_ */
