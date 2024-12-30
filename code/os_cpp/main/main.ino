#include "pin_map.h"
#include "task_interface.h"
#include "game_task.h"
#include "round_robin_os.h"
#include "loop_count_print.h"

static const uint32_t micros_for_stable_io = 100000;

// OS
RoundRobinOS rros;

// Tasks
GameTask GameTasker;
PinDebouncer ButtonDebouncer;
PinDebouncer SwitchDebouncer;
PinDebouncer Trigger0Debouncer;
PinDebouncer Trigger1Debouncer;
PinDebouncer Trigger2Debouncer;
PinDebouncer Trigger3Debouncer;
LoopCountPrint LoopCountPrinter;

// Args
GameTask::TickArgsType GameTaskArgs;

void error_handler(String &message) {
  Serial.println("Error: " + message);
  while (true) {};
}

void print_handler(String &message) {
  Serial.println(message);
}

void setup() {
  // Serial monitor
  Serial.begin(250000);

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

  ButtonDebouncer.set_name("Button Debouncer");
  SwitchDebouncer.set_name("Switch Debouncer");
  Trigger0Debouncer.set_name("Trigger 0 Debouncer");
  Trigger1Debouncer.set_name("Trigger 1 Debouncer");
  Trigger2Debouncer.set_name("Trigger 2 Debouncer");
  Trigger3Debouncer.set_name("Trigger 3 Debouncer");
  GameTasker.set_name("Game Tasker");
  LoopCountPrinter.set_name("Loop Count Printer");

  // Call the game task with the required IO args
  GameTask::TickArgsType GameTaskArgs = {
    &ButtonDebouncer,
    &SwitchDebouncer,
    &Trigger0Debouncer,
    &Trigger1Debouncer,
    &Trigger2Debouncer,
    &Trigger3Debouncer
  };

  // Load up the scheduler
  rros.push_task(&ButtonDebouncer, NULL);
  rros.push_task(&SwitchDebouncer, NULL);
  rros.push_task(&Trigger0Debouncer, NULL);
  rros.push_task(&Trigger1Debouncer, NULL);
  rros.push_task(&Trigger2Debouncer, NULL);
  rros.push_task(&Trigger3Debouncer, NULL);
  rros.push_task(&GameTasker, (void *)&GameTaskArgs);
  rros.push_task(&LoopCountPrinter, NULL);

  rros.set_error_handler_functions(&error_handler);
  rros.set_print_handler_functions(&print_handler);
}

void loop() {
  // Round Robin scheduling
  rros.tick();
}
