#include "BasicCar.h"
#include "DiMNGR.h"

#define DEBUG_CAR 1

void BasicCar::move()
{
  while( true )
  {
    double distance = mDistanceMNGR->measureDistanceCm();

#ifdef DEBUG_CAR
      Serial.print("Distance: ");
      Serial.println( distance ); 
#endif
    // can we move?
    if( canMove(distance) )
    {
      // we can move forward
      adaptSpeedByDistance(distance);
      
      double tolerance = 1.0; // cm

      if( mMotorsRunning )
      {
        if( ! mDistanceMNGR->isMoving( tolerance ) )
        {
          // if motors are running, but the car has stopped
          stop();
          obstackleAvoidance(distance);  
        }
        // if motors running and the car is moving, it's ok
      }
      else
      {
          moveForward();
      } // END motors running
      
    }
    else // we can't move
    {
      stop();
      obstackleAvoidance(distance);  
    } // END canMove
   // delay(2000);
  } // END while

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
  #ifdef DEBUG_CAR
      Serial.println("Adapting speed"); 
  #endif
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

void BasicCar::obstackleAvoidance(double distance)
{

#ifdef DEBUG_CAR
      Serial.println("ObstackleAvoidance has started"); 
#endif

  if( mMotorsRunning )
  {
    stop();
  }

  // base turn time in ms
  static const uint8_t xValue = 100;
  // in ms
  static const uint8_t tBackward = 100;
  // to prevent infinite loop
  static const uint8_t maxTry = 15;
  
  // required turn distance in cm
  //static const uint8_t turnDistance = 7.0;
  
  uint8_t turnTime = 0;

  static const double distanceTolerance = 1.0;
  
  // do we have enough space to have a turn?
   uint8_t turnCounter = 0;
 // while( distance <= turnDistance )
   while( !canMove(distance) || !turnCounter )
  {
     moveBackward();
     delay(tBackward); 
     stop();
     //prevDistance = distance;
     // distance = mDistanceMNGR->measureDistanceCm();
     if(turnCounter++ >= maxTry)
     {
#ifdef DEBUG_CAR
      Serial.println("Not able to have enough distance to have a turn. Giving up."); 
#endif
      break;
     }
     
     // turn right or left
     turnTime += xValue;
     if( turnCounter++ % 2 == 0)
     {
       turnRight();
     }
     else
     {
       turnLeft();  
     }
     delay(turnTime);
     stop();
     distance = mDistanceMNGR->measureDistanceCm();
     
     // has the distance changed ?
     // if( !(prevDistance - distanceTolerance < distance 
     //  && prevDistance + distanceTolerance > distance ) )

     if( mDistanceMNGR->isMoving(distanceTolerance) ) 
     {
       // distance has changed
    #ifdef DEBUG_CAR
         Serial.print("Distance has changed: ");
         Serial.println(distance);
    #endif

      // have enough space to go forward
        if( distance >= MIN_REQUIRED_DIST_FOR_GO )
        {
        //  break;  
          return;
        }
      }
    
    }// END while
  

#ifdef DEBUG_CAR
      Serial.println("ObstackleAvoidance has finished"); 
#endif
  return;
  
 /* uint8_t maxTry = 10;
  uint8_t tryCounter = 0;
  
  while (distance <= MIN_REQUIRED_DIST_FOR_TURN)
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
  */
}
