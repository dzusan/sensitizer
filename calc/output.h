#ifndef OUTPUT_H_
#define OUTPUT_H_

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
}Clear;

int ClearWrite(Clear final, float *timeStamp);

#endif /* OUTPUT_H_ */