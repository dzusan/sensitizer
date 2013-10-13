#ifndef STREAM_H_
#define STREAM_H_

/* Includes -----------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>

typedef struct 
{
	float Fx;
	float Fy;
	float Fz;

	float Tx;
	float Ty;
	float Tz;

	float Rx;
	float Ry;
	float Rz;
}Stream;

Stream StreamRead(long int *filePosition, float *timeStamp);

#endif /* STREAM_H_ */