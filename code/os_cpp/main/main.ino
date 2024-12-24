#include "pin_map.h"
#include "task_interface.h"
#include "game_task.h"

static const uint32_t micros_for_stable_io = 100000;
uint64_t last_time;

// Tasks
GameTask GameTask1;
PinDebouncer ButtonDebouncer;
PinDebouncer SwitchDebouncer;
PinDebouncer Trigger0Debouncer;
PinDebouncer Trigger1Debouncer;
PinDebouncer Trigger2Debouncer;
PinDebouncer Trigger3Debouncer;

void setup()
{
  // Serial monitor
  Serial.begin(9600);

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

  uint64_t last_time = micros();
}

void loop()
{
  // Track loop timing
  uint64_t current_time = micros();
  uint32_t loop_time = current_time - last_time;
  last_time = current_time;

  // Round Robin scheduling

  // Do IO debouncers first
  ButtonDebouncer.tick(loop_time, NULL);
  SwitchDebouncer.tick(loop_time, NULL);
  Trigger0Debouncer.tick(loop_time, NULL);
  Trigger1Debouncer.tick(loop_time, NULL);
  Trigger2Debouncer.tick(loop_time, NULL);
  Trigger3Debouncer.tick(loop_time, NULL);

  // Call the game task with the required IO args
  GameTask::TickArgsType GameTaskArgs = {
      &ButtonDebouncer,
      &SwitchDebouncer,
      &Trigger0Debouncer,
      &Trigger1Debouncer,
      &Trigger2Debouncer,
      &Trigger3Debouncer};
  GameTask1.tick(loop_time, (void *)&GameTaskArgs);
}
