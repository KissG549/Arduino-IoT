#include <AFMotor.h>

AF_DCMotor motor_front_left(4);
AF_DCMotor motor_front_right(3);
AF_DCMotor motor_rear_left(1);
AF_DCMotor motor_rear_right(2);

void motors(const uint8_t frl_cmd, uint8_t frr_cmd = 0, uint8_t rel_cmd = 0, uint8_t rer_cmd = 0)
{
  if(frr_cmd == 0)
  {
      frr_cmd = frl_cmd;
  }
  if(rel_cmd == 0)
  {
      rel_cmd = frl_cmd;
  }
  if(rer_cmd == 0)
  {
      rer_cmd = frl_cmd;
  }
  motor_front_left.run(frl_cmd);
  motor_front_right.run(frr_cmd);
  motor_rear_left.run(rel_cmd);
  motor_rear_right.run(rer_cmd);
}

void move_forward()
{
  motors(FORWARD);  
}

void move_backward()
{
  motors(BACKWARD);  
}

void turn_left()
{
  motors(BACKWARD, FORWARD, BACKWARD, FORWARD);
}

void turn_right()
{
    motors(FORWARD, BACKWARD, FORWARD, BACKWARD);
}
void setup() {
  /*
   * No load speed: 
      DC 3V -> 125R/min
      DC 5V -> 200R/min
      DC 6V -> 230R/min
   * Load speed: 
      DC 3V -> 95R/min
      DC 5V -> 160R/min
      DC 6V -> 175R/min        
  */

  motor_front_left.setSpeed(255);
  motor_front_right.setSpeed(255);
  motor_rear_left.setSpeed(255);
  motor_rear_right.setSpeed(255);
  Serial.begin(9600);
}

void loop() {

  move_forward();

  delay(2000);

  motors( RELEASE );

  move_backward( );

  delay(2000);

  motors( RELEASE );

  turn_left();

  delay(2000);

  motors( RELEASE );

  turn_right();

  delay(2000);

  motors( RELEASE );
  
  delay(2000);
}
