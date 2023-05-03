// This file defines the code to command the snake

// -------------------------------------------------------------------------------------
// INCLUDES
// -------------------------------------------------------------------------------------
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
//#include <WiFiServer.h>

#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

const char* ssid = "giogio_larue";
const char* password = "21ff99a2c0cd";

unsigned long lastTime = 0;
unsigned long timerDelay = 5000;

//WiFiClient client;
//WiFiServer server(80);
AsyncWebServer server(80);

// -------------------------------------------------------------------------------------
// DEFINES
// -------------------------------------------------------------------------------------
#define MIN_PULSE_WIDTH 800 // found on datasheet 
#define MAX_PULSE_WIDTH 2000 // found on datasheet
#define FREQUENCY_SERVO 50

#define N_SERVOS 6
#define HEX_CHANNEL 0x40

// -------------------------------------------------------------------------------------
// TYPEDEF
// -------------------------------------------------------------------------------------
typedef Adafruit_PWMServoDriver Driver;

// -------------------------------------------------------------------------------------
// PARAMETERS
// -------------------------------------------------------------------------------------
Driver driver = Driver(HEX_CHANNEL);


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
      if(j < -1) {
        delay(330);
      } else {
        delay(30);
      }
      rotate(j, 90 + 69 * sin(2 * i + (1 * j * 2 * 3.1415) / (N_SERVOS - 1)));
    }
    delay(100);
  }
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

  //server.begin();

  // Route for root / web page
  server.on("/mode", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", "Hello from the ESP8266");
  });

  server.begin();

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
    //server.begin();
  }

  if ((millis() - lastTime) > timerDelay) {
    if(WiFi.status() == WL_CONNECTED){

      WiFiClient client;
      HTTPClient http;

      String serverPath = "http://192.168.34.121/mode";
      
      // Your Domain name with URL path or IP address with path
      http.begin(client, serverPath.c_str());
        
      // Send HTTP GET request
      int httpResponseCode = http.GET();
      
      if (httpResponseCode>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        String payload = http.getString();
        Serial.println(payload);
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      http.end();
    }  

    lastTime = millis();
  }


/*
  client = server.available();

  if(client) {
    while(client.connected()) {
      if(client.available()) {
        String request = client.readStringUntil('\r');
        Serial.println(request);
        client.flush(); 

        if(request.indexOf("GET /sensor") != -1) {
          Serial.println("Want to get data from the sensor");
          // ...
        }
        else if(request.indexOf("POST /mode") != -1) {
          // Activate/Deactivate the snake
        }
        else if(request.indexOf("POST /motion") != -1) {
          // Type of motion of the snake
        }
        else if(request.indexOf("POST /params") != -1) {
          // Change the parameters of the motion
        }
        else {
          client.println("HTTP/1.1 404 Not Found");
          client.println("Content-Type: text/html");
          client.println("Connection: close");
        }
      }
    }

    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
  */
}