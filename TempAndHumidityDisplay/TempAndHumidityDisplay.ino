#include <SevenSegmentTM1637.h>
#include <SevenSegmentExtended.h>
#include "DHT.h"      // DHT11 Temp and relative humidity sensor

const uint8_t TM1637_CLK_PIN  = 4;
const uint8_t TM1637_DATA_PIN = 3;

const uint8_t DHT11_PIN = 5;

#define DHT_TYPE DHT11

const uint8_t DISPLAY_BRIGHTNESS = 30;

SevenSegmentExtended    display(TM1637_CLK_PIN, TM1637_DATA_PIN);
DHT                   dht(DHT11_PIN, DHT_TYPE);

void init_Serial()
{
  Serial.begin(9600);
}

void init_TM1637_display()
{
  Serial.println("Test TM1637 7 segment display");
  display.begin();            // initializes the display
  display.setBacklight(DISPLAY_BRIGHTNESS);
  display.print("INIT");

  uint8_t tm1637_test_command[4];

  tm1637_test_command[0] = B00000000;
  tm1637_test_command[1] = B00000000;
  tm1637_test_command[2] = B00000000;
  tm1637_test_command[3] = B00000000;

  for( uint8_t rounds = 0; rounds < 7; ++rounds )
  {
     Serial.println(rounds);
     
     tm1637_test_command[0] |= B00000001 << rounds;
     tm1637_test_command[1] |= B00000001 << rounds;
     tm1637_test_command[2] |= B00000001 << rounds;
     tm1637_test_command[3] |= B00000001 << rounds;
       
    display.printRaw(tm1637_test_command);
    
    delay(100);
  }

  // colon
  tm1637_test_command[0] = B00000000;
  tm1637_test_command[1] = B10000000;
  tm1637_test_command[2] = B00000000;
  tm1637_test_command[3] = B00000000;
  
  display.printRaw(tm1637_test_command);
  delay(100);
  Serial.println("Test TM1637 7 segment display finished");
  display.clear();
}

void init_DHT11M_sensor()
{
  dht.begin();
}

void setup() {
  // put your setup code here, to run once:
  init_Serial();
  init_TM1637_display();
  init_DHT11M_sensor();
}

void loop() {
  delay(2000);
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float humidity = dht.readHumidity();

  delay(1000);
  // Read temperature as Celsius (the default)
  float temperature = dht.readTemperature();

  String output;

  // Check if any reads failed and exit early (to try again).
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println(F("Failed to read from DHT sensor!"));
  }
  else
  {
    output = String(static_cast<uint8_t>(temperature));
    output += "C";

    display.clear();
    display.print( output );

    Serial.print(F("Temperature: "));
    Serial.print(temperature);
    Serial.print(F("°C "));
    Serial.print("\n");

    delay(2000);

    output = String(static_cast<uint8_t>(humidity));
    output += "H";

    display.clear();
    display.print( output );

    Serial.print(F("Humidity: "));
    Serial.print(humidity);
    Serial.print("\n");
    // delay(2000);
  }

}
