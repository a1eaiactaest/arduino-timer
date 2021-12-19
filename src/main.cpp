#include <Arduino.h>
#include <Wire.h>

// keep in mind that, only pins 2 and 3 can be used for interrupt.
#define INTERRUPT_PIN 2 // + time 
#define ADD_BUTTON 5
#define SUB_BUTTON 6
#define START_BUTTON 7
#define DEBOUNCE_TIME_MS 5

long counter;
long current_time;
volatile bool add_on;
volatile bool sub_on;
volatile bool timer_on;
volatile long last_interrupt_time;

//int hold_threshold = 250; // in miliseconds

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

void start_timer(long current_time){
  if (debounce(current_time)){
    if (counter > 0){
      Serial.println("Starting timer");
      long counter_tmp = counter; //save counter state
      while (counter_tmp > 0){
        Serial.println(counter_tmp);
        counter_tmp--;
        delay(1000);
      }
      Serial.println("Timer finished");
      Serial.println(counter);
    } else {
      Serial.println("Counter is equal to 0, increase `counter` value.");
    }

    timer_on = false;
  }
}

void handleInterrupt(){
  if (!timer_on){

    if (digitalRead(ADD_BUTTON) == LOW){
      add_on = true;
    }

    else if (digitalRead(SUB_BUTTON) == LOW){
      sub_on = true;
    }

    else if (digitalRead(START_BUTTON) == LOW){
      timer_on = true;
    }

    else {
      return;
    }
  }
}
void setup() {
  counter = 0;
  add_on = false;
  sub_on = false;
  timer_on = false;

  pinMode(INTERRUPT_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), handleInterrupt, FALLING);

  Serial.begin(9600);
  Serial.println("*** serial init ***");
  Serial.println(counter);
}

void loop() {
  current_time = millis();

  if (add_on){
    add(current_time);
  }

  else if (sub_on){
    sub(current_time);
  }

  else if (timer_on){
    start_timer(current_time);
  }

  delay(500);
}
