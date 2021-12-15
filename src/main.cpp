#include <Arduino.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

#define BUTTON1 1 // + 
#define BUTTON2 2 // - 
#define DEBOUNCE_TIME_MS 5

LiquidCrystal_I2C lcd(0x27, 16, 2);

long counter;
volatile bool add_on;
volatile bool sub_on;
volatile long last_interrupt_time;

int debounce(long current_time){
  if (current_time - last_interrupt_time > DEBOUNCE_TIME_MS){
    return 1;
  }
  return 0;
}

void add(){
  long current_time = millis();
  if (debounce(current_time)){
    last_interrupt_time = current_time;

    counter += 5; 
    add_on = false;
  }
}

void handleInterrupt(){
  add_on = true;
}

void setup() {
  counter = 0;
  add_on = false; // add
  sub_on = false; // subtract

  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);

  pinMode(BUTTON1, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTON1), handleInterrupt, FALLING);
  attachInterrupt(digitalPinToInterrupt(BUTTON2), handleInterrupt, FALLING);

  Serial.begin(9600);
  Serial.println("init");
  Serial.println("init");
  Serial.println("init");
}

void loop() {
  if(add_on){
    add();
    Serial.println(counter);
  }
  delay(100);
}