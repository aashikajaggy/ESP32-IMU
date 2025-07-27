#include <Arduino.h>
#include <Wire.h>

const int MPU = 0x68; // address of MPU device 
void setup() {
  Serial.begin(115200);
  Wire.begin(15, 4);
  Wire.beginTransmission(MPU);
  Wire.write(0x6B); // device reset register
  Wire.write(0x00); // the reset value is 0x00 for all registers except registers 107 and 117
  Wire.endTransmission(true);


}

void loop() {
}

