#ifndef _BasicCar_h_
#define _BasicCar_h_


#ifndef DEBUG_CAR
#define DEBUG_CAR 1
#endif

#include <ArduinoLog.h>
#include <AFMotor.h>
#include <SevenSegmentTM1637.h>
#include <SevenSegmentExtended.h>
#include "Arduino.h"
#include "BHCSR04.h"
#include "DiMNGR.h"
#include "Buffer.h"

static const float MIN_REQUIRED_DIST_FOR_GO = 16.0;
static const float MIN_REQUIRED_DIST_FOR_GO_AFTER_OBSTACKLE = MIN_REQUIRED_DIST_FOR_GO * 1.5;
static const float MIN_REQUIRED_DIST_FOR_TURN = 20.0;
static const float DISTANCE_CLOSE = 40.0;
static const float DISTANCE_MIDDLE = 60.0;
static const float DISTANCE_FAR = 80.0;

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
    mDisplay = nullptr;
	};

	~BasicCar() {};

	void move();
	void moveForward(uint8_t pDelayMsec = 100);
	void moveBackward(uint8_t pDelayMsec = 100);
	void turnLeft(uint8_t pDelayMsec = 100);
	void turnRight(uint8_t pDelayMsecec = 100);
	void stop();
  void setDisplay(SevenSegmentExtended* pDisp);

	void setDistanceMNGR(DistanceManager* pDistanceMNGR);

private:
	AF_DCMotor mMotorFrontLeft;
	AF_DCMotor mMotorFrontRight;
	AF_DCMotor mMotorRearLeft;
	AF_DCMotor mMotorRearRight;

	DistanceManager *mDistanceMNGR;
  bool mMotorsRunning;

  SevenSegmentExtended *mDisplay;
  
	void motorsControl(const uint8_t pFrlCmd, uint8_t pFrrCmd = 0, uint8_t pRelCmd = 0, uint8_t pRerCmd = 0);
  void setMotorSpeed(uint8_t pSpeed);
  void adaptSpeedByDistance(double pDistance);
  bool canMove(double pDistance);
  bool canTurn(double pDistance);
  void obstackleAvoidance(double pDistance);

};

#endif
