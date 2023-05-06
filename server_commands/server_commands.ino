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

typedef enum {
  UNDULATED, CONCERTINA, NONE;
} MOTION;

// -------------------------------------------------------------------------------------
// PARAMETERS
// -------------------------------------------------------------------------------------
Driver driver = Driver(HEX_CHANNEL);

const char* ssid = "giogio_larue";
const char* password = "21ff99a2c0cd";

AsyncWebServer server(80);

int is_running = 0; 
MOTION motion = MOTION.NONE;

double amplitude = 0.0;
double offset = 0.0;
double wavelength = 0.0;
double frequency = 0.0; 

double rot = 0.0;
double angles[N_SERVOS - 2];

// -------------------------------------------------------------------------------------
// MAIN FUNCTIONS
// -------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------

// This function rotates a specific servo of an certain angle
int rotate_with_min_max(int servo, double angle) {

  int pulse_wide = map(angle, 0, 180, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);

  int al = int(float(pulse_wide) / 1000000 * FREQUENCY_SERVO * 4096);
  return al;
}

// -------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------

// This function rotates a specific servo of an certain angle (simpler)
void rotate(int servo, double angle) {
  int value = driver.setPWM(servo, 0, rotate_with_min_max(servo, angle));
}

// -------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------

// This function activates the inchworm's motion (ASSUME 3 SERVOS)
void inchworm_motion() {
   for(int phi = 0; phi < 90; ++phi){
    rotate(0, 90-phi);
    rotate(1, 90-phi);
    rotate(2, 90+phi);

    delay(10);
  }

  for(int i = 0; i<=4; ++i){
    for(int phi = 0; phi<90; ++phi){
      if(i > 0){
        rotate(i-1, 180-phi);
      }

      rotate(i, 2*phi);

      rotate(i+2, 180-2*phi);
      rotate(i+3, 90+phi);

      delay(10);
    }
    delay(1000);
  }

  for(int i =0;i < N_SERVOS; i++){
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

void straight() {
  for(int i = 0; i < N_SERVOS; i++) {
    delay(100);
    rotate(i, 90);
  }
}

// -------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------

float forward_kinetics_2() {
  float val = 0.0;
  for(int i = 0; i < N_SERVOS - 2; i++) {
    val += sin(angles[i]);
  }
  return 90.0 - asin(val);
}

// -------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------

// This function activates the undulated motion of the snake
void undulated_motion_kinetics() { // NEED TO BE TESTED BETTER MAYBE
  for(int i = 0; i < 360; i++) {
    for(int j = 0; j < N_SERVOS; j++) { // n servo - 1
     // delay(30);
      rot = 90 + 55 * sin(2 * 2 * 3.1415 * i + (2 * j * 2 * 3.1415) / (N_SERVOS - 1));
      rotate(j, rot);
      //angles[j] = rot;
      //delay(10);
      //rotate(N_SERVOS - 1, forward_kinetics_2());
    }
    delay(10);
  }
}

// -------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------

// DIDNT ADD KINETICS BUT WE COULD TRY
void undulated_motion_3() {
  for(int i = 0; i < 360; i++) {
    float brads = i * 3.1415 / 180.0;     //convert from degrees to radians
    for(int j=0; j<N_SERVOS; j++){  
        if(is_running == 0) {
          return;
        }
        rotate(j, 90 + 55 * sin(4 * brads + (1 * j * 2 * 3.1415) / (N_SERVOS - 1)));
    }
   delay(10);
  }
}

float l = 1.0;
float factor = 0.0;

float amplitude_factor(float ampl, float stop_ampl, float steps) {
    return pow(ampl / stop_ampl, 1 / steps);
}    

void undulated_motion_4() {
  l = 1.0;
  factor = amplitude_factor(55, 35, N_SERVOS - 1);

  for(int i = 0; i < 360; i++) {

    float brads = i * 3.1415 / 180.0;

    for(int j=0; j<N_SERVOS; j++){  
        rotate(j, 90 + l * 55 * sin(4 * brads + (1 * j * 2 * 3.1415) / (N_SERVOS - 1))); // need to test with n and not n - 1 maybe for the period
        l /= factor;
    }

   delay(10);
  }
}

void update_mode(int m) {
  if(m == 0 || m == 1) {
    is_running = m;
  }
}

void update_amplitude(double amp) {
  if(amp >= 30 && amp <= 70) {
    amplitude = amp;
  }
}

void update_wavelength(double wl) {
  if(wl >= 0.5 && wl <= 4) {
    wavelength = wl;
  }
}

void update_frequency(double f) {
  if(f >= 0.5 && wl <= 4) {
    frequency = f;
  }
}

void update_offset(double off) {
  if(off >= 0 && off <= 20) {
    offset = off;
  }
}

void reset() {
  is_running = 0;
  offset = 0.0;
  motion = MOTION.NONE;
  frequency = 1.0;
  wavelength = 2.0;
  amplitude = 50.0;
}


// -------------------------------------------------------------------------------------
// SETUP FUNCTION
// -------------------------------------------------------------------------------------
void setup() {  

  Serial.begin(9600); 

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
    if(request->hasParam("value", true)) {
      AsyncWebParameter* p = request->getParam("value", true);
      int value = p->value().toInt();
      Serial.println("value for /mode: %d", value);

      update_mode(value);

      request->send(200, "text/html", "Received value for /mode on the ESP8266");
    } else {
      request->send(404, "text/html", "Error on /mode POST request");
    }
  });

  server.on("/motion", HTTP_POST, [](AsyncWebServerRequest *request){
    
  });

  server.on("/amp", HTTP_POST, [](AsyncWebServerRequest *request){
    
  });

  server.on("/freq", HTTP_POST, [](AsyncWebServerRequest *request){
    
  });

  server.on("/wl", HTTP_POST, [](AsyncWebServerRequest *request){
    
  });

  server.on("/offset", HTTP_POST, [](AsyncWebServerRequest *request){
    
  });

  server.begin();

  Serial.println("");
  Serial.println("Initialize System");

  driver.begin();
  driver.wakeup();
  driver.setPWMFreq(FREQUENCY_SERVO); // which frequency of our servos

  // Initialize all servo at their midpoint
  straight();

  delay(2000);
}

// -------------------------------------------------------------------------------------
// LOOP FUNCTION
// -------------------------------------------------------------------------------------
void loop() {

  undulated_motion_4();

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

  if(is_running == 1) {
    switch(motion){

      case UNDULATED:
        undulated_motion_3();
        break;

      case CONCERTINA:
        concertina_motion();
        break;

      case NONE:
        // ...
        break;

      default:
        break;    

    }  

  } else {
    reset();
  }
  
}

















