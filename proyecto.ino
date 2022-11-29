#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>
#include <Firebase_ESP_Client.h>


const char* ssid = "Totalplay-C5A1";
const char* password = "C5A163D3E3PAe6Rw";

#include "addons/TokenHelper.h"
#define API_KEY "AIzaSyDDdCSJHWu4F-Ae6O6L3g9wrqY7lkXdGZE";

#define USER_EMAIL "arduino@gmail.com";
#define USER_PASSWORD "micontra";

#define DATABASE_URL "https://smart-pod-e1793-default-rtdb.firebaseio.com/";

// Define Firebase objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;



WiFiClientSecure client;
const int Sensor_pin = 32;
float Sensor_value;

String humPath;


float Humedad = 0.0;
float valMin = 0;
float valMax = 0;
float getData = 0;

int humedad_data = 0;

void setup() {
  Serial.begin(115200);
  pinMode(22, OUTPUT);
  pinMode(27, OUTPUT);
  connectToWiFi();

}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {

    Sensor_value = getDataHumidity();
    String setValueUrl = "/macetas/" + WiFi.macAddress() + "/humedad/actual";
    sendFloat(setValueUrl, Sensor_value);


    String getValueLimitUrlMax = "/macetas/" + WiFi.macAddress() + "/humedad/max";
    String getValueLimitUrlMin = "/macetas/" + WiFi.macAddress() + "/humedad/min";

    valMin = getFloat(getValueLimitUrlMin);
    valMax = getFloat(getValueLimitUrlMax);
    if (Sensor_value <= valMin ) {
      //encender
      //foco
      digitalWrite(22, HIGH);
      //relay module
      digitalWrite(27, LOW);

    }
    if (Sensor_value >= valMin) {
      ///apagar
      //foco
      digitalWrite(22, LOW);
      //relay module
      digitalWrite(27, HIGH);
    }

    Serial.print("Min--->");
    Serial.print(valMin);
    Serial.print("Max--->");
    Serial.print(valMax);
    Serial.print("Hume -->");
    Serial.print(Sensor_value);

    delay(2000);
  }
}
float getFloat(String path) {
  if (Firebase.RTDB.getFloat(&fbdo, path)) {
    if (fbdo.dataType() == "float") {
      //Vamos a hacer la validacion para saber los limites
      return fbdo.floatData();
    }
  } else {
    return 0.0;
  }
}

void sendFloat(String path, float value) {
  if (Firebase.RTDB.setFloat(&fbdo, path.c_str(), value)) {
    Serial.println("PASSED");

  }
  else {
    Serial.println("FAILED");
    Serial.println("REASON: " + fbdo.errorReason());
  }
}

void connectFirebase() {
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
  humedad_data = analogRead(Sensor_pin);
  // Humedad = map(analogRead(Sensor_pin), 4095 , 4095 - analogRead(Sensor_pin) , 0, 100);
 Humedad = map(humedad_data, 4095 , 4095 - humedad_data , 0, 100);
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
