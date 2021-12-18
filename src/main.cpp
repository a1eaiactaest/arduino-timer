#include <Arduino.h>
#include <Wire.h>

// keep in mind that, only pins 2 and 3 can be used for interrupt.
#define INTERRUPT_PIN 2 // + time 
#define ADD_BUTTON 8
#define SUB_BUTTON 9
#define START_BUTTON 10
#define DEBOUNCE_TIME_MS 5

long counter;
long current_time;
volatile bool add_on;
volatile bool sub_on;
volatile long last_interrupt_time;

int debounce(long current_time){
  if (current_time - last_interrupt_time > DEBOUNCE_TIME_MS){
    last_interrupt_time = current_time;
    return 1;
  }
  return 0;
}

void add(long current_time){
  if (debounce(current_time)){
    counter += 5; 
    Serial.println(counter);
    add_on = false;
  }
}

void sub(long current_time){
  if (debounce(current_time)){
      if (counter-5 >= 0){
        counter -= 5;
        Serial.println(counter);
      } else {
        Serial.println("Counter is too small to subtract, try increasing it's value instead.");
      }
      sub_on = false;
  }
}

void handleInterrupt(){
  if (digitalRead(ADD_BUTTON) == LOW){
    add_on = true;
  }

  else if (digitalRead(SUB_BUTTON) == LOW){
    sub_on = true;
  }

}

void setup() {
  counter = 0;
  add_on = false;
  sub_on = false;

  pinMode(INTERRUPT_PIN, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), handleInterrupt, FALLING);

  Serial.begin(9600);
  Serial.println("*** serial init ***");
}

void loop() {
  if(add_on){
    current_time = millis();
    add(current_time);
  }

  else if(sub_on){
    current_time = millis();
    sub(current_time);
  }
  delay(100);
}
