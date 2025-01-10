/*
    The game task contains the body of logic for carrying out the game.

    Copyright January 2025 Dr. Jakob W. Kunzler
    This file is licensed under the terms of the MIT LICENSE
*/
#pragma once
#include "arduino.h"
#include "pin_map.h"
#include "task_interface.h"
#include "pin_debouncer.h"

// The game task inherit from the task interface
class GameTask : public TaskInterface {

  // private members are meant to not be used externally
private:

  // This are the states of the game
  typedef enum {
    POWER_UP,        // Start up sequence
    WAIT_ARM_0,      // The default idle and wait for a button push
    WAIT_ARM_1,      // Wait for the button to go low after push
    WAIT_TRIGGER_0,  // Waiting for either a response trigger or button clear
    WAIT_TRIGGER_1,  // Waiting for trigger to go low while buzzing
    WAIT_CLEAR_0,    // Wait for the button clear to go high
    WAIT_CLEAR_1     // Wait for the button clear to go low
  } GameStateType;

  // Start in the POWER_UP sequence
  GameStateType current_state = POWER_UP;

  // LED brightness for users set here
  const uint8_t LED_OFF = 0x00;
  const uint8_t LED_ON = 0xFF;  // Maximum

  // Pin mappings for the game
  const uint8_t LED_AUX_0 = PIN_LED_0;
  const uint8_t LED_AUX_1 = PIN_LED_1;
  const uint8_t LED_WAIT_ARM = PIN_LED_2;
  const uint8_t LED_WAIT_TRIGGER = PIN_LED_3;
  const uint8_t LED_USER_0 = PIN_LED_4;
  const uint8_t LED_USER_1 = PIN_LED_5;
  const uint8_t LED_USER_2 = PIN_LED_6;
  const uint8_t LED_USER_3 = PIN_LED_7;

  // Set the buzzer frequency in Hz
  const MicrosTrackType BUZZER_FREQ_HZ = 2000;
  // Other variable associated with the buzzer state
  const MicrosTrackType BUZZER_HALF_PERIOD_MICROS = 1E6 / BUZZER_FREQ_HZ / 2;
  MicrosTrackType buzz_time_tracker;
  bool buzzer_value;

  // A log of the last responses
  static const uint8_t N_USERS = 4;
  bool responses[N_USERS];
  const uint8_t NO_RESPONSE = N_USERS;  // Special value indicates no user in log
  uint8_t last_response = NO_RESPONSE;

  // Thi function helps display the last response
  void display_last_response() {
    digitalWrite(LED_USER_0, (0 == last_response));
    digitalWrite(LED_USER_1, (1 == last_response));
    digitalWrite(LED_USER_2, (2 == last_response));
    digitalWrite(LED_USER_3, (3 == last_response));
  }

#ifdef DEBUG_MODE
  // This function maps a state to a human readable description
  String state_to_text(GameStateType s) {
    switch (s) {
      case POWER_UP:
        {
          return "POWER UP";
        }
      case WAIT_ARM_0:
        {
          return "WAITING TO ARM 0";
        }
      case WAIT_ARM_1:
        {
          return "WAITING TO ARM 1";
        }
      case WAIT_TRIGGER_0:
        {
          return "WAITING TO TRIGGER 0";
        }
      case WAIT_TRIGGER_1:
        {
          return "WAITING TO TRIGGER 1";
        }
      case WAIT_CLEAR_0:
        {
          return "WAITING TO CLEAR 0";
        }
      case WAIT_CLEAR_1:
        {
          return "WAITING TO CLEAR 1";
        }
      default:
        {
          this->error("This should never happen");
          return "";
        }
    }
  };
#endif

  // Public means useable externally
public:

  // This is the data field that will be passed into the task
  // It contains all the IO resources this task needs
  typedef struct
  {
    PinDebouncer *pin_button;
    PinDebouncer *pin_switch;
    PinDebouncer *trigger_0;
    PinDebouncer *trigger_1;
    PinDebouncer *trigger_2;
    PinDebouncer *trigger_3;
  } TickArgsType;

  // The main tick function gets called every cycle in the operating system
  void tick(MicrosTrackType micros_since_last_call,
            void *args) {

    // Always show last last_response
    this->display_last_response();

    // Default to stay in current state
    GameStateType next_state = current_state;

    // Game state machine
    switch (current_state) {

      // Initialization
      case POWER_UP:
        {
#ifdef DEBUG_MODE  // This only compiles if DEBUG_MODE is enabled
          this->print("Game is booting up...");
#endif
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
          // Ready to move to the waiting state
          next_state = WAIT_ARM_0;
          break;
        }

      // Wait for the button to go high
      case WAIT_ARM_0:
        {
          // Signal the current state
          analogWrite(LED_WAIT_ARM, LED_ON);

          // Check if button is HIGH
          bool val = ((TickArgsType *)args)->pin_button->get_current_value();
          if (val) {
#ifdef DEBUG_MODE
            this->print("Got button HIGH");
#endif
            // Go on to waiting for the button to go LOW
            next_state = WAIT_ARM_1;
          }
          break;
        }

      // Wait for the button to go low
      case WAIT_ARM_1:
        {
          // Signal the current state
          analogWrite(LED_WAIT_ARM, LED_OFF);
          // Set the last_response to nobody
          this->last_response = this->NO_RESPONSE;
          // Check if button is low
          bool val = ((TickArgsType *)args)->pin_button->get_current_value();
          if (!val) {
#ifdef DEBUG_MODE
            this->print("Got button LOW");
#endif
            // Signal state exit
            analogWrite(LED_WAIT_ARM, LED_OFF);
            // Inform contestants to respond
            digitalWrite(PIN_READY, HIGH);
            // Transition to wait for trigger
            next_state = WAIT_TRIGGER_0;
            // Init buzzer variables
            buzz_time_tracker = 0;
            buzzer_value = false;

            pinMode(PIN_TRIGGER_1, INPUT);
            pinMode(PIN_TRIGGER_2, INPUT);
            pinMode(PIN_TRIGGER_3, INPUT);
            pinMode(PIN_TRIGGER_4, INPUT);
          }
          break;
        }
      // Wait for a trigger to go high and arbitrate first
      case WAIT_TRIGGER_0:
        {
          // Signal the present state
          analogWrite(LED_WAIT_TRIGGER, LED_ON);

          // Look for a response in rapid succession
          // NOTE: Fair arbitration implemented below
          bool responded = this->get_responses((TickArgsType *)args);

          // Check if someone responded
          if (responded) {
#ifdef DEBUG_MODE
            this->print("Got at least one response");
#endif
            // Inform contestants can no longer respond
            digitalWrite(PIN_READY, LOW);

            // Arbitrate the responses using a random score system

            // This is used in the rare event of multiple responses within a tick
            // Everyone who last_response within this tick gets a random score > 0
            // The winner is the person with highest score (who last_response)

            uint8_t best_score = 0;  // Get best
            for (uint8_t i = 0; i < 4; ++i) {
              if (this->responses[i]) {
                uint8_t score = random(0xff);
                if (score > best_score) {
                  best_score = score;
                  this->last_response = i;  // Updated
#ifdef DEBUG_MODE
                  this->print("Chose response on trigger " + String(i + 1));
#endif
                }
              }
            }

            // Going to next state
            next_state = WAIT_TRIGGER_1;

          } else {
            // If no one last_response, check button for operator clearing
            bool val = ((TickArgsType *)args)->pin_button->get_current_value();
            if (val) {
              // No longer waiting on trigger
              analogWrite(LED_WAIT_TRIGGER, LED_OFF);
              // Inform contestants cannot respond
              digitalWrite(PIN_READY, LOW);
              // Going to clear
              next_state = WAIT_CLEAR_1;

#ifdef DEBUG_MODE
              this->print("Got clear button HIGH");
#endif
            }
          }
          break;
        }
      // Wait for a trigger to go low and drive the buzzer
      case WAIT_TRIGGER_1:
        {
          // Signal the current state
          analogWrite(LED_WAIT_TRIGGER, LED_ON);

          // Check switch for if sound is enabled
          bool val = ((TickArgsType *)args)->pin_switch->get_current_value();
          if (val) {
            // Run buzzer while the trigger is still high
            buzz_time_tracker += micros_since_last_call;
            if (buzz_time_tracker > BUZZER_HALF_PERIOD_MICROS) {
              buzz_time_tracker -= BUZZER_HALF_PERIOD_MICROS;
              digitalWrite(PIN_BUZZER, buzzer_value);
              buzzer_value = !buzzer_value;
            }
          }

          // Look for if a response is still on
          bool responded = this->get_responses((TickArgsType *)args);

          // Check if all the triggers are low and go back to waiting to arm
          if (!responded) {
            // Signal a change
            analogWrite(LED_WAIT_TRIGGER, LED_OFF);
            // Reverse polarity and drive that trigger high for the LED
            pinMode(PIN_TRIGGER_1 + this->last_response, OUTPUT);
            digitalWrite(PIN_TRIGGER_1 + this->last_response, HIGH);
            // Transition
            next_state = WAIT_CLEAR_0;

#ifdef DEBUG_MODE
            this->print("All responses have now cleared");
#endif
          }
          break;
        }
      // Wait for button to go high to clear
      case WAIT_CLEAR_0:
        {
          // Check button for operator to lift clear
          bool val = ((TickArgsType *)args)->pin_button->get_current_value();
          if (val) {
            // Reverse polarity back drive low agaom
            pinMode(PIN_TRIGGER_1 + this->last_response, INPUT);
            digitalWrite(PIN_TRIGGER_1 + this->last_response, LOW);
            // Going to arm state
            next_state = WAIT_CLEAR_1;
#ifdef DEBUG_MODE
            this->print("Clear button went HIGH");
#endif
          }
          break;
        }

      case WAIT_CLEAR_1:
        {
          // Check button for operator to lift clear
          bool val = ((TickArgsType *)args)->pin_button->get_current_value();
          if (!val) {
            // Display nobody again
            this->last_response = NO_RESPONSE;
            // Going to arm state
            next_state = WAIT_ARM_0;
#ifdef DEBUG_MODE
            this->print("Clear button went LOW");
#endif
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
#ifdef DEBUG_MODE
    if (next_state != current_state) {
      this->print("Game is transitioning from state " + this->state_to_text(current_state) + " to state " + this->state_to_text(next_state));
    }
#endif
    current_state = next_state;
  }

private:
  // Function to update the response table and idicate if at least one is high
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