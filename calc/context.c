#include "context.h"

Context ContextRead(void)
{
	Context data;
	FILE *fp;

	if ((fp = fopen("context.csv","r")) == NULL)
	{
  		printf("Cant open \"context.csv\" file\n");
  		exit(1);
	}

	fscanf(fp, "Fx0,Fy0,Fz0,P,Tx0,Ty0,Tz0,Cx,Cy,Cz\n");
	fscanf(fp, "%f,%f,%f,%f,%f,%f,%f,%f,%f,%f", 
				&data.Fx0, &data.Fy0, &data.Fz0,
				&data.P,
				&data.Tx0, &data.Ty0, &data.Tz0,
				&data.Cx,  &data.Cy,  &data.Cz);

	fclose(fp);
	return data;
}