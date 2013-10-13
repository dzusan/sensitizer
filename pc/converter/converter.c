#include <stdio.h>
#include <stdint.h>

int main(void)
{
	int count;
	FILE *Fin, *Fout;
	unsigned values[8];
	uint16_t tmp;
	int16_t point;
	double timeStamp;
	float Rx = 0.5;
	float Ry = 0.5;
	float Rz = 0.5;

	if ((Fin = fopen("raw.csv","r")) == NULL)
	{
  		printf("Cant open IN file\n");
  		exit(1);
	}

	if ((Fout = fopen("stream.csv","w")) == NULL)
	{
  		printf("Cant open OUT file\n");
  		exit(1);
	}

    fscanf(Fin, "Time [s],Packet,Type,Identifier,Control,Data,CRC,ACK\n");
    fprintf(Fout, "F-PkgTimeStamp,Fx,Fy,Fz,T-PkgTimeStamp,Tx,Ty,Tz,Rx,Ry,Rz\n");
    for(count = 0; count < 10; count++)
      fscanf(Fin, "%*f,%*d,DATA,%*X,0x8,%*X %*X %*X %*X %*X %*X %*X %*X,%*X,%*[A-Z]\n");

    while(!feof(Fin))
    {
        fscanf(Fin, "%lf,%*d,DATA,%*X,0x8,%X %X %X %X %X %X %X %X,%*X,%*[A-Z]\n",
               &timeStamp, values + 0, values + 1, values + 2, values + 3,
                           values + 4, values + 5, values + 6, values + 7);

        if((values[6] == 0x00) && (values[7] == 0x01))
        {
            fprintf(Fout, "%lf,", timeStamp);
            for(count = 0; count < 6; count += 2)
            {
                tmp = (values[count] << 8) | values[count + 1];
                point = *(int16_t *) &tmp;
                fprintf(Fout, "%f,", (float)point / 100);
            }
        }
        else fprintf(Fout, "%f,%f,%f\n", Rx, Ry, Rz);
    }

	fclose(Fin);
	fclose(Fout);

	return 0;
}
