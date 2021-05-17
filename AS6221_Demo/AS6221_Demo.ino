#include "AS6221.h"
#include <Wire.h>

#define INT_PIN 13 // Digital Pin 13

AS6221 temp;

void setup() {
  Wire.begin();
  Serial.begin(115200);
  
  // Setup Interrupt 
  pinMode(INT_PIN, INPUT_PULLUP);
  cli();
  PCICR |= (1 << PCIE0);   // Pin Change Interrupt Control Register -> Pin D13
  PCMSK0 |= (1 << PCINT5); // Pin Change Mask 0 -> Pin D13
  sei();
  
  // Sensor configuration
  as6221_config_t conf = {
    .cr = AS6221_CONV_RATE4,
    .state = AS6221_STATE_ACTIVE,
    .alert_mode = AS6221_ALERT_INTERRUPT,
    .alert_polarity = AS6221_ALERT_ACTIVE_LOW,
    .cf = AS6221_CONSEC_FAULTS2
  };
  
  temp.init(AS6221_DEFAULT_ADDRESS,conf);
  temp.setAlertLimits(27,29);
}

ISR(PCINT0_vect){
  // Catches both rising and falling edge of an interrupt
  // Accept only falling edge
  if(digitalRead(INT_PIN)==0){
      Serial.print("Interrupt");
      Serial.flush();
  }
}

void loop() {
  Serial.println("Default temperature reading:");
  printTwoSamples();
  printTwoSamples();
  Serial.println("AS6221 in sleep mode:");
  temp.sleepMode();
  printTwoSamples();
  Serial.println("Trigger single shot:");
  temp.triggerSingleShot();
  printTwoSamples();
  Serial.println("AS6221 back in countinous conversion mode:");
  temp.ccMode();
  printTwoSamples();
}

void printTwoSamples(){
  delay(1000);
  Serial.println(temp.getTemp(),7);
  delay(1000);
  Serial.println(temp.getTemp(),7);
  delay(1000);
}
