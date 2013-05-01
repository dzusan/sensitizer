/*
 * utils.c
 *
 *  Created on: 25.12.2012
 *      Author: dzusan
 */

/* Includes ------------------------------------------------------------------*/
#include "utils.h"

/*Current measured sensor values*/
volatile struct SENSOR_VALUES sensorValues = {{0,0,0},{0,0,0}};

/*Current rotation matrix*/
volatile int16_t rotationMatrix[3][3] = {{0,0,0},{0,0,0},{0,0,0}};

/*Final clear vectors*/
volatile struct CLEAR_VALUES clearValues = {{0,0,0},{0,0,0}};

/*Reception byte counter*/
volatile int rxCount = 0;
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

volatile struct TOOL_PROPERTIES ToolProperties = {{0,0,0},{0,0,0}};
