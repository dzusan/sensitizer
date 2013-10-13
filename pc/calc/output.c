#include "output.h"

int ClearWrite(Clear final, float *timeStamp)
{
	FILE *fp;

	if ((fp = fopen("output.csv","a")) == NULL)
	{
  		printf("Cant open \"output.csv\" file\n");
  		exit(1);
	}

	fprintf(fp, "%f,%f,%f,%f,%f,%f,%f\n", 
			*timeStamp,
			final.Fx, final.Fy, final.Fz,
			final.Tx, final.Ty, final.Tz);

	fclose(fp);
	return 0;
}