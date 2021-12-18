#include <Arduino.h>
#include <Wire.h>

// keep in mind that, only pins 2 and 3 can be used for interrupt.
#define BUTTON1 2 // + 
#define DEBOUNCE_TIME_MS 5

long counter;
volatile bool add_on;
volatile long last_interrupt_time;

int debounce(long current_time){
  if (current_time - last_interrupt_time > DEBOUNCE_TIME_MS){
    last_interrupt_time = current_time;
    return 1;
  }
  return 0;
}

void add(){
  long current_time = millis();
  if (debounce(current_time)){
    counter += 5; 
    add_on = false;
  }
}

void handleInterrupt(){
  add_on = true;
}

void setup() {
  counter = 0;
  add_on = false;

  pinMode(BUTTON1, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTON1), handleInterrupt, FALLING);

  Serial.begin(9600);
  Serial.println("*** serial init ***");
}

void loop() {
  if(add_on){
    add();
    Serial.println(counter);
  }
  delay(100);
}