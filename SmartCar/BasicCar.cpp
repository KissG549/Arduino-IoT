#include "BasicCar.h"

void BasicCar::move()
{
	double distance = mDistanceSensor->measureDistanceCm();
	
	while (true)
	{
		Serial.print("1 Distance: ");
		Serial.println( distance );
		
		if (distance > MIN_REQUIRED_DIST_FOR_TURN)
		{
			moveForward();
		}
		else
		{
			lookForDirection();
		}

		delay(100);
		distance = mDistanceSensor->measureDistanceCm();
	}
}

void BasicCar::moveForward()
{
	motorsControl(FORWARD);
	Serial.println("Move forward");
}

void BasicCar::moveBackward()
{
	motorsControl(BACKWARD);
	Serial.println("Move backward");
}

void BasicCar::turnLeft()
{
	motorsControl(BACKWARD, FORWARD, BACKWARD, FORWARD);
	Serial.println("Turn left");
}

void BasicCar::turnRight()
{
	motorsControl(FORWARD, BACKWARD, FORWARD, BACKWARD);
	Serial.println("Turn right");
}

void BasicCar::stop()
{
	motorsControl(BRAKE);
	Serial.println("Stop");
}

void BasicCar::setDistanceSensor(UltraSonicDistanceSensor& pDistanceSensor)
{
	mDistanceSensor = &pDistanceSensor;
}

void BasicCar::lookForDirection()
{
	double distance = 9.0;
	uint8_t maxTry = 20;
	uint8_t tryCounter = 0;

	stop();

	while (distance < MIN_REQUIRED_DIST_FOR_TURN)
	{
		turnRight();
		delay(50);
		distance = mDistanceSensor->measureDistanceCm();
		Serial.print("2 Distance: ");
		Serial.println(distance);

		if (++tryCounter >= maxTry)
		{
			break;
		}
	}
	
	if (tryCounter >= maxTry) 
	{
		turnRight();
		delay(50);
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