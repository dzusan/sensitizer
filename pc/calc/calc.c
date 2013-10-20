#include "context.h"
#include "stream.h"
#include "output.h"

Clear Calc(Context constant, Stream reading)
{
	Clear final;
	
	final.Fx = reading.Fx - constant.Fx0 - constant.P * reading.Rx;
	final.Fy = reading.Fy - constant.Fy0 - constant.P * reading.Ry;
	final.Fz = reading.Fz - constant.Fz0 - constant.P * reading.Rz;

	final.Tx = reading.Tx - constant.Tx0 - constant.Cx * reading.Rx;
	final.Ty = reading.Ty - constant.Ty0 - constant.Cy * reading.Ry;
	final.Tz = reading.Tz - constant.Tz0 - constant.Cz * reading.Rz;

	return final;
}

int main(void)
{
	Context currentConst;
	Stream currentReading;
	Clear currentOutput;
	long int count;
	long int filePosition = 0;
	float timeStamp = 0;

	currentConst = ContextRead();

	for(count = 0; count < 37000; count++)/*37000 for large record sample*/
	{
		currentReading = StreamRead(&filePosition, &timeStamp);
		currentOutput = Calc(currentConst, currentReading);
		ClearWrite(currentOutput, &timeStamp);
	}

	return 0;
}
