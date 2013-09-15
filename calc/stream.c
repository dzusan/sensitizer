#include "stream.h"

Stream StreamRead(long int *filePosition)
{
	int count;
	Stream data;
	float *dataPtr;
	FILE *fp;

	if ((fp = fopen("stream","rb")) == NULL)
	{
  		printf("Cant open \"stream\" file\n");
  		exit(1);
	}
	fseek(fp, *filePosition, SEEK_SET);

	dataPtr = &data.Fx;

	for (count = 0; count < 6; count++)
	{
    	fscanf(fp, "%f\n", dataPtr);
    	dataPtr++;
	}
	*filePosition = ftell(fp);

	fclose(fp);
	return data;
}