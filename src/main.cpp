#include <Arduino.h>
#include <Wire.h>

// keep in mind that, only pins 2 and 3 can be used for interrupt.
#define INTERRUPT_PIN 2 // + time 
#define ADD_BUTTON 5
#define SUB_BUTTON 6
#define START_BUTTON 7
#define DEBOUNCE_TIME_MS 5

// RGB LED, pins below are PWM
#define RED_PIN 11
#define GREEN_PIN 10
#define BLUE_PIN 9

long counter;
long current_time;
volatile bool add_on;
volatile bool sub_on;
volatile bool timer_on;
volatile long last_interrupt_time;

//int hold_threshold = 250; // in miliseconds
String format_seconds(long seconds){
  char time[6];
  int min = seconds/60;
  int sec = seconds%60;
  if (min < 10 && sec < 10){
    snprintf(time, sizeof(time), "0%d:0%d", min, sec);
  } else if (min < 10){
    snprintf(time, sizeof(time), "0%d:%d", min, sec);
  } else if (sec < 10){
    snprintf(time, sizeof(time), "%d:0%d", min, sec);
  } else {
    snprintf(time, sizeof(time), "%d:%d", min, sec);
  }
  return time;
}

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
    add_on = false;
  }
}

void sub(long current_time){
  if (debounce(current_time)){
      if (counter-5 >= 0){
        counter -= 5;
      } else {
        Serial.println("Counter is too small to subtract, try increasing it's value instead.");
      }
      sub_on = false;
  }
}
void light_rgb(int r, int g, int b){
  analogWrite(RED_PIN, r);
  analogWrite(GREEN_PIN, g);
  analogWrite(BLUE_PIN, b);
}

void alarm(){
  int r = 255;
  int g = 0;
  int b = 0;
  for (int i = 0; i < 255; i++) {
    light_rgb(r, g, b);
    g++;
    delay(2);
  }
  for (int i = 0; i < 255; i++) {
    light_rgb(r, g, b);
    r--;
    delay(2);
  }
  for (int i = 0; i < 255; i++) {
    light_rgb(r, g, b);
    b++;
    delay(2);
  }
  for (int i = 0; i < 255; i++) {
    light_rgb(r, g, b);
    g--;
    delay(2);
  }
  for (int i = 0; i < 255; i++) {
    light_rgb(r, g, b);
    r++;
    delay(2);
  }
  for (int i = 0; i < 255; i++) {
    light_rgb(r, g, b);
    b--;
    delay(2);
  }
}

void countdown(int n){
  Serial.println("Starting timer");
  while (n > 0){
    Serial.println(format_seconds(n));
    n--;
    delay(1000);
  }
  Serial.println("Timer finished");
  Serial.println(format_seconds(counter));
  timer_on = false;
  while (!timer_on){
    alarm();
  }
}

void start_timer(long current_time){
  if (debounce(current_time)){
    if (counter > 0){
      countdown(counter);
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

  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  pinMode(INTERRUPT_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), handleInterrupt, FALLING);

  Serial.begin(9600);
  Serial.println("*** serial init ***");
  Serial.println(format_seconds(counter));
}

void loop() {
  current_time = millis();

  if (add_on){
    add(current_time);
    Serial.println(format_seconds(counter));
  }

  else if (sub_on){
    sub(current_time);
    Serial.println(format_seconds(counter));
  }

  else if (timer_on){
    start_timer(current_time);
  }
  delay(500);
}
