#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>
#include <Firebase_ESP_Client.h>

const char* ssid = "Totalplay-C5A1";
const char* password = "C5A163D3E3PAe6Rw";

#include "addons/TokenHelper.h"
#define API_KEY "AIzaSyCkEVunp97TM5RYZXri-9EwuWb6L722XZk";

#define USER_EMAIL "micontra@gmail.com";
#define USER_PASSWORD "micontra";

#define DATABASE_URL "https://login-glam.firebaseio.com/";

// Define Firebase objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;



WiFiClientSecure client;
const int Sensor_pin = 32;
float Sensor_value;

String humPath;


float Humedad=0;
void setup() {
  Serial.begin(115200);

  connectToWiFi();
   


}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
   
    Sensor_value = getDataHumidity();
    Serial.print("Soil Moisture is  = ");
    Serial.print(Sensor_value);
    sendFloat("/humedad", Sensor_value);
    Serial.println("%");
    delay(3000);
  }
}
void sendFloat(String path, float value){
  if (Firebase.RTDB.setFloat(&fbdo, path.c_str(), value)){
    Serial.print("Writing value: ");
    Serial.print (value);
    Serial.print(" on the following path: ");
    Serial.println(path);
    Serial.println("PASSED");
    Serial.println("PATH: " + fbdo.dataPath());
    Serial.println("TYPE: " + fbdo.dataType());
  }
  else {
    Serial.println("FAILED");
    Serial.println("REASON: " + fbdo.errorReason());
  }
}

void connectFirebase(){
   // Assign the api key (required)
  config.api_key = API_KEY;

  // Assign the user sign in credentials
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  // Assign the RTDB URL (required)
  config.database_url = DATABASE_URL;

  Firebase.reconnectWiFi(true);
  fbdo.setResponseSize(4096);

  // Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h

  // Assign the maximum retry of token generation
  config.max_token_generation_retry = 5;

  // Initialize the library with the Firebase authen and config
  Firebase.begin(&config, &auth);
}

float getDataHumidity() {
  Serial.println("Row data ---> " + String(analogRead(Sensor_pin)));

  Humedad = 100.0 - (analogRead(Sensor_pin)/4096.0) * 100.0;
  
  return Humedad;

}
void connectToWiFi() {
  //Initialize serial and wait for port to open:
  delay(100);

  Serial.print("Attempting to connect to SSID: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  // do nothing
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    // wait 1 second for re-trying
    delay(1000);
  }
  connectFirebase();
  // attempt to connect to Wifi network:
  Serial.print("Connected to ");
  Serial.println(ssid);

}
