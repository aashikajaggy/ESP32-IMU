#include <Arduino.h>
#include <Wire.h>


float accx, accy, accz;
const int MPU = 0x68; // address of MPU device 
void setup() {
  Serial.begin(115200);
  Wire.begin(15, 4); // don't have to explicitly use the sda/scl pins on the esp32
  Wire.beginTransmission(MPU);
  Wire.write(0x6B); // device reset register
  Wire.write(0x00); // the reset value is 0x00, bit 7 is the device reset and we are assigning this bit to zero
  Wire.endTransmission(true); // START is enabled, SDA goes low while SCL is high. This is because I2C is an open drain protocol meaning that we a switch pulling the line down low 
  // when communication start 

}

void loop() {
  Wire.beginTransmission(MPU);
  Wire.write(0x3B); 
  Wire.endTransmission(false); // send another start bit to start the i2c communication 
  Wire.requestFrom(MPU, 6); // start reading 6 bytes
  accx = Wire.read() << 8 | Wire.read(); // combining the low byte and the high byte into one number
  // everytime we call Wire.read() we are reading a byte
  accx = accx / 16384.0; // for +- 2g sensitivity
  accy = Wire.read() << 8 | Wire.read();
  accy = accy / 16384.0; // for +- 2g sensitivity
  accz = Wire.read() << 8 | Wire.read();
  accz = accz / 16384.0; // for +- 2g sensitivity
  Serial.print("accx");
  Serial.println(accx);
  Serial.print("accy");
  Serial.println(accy);
  Serial.print("accz");
  Serial.println(accz);
}
