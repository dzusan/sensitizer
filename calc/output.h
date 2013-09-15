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
}Clear;

int ClearWrite(Clear final);

#endif /* OUTPUT_H_ */