#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <SimpleBLE.h>
#include <esp_mac.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
SimpleBLE ble;

#define SERVOMIN  150 
#define SERVOMAX  600 
#define SERVO_FREQ 60 

void setup() {
  Serial.begin(115200);
  delay(500);

  ble.begin("MoccaVR");

  uint8_t mac[6];
  esp_read_mac(mac, ESP_MAC_BT);
  Serial.printf("Device Name: MoccaVR\n");
  Serial.printf("BT MAC Address: %02X:%02X:%02X:%02X:%02X:%02X\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

  Wire.begin(21, 22); 
  
  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(SERVO_FREQ);

  for(int i = 0; i < 5; i++) {
    pwm.setPWM(i, 0, angleToPulse(90));
  }
  delay(1000);
}

int angleToPulse(int angle) {
  return map(angle, 0, 180, SERVOMIN, SERVOMAX);
}

void triggerVibration(int channel, int duration_ms, int intensity) {
  int centerAngle = 90;
  int targetTime = millis() + duration_ms;
  
  while(millis() < targetTime) {
    pwm.setPWM(channel, 0, angleToPulse(centerAngle - intensity));
    delay(20); 
    pwm.setPWM(channel, 0, angleToPulse(centerAngle + intensity));
    delay(20);
  }
  
  pwm.setPWM(channel, 0, angleToPulse(centerAngle));
}

void loop() {
  triggerVibration(0, 1000, 10); 
  delay(1000);

  int targetTime = millis() + 500;
  while(millis() < targetTime) {
    for(int i = 0; i < 5; i++) pwm.setPWM(i, 0, angleToPulse(82));
    delay(25);
    for(int i = 0; i < 5; i++) pwm.setPWM(i, 0, angleToPulse(98));
    delay(25);
  }
  
  for(int i = 0; i < 5; i++) pwm.setPWM(i, 0, angleToPulse(90));
  delay(3000); 
}"# MoccaVR" 
