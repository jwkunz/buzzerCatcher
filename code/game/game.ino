/*
    This file is the top level function for the arduino framework to initialize everything and start the operating system

    Copyright January 2025 Dr. Jakob W. Kunzler
    This file is licensed under the terms of the MIT LICENSE
*/

#define DEBUG_MODE

#include "arduino.h"
#include "pin_map.h"
#include "task_interface.h"
#include "game_task.h"
#include "round_robin_os.h"
#include "loop_speedometer.h"

// Determines how long is a stable value on a pin
static const uint32_t micros_for_stable_io = 100000;

// OS
RoundRobinOS<8> RRos;

// Tasks
GameTask GameTasker;
PinDebouncer ButtonDebouncer;
PinDebouncer SwitchDebouncer;
PinDebouncer Trigger0Debouncer;
PinDebouncer Trigger1Debouncer;
PinDebouncer Trigger2Debouncer;
PinDebouncer Trigger3Debouncer;

#ifdef DEBUG_MODE
LoopSpeedometer LoopSpeedometerObj;
#endif

// Args
GameTask::TickArgsType GameTaskArgs;

void error_handler(const char *message)
{
  Serial.print("ERROR->");
  Serial.println(message);
  while (true)
  {
  };
}

void print_handler(const char *message)
{
  Serial.print("PRINT->");
  Serial.println(message);
}

void setup()
{
  // Serial monitor
  Serial.begin(115200);

  // Set the pin modes
  pinMode(PIN_READY, OUTPUT);
  pinMode(PIN_TRIGGER_1, INPUT);
  pinMode(PIN_TRIGGER_2, INPUT);
  pinMode(PIN_TRIGGER_3, INPUT);
  pinMode(PIN_TRIGGER_4, INPUT);
  pinMode(PIN_BUTTON, INPUT);
  pinMode(PIN_SWITCH, INPUT);
  pinMode(PIN_BUZZER, OUTPUT);
  pinMode(PIN_IO_0, OUTPUT);
  pinMode(PIN_IO_1, OUTPUT);
  pinMode(PIN_IO_2, OUTPUT);
  pinMode(PIN_IO_3, OUTPUT);
  pinMode(PIN_LED_0, OUTPUT);
  pinMode(PIN_LED_1, OUTPUT);
  pinMode(PIN_LED_2, OUTPUT);
  pinMode(PIN_LED_3, OUTPUT);
  pinMode(PIN_LED_4, OUTPUT);
  pinMode(PIN_LED_5, OUTPUT);
  pinMode(PIN_LED_6, OUTPUT);
  pinMode(PIN_LED_7, OUTPUT);

  // Random init
  randomSeed(analogRead(0));

  // Setup Tasks
  ButtonDebouncer.set(PIN_BUTTON, micros_for_stable_io);

  SwitchDebouncer.set(PIN_SWITCH, micros_for_stable_io);
  Trigger0Debouncer.set(PIN_TRIGGER_1, micros_for_stable_io);
  Trigger1Debouncer.set(PIN_TRIGGER_2, micros_for_stable_io);
  Trigger2Debouncer.set(PIN_TRIGGER_3, micros_for_stable_io);
  Trigger3Debouncer.set(PIN_TRIGGER_4, micros_for_stable_io);

#ifdef DEBUG_MODE
  // Give the tasks human readable names
  ButtonDebouncer.set_name("Button Debouncer");
  SwitchDebouncer.set_name("Switch Debouncer");
  Trigger0Debouncer.set_name("Trigger 0 Debouncer");
  Trigger1Debouncer.set_name("Trigger 1 Debouncer");
  Trigger2Debouncer.set_name("Trigger 2 Debouncer");
  Trigger3Debouncer.set_name("Trigger 3 Debouncer");
  GameTasker.set_name("Game Tasker");

  LoopSpeedometerObj.set_name("Loop Count Printer");
#endif

  // Call the game task with the required IO args
  GameTask::TickArgsType GameTaskArgs = {
      &ButtonDebouncer,
      &SwitchDebouncer,
      &Trigger0Debouncer,
      &Trigger1Debouncer,
      &Trigger2Debouncer,
      &Trigger3Debouncer};

  // Load up the scheduler
  RRos.push_task(&ButtonDebouncer, NULL);
  RRos.push_task(&SwitchDebouncer, NULL);
  RRos.push_task(&Trigger0Debouncer, NULL);
  RRos.push_task(&Trigger1Debouncer, NULL);
  RRos.push_task(&Trigger2Debouncer, NULL);
  RRos.push_task(&Trigger3Debouncer, NULL);
  RRos.push_task(&GameTasker, (void *)&GameTaskArgs);

#ifdef DEBUG_MODE
  // /Count the loop speed
  RRos.push_task(&LoopSpeedometerObj, NULL);
  print_handler("Finished hardware initialization");
#endif

  // Printing
  RRos.set_print_handler_functions(&print_handler);
  // Errors
  RRos.set_error_handler_functions(&error_handler);
}

void loop()
{
  // Round Robin scheduling
  RRos.tick();
}
