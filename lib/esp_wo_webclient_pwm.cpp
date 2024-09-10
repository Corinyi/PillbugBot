// // #include<WiFi.h>
// #include <Arduino.h>
// #include <Wire.h>
// #include <stdio.h>
// // #include <Adafruit_MPU6050.h>
// // #include <Adafruit_Sensor.h>

// /*d
//   The resolution of the PWM is 8 bit so the value is between 0-255
//   We will set the speed between 100 to 255.
// */

// int output = 2;
// int ledState = LOW;
// const char *PARAM_INPUT_1 = "state";

// enum speedSettings
// {
//     SLOW = 100,
//     NORMAL = 180,
//     FAST = 255
// };
// String outputState()
// {
//     if (digitalRead(output))
//     { // real contraction relaxation check
//         return "checked";
//     }
//     else
//     {
//         return "";
//     }
//     return "";
// }
// int user_pose = 0;
// // String processor(const String &var)
// // {
// //     // Serial.println(var);
// //     if (var == "BUTTONPLACEHOLDER")
// //     {
// //         String buttons = "";
// //         String outputStateValue = outputState();
// //         buttons += "<h4>Output - GPIO 2 - State <span id=\"outputState\"></span></h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"output\" " + outputStateValue + "><span class=\"slider\"></span></label>";
// //         return buttons;
// //     }
// //     return String();
// // }

// class Car
// {
// private:
//     // left motor connections
//     const uint8_t in1 = 16;
//     //  right motor connections
//     const uint8_t in2 = 18;
//     // contraction motor connections
//     const uint8_t in3 = 25;
//     // relaxation connections
//     const uint8_t in4 = 32;

//     // PWM Setup to control motor speed
//     const uint8_t SPEED_CONTROL_PIN_1 = 17;
//     const uint8_t SPEED_CONTROL_PIN_2 = 19;
//     const uint8_t SPEED_CONTROL_PIN_3 = 26;
//     const uint8_t SPEED_CONTROL_PIN_4 = 33;

//     const int photo = 5;
//     // Play around with the frequency settings depending on the motor that you are using
//     const int freq = 5000;
//     const int channel_0 = 1;
//     const int channel_1 = 2;
//     const int channel_2 = 3;
//     const int channel_3 = 4;

//     // 8 Bit resolution for duty cycle so value is between 0 - 255
//     const int resolution = 8;

//     // holds the current speed settings, see values for SLOW, NORMAL, FAST
//     speedSettings currentSpeedSettings;

// public:
//     Car()
//     {
//         // Set all pins to output
//         pinMode(in1, OUTPUT);
//         pinMode(in2, OUTPUT);
//         pinMode(in3, OUTPUT);
//         pinMode(in4, OUTPUT);
//         pinMode(SPEED_CONTROL_PIN_1, OUTPUT);
//         pinMode(SPEED_CONTROL_PIN_2, OUTPUT);
//         pinMode(SPEED_CONTROL_PIN_3, OUTPUT);
//         pinMode(SPEED_CONTROL_PIN_4, OUTPUT);

//         pinMode(photo, INPUT_PULLUP);

//         // have to define pose motor

//         // Set initial motor state to OFF
//         digitalWrite(in1, LOW);
//         digitalWrite(in2, LOW);
//         digitalWrite(in3, LOW);
//         digitalWrite(in4, LOW);

//         // // Set the PWM Settings
//         ledcSetup(channel_0, freq, resolution);
//         ledcSetup(channel_1, freq, resolution);
//         ledcSetup(channel_2, freq, resolution);
//         ledcSetup(channel_3, freq, resolution);

//         // Attach Pin to Channel
//         ledcAttachPin(SPEED_CONTROL_PIN_1, channel_0);
//         ledcAttachPin(SPEED_CONTROL_PIN_2, channel_1);
//         ledcAttachPin(SPEED_CONTROL_PIN_3, channel_2);
//         ledcAttachPin(SPEED_CONTROL_PIN_4, channel_3);

//         // initialize default speed to SLOW
//         setCurrentSpeed(speedSettings::NORMAL);
//     }

//     // Turn the car left
//     void turnLeft()
//     {
//         Serial.println("car is turning left...");
//         digitalWrite(in1, LOW);
//         digitalWrite(in2, LOW);
//         ledcWrite(channel_0, currentSpeedSettings - 70);
//         ledcWrite(channel_1, currentSpeedSettings);
//     }

//     // Turn the car right
//     void turnRight()
//     {
//         Serial.println("car is turning right...");
//         digitalWrite(in1, LOW);
//         digitalWrite(in2, LOW);
//         ledcWrite(channel_0, currentSpeedSettings);
//         ledcWrite(channel_1, currentSpeedSettings - 70);
//     }

//     // Move the car forward
//     void moveForward()
//     {
//         Serial.println("car is moving forward...");
//         setMotorSpeed();
//         digitalWrite(in1, LOW);
//         digitalWrite(in2, LOW);
//         // ledcWrite(channel_0, currentSpeedSettings);
//         // ledcWrite(channel_1, currentSpeedSettings);
//     }

//     // Move the car backward
//     void moveBackward()
//     {
//         Serial.println("car is moving backward...");
//         ledcWrite(channel_0, LOW);
//         ledcWrite(channel_1, LOW);
//         digitalWrite(in1, HIGH);
//         digitalWrite(in2, HIGH);
//     }

//     // Stop the car
//     void stop()
//     {
//         Serial.println("car is stopping...");
//         ledcWrite(channel_0, 0);
//         ledcWrite(channel_1, 0);
//         // // Turn off motors
//         digitalWrite(in1, LOW);
//         digitalWrite(in2, LOW);
//         ledcWrite(channel_2, 0);
//         ledcWrite(channel_3, 0);
//         // // Turn off motors
//         digitalWrite(in3, LOW);
//         digitalWrite(in4, LOW);
//     }

//     // Set the motor speed
//     void setMotorSpeed()
//     {
//         // change the duty cycle of the speed control pin connected to the motor
//         Serial.print("Speed Settings: ");
//         Serial.println(currentSpeedSettings);
//         ledcWrite(channel_0, currentSpeedSettings);
//         ledcWrite(channel_1, currentSpeedSettings);
//     }
//     // Set the current speed
//     void setCurrentSpeed(speedSettings newSpeedSettings)
//     {
//         Serial.println("car is changing speed...");
//         currentSpeedSettings = newSpeedSettings;
//     }
//     // Get the current speed
//     speedSettings getCurrentSpeed()
//     {
//         return currentSpeedSettings;
//     }
//     // set robot contraction
//     void contraction()
//     {
//         Serial.println("now contraction...");
//         while (digitalRead(photo) != 1)
//         {
//             // // pose motor control
//             digitalWrite(in3, LOW);
//             digitalWrite(in4, LOW);
//             ledcWrite(channel_2, 150);
//             ledcWrite(channel_3, 200);
//         }
//         user_pose = 0;
//         ledcWrite(channel_2, 0);
//         ledcWrite(channel_3, 0);
//         // // Turn off motors
//         digitalWrite(in3, LOW);
//         digitalWrite(in4, LOW);
//         Serial.println("contraction finished");
//     }
//     void relaxation()
//     {
//         // change the duty cycle of the speed control pin connected to the motor
//         Serial.println("now relaxation...");
//         // pose motor control

//         digitalWrite(in3, HIGH);
//         digitalWrite(in4, HIGH);
//         ledcWrite(channel_2, 150);
//         ledcWrite(channel_3, 200);
//         delay(2000);
//         ledcWrite(channel_2, 0);
//         ledcWrite(channel_3, 0);
//         // // Turn off motors
//         digitalWrite(in3, LOW);
//         digitalWrite(in4, LOW);
//         user_pose = 1;
//         Serial.println("relaxation finished");
//     }
// };

// // Our car object
// Car car;

// // Function to send commands to car
// void sendCarCommand(const char *command)
// {
//     // command could be either "left", "right", "forward" or "reverse" or "stop"
//     // or speed settingg "slow-speed", "normal-speed", or "fast-speed"
//     // or robot pose setting "contraction" or "relaxation"
//     if (strcmp(command, "left") == 0)
//     {
//         car.turnLeft();
//     }
//     else if (strcmp(command, "right") == 0)
//     {
//         car.turnRight();
//     }
//     else if (strcmp(command, "up") == 0)
//     {
//         car.moveForward();
//     }
//     else if (strcmp(command, "down") == 0)
//     {
//         car.moveBackward();
//     }
//     else if (strcmp(command, "stop") == 0)
//     {
//         car.stop();
//     }
//     else if (strcmp(command, "slow-speed") == 0)
//     {
//         car.setCurrentSpeed(speedSettings::SLOW);
//     }
//     else if (strcmp(command, "normal-speed") == 0)
//     {
//         car.setCurrentSpeed(speedSettings::NORMAL);
//     }
//     else if (strcmp(command, "fast-speed") == 0)
//     {
//         car.setCurrentSpeed(speedSettings::FAST);
//     }
//     else if (strcmp(command, "contraction") == 0)
//     {
//         car.contraction();
//     }
//     else if (strcmp(command, "relaxation") == 0)
//     {
//         car.relaxation();
//     }
// }

// // Processor for index.html page template.  This sets the radio button to checked or unchecked

// // Setup function
// String command;

// void setup()
// {
//     Serial.begin(115200);
//     while (!Serial.available())
//         ; // Wait for input
// }

// void loop()
// {

//     char command;
//     // command = Serial.read() 없이, if(Serial.read() == '')를 바로 사용하면 두, 세번 전송을 해주어야 읽어들이기 때문에 느리다.
//     command = Serial.read(); // 수신 데이터(시리얼 모니터에서 입력한 값) command에 저장
//     // printf("%c", command);
//     switch (command)
//     {
//     case 'w':
//         Serial.println("up"); // 시리얼 모니터에 "LED On" 출력
//         sendCarCommand("up");
//         break;
//     case 'a':                   // command 값이 'x'이면,
//         Serial.println("left"); // 시리얼 모니터에 "LED On" 출력
//         sendCarCommand("left");
//         break;
//     case 's':                   // command 값이 'x'이면,
//         Serial.println("down"); // 시리얼 모니터에 "LED On" 출력
//         sendCarCommand("down");
//         break;
//     case 'd':                    // command 값이 'x'이면,
//         Serial.println("right"); // 시리얼 모니터에 "LED On" 출력
//         sendCarCommand("right");
//         break;
//     case 'j':                   // command 값이 'x'이면,
//         Serial.println("stop"); // 시리얼 모니터에 "LED On" 출력
//         sendCarCommand("stop");
//         break;
//     case 'i':                         // command 값이 'x'이면,
//         Serial.println("slow-speed"); // 시리얼 모니터에 "LED On" 출력
//         sendCarCommand("slow-speed");
//         break;
//     case 'o':                           // command 값이 'x'이면,
//         Serial.println("normal-speed"); // 시리얼 모니터에 "LED On" 출력
//         sendCarCommand("normal-speed");
//         break;
//     case 'p':                         // command 값이 'x'이면,
//         Serial.println("fast-speed"); // 시리얼 모니터에 "LED On" 출력
//         sendCarCommand("fast-speed");
//         break;
//     case 'c':                          // command 값이 'x'이면,
//         Serial.println("contraction"); // 시리얼 모니터에 "LED On" 출력
//         sendCarCommand("contraction");
//         break;
//     case 'v':                         // command 값이 'x'이면,
//         Serial.println("relaxation"); // 시리얼 모니터에 "LED On" 출력
//         sendCarCommand("relaxation");
//         break;
//     default:
//         break;
//     }
// }

#include <Arduino.h>
#include <Wire.h>
#include "soc/rtc_wdt.h"

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

enum speedSettings
{
    SLOW = 100,
    NORMAL = 180,
    FAST = 255
};
String outputState()
{
    if (digitalRead(output))
    { // real contraction relaxation check
        return "checked";
    }
    else
    {
        return "";
    }
    return "";
}
int user_pose = 0;
// String processor(const String &var)
// {
//     // Serial.println(var);
//     if (var == "BUTTONPLACEHOLDER")
//     {
//         String buttons = "";
//         String outputStateValue = outputState();
//         buttons += "<h4>Output - GPIO 2 - State <span id=\"outputState\"></span></h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"output\" " + outputStateValue + "><span class=\"slider\"></span></label>";
//         return buttons;
//     }
//     return String();
// }

class Car
{
    // left motor connections
    const uint8_t in1 = 32;
    //  right motor connections
    const uint8_t in2 = 26;
    // contraction motor connections
    const uint8_t in3 = 16;
    // relaxation connections
    const uint8_t in4 = 18;

    // PWM Setup to control motor speed
    const uint8_t SPEED_CONTROL_PIN_1 = 33;
    const uint8_t SPEED_CONTROL_PIN_2 = 25;
    const uint8_t SPEED_CONTROL_PIN_3 = 17;
    const uint8_t SPEED_CONTROL_PIN_4 = 19;

    const int photo = 4;
    // Play around with the frequency settings depending on the motor that you are using
    const int freq = 500;
    const int channel_0 = 1;
    const int channel_1 = 2;
    const int channel_2 = 3;
    const int channel_3 = 4;

    // 8 Bit resolution for duty cycle so value is between 0 - 255
    const int resolution = 8;

    // holds the current speed settings, see values for SLOW, NORMAL, FAST
    speedSettings currentSpeedSettings;

public:
    Car()
    {
        // Set all pins to output
        pinMode(in1, OUTPUT);
        pinMode(in2, OUTPUT);
        pinMode(in3, OUTPUT);
        pinMode(in4, OUTPUT);
        pinMode(SPEED_CONTROL_PIN_1, OUTPUT);
        pinMode(SPEED_CONTROL_PIN_2, OUTPUT);
        pinMode(SPEED_CONTROL_PIN_3, OUTPUT);
        pinMode(SPEED_CONTROL_PIN_4, OUTPUT);

        pinMode(photo, INPUT_PULLUP);

        // have to define pose motor

        // Set initial motor state to OFF
        digitalWrite(in1, LOW);
        digitalWrite(in2, LOW);
        digitalWrite(in3, LOW);
        digitalWrite(in4, LOW);

        // // Set the PWM Settings
        ledcSetup(channel_0, freq, resolution);
        ledcSetup(channel_1, freq, resolution);
        ledcSetup(channel_2, freq, resolution);
        ledcSetup(channel_3, freq, resolution);

        // Attach Pin to Channel
        ledcAttachPin(SPEED_CONTROL_PIN_1, channel_0);
        ledcAttachPin(SPEED_CONTROL_PIN_2, channel_1);
        ledcAttachPin(SPEED_CONTROL_PIN_3, channel_2);
        ledcAttachPin(SPEED_CONTROL_PIN_4, channel_3);

        // initialize default speed to SLOW
        setCurrentSpeed(speedSettings::NORMAL);
    }

    // Turn the car left
    void turnLeft()
    {
        Serial.println("car is turning left...");
        digitalWrite(in1, LOW);
        digitalWrite(in2, LOW);
        ledcWrite(channel_0, currentSpeedSettings - 70);
        ledcWrite(channel_1, currentSpeedSettings);
    }

    // Turn the car right
    void turnRight()
    {
        Serial.println("car is turning right...");
        digitalWrite(in1, LOW);
        digitalWrite(in2, LOW);
        ledcWrite(channel_0, currentSpeedSettings);
        ledcWrite(channel_1, currentSpeedSettings - 70);
    }

    // Move the car forward
    void moveForward()
    {
        Serial.println("car is moving forward...");
        setMotorSpeed();
        digitalWrite(in1, LOW);
        digitalWrite(in2, LOW);
        // ledcWrite(channel_0, currentSpeedSettings);
        // ledcWrite(channel_1, currentSpeedSettings);
    }

    // Move the car backward
    void moveBackward()
    {
        Serial.println("car is moving backward...");
        digitalWrite(in1, HIGH);
        digitalWrite(in2, HIGH);
        ledcWrite(channel_0, LOW);
        ledcWrite(channel_1, LOW);
    }

    // Stop the car
    void stop()
    {
        Serial.println("car is stopping...");
        digitalWrite(in1, LOW);
        digitalWrite(in2, LOW);
        ledcWrite(channel_0, 0);
        ledcWrite(channel_1, 0);
        // // Turn off motors
        digitalWrite(in3, LOW);
        digitalWrite(in4, LOW);
        ledcWrite(channel_2, 0);
        ledcWrite(channel_3, 0);
        // // Turn off motors
    }

    // Set the motor speed
    void setMotorSpeed()
    {
        // change the duty cycle of the speed control pin connected to the motor
        Serial.print("Speed Settings: ");
        Serial.println(currentSpeedSettings);
        ledcWrite(channel_0, currentSpeedSettings);
        ledcWrite(channel_1, currentSpeedSettings);
    }
    // Set the current speed
    void setCurrentSpeed(speedSettings newSpeedSettings)
    {
        Serial.println("car is changing speed...");
        currentSpeedSettings = newSpeedSettings;
    }
    // Get the current speed
    speedSettings getCurrentSpeed()
    {
        return currentSpeedSettings;
    }
    // set robot contraction
    void manualContraction()
    {
        Serial.println("car is contracting...");
        digitalWrite(in3, LOW);
        ledcWrite(channel_2, 250);
    }
    void manualContraction_reverse()
    {
        Serial.println("car is reverse contracting...");
        digitalWrite(in3, HIGH);
        ledcWrite(channel_2, 0);
    }
    void manualRelaxation()
    {
        Serial.println("car is relaxing...");
        digitalWrite(in4, LOW);
        ledcWrite(channel_3, 250);
    }
    void manualRelaxation_reverse()
    {
        Serial.println("car is reverse relaxing...");
        digitalWrite(in4, HIGH);
        ledcWrite(channel_3, 0);
    }
    int contraction()
    {

        // Serial.println("now contraction...");
        // while (digitalRead(photo) != 1)
        // {
        // Serial.println(digitalRead(photo));
        //  // pose motor control
        digitalWrite(in3, LOW);
        digitalWrite(in4, LOW);
        ledcWrite(channel_2, 200);
        ledcWrite(channel_3, 100);
        vTaskDelay(500);
        // }

        user_pose = 1;

        if (digitalRead(photo) != 1)
        {
            return 0;
        }
        else
        {
            return 1;
        }
        // ledcWrite(channel_2, 0);
        // ledcWrite(channel_3, 0);
        // vTaskDelay(10);
        // // Turn off motors
        // digitalWrite(in3, LOW);
        // digitalWrite(in4, LOW);

        Serial.println("contraction finished");
    }
    void relaxation()
    {
        // change the duty cycle of the speed control pin connected to the motor
        Serial.println("now relaxation...");
        // pose motor control

        digitalWrite(in3, HIGH);
        digitalWrite(in4, HIGH);
        ledcWrite(channel_2, 150);
        ledcWrite(channel_3, 200);
        vTaskDelay(2000);
        ledcWrite(channel_2, 0);
        ledcWrite(channel_3, 0);
        // // Turn off motors
        digitalWrite(in3, LOW);
        digitalWrite(in4, LOW);
        user_pose = 0;
        Serial.println("relaxation finished");
    }
};

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
        // delay(300);
    }
    else if (strcmp(command, "up") == 0)
    {
        car.moveForward();
        // delay(1000);
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
        car.manualContraction();
    }
    else if (strcmp(command, "relaxation") == 0)
    {
        car.manualRelaxation();
    }
    else if (strcmp(command, "contractionR") == 0)
    {
        car.manualContraction_reverse();
    }
    else if (strcmp(command, "relaxationR") == 0)
    {
        car.manualRelaxation_reverse();
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
    // initSensor();
    Serial.begin(115200);
    // calibrateSensor();
    // past = millis();

    // Initialize the Serial monitor baud rate
}

void loop()
{
    char command;
    // command = Serial.read() 없이, if(Serial.read() == '')를 바로 사용하면 두, 세번 전송을 해주어야 읽어들이기 때문에 느리다.
    command = Serial.read(); // 수신 데이터(시리얼 모니터에서 입력한 값) command에 저장
    // printf("%c", command);
    switch (command)
    {
    case 'w':
        Serial.println("up"); // 시리얼 모니터에 "LED On" 출력
        sendCarCommand("up");
        break;
    case 'a':                   // command 값이 'x'이면,
        Serial.println("left"); // 시리얼 모니터에 "LED On" 출력
        sendCarCommand("left");
        break;
    case 's':                   // command 값이 'x'이면,
        Serial.println("down"); // 시리얼 모니터에 "LED On" 출력
        sendCarCommand("down");
        break;
    case 'd':                    // command 값이 'x'이면,
        Serial.println("right"); // 시리얼 모니터에 "LED On" 출력
        sendCarCommand("right");
        break;
    case 'j':                   // command 값이 'x'이면,
        Serial.println("stop"); // 시리얼 모니터에 "LED On" 출력
        sendCarCommand("stop");
        break;
    case 'i':                         // command 값이 'x'이면,
        Serial.println("slow-speed"); // 시리얼 모니터에 "LED On" 출력
        sendCarCommand("slow-speed");
        break;
    case 'o':                           // command 값이 'x'이면,
        Serial.println("normal-speed"); // 시리얼 모니터에 "LED On" 출력
        sendCarCommand("normal-speed");
        break;
    case 'p':                         // command 값이 'x'이면,
        Serial.println("fast-speed"); // 시리얼 모니터에 "LED On" 출력
        sendCarCommand("fast-speed");
        break;
    case 'z':                          // command 값이 'x'이면,
        Serial.println("contraction"); // 시리얼 모니터에 "LED On" 출력
        sendCarCommand("contraction");
        break;
    case 'x':                          // command 값이 'x'이면,
        Serial.println("relaxationR"); // 시리얼 모니터에 "LED On" 출력
        sendCarCommand("relaxationR");
        break;
    case 'c':                           // command 값이 'x'이면,
        Serial.println("contractionR"); // 시리얼 모니터에 "LED On" 출력
        sendCarCommand("contractionR");
        break;
    case 'v':                         // command 값이 'x'이면,
        Serial.println("relaxation"); // 시리얼 모니터에 "LED On" 출력
        sendCarCommand("relaxation");
        break;

    default:
        break;
    }
}