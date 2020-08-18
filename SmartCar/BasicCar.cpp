#include "BasicCar.h"
#include "DiMNGR.h"

#define DEBUG_CAR 1

void BasicCar::move()
{
  while( true )
  {
#ifdef DEBUG_CAR
      Serial.println("While->");
#endif
    double distance = mDistanceMNGR->measureDistanceCm();

    String output = String(static_cast<uint8_t>(distance));
    mDisplay->clear();
    mDisplay->print( output );
    
#ifdef DEBUG_CAR
      Serial.print("While-> Distance: ");
      Serial.println( distance ); 
#endif
    // can we move?
    if( canMove(distance) )
    {
#ifdef DEBUG_CAR
      Serial.println("While-> if( canMove(distance) ) We can Move");
#endif
      // we can move forward
      adaptSpeedByDistance(distance);
      
      double tolerance = 1.0; // cm

      if( mMotorsRunning )
      {
#ifdef DEBUG_CAR
      Serial.println("if( mMotorsRunning )Motors Running");
#endif
        if( ! mDistanceMNGR->isMoving( tolerance ) )
        {
#ifdef DEBUG_CAR
      Serial.println("if( ! mDistanceMNGR->isMoving( tolerance ) ) The car doesn't move");
#endif
          // if motors are running, but the car has stopped
          stop();
          obstackleAvoidance(distance);  
        }
        // if motors running and the car is moving, it's ok
      }
      else
      {
#ifdef DEBUG_CAR
      Serial.println("else Move forward");
#endif
          moveForward();
      } // END motors running
      
    }
    else // we can't move
    {
#ifdef DEBUG_CAR
      Serial.println("else We can't move");
#endif
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
      Serial.print("setMotorSpeed to ");
      Serial.println(spd); 
 #endif     
}


void BasicCar::adaptSpeedByDistance(double distance)
{  
  uint8_t spd = 255;
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
      Serial.print("Adapting speed to ");
      Serial.println(spd); 
 #endif    
  
    setMotorSpeed(spd);
}

bool BasicCar::canMove(double distance)
{
  return distance > MIN_REQUIRED_DIST_FOR_GO;  
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

     String output = String(static_cast<uint8_t>(distance));
     mDisplay->print( output );
     mDisplay->clear();
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
