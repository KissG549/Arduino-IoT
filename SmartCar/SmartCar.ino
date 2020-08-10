#include <HCSR04.h>
#include "BasicCar.h"
#include "DiMNGR.h"

BasicCar myCar(4,3,1,2);

//UltraSonicDistanceSensor distanceSensor(10, 9); // Trigger pin, echo pin
DistanceManager distanceMNGR(10,9);

void setup() 
{
  Serial.begin(9600);
}

void loop() 
{
  myCar.setDistanceMNGR(distanceMNGR);
  myCar.move();
}
