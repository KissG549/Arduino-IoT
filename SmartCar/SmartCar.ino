#include <HCSR04.h>
#include <SevenSegmentTM1637.h>
#include <SevenSegmentExtended.h>

#include "BasicCar.h"
#include "DiMNGR.h"

const uint8_t DISPLAY_BRIGHTNESS = 30;

const uint8_t TM1637_CLK_PIN  = A1;
const uint8_t TM1637_DATA_PIN = A0;

SevenSegmentExtended    display(TM1637_CLK_PIN, TM1637_DATA_PIN);


BasicCar myCar(4,3,1,2);

//UltraSonicDistanceSensor distanceSensor(10, 9); // Trigger pin, echo pin
DistanceManager distanceMNGR(10,9);

void setup() 
{
  Serial.begin(9600);
  display.begin();
  display.setBacklight(DISPLAY_BRIGHTNESS);
  display.print("INIT");
  myCar.setDisplay(&display);
  myCar.setDistanceMNGR(&distanceMNGR);
}

void loop() 
{
  display.clear();
  myCar.move();
}
