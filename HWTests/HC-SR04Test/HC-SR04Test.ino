#include <HCSR04.h>

const uint8_t triggerPin = 13;
const uint8_t echoPin = 12;

UltraSonicDistanceSensor distanceSensor(triggerPin, echoPin);

void setup() 
{
  Serial.begin(9600);
}

void loop() 
{
    double distance = distanceSensor.measureDistanceCm();
    Serial.println(distance);
    delay(500);
}
