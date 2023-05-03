// This file defines the code to command the snake

// -------------------------------------------------------------------------------------
// INCLUDES
// -------------------------------------------------------------------------------------
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

// -------------------------------------------------------------------------------------
// DEFINES
// -------------------------------------------------------------------------------------
#define MIN_PULSE_WIDTH 800 // found on datasheet 
#define MAX_PULSE_WIDTH 2000 // found on datasheet
#define FREQUENCY_SERVO 50

#define N_SERVOS 8
#define HEX_CHANNEL 0x40

// -------------------------------------------------------------------------------------
// TYPEDEF
// -------------------------------------------------------------------------------------
typedef Adafruit_PWMServoDriver Driver;

// -------------------------------------------------------------------------------------
// PARAMETERS
// -------------------------------------------------------------------------------------
Driver driver = Driver(HEX_CHANNEL);

const char* ssid = "giogio_larue";
const char* password = "21ff99a2c0cd";

AsyncWebServer server(80);

float rot = 0.0;

// -------------------------------------------------------------------------------------
// MAIN FUNCTIONS
// -------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------

// This function rotates a specific servo of an certain angle
int rotate_with_min_max(int servo, float angle) {

  int pulse_wide = map(angle, 0, 180, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);

  int al = int(float(pulse_wide) / 1000000 * FREQUENCY_SERVO * 4096);
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
   for(int phi = 0; phi <90; ++phi){
    rotate(0, 90-phi);
    rotate(1, 90-phi);
    rotate(2, 90+phi);

    delay(10);
  }

  for(int i = 0; i<=4; ++i){
    for(int phi = 0; phi<90; ++phi){
      if(i>0){
        rotate(i-1, 180-phi);
      }

      rotate(i, 2*phi);

      rotate(i+2, 180-2*phi);
      rotate(i+3, 90+phi);

      delay(10);
    }
    delay(1000);
  }

  for(int i =0;i<N_SERVOS; i++){
    rotate(i, 90);
    delay(10);
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
  for(int i = 0; i < 360; i++) {
    for(int j = 0; j < N_SERVOS; j++) {
      delay(30);
      if(j > N_SERVOS - 6) { // j > 2
        rot = 90 + 50 * sin(3 * i + (1 * j * 2 * 3.1415) / (N_SERVOS - 1));
      } else {
        rot = 90 + 62 * sin(3 * i + (1 * j * 2 * 3.1415) / (N_SERVOS - 1));
      }
      
      rotate(j, rot);
    }
    delay(100);
  }
}


// -------------------------------------------------------------------------------------
// SETUP FUNCTION
// -------------------------------------------------------------------------------------
void setup() {  

  Serial.begin(9600); 
/*
  Serial.print("WIFI ...");
  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wifi...");
  Serial.print("\n");

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("Wifi connected");
  Serial.println();
  Serial.println(WiFi.localIP());

  server.on("/mode", HTTP_POST, [](AsyncWebServerRequest *request){
    AsyncWebParameter* p = request->getParam(0); // 1: start / 0: stop
    if(p->isPost()) {
      Serial.printf("POST[%s]: %s\n", p->name().c_str(), p->value().c_str());

      request->send_P(200, "text/html", "post request for /mode success");
    }
  });

  server.on("/motion", HTTP_POST, [](AsyncWebServerRequest *request){
    //request->send_P(200, "text/html", "Hello from the ESP8266");
  });

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
}

// -------------------------------------------------------------------------------------
// LOOP FUNCTION
// -------------------------------------------------------------------------------------
void loop() {

  undulated_motion();

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
}