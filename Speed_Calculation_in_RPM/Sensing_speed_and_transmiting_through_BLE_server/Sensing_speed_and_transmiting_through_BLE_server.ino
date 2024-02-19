#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

BLEServer *pServer;
BLECharacteristic *pCharacteristic;

volatile int pulseCount = 0;
unsigned long lastTime;
unsigned long currentTime;
unsigned long timeInterval = 500; // Time interval in milliseconds

const int encoderPinA = 26; // Example GPIO pin for encoder channel A
const int encoderPinB = 14; // Example GPIO pin for encoder channel B

// Define the Bluetooth service and characteristic UUIDs
#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

void setup() {
  Serial.begin(115200);

  // Initialize BLE
  BLEDevice::init("Motor_Speed_Monitor");
  pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);
  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_READ |
                      BLECharacteristic::PROPERTY_NOTIFY
                    );
  pService->start();
  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->start();

  // Setup encoder
  pinMode(encoderPinA, INPUT_PULLUP);
  pinMode(encoderPinB, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(encoderPinA), countPulse, RISING);
  
  lastTime = millis();
}

void loop() {
  currentTime = millis();
  if ((currentTime - lastTime) >= timeInterval) {
    float rpm = (pulseCount / 400.0) * 60.0 / (timeInterval / 1000.0); // Assuming 20 counts per revolution
    Serial.print("Speed (RPM): ");
    Serial.println(rpm);

    // Send data over Bluetooth
    String rpmString = String(rpm, 2); // Format RPM to 2 decimal places
    pCharacteristic->setValue(rpmString.c_str());
    pCharacteristic->notify();

    pulseCount = 0;
    lastTime = currentTime;
  }
}

void countPulse() {
  pulseCount++;
}
