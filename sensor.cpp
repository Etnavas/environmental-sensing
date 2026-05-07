#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>

const char* ssid = "";
const char* password = "";

WebServer server(80);

const uint8_t tempSensorPin = A4;
const uint8_t photoCellPin = A7;
const uint8_t soilSensorPin = A2;

int moisture;
int moisturePercent;
int light;
int lightPercent;
int temperature;

// moisture range
const int dryVal = 3500;
const int wetVal = 1000;

// light range
const int darkVal = 0;
const int brightVal = 4100;

void handleRoot();

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp32")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);

  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();

  static unsigned long lastPrint = 0;
  if (millis() - lastPrint >= 5000) {
    Serial.print("IP: ");
    Serial.print(WiFi.localIP());
    Serial.print("  |  ");
    temp();
    photo();
    soil();
    lastPrint = millis();
  }
}

void temp(){
  int reading = analogRead(tempSensorPin);

  // converts voltage reading into temperature (Celsius)
  float temperatureC = (reading - 500)/10.0;

  // converts to fahrenheit
  temperature = (int)(temperatureC * (9.0/5.0) + 32);
  
  Serial.print("temp: ");
  Serial.print(temperature);
  Serial.print("F  |  ");
}

void photo(){
  light = analogRead(photoCellPin);

  // maps raw value to a percentage. 0% = dark, 100% = bright
  // constrain makes anything outside the range 0 or 100
  lightPercent = constrain(map(light, darkVal, brightVal, 0, 100), 0, 100);

  Serial.print("light raw: ");
  Serial.print(light);
  Serial.print("  |  light %: ");
  Serial.print(lightPercent);
  Serial.print("%  |  ");
}

void soil(){
  moisture = analogRead(soilSensorPin);

  // maps raw value to a percentage. 0% = dry, 100% = wet
  // constrain makes anything outside the range 0 or 100
  moisturePercent = constrain(map(moisture, dryVal, wetVal, 0, 100), 0, 100);

  Serial.print("moisture raw: ");
  Serial.print(moisture);
  Serial.print("  |  moisture %: ");
  Serial.print(moisturePercent);
  Serial.println("%");
}

void handleRoot() {
  char temp[2000];
  int sec = millis() / 1000;
  int min = sec / 60;
  int hr = min / 60;

  snprintf(temp, 2000,

"<html>\
  <head>\
    <meta http-equiv='refresh' content='5'/>\
    <title>ESP32 Demo</title>\
    <style>\
      body {\
        margin: 0;\
        background-color: #1a1a1a;\
        font-family: Helvetica, Sans-Serif;\
        display: flex;\
        justify-content: center;\
        align-items: center;\
        height: 100vh;\
        }\
      .grid {\
        display: flex;\
        gap: 24px;\
        }\
      .card {\
        background: #2a2a2a;\
        border-radius: 16px;\
        padding: 40px;\
        width: 180px;\
        text-align: center;\
        }\
      .label {\
        color: #aaaaaa;\
        font-size: 18px;\
        margin-bottom: 20px;\
        }\
      .val.t {\
        color: #ff6a00;\
        font-size: 64px;\
        font-weight: bold;\
        }\
      .val.m {\
        color: #4a90e2;\
        font-size: 64px;\
        font-weight: bold;\
        }\
      .val.l {\
        color: #f5a623;\
        font-size: 64px;\
        font-weight: bold;\
        }\
    </style>\
  </head>\
  <body>\
    <div class='grid'>\
      <div class='card'>\
        <div class='label'>Temperature</div>\
        <div class='val t'>%d <small style='font-size:24px'>F</small></div>\
      </div>\
      <div class='card'>\
        <div class='label'>Moisture</div>\
        <div class='val m'>%d<small style='font-size:24px'>%%</small></div>\
      </div>\
      <div class='card'>\
        <div class='label'>Light</div>\
        <div class='val l'>%d<small style='font-size:24px'>%%</small></div>\
      </div>\
    </div>\
  </body>\
</html>",
           temperature, moisturePercent, lightPercent);
  server.send(200, "text/html", temp);
}