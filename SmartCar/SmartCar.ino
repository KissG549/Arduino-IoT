#include <AFMotor.h>
#include <HCSR04.h>
#include "BasicCar.h"

UltraSonicDistanceSensor distanceSensor(2, 3); // Trigger pin, echo pin

BasicCar myCar(4,3,1,2);

void setup() {
  Serial.begin(9600);
}

void loop() {

  double distance = distanceSensor.measureDistanceCm();
  Serial.println(distance);
  
  myCar.moveForward();
  Serial.println("Move forward");

  delay(2000);

  myCar.moveBackward();  
  Serial.println("Move backward");

  delay(2000);

  myCar.turnLeft();
  Serial.println("TURN LEFT");

  delay(2000);

  myCar.turnRight();
  
  Serial.println("TURN RIGHT");
  
  delay(2000);
  myCar.stop();
  
  delay(2000);
}
