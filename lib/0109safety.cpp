#include <WiFi.h>
#include <Arduino.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"
#include <Wire.h>
// #include <Adafruit_MPU6050.h>
// #include <Adafruit_Sensor.h>

/*
  The resolution of the PWM is 8 bit so the value is between 0-255
  We will set the speed between 100 to 255.
*/
const int MPU_ADDR = 0x68;
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;

double angleAcX, angleAcY, angleAcZ;
double angleGyX, angleGyY, angleGyZ;
double angleFiX, angleFiY, angleFiZ;
double temp;

const int output = 2;
int ledState = LOW;
const char *PARAM_INPUT_1 = "state";

const double RADIAN_TO_DEGREE = 180 / 3.14159;
const double DEGREE_PER_SECOND = 32767 / 250;
const double ALPHA = 1 / (1 + 0.04);

unsigned long now = 0;
unsigned long past = 0;
double dt = 0;

double baseAcX, baseAcY, baseAcZ;
double baseGyX, baseGyY, baseGyZ;
double baseTemp;

void initSensor()
{
    Wire.begin();
    Wire.beginTransmission(MPU_ADDR);
    Wire.write(0x6B);
    Wire.write(0);
    Wire.endTransmission(true);
}

void getData()
{
    Wire.beginTransmission(MPU_ADDR);
    Wire.write(0x3B);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU_ADDR, 14, true);

    AcX = Wire.read() << 8 | Wire.read();
    AcY = Wire.read() << 8 | Wire.read();
    AcZ = Wire.read() << 8 | Wire.read();
    Tmp = Wire.read() << 8 | Wire.read();
    GyX = Wire.read() << 8 | Wire.read();
    GyY = Wire.read() << 8 | Wire.read();
    GyZ = Wire.read() << 8 | Wire.read();
}

void getDT()
{
    now = millis();
    dt = (now - past) / 1000.0;
    past = now;
    // Serial.print(dt);
}

void calibrateSensor()
{
    double sumAcX = 0, sumAcY = 0, sumAcZ = 0;
    double sumGyX = 0, sumGyY = 0, sumGyZ = 0;

    getData();
    for (int i = 0; i < 10; i++)
    {
        getData();
        sumAcX += AcX;
        sumAcY += AcY;
        sumAcZ += AcZ;
        sumGyX += GyX;
        sumGyY += GyY;
        sumGyZ += GyZ;
        delay(100);
    }
    baseAcX = sumAcX / 10;
    baseAcY = sumAcY / 10;
    baseAcZ = sumAcZ / 10;
    baseGyX = sumGyX / 10;
    baseGyY = sumGyY / 10;
    baseGyZ = sumGyZ / 10;
}



// Change this to your network SSID
const char *ssid = "OMG_SKKU";
const char *password = "abcd1234";

// AsyncWebserver runs on port 80 and the asyncwebsocket is initialize at this point also
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

// Our car object
Car car;

// Function to send commands to car
void sendCarCommand(const char *command)
{
    // command could be either "left", "right", "forward" or "reverse" or "stop"
    // or speed settingg "slow-speed", "normal-speed", or "fast-speed"
    // or robot pose setting "contraction" or "relaxation"
    if (strcmp(command, "left") == 0)
    {
        car.turnLeft();
    }
    else if (strcmp(command, "right") == 0)
    {
        car.turnRight();
    }
    else if (strcmp(command, "up") == 0)
    {
        car.moveForward();
    }
    else if (strcmp(command, "down") == 0)
    {
        car.moveBackward();
    }
    else if (strcmp(command, "stop") == 0)
    {
        car.stop();
    }
    else if (strcmp(command, "slow-speed") == 0)
    {
        car.setCurrentSpeed(speedSettings::SLOW);
    }
    else if (strcmp(command, "normal-speed") == 0)
    {
        car.setCurrentSpeed(speedSettings::NORMAL);
    }
    else if (strcmp(command, "fast-speed") == 0)
    {
        car.setCurrentSpeed(speedSettings::FAST);
    }
    else if (strcmp(command, "contraction") == 0)
    {
        car.contraction();
    }
    else if (strcmp(command, "relaxation") == 0)
    {
        car.relaxation();
    }
}

// Processor for index.html page template.  This sets the radio button to checked or unchecked
String indexPageProcessor(const String &var)
{
    String status = "";
    if (var == "SPEED_SLOW_STATUS")
    {
        if (car.getCurrentSpeed() == speedSettings::SLOW)
        {
            status = "checked";
        }
    }
    else if (var == "SPEED_NORMAL_STATUS")
    {
        if (car.getCurrentSpeed() == speedSettings::NORMAL)
        {
            status = "checked";
        }
    }
    else if (var == "SPEED_FAST_STATUS")
    {
        if (car.getCurrentSpeed() == speedSettings::FAST)
        {
            status = "checked";
        }
    }
    else if (var == "POSE_STATUS")
    {

        status = "checked";
    }
    // Serial.println(var);
    else if (var == "BUTTONPLACEHOLDER")
    {
        String buttons = "";
        String outputStateValue = outputState();
        buttons += "<h4>Real time Robot Status <span id=\"outputState\"></span></h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"output\" " + outputStateValue + "><span class=\"slider\"></span></label>";
        return buttons;
    }
    return status;
}

// cont relax sensing

// Callback function that receives messages from websocket client
void onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
               void *arg, uint8_t *data, size_t len)
{
    switch (type)
    {
    case WS_EVT_CONNECT:
    {
        Serial.printf("ws[%s][%u] connect\n", server->url(), client->id());
        // client->printf("Hello Client %u :)", client->id());
        // client->ping();
    }

    case WS_EVT_DISCONNECT:
    {
        Serial.printf("ws[%s][%u] disconnect\n", server->url(), client->id());
    }

    case WS_EVT_DATA:
    {
        // data packet
        AwsFrameInfo *info = (AwsFrameInfo *)arg;
        if (info->final && info->index == 0 && info->len == len)
        {
            // the whole message is in a single frame and we got all of it's data
            if (info->opcode == WS_TEXT)
            {
                data[len] = 0;
                char *command = (char *)data;
                sendCarCommand(command);
            }
        }
    }

    case WS_EVT_PONG:
    {
        // Serial.printf("ws[%s][%u] pong[%u]: %s\n", server->url(), client->id(), len, (len) ? (char *)data : "");
    }

    case WS_EVT_ERROR:
    {
        // Serial.printf("ws[%s][%u] error(%u): %s\n", server->url(), client->id(), *((uint16_t *)arg), (char *)data);
    }
    }
}

// Function called when resource is not found on the server
void notFound(AsyncWebServerRequest *request)
{
    request->send(404, "text/plain", "Not found");
}

double readTemperature()
{
    getData();
    temp = double(Tmp) / 340.0 + 36.53;
    return temp;
}
double readAngle()
{
    getData();
    getDT();
    angleAcY = atan(-AcX / sqrt(pow(AcY, 2) + pow(AcZ, 2)));
    angleAcY *= RADIAN_TO_DEGREE;

    angleGyY = ((GyY - baseGyY) / DEGREE_PER_SECOND) * dt;

    double angleTmp = angleFiY + angleGyY;
    angleFiY = ALPHA * angleTmp + (1.0 - ALPHA) * angleAcY;
    // Serial.println(angleFiY);
    return angleFiY;
}

// Setup function
void setup()
{
    initSensor();
    Serial.begin(115200);
    calibrateSensor();
    past = millis();

    // Initialize the serial monitor baud rate
    Serial.println("Connecting to ");
    Serial.println(ssid);

    // Connect to your wifi
    WiFi.softAP(ssid, password);
    IPAddress IP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(IP);

    // original - stamode not AP mode change to AP mode we use softAP
    // WiFi.mode(WIFI_STA);
    // WiFi.begin(ssid, password);
    // if (WiFi.waitForConnectResult() != WL_CONNECTED)
    // {
    //     Serial.printf("WiFi Failed!\n");
    //     return;
    // }

    // Serial.println(WiFi.localIP());

    // Initialize SPIFFS
    if (!SPIFFS.begin(true))
    {
        Serial.println("An Error has occurred while mounting SPIFFS");
        return;
    }

    // Add callback function to websocket server
    ws.onEvent(onWsEvent);
    server.addHandler(&ws);

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              {
              Serial.println("Requesting index page...");
              request->send(SPIFFS, "/index.html", "text/html", false, indexPageProcessor); });

    // Route to load entireframework.min.css file
    server.on("/css/entireframework.min.css", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(SPIFFS, "/css/entireframework.min.css", "text/css"); });

    // Route to load custom.css file
    server.on("/css/custom.css", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(SPIFFS, "/css/custom.css", "text/css"); });

    // Route to load custom.js file
    server.on("/js/custom.js", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(SPIFFS, "/js/custom.js", "text/javascript"); });

    server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send_P(200, "text/plain", String(readTemperature()).c_str()); });
    server.on("/angle", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send_P(200, "text/plain", String(readAngle()).c_str()); });
    // Send a GET request to <ESP_IP>/update?state=<inputMessage>
    server.on("/update", HTTP_GET, [](AsyncWebServerRequest *request)
              {
    String inputMessage;
    String inputParam;
    // GET input1 value on <ESP_IP>/update?state=<inputMessage>
    if (request->hasParam(PARAM_INPUT_1)) {
      inputMessage = request->getParam(PARAM_INPUT_1)->value();
      inputParam = PARAM_INPUT_1;
      digitalWrite(output, inputMessage.toInt());
      ledState = !ledState;
    }
    else {
      inputMessage = "No message sent";
      inputParam = "none";
    }
    Serial.println(inputMessage);
    request->send(200, "text/plain", "OK"); });

    // Send a GET request to <ESP_IP>/state
    server.on("/state", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(200, "text/plain", String(digitalRead(output)).c_str()); });
    // On Not Found
    server.onNotFound(notFound);

    // Start server
    server.begin();
}

void loop()
{
    double temp_angle = readAngle();
    // Serial.println(temp_angle);
    readTemperature();

    if (user_pose == 1 && (outputState() != "checked") && (temp_angle > 50 || temp_angle < -50))
    {
        car.contraction();
    }

    delay(50);
}