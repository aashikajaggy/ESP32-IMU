
#include <Arduino.h>
#include <cstdint>

const int mySDA = 21;
const int mySCL = 22;
const int pin4 = 4;
const int freq = 100000;
const int dutyCycle = 128;
const int resolution = 8;
int pwmChannel = 0;
volatile int countPWMCycles = 0;
volatile bool stoppingConditionReached = false;
bool starting = true;
int myAddressArray[8] = {0,1,1,0,1,0,0,0}; 
void IRAM_ATTR handleRisingEdge();

void setup() {
  // Setting up serial line baud rate
  Serial.begin(115200);
  pinMode(pin4, INPUT);


  // Using the esp32 pwn library to create a 50% duty cycle pwn with a 100khz frequency. Resolution of 8 means that the resolution granularity is 256. 
  ledcSetup(pwmChannel, freq, resolution);

  attachInterrupt(pin4, handleRisingEdge, RISING);

}

void loop() {
  if(stoppingConditionReached || starting) {
    ledcDetachPin(mySCL);
    pinMode(mySCL, OUTPUT);
    pinMode(mySDA, OUTPUT);
    GPIO.out_w1ts = (1 << mySCL);
    // signal the start condition by pulling SDA low 
    GPIO.out_w1tc = (1 << mySDA);
    delayMicroseconds(0.5);
    ledcAttachPin(mySCL, pwmChannel);
    ledcWrite(pwmChannel, dutyCycle);
    GPIO.out_w1ts = (1 << mySDA); // pulling SDA back to high
    countPWMCycles = 0;
    stoppingConditionReached = false;
    starting = false;
  }

}

void IRAM_ATTR handleRisingEdge() {
    // count the number of pwm cycles
    countPWMCycles++;
    if(countPWMCycles==9) {
      pinMode(mySDA, INPUT); // release the SDA line
      // immediately stop the pwm
      ledcWrite(pwmChannel, 0);
      stoppingConditionReached = true;
    }
}