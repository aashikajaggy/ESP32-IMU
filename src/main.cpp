#include <Arduino.h>
#include <Wire.h>
#include <cstdint>

const int MPU = 0x68; // address of MPU device 
const int SDA = 21;
const int SCL = 22;

void setup() {
  pinMode(SDA, OUTPUT);
  pinMode(SCL, OUTPUT);
  digitalWrite(SDA, HIGH);
  digitalWrite(SCL, HIGH);
}

void loop() {

}

void master_send_address(uint8_t address) {
  // pull the sda line low 
  digitalWrite(SDA, LOW);
  // trying to make 100 microseconds clock
  delayMicroseconds(5);
  int curr_bit = 0;
  for(int i=0; i<8; i++){
    digitalWrite(SCL, LOW);
    curr_bit = (address >> i) & 1;
    if(curr_bit == 1) {
      digitalWrite(SDA, HIGH);
    } else {
      digitalWrite(SDA, LOW);
    }
    delayMicroseconds(5);
    digitalWrite(SCL, HIGH);
    // samples SDA
    delayMicroseconds(5);
  }
  // sends a write bit
  digitalWrite(SCL, LOW);
  digitalWrite(SDA, LOW);
  delayMicroseconds(5);
  digitalWrite(SCL, HIGH);

  //acknowledge bit
  digitalWrite(SCL, LOW);
  delayMicroseconds(5);
}
