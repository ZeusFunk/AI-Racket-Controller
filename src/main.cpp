#include <Arduino.h>
#include <bleConnector.h>

void callBacks(BLECharacteristic *pCharWriteState);
// 全局变量定义目前球拍的模式：0->未开启；1->训练模式，2->比赛模式
int currentMode = 0;

void setup()
{
    // 初始化串口
    Serial.begin(115200);
    // 初始化蓝牙
    bleInit(callBacks);
}

void loop()
{
}

// 这里是单片机接收到手机传来的数据进行的回调处理
void callBacks(BLECharacteristic *pCharWriteState)
{
    std::string inputValue = pCharWriteState->getValue();
    if (inputValue.length() > 0)
    {
        Serial.print("Received Value: ");
        for (int i = 0; i < inputValue.length(); i++)
        {
            Serial.print(inputValue[i]);
        }
        Serial.println();
        // inputValue
        // 发送数据给客户端
        if (inputValue == "Training Mode")
        {
            currentMode = 1;
            pOutputChar->setValue("Training Mode ON!");
            pOutputChar->notify(); 
        }
        else if (inputValue == "Gaming Mode")
        {
            currentMode = 2;
            pOutputChar->setValue("Game Mode ON!");
            pOutputChar->notify(); 
        }
        else if (inputValue == "Turn off")
        {
            currentMode = 0;
            pOutputChar->setValue("Turn off now!");
            pOutputChar->notify();
        }
    }
}