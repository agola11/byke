#include <Servo.h>

#define servo_pin A0

Servo gear_servo;

// GEAR THREE: 1990, 1870
// GEAR FOUR: 2150, 2025
// GEAR FIVE: 2300, 2200
// GEAR ONe: 1690, 1510
// GEAR two: 1840, 1730


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
