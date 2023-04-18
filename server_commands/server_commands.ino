// This file defines the code to command the snake

// -------------------------------------------------------------------------------------
// INCLUDES
// -------------------------------------------------------------------------------------
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WiFiServer.h>

const char* ssid = "wifi name";
const char* password = "password";

WiFiClient client;
WiFiServer server(80);

// -------------------------------------------------------------------------------------
// DEFINES
// -------------------------------------------------------------------------------------
#define MIN_PULSE_WIDTH 800 // found on datasheet 
#define MAX_PULSE_WIDTH 2200 // found on datasheet
#define FREQUENCY_SERVO 50 // NEED TO CHECK ON DATASHEET OF SERVO (BUT NO DATASHEET BORDEL)

#define N_SERVOS 3
#define HEX_CHANNEL 0x40

#define N_SAMPLES 20 // for initialization



// -------------------------------------------------------------------------------------
// TYPEDEF
// -------------------------------------------------------------------------------------
typedef Adafruit_PWMServoDriver Driver;

// -------------------------------------------------------------------------------------
// PARAMETERS
// -------------------------------------------------------------------------------------
int min_pulse_width[N_SERVOS];
int max_pulse_width[N_SERVOS];

Driver driver = Driver(HEX_CHANNEL);

int RUNNING = 0;

// -------------------------------------------------------------------------------------
// MAIN FUNCTIONS
// -------------------------------------------------------------------------------------

// This function finds the correct minimum and maximum pulse width for extremas for each servo
void min_max_pulse_width(int servo, int* min_value, int* max_value) {
  /**
  int pulse_wide = map(0, 0, 180, min_value[servo], max_value[servo]);
  driver.setPWM(servo, 0, int(float(pulse_wide) / 1000000 * FREQUENCY_SERVO * 4096)); // send minimum pulse value according to datasheet

  Serial.println(map(driver.getPWM(servo), 0, 4095, min_value[servo], max_value[servo])); // expect 800
  delay(1000);

  pulse_wide = map(180, 0, 180, min_value[servo], max_value[servo]);
  driver.setPWM(servo, 0, int(float(pulse_wide) / 1000000 * FREQUENCY_SERVO * 4096)); // send maximum pulse value according to datasheet

  Serial.println(map(driver.getPWM(servo), 0, 4095, min_value[servo], max_value[servo])); // expect 2200
  delay(1000);
  **/
}

// -------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------

// This function rotates a specific servo of an certain angle
int rotate_with_min_max(int servo, float angle, int* min_array, int* max_array) {

  int pulse_wide = map(angle, 0, 180, min_array[servo], max_array[servo]);

  int al = int(float(pulse_wide) / 1000000 * FREQUENCY_SERVO * 4096);
  Serial.print("ANALOG VALUE (0-4095): ");
  Serial.println(al);
  return al;
}

// -------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------

// This function rotates a specific servo of an certain angle (simpler)
void rotate(int servo, float angle) {
  int value = driver.setPWM(servo, 0, rotate_with_min_max(servo, angle, min_pulse_width, max_pulse_width));
}

// -------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------

// This function activates the inchworm's motion (ASSUME 3 SERVOS)
void inchworm_motion() {
  
}

// -------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------

// This function stops the snake for some time or definitively is it's set to 0
void stop_motion(int seconds) {

}

// -------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------

// This function activates the concertina motion of the snake
void concertina_motion() {

}

// -------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------

// This function activates the undulated motion of the snake
void undulated_motion() {

}


// -------------------------------------------------------------------------------------
// SETUP FUNCTION
// -------------------------------------------------------------------------------------
void setup() {
  Serial.begin(115200);

    WiFi.begin(ssid, password);
    Serial.print("Connecting to Wifi...");
    Serial.print("\n");

    while (WiFi.status() != WL_CONNECTED) {
      Serial.print(WiFi.status());
      delay(1000);
      Serial.print(".");
    }

    Serial.println("");
    Serial.println("Wifi connected");
    Serial.println();
    Serial.println(WiFi.localIP());

    server.begin();

  for(int i = 0; i < N_SERVOS; ++i) {
    min_pulse_width[i] = MIN_PULSE_WIDTH;
    max_pulse_width[i] = MAX_PULSE_WIDTH;
  }

  Serial.println("");
  Serial.println("LETS GOO");
  Serial.println("Initialize System");

  driver.begin();
  driver.wakeup();
  driver.setPWMFreq(FREQUENCY_SERVO); // which frequency of our servos -> need to look datasheet
  
  // Calculate the min and max pulse width for each servos
  for(int i = 0; i < N_SERVOS; ++i) {
    min_max_pulse_width(i, min_pulse_width, max_pulse_width);
  }

  // Print the calibrated values
  Serial.println("Minimum and maximum pulse widths:");
  for (int i = 0; i < N_SERVOS; ++i) {
    Serial.print("Servo ");
    Serial.print(i);
    Serial.print(": ");
    Serial.print(min_pulse_width[i]);
    Serial.print(" - ");
    Serial.print(max_pulse_width[i]);
    Serial.println(" microseconds");
  }

  // Initialize all servo at their midpoint
  for(int i = 0; i < N_SERVOS; ++i) {
    rotate(i, 180);
  }

  // enable the right pins

  // set the initial value of each pin
}

// -------------------------------------------------------------------------------------
// LOOP FUNCTION
// -------------------------------------------------------------------------------------
void loop() {

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Wifi disconnected");
    WiFi.disconnect();
    delay(1000);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.print(".");
    }
    Serial.println("");
    Serial.println("Wifi reconnected");
    server.begin();
  }

  if (client.connected()) {
    if (client.available()) {
      String request = client.readStringUntil('\r');
      Serial.println(request);
      client.flush();

      if (request.indexOf("/forward") != -1) {
        Serial.println("Moving forward");
        inchworm_motion();
        }
      }
}


}
























