#ifndef B_HCSR04_H_
#define B_HCSR04_H_

#include <HCSR04.h>
#include "Buffer.h"


class BufferedUltraSonicDistanceSensor : public UltraSonicDistanceSensor {

public:
	BufferedUltraSonicDistanceSensor(int pTriggerPin, int pEchoPin)
		: UltraSonicDistanceSensor(pTriggerPin, pEchoPin)
	{
		//mDistanceBuffer = new double[MAX_BUFFER_SIZE];
	};

	double measureDistanceCm();

	Buffer<double>& buffer()
	{
		return mDistanceBuffer;
	}

private:
	Buffer<double> mDistanceBuffer;
};

#endif
