#include <Arduino.h>
#include "wit_imu.h"
#include "DataFusion.h"

// #define KEYTEST 1

// esp_err_t ret = 0;
// 定义连接LED的引脚
const int ledPin = 9;
// //触摸按键测试
// // const int gpioPin = 39;

// // bool readGPIOPin(void);

// // 初始化硬件
void hardwareInit(void)
{
//   // 初始化引脚为输出模式
//   // pinMode(gpioPin, INPUT);
  pinMode(ledPin, OUTPUT);
  
//   ret = i2c_master_init();
//   if(ret != ESP_OK) {
//     Serial.println("I2C master init failed");
//   }
//   else
//   {
//     for(int i = 0;i < 5;i++)
//     {
//       digitalWrite(ledPin, LOW); 
//       delay(300);  
      digitalWrite(ledPin, HIGH);  
//       delay(300);  
//     }
  
//     Serial.println("I2C master init success!");
//   }
//   WitInit(WIT_PROTOCOL_I2C, 0x50);
//   WitRegisterCallBack(SensorDataUpdata);
//   WitI2cFuncRegister(WitIICWrite, WitIICRead);
//   WitDelayMsRegister(Delayms);
//   AutoScanSensor();
}


void setup() {
  Serial.begin(115200);
  hardwareInit();//调用前注意初始化串口
}

void loop() {
//   float quaterdata[4] = {0};
//   float rawdata[6] = {0};
//   // float rawAccel[3] = {0};
//   float earth_accel[3] = {0};
//   // float magdata[3] = {0};
//   // float eulerdata[3] = {0};
digitalWrite(ledPin, HIGH);  
//   GetImuRawdata(rawdata);
//   GetImuQuater(quaterdata);
//   // GetImuMag(magdata);
//   // GetImuEuler(eulerdata);
//   AhrsGetEarthAcceleration(quaterdata, rawdata, earth_accel);
//   Serial.printf("accel: %0.2f ,%0.2f ,%0.2f \r\n",*earth_accel,*(earth_accel+1), *(earth_accel+2));
//   // if(readGPIOPin())
//   // {
//   //   Serial.println("GPIO HIGH");
//   // }
//   // else
//   // {
//   //   Serial.println("GPIO LOW");
//   // }
}

// // // 读取GPIO引脚状态的函数
// // bool readGPIOPin() {
// //   return digitalRead(gpioPin);
// // }


// #include <Wire.h>
// #include "Adafruit_HUSB238.h"


// #define SDA_PIN 25
// #define SCL_PIN 26


// Adafruit_HUSB238 husb238;

// void setup() {
//   Serial.begin(115200);
//   while (!Serial) delay(10);
//   Serial.println("Adafruit HUSB238 Test Sketch");
//   Wire.begin(SDA_PIN, SCL_PIN);

//   // Initialize the HUSB238
//   if (husb238.begin(HUSB238_I2CADDR_DEFAULT, &Wire)) {
//     Serial.println("HUSB238 initialized successfully.");
//   } else {
//     Serial.println("Couldn't find HUSB238, check your wiring?");
//     while (1);
//   }
// }

// void loop() {
//   delay(1000);  // Add a delay to prevent flooding the serial output
//   Serial.println(F("----------------------------------------------"));

//   if (! husb238.isAttached()) 
//     return;

//   if (husb238.getPDResponse() != SUCCESS)
//     return;

//   // What voltages and currents are available from this adapter?
//   Serial.println("Available PD Voltages and Current Detection Test:");
//   for (int i = PD_SRC_5V; i <= PD_SRC_20V; i++) {
//     bool voltageDetected = husb238.isVoltageDetected((HUSB238_PDSelection)i);

//     switch ((HUSB238_PDSelection)i) {
//       case PD_SRC_5V:
//         Serial.print("5V");
//         break;
//       case PD_SRC_9V:
//         Serial.print("9V");
//         break;
//       case PD_SRC_12V:
//         Serial.print("12V");
//         break;
//       case PD_SRC_15V:
//         Serial.print("15V");
//         break;
//       case PD_SRC_18V:
//         Serial.print("18V");
//         break;
//       case PD_SRC_20V:
//         Serial.print("20V");
//         break;
//       default:
//         continue;
//     }
//     Serial.println(voltageDetected ? " Available" : " Unavailable");

//     Serial.println("\tSetting new PD voltage");
//     // Change to that voltage
//     husb238.selectPD((HUSB238_PDSelection)i);
//     // Perform the actual PD voltage request!
//     husb238.requestPD();

//     delay(2000);
//   }
// }
