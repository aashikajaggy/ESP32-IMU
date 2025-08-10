
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

volatile bool pwmStarted = false;
int myAddressArray[8] = {0,1,1,0,1,0,0,0}; 

void stopPWM();
void master_send_address();  // forward declaration
void IRAM_ATTR handleRisingEdge();
void handleLowLevel();

void setup() {
  // Setting up serial line baud rate
  Serial.begin(115200);

  // Routing the PWM to GPIO 4
  pinMode(pin4, INPUT);

  // Using the esp32 pwn library to create a 50% duty cycle pwn with a 100khz frequency. Resolution of 8 means that the resolution granularity is 256. 
  ledcSetup(pwmChannel, freq, resolution);
\
  // immediately runs this function if the ESP32 CPU is running instructions inside of loop
  attachInterrupt(pin4, handleRisingEdge, RISING);

}

void loop() {
  if(stoppingConditionReached) {
    ledcDetachPin(mySCL);
    // default SCL to high
    pinMode(mySCL, OUTPUT);
    digitalWrite(mySCL, HIGH); 
    // signal the start condition
    pinMode(mySDA, OUTPUT);
    digitalWrite(mySDA, LOW);
    delayMicroseconds(5);
    countPWMCycles = 0;
    pwmStarted = false;
    stoppingConditionReached = false;
    // little buffer before the pwm starts
    delayMicroseconds(5);
  }

  // only initialize the pwm once
  if(!pwmStarted) {
    ledcAttachPin(mySCL, pwmChannel);
    ledcWrite(pwmChannel, dutyCycle);
    pwmStarted = true;
  }
}

void master_send_address() {
  int myAddressArray[8] = {0,1,1,0,1,0,0,0}; 
  if(myAddressArray[countPWMCycles-1]==1){
    digitalWrite(mySDA, HIGH);
  } else {
    digitalWrite(mySDA, LOW);
  }
  
}

// interrupt function
void IRAM_ATTR handleRisingEdge() {
  // count the number of pwm cycles
  countPWMCycles++;
  if(countPWMCycles==9) {
    pinMode(mySDA, INPUT); // release the SDA line
    // immediately stop the pwm
    ledcWrite(pwmChannel, 0);
    stoppingConditionReached = true;
  } else if (countPWMCycles <= 7) {
    if(myAddressArray[countPWMCycles-1]==1){
      GPIO.out_w1ts = (1 << mySDA);
    } else {
      GPIO.out_w1tc = (1 << mySDA);
    }
  } else if (countPWMCycles == 8) {
    GPIO.out_w1tc = (1 << mySDA);
  }
}
