#include "pin_map.h"
#include "game_task.h"
#include <TimerOne.h>

// Tasks
game_task GT1;

void setup() {
  // Serial monitor
  Serial.begin(9600);
 
  // Set the pin modes
  pinMode(PIN_READY,OUTPUT);
  pinMode(PIN_TRIGGER_1,INPUT);
  pinMode(PIN_TRIGGER_2,INPUT);
  pinMode(PIN_TRIGGER_3,INPUT);
  pinMode(PIN_TRIGGER_4,INPUT);
  pinMode(PIN_BUTTON,INPUT);
  pinMode(PIN_SWITCH,INPUT);
  pinMode(PIN_BUZZER,OUTPUT);
  pinMode(PIN_IO_0,OUTPUT);
  pinMode(PIN_IO_1,OUTPUT);
  pinMode(PIN_IO_2,OUTPUT);
  pinMode(PIN_IO_3,OUTPUT);
  pinMode(PIN_LED_0,OUTPUT);
  pinMode(PIN_LED_1,OUTPUT);
  pinMode(PIN_LED_2,OUTPUT);
  pinMode(PIN_LED_3,OUTPUT);
  pinMode(PIN_LED_4,OUTPUT);
  pinMode(PIN_LED_5,OUTPUT);
  pinMode(PIN_LED_6,OUTPUT);
  pinMode(PIN_LED_7,OUTPUT);

  // Random init
  randomSeed(analogRead(0));

  // Interupt Timer
  Timer1.initialize(10000);
  Timer1.attachInterrupt(tick_function); // blinkLED to run every 0.15 seconds
}

void loop() {
}

void tick_function(){
  GT1.tick();
}
