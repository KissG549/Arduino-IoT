#include "BasicCar.h"
#include "DiMNGR.h"

#define DEBUG_CAR 1

void BasicCar::move()
{
}

void BasicCar::moveForward()
{
	motorsControl(FORWARD);
	Serial.println("Move forward");
  mMotorsRunning = true;
  mDisplay->clear();
  mDisplay->print( "F" );
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
  mDisplay->clear();
  mDisplay->print( "L" );
  mMotorsRunning = true;
}

void BasicCar::turnRight()
{
	motorsControl(FORWARD, BACKWARD, FORWARD, BACKWARD);
	Serial.println("Turn right");
  mDisplay->clear();
  mDisplay->print( "R" );
  mMotorsRunning = true;
}

void BasicCar::stop()
{
  mMotorsRunning = false;
	motorsControl(RELEASE);
	Serial.println("Stop");
  mDisplay->clear();
  mDisplay->print( "STOP" );
}

void BasicCar::setDisplay(SevenSegmentExtended* disp)
{
  mDisplay = disp;
}

void BasicCar::setDistanceMNGR(DistanceManager* pDistanceMNGR)
{
	mDistanceMNGR = pDistanceMNGR;
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

void BasicCar::setMotorSpeed(uint8_t spd)
{
    mMotorFrontLeft.setSpeed(spd);
    mMotorFrontRight.setSpeed(spd);
    mMotorRearLeft.setSpeed(spd);
    mMotorRearRight.setSpeed(spd);

 #ifdef DEBUG_CAR
     // Serial.print("setMotorSpeed to ");
     // Serial.println(spd); 
 #endif     
}


void BasicCar::adaptSpeedByDistance(double distance)
{  
  uint8_t spd = 255;

#ifdef DEBUG_CAR
  distance = 10.0;
#endif
  
    if ( distance > 50.0 )
        {
          spd = 250;
        }
    else if ( distance <= 50.0 && distance > 30.0 )
        {
          // slowing down
          spd = 220;
        }
    else if ( distance <= 30.0 && distance > MIN_REQUIRED_DIST_FOR_GO )
        {
          // slowing down  
          spd = 180;
        } 
    else if ( distance <= MIN_REQUIRED_DIST_FOR_GO )
        {
          spd = 130;
        }
    else
        {
          spd = 200;
        }
 
 #ifdef DEBUG_CAR
     // Serial.print("Adapting speed to ");
     // Serial.println(spd); 
 #endif    
  
    setMotorSpeed(spd);
}

bool BasicCar::canMove(double distance)
{
  return distance > MIN_REQUIRED_DIST_FOR_GO;  
}

bool BasicCar::canTurn(double distance)
{
  return distance > MIN_REQUIRED_DIST_FOR_TURN;
}

void BasicCar::obstackleAvoidance(double distance)
{
}
