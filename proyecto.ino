#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>

const char* ssid = "Totalplay-C5A1";
const char* password = "C5A163D3E3PAe6Rw";


WiFiClientSecure client;

void setup() {
  Serial.begin(115200);
  connectToWiFi();

  while (WiFi.status() == WL_CONNECTED) {
    Serial.println("");
    Serial.print("peticion");
    // wait 1 second for re-trying
    char peti = petition("https://pokeapi.co/api/v2/pokemon/ditto", "GET", "");
    delay(2000);
  }
}

void loop() {

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
  // attempt to connect to Wifi network:
  Serial.print("Connected to ");
  Serial.println(ssid);

}

//GET, POST, PATCH, PUT, DELETE
void petition(char * serverName, char * metodo, char * body) {

  HTTPClient http;

  char * payload = "";

  // Your Domain name with URL path or IP address with path
  http.begin(serverName.c_str());

  // If you need Node-RED/server authentication, insert user and password below
  //http.setAuthorization("REPLACE_WITH_SERVER_USERNAME", "REPLACE_WITH_SERVER_PASSWORD");

  // Send HTTP GET request
  int httpResponseCode = 0;
  if (metodo != 'GET' || metodo != 'DELETE' ) {
    http.addHeader("Content-Type", "application/json");
  }

  switch (metodo) {
    case ‘GET’:
      httpResponseCode = http.GET();
                         break;
    case ‘POST’:
      httpResponseCode = http.POST(body);
                         break;
    case ‘PATCH’:
      httpResponseCode = http.PATCH(body);
                         break;
    case ‘PUT’:
      httpResponseCode = http.PUT(body);
                         break;
    case ‘DELETE’:
      httpResponseCode = http.DELETE();
                         break;
  }
  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println("HTTPResponseCode" + httpResponseCode);
    payload = http.getString();

  } else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
    payload = 'ERROR ' + httpResponseCode
  }
  // Free resources
  http.end();
  return payload
}



}
