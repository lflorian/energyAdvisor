#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

int redPin = 25;
int greenPin = 26;
int bluePin = 27;

const char* ssid = "";
const char* password = "";

void setup() {

  WiFi.begin(ssid, password);
  

  delay(500);
  pinMode(redPin,OUTPUT);
  pinMode(greenPin,OUTPUT);
  pinMode(bluePin,OUTPUT);
    analogWrite(redPin, 0);
    analogWrite(greenPin, 0);
    analogWrite(bluePin, 255);
    delay(500);

  while (WiFi.status() != WL_CONNECTED) {
    analogWrite(bluePin, 255);
    delay(500);
    analogWrite(bluePin, 25);
    delay(500);
  }
  delay(100);
}

void loop() {
  if ((WiFi.status() == WL_CONNECTED)) {
    HTTPClient client;

    client.begin("https://api-access.electricitymaps.com/free-tier/power-breakdown/latest?zone=DE");
    client.addHeader("auth-token", "");
    int httpCode = client.GET();

    if (httpCode > 0) {
      String input = client.getString();

      //JSON Deserialization
      StaticJsonDocument<16> filter;
      filter["renewablePercentage"] = true;

      StaticJsonDocument<96> doc;

      DeserializationError error = deserializeJson(doc, input, DeserializationOption::Filter(filter));

      if (error) {
        analogWrite(bluePin, 255);
        return;
      }

      int renewablePercentage = doc["renewablePercentage"];
      
      
      client.end();

        //AdvisorLED-Agent
  if (renewablePercentage < 50) {
  analogWrite(redPin, 255);
  analogWrite(greenPin, 0);
  analogWrite(bluePin, 0);
}
  else if (renewablePercentage >= 50 && renewablePercentage <= 75) {
    analogWrite(redPin, 255);
    analogWrite(greenPin, 55);
    analogWrite(bluePin,LOW);
  }
    else if (renewablePercentage > 75) {
      analogWrite(redPin, 0);
      analogWrite(greenPin, 255);
      analogWrite(bluePin, 0);
    }
      else {
        analogWrite(redPin, 0);
        analogWrite(greenPin, 0);
        analogWrite(bluePin, 255);
      }
        }

          else {
             
            }
  }

  else {
    
  }
  delay(12000);
}
