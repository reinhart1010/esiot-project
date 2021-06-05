#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <Arduino_JSON.h>
#include <DHTesp.h>
#include "credentials.h"

/** IMPORTANT
 * Please ensure that the configuration at "credentials-example.h" has been
 * copied and filled into "credentials.h".
*/

char *ssid = WIFI_SSID;
char *pass = WIFI_PASSWORD;

String deviceId = DEVICE_ID;

short int DHT22_PIN = 15;

DHTesp dht;

WiFiClient client;

void setup(){
  // Connect
  dht.setup(DHT22_PIN, DHTesp::DHT22);

  Serial.begin(9600);
  delay(10);

  Serial.println("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println(WiFi.localIP());
  HTTPClient http;
  char *dest = "http://192.168.10.2:3000/";
  http.begin(client, dest);
  int httpResponseCode = http.GET();

  if (httpResponseCode>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    String payload = http.getString();
    Serial.println(payload);
  } else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();
}

void loop()
{
  JSONVar data;

  float humidity = dht.getHumidity();
  float temperature = dht.getTemperature();
  data["deviceId"] = deviceId;
  data["humidity"] = humidity;
  data["temperature"] = temperature;
  data["heatIndex"] = dht.computeHeatIndex(temperature, humidity, true);

  String payload = JSON.stringify(data);

  HTTPClient http;
  char *dest = "http://192.168.10.2:3000/updateData";
  http.begin(client, dest);
  http.addHeader("Content-Type", "application/json");
  int httpResponseCode = http.POST(payload);

  Serial.println(payload);

  if (httpResponseCode>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    String payload = http.getString();
    Serial.println(payload);
  } else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();

  delay(10000);
}
