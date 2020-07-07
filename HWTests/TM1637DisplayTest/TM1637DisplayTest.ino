
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
  display.clear();
  display.print("1H");
  delay(1000);
  display.clear();

  uint8_t  my_command[5];

  my_command[0] = B00000000;
  my_command[1] = B00000000;
  my_command[2] = B00000000;
  my_command[3] = B00000000;
  my_command[4] = B00000000;

  for( uint8_t rounds = 0; rounds < 7; ++rounds )
  {
     Serial.println(rounds);
     
     my_command[0] |= B00000001 << rounds;
     my_command[1] |= B00000001 << rounds;
     my_command[2] |= B00000001 << rounds;
     my_command[3] |= B00000001 << rounds;
     my_command[4] |= B00000001 << rounds;
       
    display.printRaw(my_command);
    
    delay(100);
  }
  
  delay(2000);

  my_command[0] = B00000000;
  my_command[1] = B10000000;
  my_command[2] = B00000000;
  my_command[3] = B00000000;
  my_command[4] = B00000000;

  display.clear();

  display.printRaw(my_command);

  delay(2000);
}
