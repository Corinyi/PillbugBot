// #include<WiFi.h>
#include <Arduino.h>
#include <Wire.h>
#include <stdio.h>
// #include <Adafruit_MPU6050.h>
// #include <Adafruit_Sensor.h>

/*d
  The resolution of the PWM is 8 bit so the value is between 0-255
  We will set the speed between 100 to 255.
*/

int output = 2;
int ledState = LOW;
const char *PARAM_INPUT_1 = "state";

enum speedSettings
{
    SLOW = 100,
    NORMAL = 180,
    FAST = 250
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

class Car
{
private:
    // Motor 1 connections
    int in1 = 16;
    int in2 = 17;
    // Motor 2 connections
    int in3 = 18;
    int in4 = 19;

    //  contractionmotor
    int in5 = 25;
    int in6 = 26;
    // relaxation motor
    int in7 = 32;
    int in8 = 33;

    int in9 = 2;

    speedSettings currentSpeedSettings;

public:
    Car()
    {
        // Set all pins to output
        pinMode(in1, OUTPUT);
        pinMode(in2, OUTPUT);
        pinMode(in3, OUTPUT);
        pinMode(in4, OUTPUT);
        pinMode(in5, OUTPUT);
        pinMode(in6, OUTPUT);
        pinMode(in7, OUTPUT);
        pinMode(in8, OUTPUT);
        pinMode(in9, INPUT_PULLUP);
        // pinMode(SPEED_CONTROL_PIN_1, OUTPUT);
        // pinMode(SPEED_CONTROL_PIN_2, OUTPUT);

        // have to define pose motor

        // Set initial motor state to OFF
        analogWrite(in1, 0);
        analogWrite(in2, 0);
        analogWrite(in3, 0);
        analogWrite(in4, 0);
        analogWrite(in5, 0);
        analogWrite(in6, 0);
        analogWrite(in7, 0);
        analogWrite(in8, 0);
        setCurrentSpeed(speedSettings::NORMAL);
    }

    void turnLeft()
    {
        Serial.println("car is turning left...");
        int motorSpeed = setMotorSpeed();
        analogWrite(in1, 0);
        analogWrite(in2, motorSpeed);
        analogWrite(in3, 0);
        analogWrite(in4, motorSpeed - 60);
    }

    // Turn the car right
    void turnRight()
    {
        Serial.println("car is turning right...");
        int motorSpeed = setMotorSpeed();
        analogWrite(in1, 0);
        analogWrite(in2, motorSpeed - 60);
        analogWrite(in3, 0);
        analogWrite(in4, motorSpeed);
    }
    // Move the car forward
    void moveForward()
    {
        Serial.println("car is moving forward...");
        int motorSpeed = setMotorSpeed();
        analogWrite(in1, 0);
        analogWrite(in2, motorSpeed);
        analogWrite(in3, 0);
        analogWrite(in4, motorSpeed);
    }

    // Move the car backward
    void moveBackward()
    {
        Serial.println("car is moving backward...");
        int motorSpeed = setMotorSpeed();
        analogWrite(in1, motorSpeed);
        analogWrite(in2, 0);
        analogWrite(in3, motorSpeed);
        analogWrite(in4, 0);
    }

    // Stop the car
    void stop()
    {
        Serial.println("car is stopping...");
        // ledcWrite(channel_0, 0);
        // ledcWrite(channel_1, 0);

        // // Turn off motors
        analogWrite(in1, 0);
        analogWrite(in2, 0);
        analogWrite(in3, 0);
        analogWrite(in4, 0);
    }

    // Set the motor speed
    int setMotorSpeed()
    {
        // change the duty cycle of the speed control pin connected to the motor
        Serial.print("Speed Settings: ");
        Serial.println(currentSpeedSettings);
        // ledcWrite(channel_0, currentSpeedSettings);
        // ledcWrite(channel_1, currentSpeedSettings);
        return currentSpeedSettings;
        // if (speedSettings::FAST)
        //     return 200;
        // else if (speedSettings::NORMAL)
        //     return 180;
        // else
        //     return 100;
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
    void contraction()
    {
        Serial.println("now contraction...");
        while (digitalRead(in9) != 1)
        {
            // change the duty cycle of the speed control pin connected to the motor

            // pose motor control
            analogWrite(in5, 255);
            analogWrite(in6, 0);
            analogWrite(in7, 0);
            analogWrite(in8, 255);
            
        }
        user_pose = 0;
        Serial.println("contraction finished");
    }
    void relaxation()
    {

        // change the duty cycle of the speed control pin connected to the motor
        Serial.println("now relaxation...");
        // pose motor control
        analogWrite(in5, 0);
        analogWrite(in6, 255);
        analogWrite(in7, 255);
        analogWrite(in8, 0);

        delay(2000);
        user_pose = 1;
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

// Setup function
String command;

void setup()
{
    Serial.begin(115200);
    while (!Serial.available())
        ; // Wait for input
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
    case 'c':                          // command 값이 'x'이면,
        Serial.println("contraction"); // 시리얼 모니터에 "LED On" 출력
        sendCarCommand("contraction");
        break;
    case 'v':                         // command 값이 'x'이면,
        Serial.println("relaxation"); // 시리얼 모니터에 "LED On" 출력
        sendCarCommand("relaxation");
        break;
    default:
        break;
    }
}
