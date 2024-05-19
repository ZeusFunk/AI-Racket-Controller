#include <Arduino.h>
#include "wit_imu.h"
#include "DataFusion.h"

esp_err_t ret = 0;

// 初始化硬件
void hardwareInit(void)
{
  
  ret = i2c_master_init();
  if(ret != ESP_OK) {
    Serial.println("I2C master init failed");
  }
  WitInit(WIT_PROTOCOL_I2C, 0x50);
  WitRegisterCallBack(SensorDataUpdata);
  WitI2cFuncRegister(WitIICWrite, WitIICRead);
  WitDelayMsRegister(Delayms);
  AutoScanSensor();
}


void setup() {
  Serial.begin(115200);
  //调用前注意初始化串口
  hardwareInit();
}

void loop() {
  float quaterdata[4] = {0};
  float rawdata[6] = {0};
  // float rawAccel[3] = {0};
  float earth_accel[3] = {0};
  // float magdata[3] = {0};
  float eulerdata[3] = {0};
  GetImuRawdata(rawdata);
  GetImuQuater(quaterdata);
  // GetImuMag(magdata);
  GetImuEuler(eulerdata);
  //获取地球坐标系加速度
  AhrsGetEarthAcceleration(quaterdata, rawdata, earth_accel);
  Serial.printf("accel: %0.2f ,%0.2f ,%0.2f \r\n",*earth_accel,*(earth_accel+1), *(earth_accel+2));
  Serial.printf("quater: %0.2f ,%0.2f ,%0.2f ,%0.2f \r\n",*quaterdata,*(quaterdata+1), *(quaterdata+2), *(quaterdata+3));
  Serial.printf("euler: %0.2f ,%0.2f ,%0.2f \r\n",*eulerdata,*(eulerdata+1), *(eulerdata+2));
  Serial.printf("Ahre: %0.2f ,%0.2f ,%0.2f \r\n",*earth_accel,*(earth_accel+1), *(earth_accel+2));
  delay(100);
}
