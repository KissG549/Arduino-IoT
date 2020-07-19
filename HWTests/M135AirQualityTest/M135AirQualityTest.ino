#include <MQ135.h>

const uint8_t MQ135_DPIN = 6; // digital pin
const uint8_t MQ135_APIN = A1; // analog pin

MQ135 gasSensor = MQ135(MQ135_APIN);

void setup() {
  // put your setup code here, to run once:
  pinMode(MQ135_DPIN, INPUT);
  Serial.begin(9600);
  float rzero = gasSensor.getRZero();
  Serial.print("MQ135 ZERO: ");
  Serial.println(rzero);
}

void loop() {
  // put your main code here, to run repeatedly:
  int value = digitalRead(MQ135_DPIN);
  float ppm = gasSensor.getPPM();
  Serial.print("PPM: ");
  Serial.println(ppm);
  delay(1000);
}
