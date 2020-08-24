#include "BasicCar.h"
#include "DiMNGR.h"

void BasicCar::move()
{
/*  
 *   Identify minimal speed to mooving
  uint8_t spd = 0;
  while(!mDistanceMNGR->isMoving(2.0))
  {
    setMotorSpeed(++spd);
    moveForward(50);
    mDistanceMNGR->measureDistanceCm();
    mDisplay->clear();
    mDisplay->print( spd );
#ifdef DEBUG_CAR
     Serial.print("Speed: ");
     Serial.println(spd);
#endif
  }
  delay(10000);
 */
  while(true)
  {
     double distance = mDistanceMNGR->measureDistanceCm();

     String output = String(static_cast<uint8_t>(distance));
     mDisplay->clear();
     mDisplay->print( output );
#ifdef DEBUG_CAR
     Serial.print("Distance: ");
     Serial.println(distance);
#endif
      
     adaptSpeedByDistance(distance);

      if( canMove(distance) && !mMotorsRunning )
      {
          moveForward(0);
      }
      else
      {
        obstackleAvoidance(distance);
      }
      delay(10);
  } // END while
}

void BasicCar::moveForward(uint8_t pDelayMsec )
{
	motorsControl(FORWARD);
	Serial.println("Move forward");
  mMotorsRunning = true;
  mDisplay->clear();
  mDisplay->print( "F" );
  delay(pDelayMsec);
}

void BasicCar::moveBackward(uint8_t pDelayMsec )
{
	motorsControl(BACKWARD);
	Serial.println("Move backward");
  mMotorsRunning = true;
  delay(pDelayMsec);
}

void BasicCar::turnLeft(uint8_t pDelayMsec )
{
	motorsControl(BACKWARD, FORWARD, BACKWARD, FORWARD);
	Serial.println("Turn left");
  mDisplay->clear();
  mDisplay->print( "L" );
  mMotorsRunning = true;
  delay(pDelayMsec);
}

void BasicCar::turnRight(uint8_t pDelayMsec )
{
	motorsControl(FORWARD, BACKWARD, FORWARD, BACKWARD);
	Serial.println("Turn right");
  mDisplay->clear();
  mDisplay->print( "R" );
  mMotorsRunning = true;
  delay(pDelayMsec);
}

void BasicCar::stop()
{
  mMotorsRunning = false;
	motorsControl(RELEASE);
	Serial.println("Stop");
  mDisplay->clear();
  mDisplay->print( "STOP" );
}

void BasicCar::setDisplay(SevenSegmentExtended* pDisp)
{
  mDisplay = pDisp;
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

void BasicCar::setMotorSpeed(uint8_t pSpd)
{
    mMotorFrontLeft.setSpeed(pSpd);
    mMotorFrontRight.setSpeed(pSpd);
    mMotorRearLeft.setSpeed(pSpd);
    mMotorRearRight.setSpeed(pSpd);

 #ifdef DEBUG_CAR
      //Serial.print("setMotorSpeed to ");
      //Serial.println(spd); 
 #endif     
}

void BasicCar::adaptSpeedByDistance(double pDistance)
{  
  if(!mMotorsRunning)
  {
    return;
  }
  
  uint8_t spd = 255;

    if ( pDistance > DISTANCE_FAR )
        {
          spd = 250;
        }
    else if ( pDistance <= DISTANCE_FAR && pDistance > DISTANCE_MIDDLE )
        {
          spd = 200;
        }
    else if ( pDistance <= DISTANCE_MIDDLE && pDistance > DISTANCE_CLOSE )
        { 
          spd = 150;
        } 
    else if ( pDistance <= DISTANCE_CLOSE )
        {
          spd = 100;
        }
    else
        {
          spd = 200;
        }
 
 #ifdef DEBUG_CAR
     Serial.print("Adapting speed to ");
     Serial.println(spd); 
 #endif    
  
    setMotorSpeed(spd);
}

bool BasicCar::canMove(double pDistance)
{
  return pDistance > MIN_REQUIRED_DIST_FOR_GO;  
}

bool BasicCar::canTurn(double pDistance)
{
  return pDistance > MIN_REQUIRED_DIST_FOR_TURN;
}

void BasicCar::obstackleAvoidance(double pDistance)
{
   if(mMotorsRunning)
   {
    stop();
   }
}
