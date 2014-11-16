#include <Servo.h>

#define servo_pin A0

Servo gear_servo;

// GEAR THREE: 2050
// GEAR FOUR: 2150
// GEAR FIVE: 2300
// GEAR ONe: 1690
// GEAR two: 1890


void setup()
{
  Serial.begin(9600);
  gear_servo.attach(servo_pin);
}

void loop()
{
  if (Serial.available() > 0) {
    int output = (int)Serial.parseInt();
    gear_servo.writeMicroseconds(output);
  }
}
