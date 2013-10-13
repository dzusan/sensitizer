#include <stdio.h>
#include <stdint.h>

int main(void)
{
	int count;
	FILE *Fin, *Fout;
	float Fx0,Fy0,Fz0,Tx0,Ty0,Tz0;
  float P  = 0;
	float Cx = 0;
	float Cy = 0;
	float Cz = 0;

	if ((Fin = fopen("stream.csv","r")) == NULL)
	{
  		printf("Cant open IN file\n");
  		exit(1);
	}

	if ((Fout = fopen("context.csv","w")) == NULL)
	{
  		printf("Cant open OUT file\n");
  		exit(1);
	}

    fscanf(Fin, "F-PkgTimeStamp,Fx,Fy,Fz,T-PkgTimeStamp,Tx,Ty,Tz,Rx,Ry,Rz\n");

    for(count = 0; count < 50; count ++)
      fscanf(Fin, "%*f,%f,%f,%f,%*f,%f,%f,%f,%*f,%*f,%*f,\n",
             &Fx0, &Fy0, &Fz0, &Tx0, &Ty0, &Tz0);

    fprintf(Fout, "Fx0,Fy0,Fz0,P,Tx0,Ty0,Tz0,Cx,Cy,Cz\n%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,",
                   Fx0,Fy0,Fz0,P,Tx0,Ty0,Tz0,Cx,Cy,Cz);

	fclose(Fin);
	fclose(Fout);

	return 0;
}
