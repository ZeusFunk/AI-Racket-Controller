#include <bleConnector.h>
BLECharacteristic *pOutputChar; // 定义 pOutputChar

// 当设备连接和断开连接时，类定义被调用的方法
class ServerCallbacks : public BLEServerCallbacks
{
    void onConnect(BLEServer *pServer)
    {
        Serial.println("BLE Client Connected");
    }
    void onDisconnect(BLEServer *pServer)
    {
        BLEDevice::startAdvertising(); // 重新开始广播
        Serial.println("BLE Client Disconnected");
    }
};

class InputReceivedCallbacks : public BLECharacteristicCallbacks
{
public:
    // 使用std::function来存储无参数无返回值的函数
    using CallbackFunction = std::function<void(BLECharacteristic*)>;

    // 构造函数，只接受一个函数作为参数
    InputReceivedCallbacks(CallbackFunction callback)
        : callbackFunc(callback) {}

    void onWrite(BLECharacteristic *pCharWriteState)
    {
        callbackFunc(pCharWriteState);
    }

private:
    CallbackFunction callbackFunc; // 存储回调函数
};

void bleInit(void (*onWriteCallBacks)(BLECharacteristic*))
{
    // 初始化BLE
    BLEDevice::init(PERIPHERAL_NAME);
    // 创建服务器
    BLEServer *pServer = BLEDevice::createServer();
    // 创建服务
    BLEService *pService = pServer->createService(SERVICE_UUID);
    // 处理输入（从应用程序发送）
    BLECharacteristic *pInputChar = pService->createCharacteristic(
        CHARACTERISTIC_INPUT_UUID,
        BLECharacteristic::PROPERTY_WRITE_NR |
            BLECharacteristic::PROPERTY_WRITE);

    pOutputChar = pService->createCharacteristic(
        CHARACTERISTIC_OUTPUT_UUID,
        BLECharacteristic::PROPERTY_READ |
            BLECharacteristic::PROPERTY_NOTIFY);

    pServer->setCallbacks(new ServerCallbacks());
    pInputChar->setCallbacks(new InputReceivedCallbacks(onWriteCallBacks));

    // 开始服务
    pService->start();

    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->setScanResponse(true);
    BLEDevice::startAdvertising(); // 开始广播
}