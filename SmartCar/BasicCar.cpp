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

  for( uint8_t it = 0; it < 5; ++it )
  {
    mDistanceMNGR->measureDistanceCm();  
    delay(50);
  }
  
  while(true)
  {
     double distance = mDistanceMNGR->measureDistanceCm();

     String output = String(static_cast<uint8_t>(distance));
     mDisplay->clear();
     mDisplay->print( output );
#ifdef DEBUG_CAR
     Serial.print("Distance: ");
     Serial.println(distance);
     Serial.print("canMove? ");
     Serial.println(canMove(distance));
     Serial.print("!Motors running? ");
     Serial.println(!mMotorsRunning);     
#endif
      
     adaptSpeedByDistance(distance);

     if( canMove(distance) )
     {
      if( !mMotorsRunning )
      {
        moveForward();
      }
      else if(!mDistanceMNGR->isMoving(1.0))
      {
        moveBackward();
        delay(200);
        stop();
        obstackleAvoidance(distance);
      }
     }
     else
     {
#ifdef DEBUG_CAR
     Serial.println("Start obstackle avoidance here>>>>");
#endif
        obstackleAvoidance(distance);
#ifdef DEBUG_CAR
     Serial.println("<<<<END obstackle avoidance here");
#endif
     }
      //delay(1000);
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
  /*
   * Stop => check distance => make turn possible (move backward) => make forward move possible (turn) => return
   * 
  */
   if(mMotorsRunning)
   {
      stop();
   }

   setMotorSpeed( 150 );
   
   moveBackward();

   while( !canTurn(pDistance) )
   {
     pDistance = mDistanceMNGR->measureDistanceCm();

     String output = String(static_cast<uint8_t>(pDistance));
     mDisplay->clear();
     mDisplay->print( output );
#ifdef DEBUG_CAR
     Serial.print("Moving backward - Distance: ");
     Serial.println(pDistance);
#endif
     delay(50);
   }
   stop();

   static const uint8_t xValue     = 200; // base turn time in ms
   static const uint8_t tBackward  = 200; // in ms
   static const uint8_t maxTry     = 30; // to prevent infinite loop
   uint16_t turnTime = 0;

   static const double distanceTolerance = 1.0;
   uint8_t turnCounter = 0;

   pDistance = mDistanceMNGR->measureDistanceCm();

   double minRequiredDistance = (pDistance > MIN_REQUIRED_DIST_FOR_GO_AFTER_OBSTACKLE) ? pDistance : MIN_REQUIRED_DIST_FOR_GO_AFTER_OBSTACKLE;

#ifdef DEBUG_CAR
     Serial.print("After backward - distance: ");
     Serial.println(pDistance);
     Serial.print("Min req dist: ");
     Serial.println(minRequiredDistance);
#endif

   bool canWeGo = false;

   while( pDistance <= minRequiredDistance )
   {
      pDistance = mDistanceMNGR->measureDistanceCm();
     
      String output = String(static_cast<uint8_t>(pDistance));
      mDisplay->clear();
      mDisplay->print( output );
      
#ifdef DEBUG_CAR
      Serial.print("Distance: ");
      Serial.println(pDistance);
#endif
      //uint8_t positions = buff.size() - 1;
     
    /*  if( buff.size() > 1 )
      {
        canWeGo = buff.at( positions ) >  minRequiredDistance && buff.at( positions - 1 ) >  minRequiredDistance;
      }
      else
      {
        pDistance = mDistanceMNGR->measureDistanceCm();
        buff.add(pDistance);
        continue;
      }
      */

      if(turnCounter++ >= maxTry)
      {
        break;
      }
      turnTime += xValue;
      if( turnTime > 2500 )
      {
        return;  
      }
        
      if( turnCounter % 2 == 0)
      {
        turnRight();
#ifdef DEBUG_CAR
      Serial.print("->>>>>>>>>>> TurnRight: ");
#endif
      }
      else
      {
        turnLeft();  
#ifdef DEBUG_CAR
      Serial.print("->>>>>>>>>>> TurnLeft ");
#endif
      }
#ifdef DEBUG_CAR
      Serial.println(turnTime);
#endif
      delay(turnTime);
      
      stop();
      // pDistance = mDistanceMNGR->measureDistanceCm();
      
     

     // delay(500);
   }
   
}
