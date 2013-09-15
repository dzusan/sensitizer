#include "output.h"

int ClearWrite(Clear final)
{
	int count;
	float *dataPtr;
	FILE *fp;

	if ((fp = fopen("output","a")) == NULL)
	{
  		printf("Cant open \"output\" file\n");
  		exit(1);
	}

	dataPtr = &final.Fx;

	for (count = 0; count < 3; count++)
	{
		fprintf(fp, "%f\n", *dataPtr);
    	dataPtr++;
	}
	fprintf(fp, "\n");

	fclose(fp);
	return 0;
}