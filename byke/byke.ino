
/* Byke: an automatic bike transmission.
   Designed and developed by Joseph Bolling, Ankush Gola, and Ted Brundage.
*/
#include <Servo.h>

#define servo_pin A0

volatile unsigned int wheel_count;
int pin = 2;
double w_time_stamps[] = {0.0, 0.0, 0.0, 0.0};
double w_speed_constant;
double w_current_speed;
Servo gear_servo;

void setup() 
{
  Serial.begin(9600);
  
  // Set up wheel hall-effect interrupt
  pinMode(pin, INPUT);
  attachInterrupt(0, update_wheel_count, RISING);
  
  // Initialize wheel speed variables
  wheel_count = 0;
  w_speed_constant = 1.0; /* TODO: Change */
  w_current_speed;
  
  // Initialize pedal speed variables
  
  // Initialize servo
  gear_servo.attach(servo_pin);
  
  
}

void loop() 
{
  Serial.println(w_current_speed);
  

}

void update_wheel_count() 
{
  if (wheel_count < 4) {
    w_time_stamps[wheel_count] = millis();
    wheel_count++;
  }
  else {
    unsigned int index = wheel_count % 4;
    double now = millis();
    w_current_speed = w_speed_constant / (now - w_time_stamps[index]);
    w_time_stamps[index] = now;
  }
}
