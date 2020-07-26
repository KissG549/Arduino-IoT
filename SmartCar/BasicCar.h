#ifndef _BasicCar_h_
#define _BasicCar_h_

#include <AFMotor.h>
#include <HCSR04.h>
#include <inttypes.h>

class BasicCar
{
public:
	BasicCar(uint8_t pMotorFrontLeftPin,
		uint8_t pMotorFrontRightPin,
		uint8_t pMotorRearLeftPin,
		uint8_t pMotorRearRightPin)
		:mMotorFrontLeft(pMotorFrontLeftPin),
		mMotorFrontRight(pMotorFrontRightPin),
		mMotorRearLeft(pMotorRearLeftPin),
		mMotorRearRight(pMotorRearRightPin)
	{
		/*
		 * No load speed:
			DC 3V -> 125R/min
			DC 5V -> 200R/min
			DC 6V -> 230R/min
		 * Load speed:
			DC 3V -> 95R/min
			DC 5V -> 160R/min
			DC 6V -> 175R/min
	*/
		mMotorFrontLeft.setSpeed(255);
		mMotorFrontRight.setSpeed(255);
		mMotorRearLeft.setSpeed(255);
		mMotorRearRight.setSpeed(255);
	};

	~BasicCar() {};

	void moveForward();
	void moveBackward();
	void turnLeft();
	void turnRight();
	void stop();

	void avoidObstackle(UltraSonicDistanceSensor& pDistanceSensor);

private:
	AF_DCMotor mMotorFrontLeft;
	AF_DCMotor mMotorFrontRight;
	AF_DCMotor mMotorRearLeft;
	AF_DCMotor mMotorRearRight;

	void motorsControl(const uint8_t pFrlCmd, uint8_t pFrrCmd = 0, uint8_t pRelCmd = 0, uint8_t pRerCmd = 0);
};

#endif