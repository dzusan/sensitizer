#include "stream.h"

Stream StreamRead(long int *filePosition, float *timeStamp)
{
	Stream data;
	FILE *fp;

	if ((fp = fopen("stream.csv","rb")) == NULL)
	{
  		printf("Cant open \"stream.csv\" file\n");
  		exit(1);
	}
	fseek(fp, *filePosition, SEEK_SET);

	if(*filePosition == SEEK_SET)
		fscanf(fp, "F-PkgTimeStamp,Fx,Fy,Fz,T-PkgTimeStamp,Tx,Ty,Tz,Rx,Ry,Rz\n");

	fscanf(fp, "%f,%f,%f,%f,%*f,%f,%f,%f,%f,%f,%f,\n",
				timeStamp,
				&data.Fx, &data.Fy, &data.Fz,
				&data.Tx, &data.Ty, &data.Tz,
				&data.Rx, &data.Ry, &data.Rz);

	*filePosition = ftell(fp);

	fclose(fp);
	return data;
}