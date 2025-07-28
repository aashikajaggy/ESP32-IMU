
#include <Arduino.h>
#include <Wire.h>

void setup() {
  Wire.begin(15, 4); // initializing the I2C interfacing, starting communication at the GPIO pins 15 and 4
  Serial.begin(115200);

}

void loop() {
  byte error, address;
  int numDevicesFound = 0;
  for(address = 1; address < 127; address++) {
    Wire.beginTransmission(address); // start reading from that 7 bit address
    error = Wire.endTransmission(); // check for errors in the scanning 
    if(error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address < 16) Serial.print("0");
      Serial.print(address, HEX);
      numDevicesFound++;
    }
  }
  if (numDevicesFound == 0) Serial.println("No I2C devices found\n");
  delay(5000);
}
