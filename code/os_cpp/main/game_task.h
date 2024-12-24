#pragma once
#include "arduino.h"
#include "pin_map.h"
#include "task_interface.h"
#include "pin_debouncer.h"

class GameTask : public TaskInterface
{
private:
    typedef enum
    {
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

    uint8_t responded = 4;

    void display_responded()
    {
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

    StatusCodeType
    tick(MicrosTrackType micros_since_last_call,
         void *args)
    {
        // Interpret the args
        TickArgsType io_objects = *(TickArgsType *)args;
        // Default to stay in current state
        GameStateType next_state = current_state;

        // Game state machine
        switch (current_state)
        {

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
            analogWrite(LED_WAIT_ARM, LED_ON); // Signal state
            // Check button
            bool val = io_objects.pin_button->get_current_value();
            if (val)
            {
                next_state = WAIT_ARM_1;
            }
            break;
        }

        // Wait for the button to go low
        case WAIT_ARM_1:
        {
            analogWrite(LED_WAIT_ARM, LED_ON); // Signal state
            // Check button
            bool val = io_objects.pin_button->get_current_value();
            if (!val)
            {
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
            analogWrite(LED_WAIT_TRIGGER, LED_ON); // Signal state

            // Look for a response in rapid succession
            // NOTE: Fair arbitration implemented below
            bool response_0 = io_objects.trigger_0->get_current_value();
            bool response_1 = io_objects.trigger_1->get_current_value();
            bool response_2 = io_objects.trigger_2->get_current_value();
            bool response_3 = io_objects.trigger_3->get_current_value();
            bool responded = response_0 | response_1 | response_2 | response_3;

            // Someone responded
            if (responded)
            {
                // Going to next state
                next_state = WAIT_TRIGGER_1;

                // Arbitrate the responses using a random score system

                // This is used in the rare event of multiple responses within a tick
                // Everyone who responded within this tick gets a random score > 0
                // The winner is the person with highest score (who responded)

                uint8_t scores[4];
                if (response_0)
                {
                    scores[0] = random(1, 0xff);
                }
                else
                {
                    scores[0] = 0;
                }
                if (response_1)
                {
                    scores[1] = random(1, 0xff);
                }
                else
                {
                    scores[1] = 0;
                }
                if (response_2)
                {
                    scores[2] = random(1, 0xff);
                }
                else
                {
                    scores[2] = 0;
                }
                if (response_3)
                {
                    scores[3] = random(1, 0xff);
                }
                else
                {
                    scores[3] = 0;
                }

                // Get best
                uint8_t best_score = 0;
                for (uint8_t i = 0; i < 4; ++i)
                {
                    if (scores[i] > best_score)
                    {
                        best_score = scores[i];
                        this->responded = i; // Updated
                    }
                }
            }
            break;
        }
        // Wait for a trigger to go low and drive the buzzer
        case WAIT_TRIGGER_1:
        {
            analogWrite(LED_WAIT_TRIGGER, LED_ON); // Signal state
            this->display_responded();             // Signal the owner

            // Run buzzer while the trigger is still high
            buzz_time_tracker += micros_since_last_call;
            if (buzz_time_tracker > BUZZER_HALF_PERIOD_MICROS)
            {
                buzz_time_tracker -= BUZZER_HALF_PERIOD_MICROS;
                digitalWrite(PIN_BUZZER, buzzer_value);
                buzzer_value != buzzer_value;
            }

            // Look for a response in rapid succession
            // NOTE: Fair arbitration implemented below
            bool response_0 = io_objects.trigger_0->get_current_value();
            bool response_1 = io_objects.trigger_1->get_current_value();
            bool response_2 = io_objects.trigger_2->get_current_value();
            bool response_3 = io_objects.trigger_3->get_current_value();
            bool responded = response_0 | response_1 | response_2 | response_3;

            // Check if all the triggers are low
            if (!responded)
            {
                analogWrite(LED_WAIT_TRIGGER, LED_OFF);
                next_state = WAIT_CLEAR_0;
            }
            break;
        }

        // Wait for the clear to go high
        case WAIT_CLEAR_0:
        {
            analogWrite(LED_WAIT_CLEAR, LED_ON); // Signal state
            this->display_responded();           // Signal the owner

            // Check the button state
            bool val = io_objects.pin_button->get_current_value();
            if (val)
            {
                next_state = WAIT_CLEAR_1;
            }
            break;
        }
        // Wait for the clear to go low
        case WAIT_CLEAR_1:
        {
            analogWrite(LED_WAIT_CLEAR, LED_ON); // Signal state
            this->display_responded();           // Signal the owner

            // Check the button
            bool val = io_objects.pin_button->get_current_value();
            if (!val)
            {
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
            return GENERIC_ERROR;
            break;
        }
        }
        current_state = next_state;
        return SUCCESS;
    }
};