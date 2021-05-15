#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>
#include "InternalController.h"
#include "Light.h"

#define DEVICENAME "The Fucking Car"

#define GENERAL_SERVICE "f2f9a4de-ef95-4fe1-9c2e-ab5ef6f0d6e9"
#define SEND_INT "e376bd46-0d9a-44ab-bb71-c262d06f60c7"
#define RECIVE_INT "ec693074-43fe-489d-b63b-94456f83beb5"
#define SEND_BOOL "5c409aab-50d4-42c2-bf57-430916e5eaf4"
#define RECIVE_BOOL "45db5a06-5481-49ee-a8e9-10b411d73de7"
#define SEND_STRING "9e8fafe1-8966-4276-a3a3-d0b00269541e"
#define RECIVE_STRING "9393c756-78ea-4629-a53e-52fb10f9a63f"

#define LIGHT_SERVICE "e0992b8e-3deb-4644-8901-a8241064bfb9"
#define SEND_LIGHT_STATUS "6a00e857-2e06-43ab-bb43-618a156e572a"
#define RECIVE_LIGHT_STATUS "c10cfc4b-5605-4db2-be14-bdd67ede7f26"

#define CONTROL_SERVICE "eebb9f2e-e0b4-40e6-8269-4524d07b972c"
#define SEND_STEARING_ANGLE "21909a07-0fbf-42fd-b329-d6e29d33a2c6"
#define RECIVE_STEARING_ANGLE "8d584813-7e56-48ab-92fc-f38c54d391e7"

bool deviceConnected = false;

String strToString(std::string str)
{
    return str.c_str();
}

int strToInt(std::string str)
{
    const char *encoded = str.c_str();
    return 256 * int(encoded[1]) + int(encoded[0]);
}

double intToDouble(int value, double max)
{
    return (1.0 * value) / max;
}

bool intToBool(int value)
{
    if (value == 0)
    {
        return false;
    }
    return true;
}

class ConnectionServerCallbacks : public BLEServerCallbacks
{
    void onConnect(BLEServer *pServer)
    {
        deviceConnected = true;
        Serial.println("Connected");
    };

    void onDisconnect(BLEServer *pServer)
    {
        deviceConnected = false;
        BLEAdvertising *pAdvertising = pServer->getAdvertising();
        pAdvertising->stop();
        pAdvertising->start();
        Serial.println("Disconnected");
    }
};

class WriteString : public BLECharacteristicCallbacks
{
    void onWrite(BLECharacteristic *pCharacteristic)
    {
        String str = strToString(pCharacteristic->getValue());
        Serial.print("Recived String:");
        Serial.println(str);
    }
};

class WriteInt : public BLECharacteristicCallbacks
{
    void onWrite(BLECharacteristic *pCharacteristic)
    {
        int rint = strToInt(pCharacteristic->getValue());
        Serial.print("Recived Int:");
        Serial.println(rint);
    }
};

class WriteBool : public BLECharacteristicCallbacks
{
    void onWrite(BLECharacteristic *pCharacteristic)
    {
        bool rbool = intToBool(strToInt(pCharacteristic->getValue()));
        Serial.print("Recived Bool:");
        Serial.println(rbool ? "ON" : "OFF");
    }
};

class OnReciveLightStatus : public BLECharacteristicCallbacks
{
    void onWrite(BLECharacteristic *pCharacteristic)
    {
        bool rbool = intToBool(strToInt(pCharacteristic->getValue()));
        Serial.print("Recived Light Status:");
        Serial.println(rbool ? "ON" : "OFF");
        setLightStage(rbool ? LIGHT_STAGE_ON : LIGHT_STAGE_OFF);
    }
};

class OnReciveStearingAngle : public BLECharacteristicCallbacks
{
    void onWrite(BLECharacteristic *pCharacteristic)
    {
        int rint = strToInt(pCharacteristic->getValue());
        Serial.print("Recived Stearing Angle:");
        Serial.println(rint);
        setStearingAngle(rint);
    }
};

BLECharacteristic *sSendInt;
BLECharacteristic *sSendBool;
BLECharacteristic *sSendString;

void setupFlutterBlue()
{
    Serial.print("Device Name:");
    Serial.println(DEVICENAME);

    BLEDevice::init(DEVICENAME);
    BLEServer *btServer = BLEDevice::createServer();
    btServer->setCallbacks(new ConnectionServerCallbacks());

    uint32_t cwrite = BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE;
    uint32_t cnotify = BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_NOTIFY | BLECharacteristic::PROPERTY_INDICATE;

    // Recive
    BLEService *generalService = btServer->createService(GENERAL_SERVICE);

    BLECharacteristic *sReciveInt = generalService->createCharacteristic(RECIVE_INT, cwrite);
    sReciveInt->setCallbacks(new WriteInt());

    BLECharacteristic *sReciveBool = generalService->createCharacteristic(RECIVE_BOOL, cwrite);
    sReciveBool->setCallbacks(new WriteBool());

    BLECharacteristic *sReciveString = generalService->createCharacteristic(RECIVE_STRING, cwrite);
    sReciveString->setCallbacks(new WriteString());

    sSendInt = generalService->createCharacteristic(SEND_INT, cnotify);
    sSendInt->addDescriptor(new BLE2902());
    sSendInt->setValue("9000");

    sSendBool = generalService->createCharacteristic(SEND_BOOL, cnotify);
    sSendBool->addDescriptor(new BLE2902());
    sSendBool->setValue("0");

    sSendString = generalService->createCharacteristic(SEND_STRING, cnotify);
    sSendString->addDescriptor(new BLE2902());
    sSendString->setValue("Hi");

    // Light service
    BLEService *lightService = btServer->createService(LIGHT_SERVICE);

    BLECharacteristic *sReciveLightStatus = lightService->createCharacteristic(RECIVE_LIGHT_STATUS, cwrite);
    sReciveLightStatus->setCallbacks(new OnReciveLightStatus());

    // Control service
    BLEService *controlService = btServer->createService(CONTROL_SERVICE);

    BLECharacteristic *sReciveStearingAngle = controlService->createCharacteristic(RECIVE_STEARING_ANGLE, cwrite);
    sReciveStearingAngle->setCallbacks(new OnReciveStearingAngle());

    // Start things
    generalService->start();
    lightService->start();
    controlService->start();

    BLEAdvertising *pAdvertising = btServer->getAdvertising();
    pAdvertising->start();
}