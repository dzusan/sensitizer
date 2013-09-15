#include "context.h"
#include "stream.h"
#include "output.h"

Clear Calc(Context constant, Stream reading)
{
	Clear final;
	
	final.Fx = reading.Fx - constant.Fx0 - constant.P * reading.Rx;
	final.Fy = reading.Fy - constant.Fy0 - constant.P * reading.Ry;
	final.Fz = reading.Fz - constant.Fz0 - constant.P * reading.Rz;

	return final;
}

int main(void)
{
	Context currentConst;
	Stream currentReading;
	Clear currentOutput;
	int count;
	long int filePosition = 0;

	currentConst = ContextRead();

	for(count = 0; count < 3; count++)
	{
		currentReading = StreamRead(&filePosition);
		currentOutput = Calc(currentConst, currentReading);
		ClearWrite(currentOutput);
	}

	return 0;
}