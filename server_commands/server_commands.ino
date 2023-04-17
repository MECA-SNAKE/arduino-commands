// This file defines the code to command the snake

// -------------------------------------------------------------------------------------
// INCLUDES
// -------------------------------------------------------------------------------------
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#define MIN_PULSE_WIDTH 800 // found on datasheet (but we will not use that)
#define MAX_PULSE_WIDTH 2200 // found on datasheet (but we will not use that)
#define FREQUENCY_SERVO 60 // NEED TO CHECK ON DATASHEET OF SERVO (BUT NO DATASHEET BORDEL)

#define N_SERVOS 3
#define HEX_CHANNEL 0x40

#define N_SAMPLES 20 // for initialization


// -------------------------------------------------------------------------------------
// TYPEDEF
// -------------------------------------------------------------------------------------
typedef Adafruit_PWMServoDriver Driver

// -------------------------------------------------------------------------------------
// PARAMETERS
// -------------------------------------------------------------------------------------
int min_pulse_width[n_servos] = {0};
int max_pulse_width[n_servos] = {0};

Driver driver = Driver(HEX_CHANNEL);

// -------------------------------------------------------------------------------------
// MAIN FUNCTIONS
// -------------------------------------------------------------------------------------

// This function finds the correct minimum and maximum pulse width for extremas for each servo
void min_max_pulse_width(int servo, int min_value, int max_value) {
  //TODO
}

// -------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------

// This function rotates a specific servo of an certain angle
int rotate_with_min_max(int angle, int* min_array, int* max_array) {

  int pulse_wide, analog_value = 0;

  pulse_wide = map(angle, 0, 180, min_array[servo], max_array[servo]);

  analog_value = int(float(pulse_wide) / 1000000 * FREQUENCY_SERVO * 4096);
  return analog_value;
}

// -------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------

// This function rotates a specific servo of an certain angle (simpler)
void rotate(int servo, int angle) {
  driver.setPMW(servo, 0, rotate_with_min_max(angle, &min_pulse_width, &max_pulse_width));
}

// -------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------

// This function activates the inchworm's motion
void inchworm_motion() {

}

// -------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------

// This function stops the snake for some time or definitively is it's set to 0
void stop_motion(int seconds) {

}



// -------------------------------------------------------------------------------------
// SETUP FUNCTION
// -------------------------------------------------------------------------------------
void setup() {

  Serial.begin() // put nbr for esp
  Serial.println(F("Initialize System"));

  driver.begin();
  driver.wakeup();
  driver.setPMWFreq(FREQUENCY_SERVO); // which frequency of our servos -> need to look datasheet
  
  // Calculate the min and max pulse width for each servos
  for(int i = 0; i < N_SERVOS; ++i) {
    min_max_pulse_width(i, &min_pulse_width[i], &max_pulse_width[i]);
  }

  // Initialize all servo at their midpoint
  for(int i = 0; i < N_SERVOS; ++i) {
    rotate(i, 90);
  }

  // enable the right pins

  // set the initial value of each pin
}

// -------------------------------------------------------------------------------------
// LOOP FUNCTION
// -------------------------------------------------------------------------------------
void loop() {
}



























