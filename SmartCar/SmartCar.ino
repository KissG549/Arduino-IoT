#include <HCSR04.h>
#include "BasicCar.h"

BasicCar myCar(4,3,1,2);

UltraSonicDistanceSensor distanceSensor(2, 3); // Trigger pin, echo pin

void setup() 
{
  Serial.begin(9600);
}

void loop() 
{
  double distance = distanceSensor.measureDistanceCm();
  myCar.setDistanceSensor(distanceSensor);
  Serial.print("0 Distance: ");
  Serial.println( distance );
  myCar.move();
  delay(100);
}
