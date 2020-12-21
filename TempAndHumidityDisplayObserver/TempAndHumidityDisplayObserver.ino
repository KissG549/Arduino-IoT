/*
 * Used libraries: https://github.com/mike-matera/ArduinoSTL
 * 
*/

#include "DisplayObserver.h"
#include "TM1637Observer.h"
#include "DHT11Subject.h"

const static uint8_t DHT11_PIN = 5;
const static uint8_t TM1637_CLK_PIN  = 4;
const static uint8_t TM1637_DATA_PIN = 3;

/* Create new subject for dht11 sensor */
DHT11Subject* dht11sensor = new DHT11Subject(DHT11_PIN, DHT11);
/* Create new display object and subscribe to the dht11 notifications */
TM1637Observer* TM1637display = new TM1637Observer(dht11sensor, TM1637_CLK_PIN, TM1637_DATA_PIN);

void setup() {
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  dht11sensor->read();
}
