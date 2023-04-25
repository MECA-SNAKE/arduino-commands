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
/*
WiFiClient client;
WiFiServer server(80);
*/

// -------------------------------------------------------------------------------------
// DEFINES
// -------------------------------------------------------------------------------------
#define MIN_PULSE_WIDTH 800 // found on datasheet 
#define MAX_PULSE_WIDTH 2000 // found on datasheet
#define MID_PULSE_WIDTH ((MIN_PULSE_WIDTH + MAX_PULSE_WIDTH) / 2)
#define FREQUENCY_SERVO 50

#define N_SERVOS 3
#define HEX_CHANNEL 0x40


// -------------------------------------------------------------------------------------
// TYPEDEF
// -------------------------------------------------------------------------------------
typedef Adafruit_PWMServoDriver Driver;

// -------------------------------------------------------------------------------------
// PARAMETERS
// -------------------------------------------------------------------------------------
//int min_pulse_width[N_SERVOS];
//int max_pulse_width[N_SERVOS];

Driver driver = Driver(HEX_CHANNEL);

int RUNNING = 0;

// -------------------------------------------------------------------------------------
// MAIN FUNCTIONS
// -------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------

// This function rotates a specific servo of an certain angle
int rotate_with_min_max(int servo, float angle) {

  int pulse_wide = map(angle, 0, 180, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);

  int al = int(float(pulse_wide) / 1000000 * FREQUENCY_SERVO * 4096);
  Serial.print("ANALOG VALUE (0-4095): ");
  Serial.println(al);
  return al;
}

// -------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------

// This function rotates a specific servo of an certain angle (simpler)
void rotate(int servo, float angle) {
  int value = driver.setPWM(servo, 0, rotate_with_min_max(servo, angle));
}

// -------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------

// This function activates the inchworm's motion (ASSUME 3 SERVOS)
void inchworm_motion() {

  for(int phi = 0; phi <45; ++phi){
    rotate(0, 90-phi);
    rotate(1, 90-phi);
    rotate(2, 90+phi);

    delay(10);
  }

  for(int i = 0; i<=4; ++i){
    for(int phi = 0; phi<45; ++phi){

      rotate(i, 45+2*phi);
      rotate(i+1, 135-2*phi);
      rotate(i+2, 135-2*phi);
      rotate(i+3, 90+phi);
    }
  }

  for(int phi = 0; phi <45; ++phi){
    rotate(4, 45+phi);
    rotate(5, 45+phi);
    rotate(6, 135-phi);
  } 
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
/*
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
*/

  Serial.println("");
  Serial.println("Initialize System");

  driver.begin();
  driver.wakeup();
  driver.setPWMFreq(FREQUENCY_SERVO); // which frequency of our servos -> need to look datasheet

  // Initialize all servo at their midpoint
  for(int i = 0; i < N_SERVOS; i++) {
    delay(100);
    rotate(i, 90);
  }

  

  // enable the right pins

  // set the initial value of each pin
}

// -------------------------------------------------------------------------------------
// LOOP FUNCTION
// -------------------------------------------------------------------------------------
void loop() {



/*
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
*/

}
























