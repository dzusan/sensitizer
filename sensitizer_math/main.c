/*
 * main.c
 *
 *  Created on: 14.12.2012
 *      Author: dzusan
 */

/* Includes -----------------------*/
#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct
{
	int16_t gravityForceVector[3];
	int16_t CMVector[3];
}ToolProperties;

typedef struct
{
	int16_t forceVector[3];
	int16_t torqueVector[3];
}SensorValues;

uint8_t VectorSubtraction(ToolProperties*, double*, SensorValues*);

int main(void)
{

	FILE *data;

	if ((data = fopen("data", "r")) == NULL)
	{
	  printf("Cannot open file\n");
	  exit(1);
	}


	long dec[6];
	long rate = 10000;
	int count, countEx;
	char tmpStr[200];



	ToolProperties probeProp;
	probeProp.gravityForceVector[0] = 0;
	probeProp.gravityForceVector[1] = -523;
	probeProp.gravityForceVector[2] = 0;
	probeProp.CMVector[0] = 623;
	probeProp.CMVector[1] = 562;
	probeProp.CMVector[2] = 763;

	SensorValues probeMeasuredValues;

	double probeAngles[2] = {0.27, 0.19};

	ToolProperties *probePropPointer = &probeProp;
	double *probeAnglesPointer = probeAngles;
	SensorValues *probeMeasuredValuesPointer = &probeMeasuredValues;


	for(countEx = 0; countEx < 10000; countEx++)
	{
		printf("\n%d:   ",countEx);
		fgets(tmpStr, 200, data);
		sscanf(tmpStr, "%*x,%*d,%*d,%d,%d,%d,%d,%d,%d\n", &dec[0], &dec[1], &dec[2], &dec[3], &dec[4], &dec[5]);

		probeMeasuredValues.forceVector[0]  = (int16_t)(dec[0] / rate);
		probeMeasuredValues.forceVector[1]  = (int16_t)(dec[1] / rate);
		probeMeasuredValues.forceVector[2]  = (int16_t)(dec[2] / rate);
		probeMeasuredValues.torqueVector[0] = (int16_t)(dec[3] / rate);
		probeMeasuredValues.torqueVector[1] = (int16_t)(dec[4] / rate);
		probeMeasuredValues.torqueVector[2] = (int16_t)(dec[5] / rate);

		for(count = 0; count < 6; count++) printf("%d   ", dec[count]);
		printf("\n");

		VectorSubtraction(probePropPointer, probeAnglesPointer, probeMeasuredValuesPointer);
	}

	fclose(data);

	return 0;

}

uint8_t VectorSubtraction(ToolProperties *prop, double *angles, SensorValues *measuredValues)
{
	double verticalSinus   = 0.2667;//sin(angles[0]);
	double verticalCosinus = 0.9637;//cos(angles[0]);
	double radialSinus     = 0.1888;//sin(angles[1]);
	double radialCosinus   = 0.9820;//cos(angles[1]);

	double verticalRotationMatrix[3][3] = {{ 1, 0,                0               },
										   { 0, verticalCosinus, -verticalSinus   },
										   { 0, verticalSinus,    verticalCosinus }};

	double radialRotationMatrix[3][3] = {{ radialCosinus, -radialSinus,   0 },
										 { radialSinus,    radialCosinus, 0 },
										 { 0,              0,             1 }};

	double totalRotationMatrix[3][3];

	/*Matrix multiplication*/

	int countEx, countIn, countSum;

	for(countEx = 0; countEx < 3; countEx++)
		for(countIn = 0; countIn < 3; countIn++)
			for(countSum = 0; countSum < 3; countSum++)
				totalRotationMatrix[countEx][countIn] += verticalRotationMatrix[countEx][countSum] * radialRotationMatrix[countSum][countIn];

	/*Vector rotation:
	 *[totalRotationMatrix X gravityForceVector]
	 *[totalRotationMatrix X CMVector]*/

	double tmpCoordinateGravityForceVector[3] = {0, 0, 0};
	double tmpCoordinateCMVector[3] = {0, 0, 0};

	for(countIn = 0; countIn < 3; countIn++)
		for(countSum = 0; countSum < 3; countSum++)
		{
			tmpCoordinateGravityForceVector[countIn] += totalRotationMatrix[countIn][countSum] * (double)prop->gravityForceVector[countSum];
			tmpCoordinateCMVector[countIn]           += totalRotationMatrix[countIn][countSum] * (double)prop->CMVector[countSum];
		}

	for(countEx = 0; countEx < 3; countEx++)
	{
		prop->gravityForceVector[countEx] = tmpCoordinateGravityForceVector[countEx];
		prop->CMVector[countEx] = tmpCoordinateCMVector[countEx];
	}

	/*Gravity force torque:
	 *[CMVector X  gravityForceVector]*/

	int16_t gravityForceTorqueVector[3];
	gravityForceTorqueVector[0] = prop->CMVector[1] * prop->gravityForceVector[2] - prop->CMVector[2] * prop->gravityForceVector[1];
	gravityForceTorqueVector[1] = prop->CMVector[2] * prop->gravityForceVector[0] - prop->CMVector[0] * prop->gravityForceVector[2];
	gravityForceTorqueVector[2] = prop->CMVector[0] * prop->gravityForceVector[1] - prop->CMVector[1] * prop->gravityForceVector[0];

	/*Checking*/
	int count;
	printf("----------------------------------------------------");
	printf("\n[before]forceVector = ");
	for(count = 0; count < 3; count++) printf("%d   ", measuredValues->forceVector[count]);
	printf("\n[before]torqueVector = ");
	for(count = 0; count < 3; count++) printf("%d   ", measuredValues->torqueVector[count]);

	/*Subtraction*/

	for(countEx = 0; countEx < 3; countEx++)
	{
		measuredValues->forceVector[countEx]  -= prop->gravityForceVector[countEx];
		measuredValues->torqueVector[countEx] -= gravityForceTorqueVector[countEx];
	}

	/*Checking*/
	//int count;
	printf("\n----------------------------------------------------");
	printf("\nforceVector = ");
	for(count = 0; count < 3; count++) printf("%d   ", measuredValues->forceVector[count]);
	printf("\ntorqueVector = ");
	for(count = 0; count < 3; count++) printf("%d   ", measuredValues->torqueVector[count]);
	printf("\n----------------------------------------------------");
	printf("\n[tmp]gravityForceVector = ");
	for(count = 0; count < 3; count++) printf("%f   ", tmpCoordinateGravityForceVector[count]);
	printf("\n[tmp]CMVector = ");
	for(count = 0; count < 3; count++) printf("%f   ", tmpCoordinateCMVector[count]);
	printf("\n----------------------------------------------------");
	printf("\ngravityForceVector = ");
	for(count = 0; count < 3; count++) printf("%d   ", prop->gravityForceVector[count]);
	printf("\nCMVector = ");
	for(count = 0; count < 3; count++) printf("%d   ", prop->CMVector[count]);
	printf("\n----------------------------------------------------");
	printf("\ngravityForceTorqueVector = ");
	for(count = 0; count < 3; count++) printf("%d   ", gravityForceTorqueVector[count]);
	printf("\n=====================================================\n");
	return 1;
}
