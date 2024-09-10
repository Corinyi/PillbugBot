// #include <Arduino.h>
// #include <wire.h>

// void setup()
// {
//   pinMode(LED_BUILTIN, OUTPUT);
//   Serial.begin(11520);
//   Serial.println("hello from setup");
// }

// void loop()
// {
//   delay(1000);
//   digitalWrite(LED_BUILTIN, HIGH);
//   // put your main code here, to run repeatedly:
//   delay(1000);
//   Serial.println("hello from loop");

//   digitalWrite(LED_BUILTIN, LOW);
// }

// #include <Adafruit_MPU6050.h>
// #include <Adafruit_Sensor.h>
// #include <Wire.h>

// Adafruit_MPU6050 mpu;

// void setup(void)
// {
//   Serial.begin(115200);
//   while (!Serial)
//     delay(10); // will pause Zero, Leonardo, etc until serial console opens

//   Serial.println("Adafruit MPU6050 test!");

//   // Try to initialize!
//   if (!mpu.begin())
//   {
//     Serial.println("Failed to find MPU6050 chip");
//     while (1)
//     {
//       delay(10);
//     }
//   }
//   Serial.println("MPU6050 Found!");

//   mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
//   Serial.print("Accelerometer range set to: ");
//   switch (mpu.getAccelerometerRange())
//   {
//   case MPU6050_RANGE_2_G:
//     Serial.println("+-2G");
//     break;
//   case MPU6050_RANGE_4_G:
//     Serial.println("+-4G");
//     break;
//   case MPU6050_RANGE_8_G:
//     Serial.println("+-8G");
//     break;
//   case MPU6050_RANGE_16_G:
//     Serial.println("+-16G");
//     break;
//   }
//   mpu.setGyroRange(MPU6050_RANGE_500_DEG);
//   Serial.print("Gyro range set to: ");
//   switch (mpu.getGyroRange())
//   {
//   case MPU6050_RANGE_250_DEG:
//     Serial.println("+- 250 deg/s");
//     break;
//   case MPU6050_RANGE_500_DEG:
//     Serial.println("+- 500 deg/s");
//     break;
//   case MPU6050_RANGE_1000_DEG:
//     Serial.println("+- 1000 deg/s");
//     break;
//   case MPU6050_RANGE_2000_DEG:
//     Serial.println("+- 2000 deg/s");
//     break;
//   }

//   mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);
//   Serial.print("Filter bandwidth set to: ");
//   switch (mpu.getFilterBandwidth())
//   {
//   case MPU6050_BAND_260_HZ:
//     Serial.println("260 Hz");
//     break;
//   case MPU6050_BAND_184_HZ:
//     Serial.println("184 Hz");
//     break;
//   case MPU6050_BAND_94_HZ:
//     Serial.println("94 Hz");
//     break;
//   case MPU6050_BAND_44_HZ:
//     Serial.println("44 Hz");
//     break;
//   case MPU6050_BAND_21_HZ:
//     Serial.println("21 Hz");
//     break;
//   case MPU6050_BAND_10_HZ:
//     Serial.println("10 Hz");
//     break;
//   case MPU6050_BAND_5_HZ:
//     Serial.println("5 Hz");
//     break;
//   }

//   Serial.println("");
//   delay(100);
// }

// void loop()
// {
//   /* Get new sensor events with the readings */
//   sensors_event_t a, g, temp;
//   mpu.getEvent(&a, &g, &temp);

//   /* Print out the values */
//   Serial.print("Acceleration X: ");
//   Serial.print(a.acceleration.x);
//   Serial.print(", Y: ");
//   Serial.print(a.acceleration.y);
//   Serial.print(", Z: ");
//   Serial.print(a.acceleration.z);
//   Serial.println(" m/s^2");

//   Serial.print("Rotation X: ");
//   Serial.print(g.gyro.x);
//   Serial.print(", Y: ");
//   Serial.print(g.gyro.y);
//   Serial.print(", Z: ");
//   Serial.print(g.gyro.z);
//   Serial.println(" rad/s");

//   Serial.print("Temperature: ");
//   Serial.print(temp.temperature);
//   Serial.println(" degC");

//   Serial.println("");
//   delay(10);
// }

// 《 상보필터를 적용한 Roll과 Pitch, Yaw의 각도 구하기 실습  》
// 아래 코드관련 실습에 대한 설명과 회로도 및 자료는 https://rasino.tistory.com/ 에 있습니다
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

const int MPU_ADDR = 0x68;                 // I2C통신을 위한 MPU6050의 주소
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ; // 가속도(Acceleration)와 자이로(Gyro)
double angleAcX, angleAcY, angleAcZ;
double angleGyX, angleGyY, angleGyZ;
double angleFiX, angleFiY, angleFiZ;

const double RADIAN_TO_DEGREE = 180 / 3.14159;
const double DEG_PER_SEC = 32767 / 250; // 1초에 회전하는 각도
const double ALPHA = 1 / (1 + 0.04);
// GyX, GyY, GyZ 값의 범위 : -32768 ~ +32767 (16비트 정수범위)

unsigned long now = 0;  // 현재 시간 저장용 변수
unsigned long past = 0; // 이전 시간 저장용 변수
double dt = 0;          // 한 사이클 동안 걸린 시간 변수

double averAcX, averAcY, averAcZ;
double averGyX, averGyY, averGyZ;

void initSensor()
{
  Wire.begin();
  Wire.beginTransmission(MPU_ADDR); // I2C 통신용 어드레스(주소)
  Wire.write(0x6B);                 // MPU6050과 통신을 시작하기 위해서는 0x6B번지에
  Wire.write(0);
  Wire.endTransmission(true);
}

void getData()
{
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B); // AcX 레지스터 위치(주소)를 지칭합니다
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDR, 14, true); // AcX 주소 이후의 14byte의 데이터를 요청
  AcX = Wire.read() << 8 | Wire.read(); // 두 개의 나뉘어진 바이트를 하나로 이어 붙여서 각 변수에 저장
  AcY = Wire.read() << 8 | Wire.read();
  AcZ = Wire.read() << 8 | Wire.read();
  Tmp = Wire.read() << 8 | Wire.read();
  GyX = Wire.read() << 8 | Wire.read();
  GyY = Wire.read() << 8 | Wire.read();
  GyZ = Wire.read() << 8 | Wire.read();
}

// loop 한 사이클동안 걸리는 시간을 알기위한 함수
void getDT()
{
  now = millis();
  dt = (now - past) / 1000.0;
  past = now;
}

// 센서의 초기값을 10회 정도 평균값으로 구하여 저장하는 함수
void caliSensor()
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
    delay(50);
  }
  averAcX = sumAcX / 10;
  averAcY = sumAcY / 10;
  averAcZ = sumAcY / 10;
  averGyX = sumGyX / 10;
  averGyY = sumGyY / 10;
  averGyZ = sumGyZ / 10;
}

void setup()
{
  initSensor();
  Serial.begin(115200);
  caliSensor();    //  초기 센서 캘리브레이션 함수 호출
  past = millis(); // past에 현재 시간 저장
}

void loop()
{
  getData();
  getDT();

  angleAcX = atan(AcY / sqrt(pow(AcX, 2) + pow(AcZ, 2)));
  angleAcX *= RADIAN_TO_DEGREE;
  angleAcY = atan(-AcX / sqrt(pow(AcY, 2) + pow(AcZ, 2)));
  angleAcY *= RADIAN_TO_DEGREE;
  // 가속도 센서로는 Z축 회전각 계산 불가함.

  // 가속도 현재 값에서 초기평균값을 빼서 센서값에 대한 보정
  angleGyX += ((GyX - averGyX) / DEG_PER_SEC) * dt; // 각속도로 변환
  angleGyY += ((GyY - averGyY) / DEG_PER_SEC) * dt;
  angleGyZ += ((GyZ - averGyZ) / DEG_PER_SEC) * dt;

  // 상보필터 처리를 위한 임시각도 저장
  double angleTmpX = angleFiX + angleGyX * dt;
  double angleTmpY = angleFiY + angleGyY * dt;
  double angleTmpZ = angleFiZ + angleGyZ * dt;

  // (상보필터 값 처리) 임시 각도에 0.96가속도 센서로 얻어진 각도 0.04의 비중을 두어 현재 각도를 구함.
  angleFiX = ALPHA * angleTmpX + (1.0 - ALPHA) * angleAcX;
  angleFiY = ALPHA * angleTmpY + (1.0 - ALPHA) * angleAcY;
  angleFiZ = angleGyZ; // Z축은 자이로 센서만을 이용하열 구함.

  Serial.print(AcX);
  Serial.print(" ");
  Serial.print(AcY);
  Serial.print(" ");
  Serial.print(AcZ);
  Serial.print(" ");
  Serial.print(GyX);
  Serial.print(" ");
  Serial.print(GyY);
  Serial.print(" ");
  Serial.print(GyX);
  Serial.print(" ");
  Serial.print(angleFiX);
  Serial.print(" ");
  Serial.print(angleFiY);
  Serial.print(" ");
  Serial.print(angleFiZ);
  Serial.println(" ");
  // Serial.print("AngleAcX:");
  // Serial.print(angleAcX);
  // Serial.print("\t FilteredX:");
  // Serial.print(angleFiX);
  //  Serial.print("\t AngleAcY:");
  //  Serial.print(angleAcY);
  // Serial.print("\t FilteredY:");
  // Serial.println(angleFiY);
  //  Serial.print("\t AngleGyZ:");
  //  Serial.print(angleGyZ);
  //  Serial.print("\t FilteredZ:");
  //  Serial.println(angleFiZ);

  //  Serial.print("Angle Gyro X:");
  //  Serial.print(angleGyX);
  //  Serial.print("\t\t Angle Gyro y:");
  //  Serial.print(angleGyY);
  //  Serial.print("\t\t Angle Gyro Z:");
  //  Serial.println(angleGyZ);
  Serial.println(xPortGetCoreID());
  delay(20);
}
