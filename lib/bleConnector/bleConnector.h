#include <Arduino.h>

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

// 广播给外部世界的服务名
#define PERIPHERAL_NAME "S101-Racket"
#define SERVICE_UUID "CD9CFC21-0ECC-42E5-BF22-48AA715CA112"
#define CHARACTERISTIC_INPUT_UUID "66E5FFCE-AA96-4DC9-90C3-C62BBCCD29AC"
#define CHARACTERISTIC_OUTPUT_UUID "142F29DD-B1F0-4FA8-8E55-5A2D5F3E2471"

// 输出特征用于将响应发送回已连接的手机
extern BLECharacteristic *pOutputChar;

void bleInit(void (*onWriteCallBacks)(BLECharacteristic*));