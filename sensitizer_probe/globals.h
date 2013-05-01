/*
 * globals.h
 *
 *  Created on: 23.12.2012
 *      Author: dzusan
 */

#ifndef GLOBALS_H_
#define GLOBALS_H_

/* Includes ------------------------------------------------------------------*/
/*#include "stdint.h"*/
#include "stm32f10x.h"


/*Current measured sensor values*/
volatile struct
{
	uint16_t forceVector[3];
	uint16_t torqueVector[3];
}sensorValues;

/*Current rotation matrix*/
volatile int16_t rotationMatrix[3][3];

/*Final clear vectors*/
volatile struct
{
	uint16_t forceVector[3];
	uint16_t torqueVector[3];
}clearValues;

/*Reception byte counter*/
volatile int rxCount = 0;
#define END_RX_SEQUENCE 15
const uint16_t START_RX_BYTE = 0;
/*Reception data sequence*/
uint16_t * const rxSeq[16] =
{
	&START_RX_BYTE,

	&sensorValues.forceVector[0],
	&sensorValues.forceVector[1],
	&sensorValues.forceVector[2],

	&sensorValues.torqueVector[0],
	&sensorValues.torqueVector[1],
	&sensorValues.torqueVector[2],

	&rotationMatrix[0][0],
	&rotationMatrix[0][1],
	&rotationMatrix[0][2],

	&rotationMatrix[1][0],
	&rotationMatrix[1][1],
	&rotationMatrix[1][2],

	&rotationMatrix[2][0],
	&rotationMatrix[2][1],
	&rotationMatrix[2][2]
};

/*Transfer byte counter*/
volatile int txCount = 0;
#define END_TX_SEQUENCE 6
const uint16_t START_TX_BYTE = 0;
/*Transfer data sequence*/
uint16_t * const txSeq[7] =
{
	&START_TX_BYTE,

	&clearValues.forceVector[0],
	&clearValues.forceVector[1],
	&clearValues.forceVector[2],

	&clearValues.torqueVector[0],
	&clearValues.torqueVector[1],
	&clearValues.torqueVector[2]
};

volatile struct
{
	int16_t gravityForceVector[3];
	int16_t CMVector[3];
}ToolProperties;

#endif /* GLOBALS_H_ */
