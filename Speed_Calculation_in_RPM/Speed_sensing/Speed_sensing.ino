volatile int pulseCount = 0;
unsigned long lastTime;
unsigned long currentTime;
unsigned long timeInterval = 1000; // Time interval in milliseconds

const int encoderPinA = 26; // Example GPIO pin for encoder channel A
const int encoderPinB = 14; // Example GPIO pin for encoder channel B

void setup() {
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
    pulseCount = 0;
    lastTime = currentTime;
  }
}

void countPulse() {
  pulseCount++;
}
