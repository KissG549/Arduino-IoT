#ifndef _BasicCar_h_
#define _BasicCar_h_

#include <AFMotor.h>
#include <inttypes.h>
#include "Arduino.h"
#include "BHCSR04.h"
#include "DiMNGR.h"

const double MIN_REQUIRED_DIST_FOR_TURN = 10.0;

class BasicCar
{
public:
	BasicCar(
		uint8_t pMotorFrontLeftPin,
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
		mMotorFrontLeft.setSpeed(240);
		mMotorFrontRight.setSpeed(240);
		mMotorRearLeft.setSpeed(240);
		mMotorRearRight.setSpeed(240);
		mDistanceMNGR = nullptr;
    mMotorsRunning = false;
	};

	~BasicCar() {};

	void move();
	void moveForward();
	void moveBackward();
	void turnLeft();
	void turnRight();
	void stop();

	void setDistanceMNGR(DistanceManager& pDistanceMNGR);

private:
	AF_DCMotor mMotorFrontLeft;
	AF_DCMotor mMotorFrontRight;
	AF_DCMotor mMotorRearLeft;
	AF_DCMotor mMotorRearRight;

	DistanceManager *mDistanceMNGR;
  bool mMotorsRunning;

	void motorsControl(const uint8_t pFrlCmd, uint8_t pFrrCmd = 0, uint8_t pRelCmd = 0, uint8_t pRerCmd = 0);
  void setMotorSpeed(uint8_t speed);
  void adaptSpeedByDistance(double distance);
  bool canMove(double distance);
  void obstackleAvoidance(double distance);
  
};

#endif
