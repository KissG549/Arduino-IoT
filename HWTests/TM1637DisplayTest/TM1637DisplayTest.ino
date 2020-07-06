
#include <SevenSegmentTM1637.h>
#include <SevenSegmentExtended.h>

const byte PIN_CLK = 4;   // define CLK pin (any digital pin)
const byte PIN_DIO = 3;   // define DIO pin (any digital pin)

SevenSegmentExtended    display(PIN_CLK, PIN_DIO);

const uint8_t DISPLAY_BRIGHTNESS = 30;

void setup() {
  Serial.begin(9600);         // initializes the Serial connection @ 9600 baud
  display.begin();            // initializes the display
  display.setBacklight(DISPLAY_BRIGHTNESS);  // set the brightness to 100 %
  display.print("INIT");      // display INIT on the display
  // display.setBacklight(50);
 // display.setContrast(100);
  delay(1000);                // wait 1000 ms
}

void loop() {
// set brightness
  display.print("8888");
  //display.clear();
  display.print("1H");
  delay(1000);
  for( uint8_t i = 0; i < 255; ++i )
  {
    display.printNumber( i , false, true );
    
    delay(500);
  }
  
  delay(1000);
}
