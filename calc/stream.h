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

	float Rx;
	float Ry;
	float Rz;
}Stream;

Stream StreamRead(long int *filePosition);

#endif /* STREAM_H_ */