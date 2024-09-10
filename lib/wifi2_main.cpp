#include <WiFi.h>
#include <Arduino.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

// Replace with your network credentials
const char *ssid = "ESP32-Access-Point";
const char *password = "123456789";
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

void setup()
{

    Serial.begin(115200);

    WiFi.softAP(ssid, password);

    Serial.println();
    Serial.print("IP address: ");
    Serial.println(WiFi.softAPIP());

    server.on("/hello", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(200, "text/plain", "Hello World"); });

    server.begin();
}

void loop() {}