#include "context.h"

Context ContextRead(void)
{
	int count;
	Context data;
	float *dataPtr;
	FILE *fp;

	if ((fp = fopen("context","r")) == NULL)
	{
  		printf("Cant open \"context\" file\n");
  		exit(1);
	}

	dataPtr = &data.Fx0;

	for (count = 0; count < 4; count++)
	{
    	fscanf(fp, "%f\n", dataPtr);
    	dataPtr++;
	}

	fclose(fp);
	return data;
}