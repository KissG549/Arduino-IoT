#include "EnvironmentSubject.h"
#include "DHT.h"      // DHT11 Temp and relative humidity sensor

class DHT11Subject : public EnvironmentSubject
{
  public:
    DHT11Subject(uint8_t pPin, uint8_t pType)
    {
      dht11_error_counter = 0;
      dht = new DHT(pPin, pType);
      dht->begin();
    };
    
    ~DHT11Subject(){};
    
    void read()
    {
      // Reading temperature or humidity takes about 250 milliseconds!
      // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
      float humidity    = dht->readHumidity(false);
      float temperature = dht->readTemperature(false, false);

       // Check if any reads failed and exit early (to try again).
      if (isnan(humidity) || isnan(temperature)) 
      {
        Serial.println(F("Failed to read from DHT sensor!"));
        ++dht11_error_counter;
        Serial.print(F("Error counter: "));
        Serial.println(dht11_error_counter);
      }
      else
      {
        String output = String(static_cast<uint8_t>(temperature));
        output += "C";
        // notify observers with temperature value
        createMessage(output);

        delay(2000);
        
        output = String(static_cast<uint8_t>(humidity));
        output += "H";
        // notify observers with temperature value
        createMessage(output);
        
        delay(2000);
      }
    };

  private:
    DHT *dht;
    uint16_t dht11_error_counter;

  
  };
