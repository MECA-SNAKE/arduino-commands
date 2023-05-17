// This file defines the code to command the snake

// -------------------------------------------------------------------------------------
// INCLUDES
// -------------------------------------------------------------------------------------
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#include <ESP8266WiFi.h>
//#include <ESPAsyncTCP.h>
//#include <ESPAsyncWebServer.h>

// -------------------------------------------------------------------------------------
// DEFINES
// -------------------------------------------------------------------------------------
#define MIN_PULSE_WIDTH 800 // found on datasheet 
#define MAX_PULSE_WIDTH 2000 // found on datasheet
#define FREQUENCY_SERVO 50

#define N_SERVOS 12
#define HEX_CHANNEL 0x40

// -------------------------------------------------------------------------------------
// TYPEDEF
// -------------------------------------------------------------------------------------
typedef Adafruit_PWMServoDriver Driver;

typedef enum {
  UNDULATED, CONCERTINA, INCHWORM, NONE
} motion;

typedef enum {
  FORWARD, BACKWARD
} direction;

// -------------------------------------------------------------------------------------
// PARAMETERS
// -------------------------------------------------------------------------------------
Driver driver = Driver(HEX_CHANNEL);

const char* ssid = "";
const char* password = "";

//AsyncWebServer server(80);

int is_running = 0; 
int reset_snake = 0;
motion motion_snake = NONE;
direction dir_snake = FORWARD;

double speed_inchworm = 0.0;

double amplitude = 0.0;
double offset = 0.0;
double wavelength = 0.0;
double frequency = 0.0; 

double angles[N_SERVOS - 1] = {0};

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
/*
// This function activates the inchworm's motion
void inchworm_motion() {

  // ROBIN update here for backward and the speed (inchworm_speed variable) the inchworm
  // giovanni handle here the fact that when we change the motion or the direction, we need to return (tomorrow)

  for(int phi = 0; phi <90; ++phi){
    if(reset_snake == 0) {
      while(is_running == 0) {
        if(reset_snake == 1) {
          return;
        } else {
          continue;
        }
      }
      rotate(0, 90-phi);
      rotate(1, 90-phi);
      rotate(2, 90+phi);

      delay(10);
    } else {
      return;
    } 
  }

  for(int i = 0; i <= 4; ++i){
    for(int phi = 0; phi < 90; ++phi){
      
      if(reset_snake == 0) {
        
        while(is_running == 0) {
          if(reset_snake == 1) {
            return;
          } else {
            continue;
          }  
        }
        if(i>0){
          rotate(i-1, 180-phi);
        }

        rotate(i, 2*phi);
        rotate(i+2, 180-2*phi);
        rotate(i+3, 90+phi);
        delay(10);
        

      } else {
        return;
      }
      
    }  
    delay(1000);
  }

 
  for(int i = 0; i < N_SERVOS; i++) {
    if(reset_snake == 0) {
      while(is_running == 0) {
        if(reset_snake == 1) {
          return;
        } else {
          continue;
        }  
      }
      rotate(i, 90);
      delay(10);
    } else {
      return;
    }
  }
}
*/
// -------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------

// NEED TO BE TESTED
float g = 1.0;
float temp = 0.0;
float sum = 0.0;
float factor = 0.0;
float amp = 5.0;
float ampf = 70.0;
float st = 150.0;

float test(float a, float a_f, float s) {
    return pow(a_f / a, 1 / s);
}    

// This function activates the conertina motion of the snake
void concertina_motion_v1() {
  g = 1.0;
  factor = test(amp, ampf, st);

  for(int i = 0; i < 150; i++) {
    for(int j = 0; j < N_SERVOS - 2; j++) {
      temp = g * amp * sin((1 * j * 2 * 3.1415) / (N_SERVOS - 3));
      rotate(j, 90 + temp);
      sum += temp;
    }
    delay(10);
    rotate(N_SERVOS - 2, 90 - sum);
    g *= factor;
  } 

  g = 1.0;
  factor = test(ampf, amp, 50.0);

  for(int i = 0; i < 50; i++) {
    for(int j = 0; j < N_SERVOS - 2; j++) {
      temp = g * ampf * sin((1 * j * 2 * 3.1415) / (N_SERVOS - 3));
      rotate(j, 90 + temp);
      sum += temp;
    }
    delay(10);
    rotate(N_SERVOS - 2, 90 - sum);
    g *= factor;
  } 
}

void concertina_motion_v2() {

}

// -------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------

// This function sets the snake in a straight line
void straight() {
  for(int i = 0; i < N_SERVOS; i++) {
    delay(10);
    rotate(i, 90);
  }
}

// -------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------

double s = 0.0;
double rot = 0.0;

// TEST FUNCTION
void undulated_motion_kinetics_v1() {
  for(int i = 0; i < 360; i++) {
    for(int j = 0; j < N_SERVOS - 1; j++) { // n servo - 1
      delay(30);
      rot = 55 * sin(2 * 2 * 3.1415 * i + (2 * j * 2 * 3.1415) / (N_SERVOS - 2));
      rotate(j, 90 + rot);
      s += rot;
    }
    rotate(N_SERVOS - 1, 90 - sum);
    s = 0.0;
  }
}

double sum_angles() {
  double su = 0.0;
  for(int i = 0; i < N_SERVOS - 1; i++) {
    su += angles[i];    
  }  
  return su;
}

void undulated_motion_kinetics_v2() {
  for(int i = 0; i < 360; i++) {
    float brads = i * 3.1415 / 180.0; 
    
    for(int j = 0; j < N_SERVOS - 1; j++) { // n servo - 1
      rot = 40 * sin(2 * brads + (1.5 * j * 2 * 3.1415) / (N_SERVOS - 2));
      rotate(j, 90 + rot);
      s += rot;
      angles[j] = rot;
      delay(10);
      rotate(N_SERVOS - 1, 90 - sum_angles());
    }
    s = 0.0;
    delay(10);
  }
}

// -------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------

// This function activates the undulated motion of the snake
void undulated_motion_3() {
  for(int i = 0; i < 360; i++) {
    float brads = i * 3.1415 / 180.0; 

    for(int j=0; j<N_SERVOS; j++){  
      rotate(j, 94 + 40 * sin(2 * brads + (1.5 * j * 2 * 3.1415) / (N_SERVOS))); 
      /*if(reset_snake == 0 || motion_snake != UNDULATED) {
        while(is_running == 0) {
          if(reset_snake == 1) {
            return;
          } else {
            continue;
          }
        }
        if(dir_snake == FORWARD) {
          rotate(j, 90 + 55 * sin(4 * brads + (1 * j * 2 * 3.1415) / (N_SERVOS - 1))); 
        }
        else {
          rotate(j, 90 + 55 * sin(-4 * brads + (1 * j * 2 * 3.1415) / (N_SERVOS - 1))); 
        }

      } else {
        return;
      }*/
    }

   delay(10);
  }
}
/*
// Updates the mode
void update_mode(int m) {
  if(m == 0 || m == 1) {
    is_running = m;
  }
}

// Updates the amplitude
void update_amplitude(double amp) {
  if(amp >= 30 && amp <= 70) {
    amplitude = amp;
  }
}

// Updates the wavelength
void update_wavelength(double wl) {
  if(wl >= 0.5 && wl <= 4) {
    wavelength = wl;
  }
}

// Updates the frequency
void update_frequency(double f) {
  if(f >= 0.5 && f <= 4) {
    frequency = f;
  }
}

// Updates the offset
void update_offset(double off) {
  if(off >= 0 && off <= 20) {
    offset = off;
  }
}

// Updates the reset
void update_reset(int res) {
  if(res == 0 || res == 1) {
    reset = res;
  }
}

void update_direction(direction d) {
  if(d == FORWARD || d == BACKWARD) {
    dir_snake = d;
  }
}

// This function resets the snake
void reset() {
  reset = 1;
  is_running = 0;
  offset = 0.0;
  motion_snake = NONE;
  frequency = 1.0;
  wavelength = 2.0;
  amplitude = 50.0;
  straight();
}
*/

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
  Serial.println(WiFi.localIP());
  
  server.on("/reset", HTTP_POST, [](AsyncWebServerRequest *request) {
    
  });

  server.on("/mode", HTTP_POST, [](AsyncWebServerRequest *request){
    if(request->hasParam("value", true)) {
      AsyncWebParameter* p = request->getParam("value", true);
      long value = p->value().toInt();

      update_mode(value);

      request->send(200, "text/html", "Received value for /mode on the ESP8266");
      // FAIRE EN SORTE D'AVOIR UN PETIT LOGO VERT QUI DIT QUE TANT QUE JE RECOIS CETTE RESPONSE C'EST GOOD
    } else {
      request->send(404, "text/html", "Error on /mode POST request"); 
      // TRAITER L'ERREUR EST AFFICHER SUR LE TEL EN ROUGE -> PROBLEME DE L'APP AVEC LES REQUETES
    }
  });

  server.on("/motion", HTTP_POST, [](AsyncWebServerRequest *request){
    
  });

  server.on("/paramsAmpl", HTTP_POST, [](AsyncWebServerRequest *request){
    
  });

  server.on("/paramsFreq", HTTP_POST, [](AsyncWebServerRequest *request){
    
  });

  server.on("/paramsWL", HTTP_POST, [](AsyncWebServerRequest *request){
    
  });

  server.on("/offset", HTTP_POST, [](AsyncWebServerRequest *request){
    
  });

  server.on("/direction", HTTP_POST, [](AsyncWebServerRequest *request){
    
  });

  server.on("/paramsSpeed", HTTP_POST, [](AsyncWebServerRequest *request){
    
  });

  server.begin();
  
  */
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
  undulated_motion_3(); 
/*
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Wifi disconnected");
    // SEND PACKET TO KNOW THE WIFI IS OFF ! WITH SOMETHING RED ON THE SCREEN
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
    // SEND PACKET TO KNOW THE WIFI IS ON ! WITH SOMETHING GREEN ON THE SCREEN
  }



  if(is_running == 1) {
    switch(motion_snake){

      case INCHWORM:
        inchworm_motion();
        break;

      case UNDULATED:
        undulated_motion_3();
        break;

      case NONE:
        // SEND PACKET TO APP TO INDICATE TO SELECT A TYPE OF MOTION ? ELSE WE DO NOTHING
        break;  

      default:
        break;    
    } 
  }
*/
}

















