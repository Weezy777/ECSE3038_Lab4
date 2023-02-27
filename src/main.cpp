#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "env.h"
#include <Arduino.h>
#include <Wifi.h>


const char* webpage = "ecse-three-led-api.onrender.com";


const int led_Pin1 = 35;
const int led_Pin2 = 32;
const int led_Pin3 = 33;

void setup() {
  Serial.begin(9600);
  pinMode(led_Pin1, OUTPUT);
  pinMode(led_Pin2, OUTPUT);
  pinMode(led_Pin3, OUTPUT);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  
  Serial.println("");

  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {

  if(WiFi.status()== WL_CONNECTED){
    HTTPClient http;
    
    String http_response;


    String url = "https://ecse-three-led-api.onrender.com/api/state";
    http.begin(url);
    http.addHeader("X-API-Key", API_KEY);

    int httpResponseCode = http.GET();
    Serial.println("");
    Serial.println("");

    if (httpResponseCode>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);

        Serial.print("Response from server: ");
        http_response = http.getString();
        Serial.println(http_response);
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
  
      http.end();

      StaticJsonDocument<1024> doc;

      DeserializationError error = deserializeJson(doc, http_response);

      if (error) {
        Serial.print("deserializeJson() failed: ");
        Serial.println(error.c_str());
        return;
      }
      
      bool light_switch_1 = doc["light_switch_1"]; 
      bool light_switch_2 = doc["light_switch_2"]; 
      bool light_switch_3 = doc["light_switch_3"]; 
  
      Serial.println("");
      Serial.println(light_switch_1);
      Serial.println(light_switch_2);
      Serial.println(light_switch_3);
      digitalWrite(led_Pin1, light_switch_1);
      digitalWrite(led_Pin2,light_switch_2);
      digitalWrite(led_Pin3, light_switch_3);
      Serial.println("Updated LED states");
      
  }
  else {
    return;
  }
}