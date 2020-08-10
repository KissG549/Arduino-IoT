#include "BHCSR04.h"

double BufferedUltraSonicDistanceSensor::measureDistanceCm()
{
	double distance = UltraSonicDistanceSensor::measureDistanceCm();
	mDistanceBuffer.add(distance);
	return distance;
}
