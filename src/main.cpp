
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

void stopPWM();
void master_send_address(uint8_t address);  // forward declaration
void IRAM_ATTR handleRisingEdge();

void setup() {
  // Setting up serial line baud rate
  Serial.begin(115200);

  // start the clock on high
  //pinMode(mySCL, OUTPUT);
  //digitalWrite(mySCL, HIGH); 

  //count the highs of pwm
  pinMode(pin4, INPUT);


  // Using the esp32 pwn library to create a 50% duty cycle pwn with a 100khz frequency. Resolution of 8 means that the resolution granularity is 256. 
  ledcSetup(pwmChannel, freq, resolution);

  attachInterrupt(pin4, handleRisingEdge, RISING);



  // signal the start condition
  pinMode(mySDA, OUTPUT);
  digitalWrite(mySDA, LOW);
  delayMicroseconds(5);

  // configure the timer
  //timer = timerBegin(0, 80, true);
  //timerAttachInterrupt(timer, &stopPWM, true);
}

void loop() {
  if(stoppingConditionReached) {
    ledcDetachPin(mySCL);
    pinMode(mySCL, OUTPUT);
    digitalWrite(mySCL, HIGH); 
    countPWMCycles = 0;
    pwmStarted = false;
    stoppingConditionReached = false;
    Serial.println("caught the stopping condition");
    //delay(1000);
  }

  if(!pwmStarted) {
    ledcAttachPin(mySCL, pwmChannel);
    ledcWrite(pwmChannel, dutyCycle);
    pwmStarted = true;
  }

  // we want one iteration of loop to be a complete iteration of 9 SCL cycles 
  /*
  if(!pwmStarted) {
    ledcAttachPin(mySCL, pwmChannel);
    ledcWrite(pwmChannel, dutyCycle);
    timerAlarmWrite(timer, 90, false);
    timerAlarmEnable(timer);
    pwmStarted = true;
  }

  if(!cycleDone) {

  }
  
  if(cycleDone) {
    delay(1000);
    cycleDone = false;
    pwmStarted = false;
  }
    */
}

void master_send_address(uint8_t address) {
}

void IRAM_ATTR handleRisingEdge() {
  countPWMCycles++;
  if(countPWMCycles==9) {
    ledcWrite(pwmChannel, 0);
    stoppingConditionReached = true;
  }
}
