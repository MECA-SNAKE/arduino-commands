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
#define MIN_PULSE_WIDTH 850
#define MAX_PULSE_WIDTH 2000
#define FREQUENCY_SERVO 50

#define N_SERVOS 12
#define HEX_CHANNEL 0x40

#define DEFAULT_AMP 50.0
#define DEFAULT_FREQ 2.0
#define DEFAULT_OFF 0.0
#define DEFAULT_WL 2.0
#define DEFAULT_SPEED_INCH 10

#define TRIGGER_PIN = 12
#define ECHO_PIN = 14
#define SOUND_VELOCITY 0.034
#define CM_TO_INCH 0.393701

// -------------------------------------------------------------------------------------
// TYPEDEF
// -------------------------------------------------------------------------------------
typedef Adafruit_PWMServoDriver Driver;

typedef enum {
  UNDULATED, CONCERTINA, INCHWORM, NONE
} motion;

typedef enum {
  FORWARD, BACKWARD, NO_MOVE
} direction;

// -------------------------------------------------------------------------------------
// PARAMETERS
// -------------------------------------------------------------------------------------
Driver driver = Driver(HEX_CHANNEL);

const char* ssid = "giogio_larue";
const char* password = "21ff99a2c0cd";

AsyncWebServer server(80);

int is_running = 0; // 0 -> not running, 1 -> running
motion motion_snake = NONE;
direction dir_snake = NO_MOVE;

double speed_inchworm = 0.0;
double amplitude = 0.0;
double offset = 0.0;
double wavelength = 0.0;
double frequency = 0.0; 

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
/*
// This function return the distance of the first object in the direction of the sensor
float trigger_sensor() {

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Calculate the distance
  return pulseIn(echoPin, HIGH) * SOUND_VELOCITY/2;
}
*/
// -------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------

// This function activates the inchworm's motion
void inchworm_motion() {

  // ROBIN update here for backward and the speed (inchworm_speed variable) the inchworm
  // handle here the fact that when we change the motion or the direction, we need to return (tomorrow)

  for(int phi = 0; phi <90; ++phi){
    if(motion_snake == INCHWORM) {
      while(is_running == 0) {
        yield();
      }
      rotate(0, 90-phi);
      rotate(1, 90-phi);
      rotate(2, 90+phi);

      delay(10);
    } else {
      delay(2000);
      straight();
      delay(2000);
      return;
    } 
  }

  for(int i = 0; i <= 4; ++i){
    for(int phi = 0; phi < 90; ++phi){
      
      if(motion_snake == INCHWORM) {
        
        while(is_running == 0) {
          yield();  
        }

        if(i>0){
          rotate(i-1, 180-phi);
        }

        rotate(i, 2*phi);
        rotate(i+2, 180-2*phi);
        rotate(i+3, 90+phi);
        delay(10);

      } else {
        delay(2000);
        straight();
        delay(2000);
        return;
      }
    }  
    delay(1000);
  }
 
  for(int i = 0; i < N_SERVOS; i++) {
    if(motion_snake == INCHWORM) {
      while(is_running == 0) {
        yield(); 
      }
      rotate(i, 90);
      delay(10);
    } else {
      delay(2000);
      straight();
      delay(2000);
      return;
    }
  }
}

// -------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------

// NEED TO BE TESTED
float g = 1.0;
float temp = 0.0;
float sum = 0.0;
float factor = 0.0;
float amp = 2.0;
float ampf = 80.0;
float st = 150.0;

float test(float a, float a_f, float s) {
    return pow(a_f / a, 1 / s);
}    

// WANT TO KNOW AS BELOW WHY NOT WORK
// NOT TEST WITH REACT APP BECAUSE NO CONTROL OF RUNNING AND SO ON
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
    rotate(N_SERVOS - 2, 90 + sum);
    g *= factor;
    sum = 0.0;
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
    rotate(N_SERVOS - 2, 90 + sum);
    g *= factor;
    sum = 0.0;
  } 
}

// TO TEST BUT NOT WITH REACT APP BECAUSE NO TEST ON RUNNING AND SO ON
void concertina_motion_v2() {

  // first step
  for(int i = 0; i < 100; i++) {
    for(int phi = 0; phi <= 90; phi++) {
      rotate(0, 90 - phi);
      rotate(2, 90 + phi);
      rotate(3, 90 + phi);
      rotate(6, 90 - phi);
      rotate(7, 90 - phi);
      rotate(9, 90 + phi);
      delay(10);
    }
    delay(10);
  }

  //second step
  for(int i = 0; i < 100; i++) {
    for(int phi = 0; phi <= 90; phi++) {
      rotate(0, phi);
      rotate(2, 180 - phi);
      rotate(3, 180 - phi);
      rotate(5, 90 - phi);
      rotate(6, 2*phi);
      rotate(7, 2*phi);
      rotate(9, 2*(90 - phi));
      rotate(10, 90 - phi);
      rotate(11, 90 + phi);
      delay(10);
    }
    delay(10);
  }  

  // third step
  for(int i = 0; i < 100; i++) {
    for(int phi = 0; phi <= 90; phi++) {
      rotate(5, phi);
      rotate(6, 180 - phi);
      rotate(7, 180 - phi);
      rotate(9, phi);
      rotate(10, phi);
      rotate(11, 180 - phi);
      delay(10);
    }
    delay(10);
  }
}

// -------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------

// This function sets the snake in a straight line
void straight() {
  for(int i = 0; i < N_SERVOS; i++) {
    delay(500);
    rotate(i, 90);
  }
}

// -------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------
double rot = 0.0;

// WANT TO KNOW WHY NOT THE HEAD MOVING FUCK THIS SHIT IT'S NORMAL PIECE OF CODE FUUUUCK
void undulated_motion_kinetics_v2() {
  for(int i = 0; i < 360; i++) {
    float brads = i * 3.1415 / 180.0; 
    
    for(int j = 0; j < N_SERVOS - 1; j++) { // n servo - 1
      rot = 55 * sin(2 * brads + (2 * j * 2 * 3.1415) / (N_SERVOS - 1));
      rotate(j, 90 + rot);
      sum += rot;
    }
    delay(10);
    rotate(N_SERVOS - 1, 90 + sum);
    sum = 0.0;
  }
}

// -------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------

// This function activates the undulated motion of the snake
void undulated_motion_3() {
  for(int i = 0; i < 360; i++) {
    float brads = i * 3.1415 / 180.0; 

    for(int j=0; j<N_SERVOS; j++){  
      /*
      if(j >= 9) {
         rotate(j, 93 + 55 * sin(4 * brads + (1 * j * 2 * 3.1415) / (N_SERVOS - 1))); 
      } else {
        rotate(j, 90 + 55 * sin(4 * brads + (1 * j * 2 * 3.1415) / (N_SERVOS - 1)));
      }   
      */
      
      if(motion_snake == UNDULATED) {
        while(is_running == 0) {
          if(motion_snake != UNDULATED) return;
        }
       if(j >= 9) {
          if(dir_snake == FORWARD) {
            rotate(j, 90 + 25 * sin(5 * brads + (1 * j * 2 * 3.1415) / (N_SERVOS - 1))); 
          } else {
            rotate(j, 90 + 25 * sin(-5 * brads + (1 * j * 2 * 3.1415) / (N_SERVOS - 1))); 
          }  

        } else {
          if(dir_snake == FORWARD) {
            rotate(j, 90 + 25 * sin(5 * brads + (1 * j * 2 * 3.1415) / (N_SERVOS - 1))); 
          } else {
            rotate(j, 90 + 25 * sin(-5 * brads + (1 * j * 2 * 3.1415) / (N_SERVOS - 1))); 
          }
        }  
      } else {
        Serial.println("reset activated");
        reset();
        delay(10);
        return;
      }
    }
   delay(10);
  }
}

// TO TEST -------> make circles !!
void undulated_cos() {
  for(int i = 0; i < 360; i++) {
    float brads = i * 3.1415 / 180.0; 
    for(int j=0; j<N_SERVOS; j++){  

      if(motion_snake == UNDULATED) {
        while(is_running == 0) {
          yield();
        }
        
        if(dir_snake == FORWARD) {
          rotate(j, 90 + 55 * cos(4 * brads + (((j - 5) * 2 * 3.1415 / (N_SERVOS - 1))))); 
        } else {
          rotate(j, 90 + 55 * cos(-4 * brads + (((j - 5) * 2 * 3.1415 / (N_SERVOS - 1))))); 
        }  

      } else {
        Serial.println("reset activated");
        reset();
        delay(10);
        return;
      }
    }
   delay(7);
  }
}

// Updates the speed of the inchworm
void update_speed_inchworm(double s) {
  speed_inchworm = s;
}

// Updates the mode (0 -> stop, 1 -> start)
void update_mode(int m) {
  if(m == 0 || m == 1) {
    is_running = m;
  }
}

// Updates the amplitude
void update_amplitude(double amp) {
  if(amp >= 0 && amp <= 80) {
    amplitude = amp;
  }
}

// Updates the wavelength
void update_wavelength(double wl) {
  if(wl >= 0 && wl <= 4) {
    wavelength = wl;
  }
}

// Updates the frequency
void update_frequency(double f) {
  if(f >= 0 && f <= 4) {
    frequency = f;
  }
}

// Updates the offset
void update_offset(double off) {
  if(off >= 0 && off <= 20) {
    offset = off;
  }
}

void update_direction(direction d) {
  if(d == FORWARD || d == BACKWARD) {
    dir_snake = d;
  }
}

void update_motion(motion mo) {
  motion_snake = mo;
}

// This function resets the snake
void reset() {
  dir_snake = NO_MOVE;
  frequency = DEFAULT_FREQ;
  wavelength = DEFAULT_WL;
  offset = DEFAULT_OFF;  
  amplitude = DEFAULT_AMP;
  speed_inchworm = DEFAULT_SPEED_INCH;
}


// -------------------------------------------------------------------------------------
// SETUP FUNCTION
// -------------------------------------------------------------------------------------
void setup() {  

  Serial.begin(9600); 
  Serial.println("Initialize System");

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
/*
  server.on("/sensor", HTTP_GET, [](AsyncWebServerRequest *request) {
    if(distance_sensor() >= 4.0) {
      request->send(200, "text/html", "1");

      // now if it recieves a "1" then it stops sending GET request until we do what?
    }
  });
  */
  server.on("/reset", HTTP_POST, [](AsyncWebServerRequest *request) {
    if(request->hasParam("value", true)) {
      AsyncWebParameter* p = request->getParam("value", true);
      int value = p->value().toInt();
      motion_snake = NONE;

      request->send(200, "text/html", "Received value for /reset on the ESP8266");
      // FAIRE EN SORTE D'AVOIR UN PETIT LOGO VERT QUI DIT QUE TANT QUE JE RECOIS CETTE RESPONSE C'EST GOOD
    } else {
      request->send(404, "text/html", "Error on /mode POST request"); 
      // TRAITER L'ERREUR EST AFFICHER SUR LE TEL EN ROUGE -> PROBLEME DE L'APP AVEC LES REQUETES
    }
  });

  server.on("/mode", HTTP_POST, [](AsyncWebServerRequest *request){
    if(request->hasParam("value", true)) {
      AsyncWebParameter* p = request->getParam("value", true);
      int value = p->value().toInt();

      update_mode(value);

      request->send(200, "text/html", "Received value for /mode on the ESP8266");
      // FAIRE EN SORTE D'AVOIR UN PETIT LOGO VERT QUI DIT QUE TANT QUE JE RECOIS CETTE RESPONSE C'EST GOOD
    } else {
      request->send(404, "text/html", "Error on /mode POST request"); 
      // TRAITER L'ERREUR EST AFFICHER SUR LE TEL EN ROUGE -> PROBLEME DE L'APP AVEC LES REQUETES
    }
  });

  server.on("/motion", HTTP_POST, [](AsyncWebServerRequest *request){
    if(request->hasParam("value", true)) {
      AsyncWebParameter* p = request->getParam("value", true);
      int value = p->value().toInt();

      if(value == 1) {
        update_motion(UNDULATED);
      } else if(value == 0) {
        update_motion(CONCERTINA);
      } else if(value == 2) {
        update_motion(INCHWORM);
      }

      request->send(200, "text/html", "Received value for /motion on the ESP8266");
      // FAIRE EN SORTE D'AVOIR UN PETIT LOGO VERT QUI DIT QUE TANT QUE JE RECOIS CETTE RESPONSE C'EST GOOD
    } else {
      request->send(404, "text/html", "Error on /motion POST request"); 
      // TRAITER L'ERREUR EST AFFICHER SUR LE TEL EN ROUGE -> PROBLEME DE L'APP AVEC LES REQUETES
    }
  });

  server.on("/params", HTTP_POST, [](AsyncWebServerRequest *request){
    if(request->hasParam("value", true)) {
      AsyncWebParameter* p = request->getParam("value", true);
      double value = p->value().toInt();

      update_amplitude(value);

      request->send(200, "text/html", "Received value for /motion on the ESP8266");
      // FAIRE EN SORTE D'AVOIR UN PETIT LOGO VERT QUI DIT QUE TANT QUE JE RECOIS CETTE RESPONSE C'EST GOOD
    } else {
      request->send(404, "text/html", "Error on /motion POST request"); 
      // TRAITER L'ERREUR EST AFFICHER SUR LE TEL EN ROUGE -> PROBLEME DE L'APP AVEC LES REQUETES
    }
  });

  server.on("/direction", HTTP_POST, [](AsyncWebServerRequest *request){
    if(request->hasParam("value", true)) {
      AsyncWebParameter* p = request->getParam("value", true);
      int value = p->value().toInt();

      if(value == 1) {
        update_direction(FORWARD);
      } else if(value == 0) {
        update_direction(BACKWARD);
      } 

      request->send(200, "text/html", "Received value for /mode on the ESP8266");
      // FAIRE EN SORTE D'AVOIR UN PETIT LOGO VERT QUI DIT QUE TANT QUE JE RECOIS CETTE RESPONSE C'EST GOOD
    } else {
      request->send(404, "text/html", "Error on /mode POST request"); 
      // TRAITER L'ERREUR EST AFFICHER SUR LE TEL EN ROUGE -> PROBLEME DE L'APP AVEC LES REQUETES
    }
  });

  server.begin();

  driver.begin();
  driver.wakeup();
  driver.setPWMFreq(FREQUENCY_SERVO); // which frequency of our servos

  // Initialize all servo at their midpoint
  straight();

  delay(2000);
  //pinMode(TRIGGER_PIN, OUTPUT); // Sets the trigPin as an Output
  //pinMode(ECHO_PIN, INPUT); // Sets the echoPin as an Input
}

// -------------------------------------------------------------------------------------
// LOOP FUNCTION
// -------------------------------------------------------------------------------------
void loop() {

  // WHAT DO WE DO IF WIFI GOES OFF -> STOP THE SNAKE BECAUSE MORE SECURE ? OTHER THING ?  

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
        is_running = 0;
        straight();
        break;  

      default:
        break;    
    } 
  }
}

















