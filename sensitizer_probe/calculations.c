/*
 * calculations.c
 *
 *  Created on: 23.12.2012
 *      Author: dzusan
 */


#include "calculations.h"

float typeConversion(uint16_t input)
{
	float tmp;
	tmp = (float)((input & 0b0011111111111111)/10000);
	if(input & 0b1000000000000000) tmp *= -1;
	if(tmp > 1)  return  1;
	if(tmp < -1) return -1;
	return tmp;
}

void Calculations(void)
{
	int countEx, countIn, countSum;

	float tmpCoordinateGravityForceVector[3] = {0, 0, 0};
	float tmpCoordinateCMVector[3] = {0, 0, 0};

	/*Vector rotation:
	 *[totalRotationMatrix X gravityForceVector]
	 *[totalRotationMatrix X CMVector]*/

	for(countIn = 0; countIn < 3; countIn++)
		for(countSum = 0; countSum < 3; countSum++)
		{
			tmpCoordinateGravityForceVector[countIn] += typeConversion(rotationMatrix[countIn][countSum]) * (float)ToolProperties.gravityForceVector[countSum];
			tmpCoordinateCMVector[countIn]           += typeConversion(rotationMatrix[countIn][countSum]) * (float)ToolProperties.CMVector[countSum];
		}

	/*Gravity force torque:
	 *[CMVector X  gravityForceVector]*/

	int16_t gravityForceTorqueVector[3];
	gravityForceTorqueVector[0] = tmpCoordinateCMVector[1] * tmpCoordinateGravityForceVector[2] - tmpCoordinateCMVector[2] * tmpCoordinateGravityForceVector[1];
	gravityForceTorqueVector[1] = tmpCoordinateCMVector[2] * tmpCoordinateGravityForceVector[0] - tmpCoordinateCMVector[0] * tmpCoordinateGravityForceVector[2];
	gravityForceTorqueVector[2] = tmpCoordinateCMVector[0] * tmpCoordinateGravityForceVector[1] - tmpCoordinateCMVector[1] * tmpCoordinateGravityForceVector[0];

	/*Subtraction*/

	for(countEx = 0; countEx < 3; countEx++)
	{
		clearValues.forceVector[countEx]  -= tmpCoordinateGravityForceVector[countEx];
		clearValues.torqueVector[countEx] -= gravityForceTorqueVector[countEx];
	}

}
