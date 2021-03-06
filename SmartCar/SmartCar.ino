/*
  Log: https://github.com/thijse/Arduino-Log/
*/
#include <ArduinoLog.h>
#include <HCSR04.h>
#include <SevenSegmentTM1637.h>
#include <SevenSegmentExtended.h>
#include "BasicCar.h"
#include "DiMNGR.h"
 
#ifndef DEBUG_CAR
#define DEBUG_CAR 1
#endif

const uint8_t DISPLAY_BRIGHTNESS = 30;
const uint8_t TM1637_CLK_PIN    = A1;
const uint8_t TM1637_DATA_PIN   = A0;

SevenSegmentExtended    display(TM1637_CLK_PIN, TM1637_DATA_PIN); 
BasicCar myCar(4,3,1,2);

DistanceManager distanceMNGR(10,9);

void setup() 
{
  Serial.begin(9600);
  display.begin();
  display.setBacklight(DISPLAY_BRIGHTNESS);
  display.print("INIT");
  myCar.setDisplay(&display);
  myCar.setDistanceMNGR(&distanceMNGR);

  Log.begin(LOG_LEVEL_VERBOSE, &Serial);
}

void loop() 
{
  display.clear();
  myCar.move();
}
