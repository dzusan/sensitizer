/*
 * utils.h
 *
 *  Created on: 25.12.2012
 *      Author: dzusan
 */

#ifndef UTILS_H_
#define UTILS_H_

#pragma once

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>


/*Current measured sensor values*/
extern volatile struct SENSOR_VALUES
{
	uint16_t forceVector[3];
	uint16_t torqueVector[3];
}sensorValues;

/*Current rotation matrix*/
extern volatile int16_t rotationMatrix[3][3];

/*Final clear vectors*/
extern volatile struct CLEAR_VALUES
{
	uint16_t forceVector[3];
	uint16_t torqueVector[3];
}clearValues;

/*Reception byte counter*/
extern volatile int rxCount;
#define END_RX_SEQUENCE 15
extern const uint16_t START_RX_BYTE;
/*Reception data sequence*/
extern uint16_t * const rxSeq[16];

/*Transfer byte counter*/
extern volatile int txCount;
#define END_TX_SEQUENCE 6
extern const uint16_t START_TX_BYTE;
/*Transfer data sequence*/
uint16_t * const txSeq[7];

extern volatile struct TOOL_PROPERTIES
{
	int16_t gravityForceVector[3];
	int16_t CMVector[3];
}ToolProperties;

#endif /* UTILS_H_ */
