#include "BasicCar.h"
#include "DiMNGR.h"

void BasicCar::move()
{
  while( true )
  {
    double distance = mDistanceMNGR->measureDistanceCm();

    // can we move?
    if( canMove(distance) )
    {
      // we can move forward
      adaptSpeedByDistance(distance);
      
      double tolerance = 1.0; // cm
      uint8_t isMoving = mDistanceMNGR->isMoving( tolerance );

      if( mMotorsRunning )
      {
        if( !isMoving )
        {
          // if motors are running, but the car has stopped
          stop();
          moveBackward();
          delay(100);
          obstackleAvoidance(distance);  
        }
        // if motors running and the car is moving, it's ok
          
      }
      else
      {
          moveForward();
      }
      
    }
    else // we can't move
    {
      stop();
      obstackleAvoidance(distance);  
    }
    
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


void BasicCar::adaptSpeedByDistance(double distance)
{  
    if ( distance > 50.0 )
        {
          setMotorSpeed(250);
        }
        else if ( distance <= 50.0 && distance > 30.0 )
        {
          // slowing down
          setMotorSpeed(220);
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

bool BasicCar::canMove(double distance)
{
  return distance > 10.0;  
}
