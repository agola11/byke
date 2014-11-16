
/* Byke: an automatic bike transmission.
   Designed and developed by Joseph Bolling, Ankush Gola, and Ted Brundage.
   HackPrinceton Fall 2014
*/
#include <Servo.h>

#define w_pin 2 
#define p_pin 3
#define servo_pin A0
#define NUM_W_MAGS 8
#define NUM_P_MAGS 4

volatile unsigned int wheel_count; // number of wheel interrupts
volatile unsigned int pedal_count; // number of pedal interrupts

double w_time_stamps[] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}; 
double w_speed_constant;
double w_current_speed;

double p_time_stamps[] = {0.0, 0.0, 0.0, 0.0};
double p_speed_constant;
double p_current_speed;

Servo gear_servo;

void setup() 
{
  Serial.begin(9600);
  
  // Set up wheel hall-effect interrupt
  pinMode(w_pin, INPUT);
  attachInterrupt(0, update_wheel_count, CHANGE);
  
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
  Serial.println(p_current_speed);
}

void update_wheel_count() 
{
  if (wheel_count < NUM_W_MAGS) {
    w_time_stamps[wheel_count] = millis();
    wheel_count++;
  }
  else {
    unsigned int index = wheel_count % NUM_W_MAGS;
    double now = millis();
    w_current_speed = w_speed_constant / (now - w_time_stamps[index]);
    w_time_stamps[index] = now;
    wheel_count++;
  }
}

void update_pedal_count()
{
  if (pedal_count < NUM_P_MAGS) {
    p_time_stamps[pedal_count] = millis();
    pedal_count++;
  }
  else {
    unsigned int index = pedal_count % NUM_P_MAGS;
    double now = millis();
    p_current_speed = p_speed_constant / (now - p_time_stamps[index]);
    p_time_stamps[index] = now;
    pedal_count++;
  }
}
