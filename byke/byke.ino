
/* Byke: an automatic bike transmission.
   Designed and developed by Joseph Bolling, Ankush Gola, and Ted Brundage.
   HackPrinceton Fall 2014
*/
#include <Servo.h>

#define w_pin 2 
#define p_pin 3
#define servo_pin A0

volatile unsigned int wheel_count; // number of wheel interrupts
volatile unsigned int pedal_count; // number of pedal interrupts

double w_time_stamps[] = {0.0, 0.0, 0.0, 0.0}; 
double w_speed_constant;
double w_current_speed;

double p_time_stamps[] = {0.0, 0.0};
double p_speed_constant;
double p_current_speed;

Servo gear_servo;

void setup() 
{
  Serial.begin(9600);
  
  // Set up wheel hall-effect interrupt
  pinMode(w_pin, INPUT);
  attachInterrupt(0, increase_wheel_count, CHANGE);
  
  // Set up pedal hall-effect interrupt
  pinMode(p_pin, INPUT);
  attachInterrupt(1, update_pedal_count, RISING);
  
  // Initialize wheel speed variables
  wheel_count = 0;
  w_speed_constant = 1.0; /* TODO: Change */
  w_current_speed = 0.0;
  
  // Initialize pedal speed variables
  pedal_count = 0;
  p_speed_constant = 1.0; /* TODO: Change */
  p_current_speed = 0.0;
  
  // Initialize servo
  gear_servo.attach(servo_pin);
  
  
}

void loop() 
{
  Serial.print(w_current_speed);
  Serial.print(", ");
  Serial.println(wheel_count);
  gear_servo.write(0);
  //delay(1000);
  gear_servo.write(90);
  //delay(1000);
  gear_servo.write(180);
  //delay(1000);
  gear_servo.write(90);
  delay(1000);
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
    wheel_count++;
  }
}

void increase_wheel_count()
{
  wheel_count++;
}

void update_pedal_count()
{
  if (pedal_count < 2) {
    p_time_stamps[pedal_count] = millis();
    pedal_count++;
  }
  else {
    unsigned int index = pedal_count % 2;
    double now = millis();
    p_current_speed = p_speed_constant / (now - p_time_stamps[index]);
    p_time_stamps[index] = now;
    pedal_count++;
  }
}
