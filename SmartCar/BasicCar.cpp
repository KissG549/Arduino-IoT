#include "BasicCar.h"
#include "DiMNGR.h"

void BasicCar::move()
{
	double distance = mDistanceMNGR->measureDistanceCm();

  double tolerance = 1.0; // cm

  uint8_t maxDirectionTry = 10;
  
	while (true)
	{
		Serial.print("1 Distance: ");
		Serial.println( distance );

    uint8_t isMoving =  mDistanceMNGR->isMoving( tolerance );

    uint8_t maxDirectionTry = 10;

    Serial.print("Speed: ");
    Serial.println( isMoving );


    if( !isMoving && !mMotorsRunning )
    {
        /* 
             can we move? is there enough space to move forward?
              move
    
              else // we can not move forward
                look for direction
                  is the lookup reached it's limit?
                    go backward for a short time
                    and stop
                    look for direction
        */

        if( distance >= 10.0 )
        {
            moveForward();
        }
        else
        {
            delay(200);
            lookForDirection(distance);
        }
    }
    else if( !isMoving && mMotorsRunning )
    {
      stop();  
    }
    else if( isMoving && mMotorsRunning )
    {
        if ( distance > 50.0 )
        {
          setMotorSpeed(255);
        }
        else if ( distance <= 50.0 && distance > 30.0 )
        {
          // slowing down
          setMotorSpeed(150);
        }
        else if ( distance <= 30.0 && distance > 10.0 )
        {
          // slowing down  
          setMotorSpeed(180);
        } 
        else if ( distance < 10.0)
        {
          setMotorSpeed(130);
        }
        else
        {
          setMotorSpeed(200);
        }
    }
    else if( isMoving && !mMotorsRunning )
    {
       stop();  
    }
    else
    {
      Serial.println("Unexpected state");
      stop();
     }


    Serial.println("After if");

		delay(1000);
    Serial.println("Before next ");
		distance = mDistanceMNGR->measureDistanceCm();
    Serial.println("After next ");
	}
}

void BasicCar::moveForward()
{
	motorsControl(FORWARD);
	Serial.println("Move forward");
  mMotorsRunning = true;
}

void BasicCar::moveBackward()
{
	motorsControl(BACKWARD);
	Serial.println("Move backward");
  mMotorsRunning = true;
}

void BasicCar::turnLeft()
{
	motorsControl(BACKWARD, FORWARD, BACKWARD, FORWARD);
	Serial.println("Turn left");
  mMotorsRunning = true;
}

void BasicCar::turnRight()
{
	motorsControl(FORWARD, BACKWARD, FORWARD, BACKWARD);
	Serial.println("Turn right");
  mMotorsRunning = true;
}

void BasicCar::stop()
{
  mMotorsRunning = false;
	motorsControl(RELEASE);
	Serial.println("Stop");
}

void BasicCar::setDistanceMNGR(DistanceManager& pDistanceMNGR)
{
	mDistanceMNGR = &pDistanceMNGR;
}

void BasicCar::lookForDirection(double distance)
{
	uint8_t maxTry = 10;
	uint8_t tryCounter = 0;

	stop();

	while (distance < MIN_REQUIRED_DIST_FOR_TURN)
	{
		turnRight();
		delay(100);
    stop();
		distance = mDistanceMNGR->measureDistanceCm();
		Serial.print("2 Distance: ");
		Serial.println(distance);

		if (++tryCounter >= maxTry)
		{
			break;
		}
	}
	
	if (tryCounter >= maxTry) 
	{
		stop();
	}

}

void BasicCar::motorsControl(const uint8_t pFrlCmd, uint8_t pFrrCmd = 0, uint8_t pRelCmd = 0, uint8_t pRerCmd = 0)
{
	if (pFrrCmd == 0)
	{
		pFrrCmd = pFrlCmd;
	}
	if (pRelCmd == 0)
	{
		pRelCmd = pFrlCmd;
	}
	if (pRerCmd == 0)
	{
		pRerCmd = pFrlCmd;
	}
	mMotorFrontLeft.run(pFrlCmd);
	mMotorFrontRight.run(pFrrCmd);
	mMotorRearLeft.run(pRelCmd);
	mMotorRearRight.run(pRerCmd);
}

void BasicCar::setMotorSpeed(uint8_t speed)
{
    mMotorFrontLeft.setSpeed(speed);
    mMotorFrontRight.setSpeed(speed);
    mMotorRearLeft.setSpeed(speed);
    mMotorRearRight.setSpeed(speed);
}
