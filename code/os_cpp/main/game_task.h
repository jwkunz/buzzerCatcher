#pragma once
#include "arduino.h"
#include "pin_map.h"
#include "task_interface.h"
#include "pin_debouncer.h"

class GameTask : public TaskInterface {
private:
  typedef enum {
    POWER_UP,
    WAIT_ARM_0,
    WAIT_ARM_1,
    WAIT_TRIGGER_0,
    WAIT_TRIGGER_1,
    WAIT_CLEAR_0,
    WAIT_CLEAR_1,
  } GameStateType;

  GameStateType current_state = POWER_UP;

  const uint8_t LED_OFF = 0x00;
  const uint8_t LED_ON = 0xFF;
  const uint8_t LED_USER_0 = PIN_LED_0;
  const uint8_t LED_USER_1 = PIN_LED_1;
  const uint8_t LED_USER_2 = PIN_LED_2;
  const uint8_t LED_USER_3 = PIN_LED_3;
  const uint8_t LED_WAIT_ARM = PIN_LED_4;
  const uint8_t LED_WAIT_TRIGGER = PIN_LED_5;
  const uint8_t LED_WAIT_CLEAR = PIN_LED_6;

  const MicrosTrackType BUZZER_FREQ_HZ = 1000;
  const MicrosTrackType BUZZER_HALF_PERIOD_MICROS = 1E6 / BUZZER_FREQ_HZ / 2;
  MicrosTrackType buzz_time_tracker;
  bool buzzer_value;
  bool responses[4];
  uint8_t responded = 4;

  void display_responded() {
    digitalWrite(LED_USER_0, (0 == responded));
    digitalWrite(LED_USER_1, (1 == responded));
    digitalWrite(LED_USER_2, (2 == responded));
    digitalWrite(LED_USER_3, (3 == responded));
  }

public:
  typedef struct
  {
    PinDebouncer *pin_button;
    PinDebouncer *pin_switch;
    PinDebouncer *trigger_0;
    PinDebouncer *trigger_1;
    PinDebouncer *trigger_2;
    PinDebouncer *trigger_3;
  } TickArgsType;

  void tick(MicrosTrackType micros_since_last_call,
       void *args) {
    // Default to stay in current state
    GameStateType next_state = current_state;

    // Game state machine
    switch (current_state) {

      // Initialization
      case POWER_UP:
        {
          // Set all IO low
          digitalWrite(PIN_READY, LOW);
          digitalWrite(PIN_BUZZER, LOW);
          analogWrite(PIN_LED_0, LED_OFF);
          analogWrite(PIN_LED_1, LED_OFF);
          analogWrite(PIN_LED_2, LED_OFF);
          analogWrite(PIN_LED_3, LED_OFF);
          analogWrite(PIN_LED_4, LED_OFF);
          analogWrite(PIN_LED_5, LED_OFF);
          analogWrite(PIN_LED_6, LED_OFF);
          analogWrite(PIN_LED_7, LED_OFF);
          next_state = WAIT_ARM_0;
          break;
        }

      // Wait for the button to go high
      case WAIT_ARM_0:
        {
          analogWrite(LED_WAIT_ARM, LED_ON);  // Signal state
          // Check button
          bool val = ((TickArgsType *)args)->pin_button->get_current_value();
          if (val) {
            next_state = WAIT_ARM_1;
          }
          break;
        }

      // Wait for the button to go low
      case WAIT_ARM_1:
        {
          analogWrite(LED_WAIT_ARM, LED_ON);  // Signal state
          // Check button
          bool val = ((TickArgsType *)args)->pin_button->get_current_value();
          if (!val) {
            analogWrite(LED_WAIT_ARM, LED_OFF);
            next_state = WAIT_TRIGGER_0;
            buzz_time_tracker = 0;
            buzzer_value = false;
          }
          break;
        }
      // Wait for a trigger to go high and arbitrate first
      case WAIT_TRIGGER_0:
        {
          analogWrite(LED_WAIT_TRIGGER, LED_ON);  // Signal state

          // Look for a response in rapid succession
          // NOTE: Fair arbitration implemented below
          bool responded = this->get_responses((TickArgsType *)args);

          // Someone responded
          if (responded) {
            // Going to next state
            next_state = WAIT_TRIGGER_1;

            // Arbitrate the responses using a random score system

            // This is used in the rare event of multiple responses within a tick
            // Everyone who responded within this tick gets a random score > 0
            // The winner is the person with highest score (who responded)

            uint8_t best_score = 0;  // Get best
            for (uint8_t i = 0; i < 4; ++i) {
              if (this->responses[i]) {
                uint8_t score = random(0xff);
                if (score > best_score) {
                  best_score = score;
                  this->responded = i;  // Updated
                }
              }
            }
          }
          break;
        }
      // Wait for a trigger to go low and drive the buzzer
      case WAIT_TRIGGER_1:
        {
          analogWrite(LED_WAIT_TRIGGER, LED_ON);  // Signal state
          this->display_responded();              // Signal the owner

          // Run buzzer while the trigger is still high
          buzz_time_tracker += micros_since_last_call;
          if (buzz_time_tracker > BUZZER_HALF_PERIOD_MICROS) {
            buzz_time_tracker -= BUZZER_HALF_PERIOD_MICROS;
            digitalWrite(PIN_BUZZER, buzzer_value);
            buzzer_value != buzzer_value;
          }

          // Look for a response is still on
          bool responded = this->get_responses((TickArgsType *)args);

          // Check if all the triggers are low
          if (!responded) {
            analogWrite(LED_WAIT_TRIGGER, LED_OFF);
            next_state = WAIT_CLEAR_0;
          }
          break;
        }

      // Wait for the clear to go high
      case WAIT_CLEAR_0:
        {
          analogWrite(LED_WAIT_CLEAR, LED_ON);  // Signal state
          this->display_responded();            // Signal the owner

          // Check the button state
          bool val = ((TickArgsType *)args)->pin_button->get_current_value();
          if (val) {
            next_state = WAIT_CLEAR_1;
          }
          break;
        }
      // Wait for the clear to go low
      case WAIT_CLEAR_1:
        {
          analogWrite(LED_WAIT_CLEAR, LED_ON);  // Signal state
          this->display_responded();            // Signal the owner

          // Check the button
          bool val = ((TickArgsType *)args)->pin_button->get_current_value();
          if (!val) {
            analogWrite(LED_WAIT_CLEAR, LED_OFF);
            next_state = WAIT_ARM_0;
          }
          break;
        }

      // Should not occur
      default:
        {
          // ERROR CASE
          next_state = POWER_UP;
          this->error("Ended up in the default case. This should not happen");
          break;
        }
    }
    current_state = next_state;
  }

private:
  bool get_responses(TickArgsType *io_objects) {
    this->responses[0] = io_objects->trigger_0->get_current_value();
    this->responses[1] = io_objects->trigger_1->get_current_value();
    this->responses[2] = io_objects->trigger_2->get_current_value();
    this->responses[3] = io_objects->trigger_3->get_current_value();
    // reduction or
    for (uint8_t i = 0; i < 4; ++i) {
      if (this->responses[i]) {
        return true;
      }
    }
    return false;
  }
};