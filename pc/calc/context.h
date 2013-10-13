#ifndef CONTEXT_H_
#define CONTEXT_H_

/* Includes -----------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>

typedef struct 
{
	float Fx0;
	float Fy0;
	float Fz0;

	float P;

	float Tx0;
	float Ty0;
	float Tz0;
	
	float Cx;
	float Cy;
	float Cz;
}Context;

Context ContextRead(void);

#endif /* CONTEXT_H_ */