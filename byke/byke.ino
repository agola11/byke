
/*
   Byke: an automatic bike transmission.
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
volatile unsigned int change_w_count; // for zero speed detection
volatile unsigned int change_p_count; // for zero speed detection

int servo_outs[] = {1680, 1840, 2000, 2150, 2300};

// constants associated with wheel speed
double w_time_stamps[] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}; 
double w_speed_constant;
double w_current_speed;

// constants associated with pedal speed
double p_time_stamps[] = {0.0, 0.0, 0.0, 0.0};
double p_speed_constant;
double p_current_speed;

double curr_time; // current time for speed measurements
double gear_ratios[] = {0.44680851063829785, 0.3148148148148148, 0.29, 0.2619047619047619, 0.23529411764705882}; // pedal cadence / wheel cadence
double target_cadence = 6.5; 

volatile int CURRENT_GEAR;
volatile int CAN_CHANGE = 1;

Servo gear_servo;

// to reduce too much shifting
double time_of_last_shift;

void setup() 
{
  Serial.begin(9600);
  
  // Set up wheel hall-effect interrupt
  pinMode(w_pin, INPUT);
  attachInterrupt(0, update_wheel_count, CHANGE);
  
  // Set up pedal hall-effect interrupt
  pinMode(p_pin, INPUT);
  attachInterrupt(1, update_pedal_count, CHANGE);
  
  // Initialize wheel speed variables
  wheel_count = 0;
  w_speed_constant = 10000.0;
  w_current_speed = 0.0;
  change_w_count = 0;
  
  // Initialize pedal speed variables
  pedal_count = 0;
  p_speed_constant = 10000.0;
  p_current_speed = 0.0;
  change_p_count = 0;
  
  // Initialize servo
  gear_servo.attach(servo_pin);
  
  // Initialize gear/ servo out
  CURRENT_GEAR = 2;
  gear_servo.writeMicroseconds(servo_outs[CURRENT_GEAR]);
  
  time_of_last_shift = millis();
  curr_time = millis();
  
}

void do_delay() {
  delay(100);
}

// Shift gear down
void shift_down() {
  
  CAN_CHANGE = 0;
  if (CURRENT_GEAR != 0 && p_current_speed >= 3.50) {
    CURRENT_GEAR--;
    gear_servo.writeMicroseconds(servo_outs[CURRENT_GEAR]-50);
    do_delay();
    gear_servo.writeMicroseconds(servo_outs[CURRENT_GEAR]);
  }
  //do_delay();
  time_of_last_shift = millis();
  CAN_CHANGE = 1;
  
}

// Shift gear up
void shift_up() {

  CAN_CHANGE = 0;
  if (CURRENT_GEAR != 4 && p_current_speed >= 3.50) {
    CURRENT_GEAR++;
    gear_servo.writeMicroseconds(servo_outs[CURRENT_GEAR]);
  }
  //do_delay();
  time_of_last_shift = millis();
  CAN_CHANGE = 1;
 
}

void loop() 
{
  Serial.print(w_current_speed);
  Serial.print(", ");
  Serial.println(p_current_speed);
  
  // Handle zero speed case
  if ((millis() - curr_time) >= 1250.0) {
    if (change_p_count <= 0) {p_current_speed = 0;}
    if (change_w_count <= 0) {w_current_speed = 0;}
    change_p_count = 0;
    change_w_count = 0;
    curr_time = millis();
  }
  
  double error = (w_current_speed*gear_ratios[CURRENT_GEAR]) - target_cadence;
  
  if (error <= 0) 
  {
    if (CURRENT_GEAR != 0) {
      // ( wheelcad_low + (wheelcad_hi - wheelcad_low)*
      if (error < -2.00 && (millis()-time_of_last_shift > 2900.0 )) {shift_down();}
    }
  }
  else {
    if (CURRENT_GEAR != 4) {
      // wheelcad_cur + (wheelcad_hi - wheelcad_cur) * 0.75
      if (error > 2.00 && (millis()-time_of_last_shift > 2900.0 )) {shift_up();}
    }
  }
  
}

// ISR for hall effect change, use to calculate rear wheel speed.
void update_wheel_count() 
{
  if (wheel_count < NUM_W_MAGS) {
    w_time_stamps[wheel_count] = millis();
  }
  else {
    unsigned int index = wheel_count % NUM_W_MAGS;
    double now = millis();
    w_current_speed = w_speed_constant / (now - w_time_stamps[index]);
    w_time_stamps[index] = now;
  }
  wheel_count++;
  change_w_count++;
}

// ISR for hall effect change, use to calculate pedal cadence
void update_pedal_count()
{
  if (pedal_count < NUM_P_MAGS) {
    p_time_stamps[pedal_count] = millis();
  }
  else {
    unsigned int index = pedal_count % NUM_P_MAGS;
    double now = millis();
    p_current_speed = p_speed_constant / (now - p_time_stamps[index]);
    p_time_stamps[index] = now;
  }
  pedal_count++;
  change_p_count++;
}
