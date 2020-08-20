#ifndef _DIMNGR_H_
#define _DIMNGR_H_

#include "Arduino.h"
#include "BHCSR04.h"
#include "Buffer.h"

class DistanceManager
{
public:
	DistanceManager(BufferedUltraSonicDistanceSensor* pBUltrasonicSensor) :
		mBUltrasonicSensor(pBUltrasonicSensor) {};

	DistanceManager(const uint8_t pHcsrTriggerPin, const uint8_t pHcsrEchoPin)
		:mBUltrasonicSensor(new BufferedUltraSonicDistanceSensor(pHcsrTriggerPin, pHcsrEchoPin)) {};

	/*
		@return avarage speed of moving
			+ getting closer
			- getting away
		@parameter pTolerance 
			in cm
	*/
	uint8_t isMoving(double pTolerance)
	{
		return _isMoving(pTolerance);
	}

	double measureDistanceCm()
	{
		return mBUltrasonicSensor->measureDistanceCm();
	}

private:
	BufferedUltraSonicDistanceSensor* mBUltrasonicSensor;

	/*
		@pTolerance 
			+/- tolerance in cm
	*/
	uint8_t _isMoving(double pTolerance)
	{
		/*
     * Getting closer or away?
     * 
     * Processing values starting with the remote end
     * 
     * If the last X is in tolerance, than return 0, because it doesn't moving
     * 
     * Find the last X different items
     * 
     *    are these items in descending order?  => getting closer to something
     *  
     *    are these items in ascending order?   => getting away from something
     *  
     *  
		*/
		
		Buffer<double>* distanceBuff = &(mBUltrasonicSensor->buffer());
   Serial.println("Buffer copied");

		if (distanceBuff->size() < 3)
		{
      Serial.println("Distance buffer size less than 3");
			return 0;
		}
       
		Buffer<double> historyBuffer;

    Serial.println("Hist Buffer created");
		int counter = 0;
		uint8_t increasing = 0;
		uint8_t decreasing = 0;
		uint8_t stopped	   = 0;
		/*
			buffer
			[0] [1] [2] ...[N-1] [N]
			i = N
			buffer[i-1] - buffer[i] 
			= 
			when buffer[i-1] is greater than buffer[i], the result is positive
				a list of decreasing values (getting closer to an obstacle)
			when buffer[i-1] is lower than buffer[i], the result is negative
				a list of increasing value (getting away from an obstacle)
			else they are the same
				stopped

		*/

    Serial.println("Checking distances");
		for ( int it = distanceBuff->size() - 1; it > 0; --it )
		{
			double diff = distanceBuff->at(it - 1) - distanceBuff->at(it);
      Serial.print("Distance is");
      Serial.println(diff);

			if ( diff >= 0.0 - pTolerance && diff <= pTolerance )
			{
				// stopped
				if (increasing || decreasing)
				{
					break;
				}
				++stopped;
			}
			// positive and out of the tolerance
			else if ( diff > pTolerance )
			{
				// getting closer to an obstacle
				if (stopped || decreasing)
				{
					break;
				}
				++increasing;
			}
			else 
			{
				// getting away from an obstacle
				if (increasing || stopped)
				{
					break;
				}
				++decreasing;
			}
			historyBuffer.add( diff );
		}

    Serial.println("Calc avg distance");
		double avgSpeed = 0.0;
		for (int it = 0; it < historyBuffer.size(); ++it)
		{
			avgSpeed += historyBuffer[it];
		}

		avgSpeed /= historyBuffer.size();

    Serial.print("Avg speed is:");
    Serial.println(avgSpeed);
		return avgSpeed;
	}
};


#endif // !_DIMNGR_H_
