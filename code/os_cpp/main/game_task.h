#include "arduino.h"
#include "pin_map.h"
#include <stdint.h>

class game_task
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
    } game_state_t;

    game_state_t current_state = POWER_UP;

    const uint8_t LED_OFF = 0x00;
    const uint8_t LED_ON = 0xFF;
    const uint8_t LED_USER_0 = PIN_LED_0;
    const uint8_t LED_USER_1 = PIN_LED_1;
    const uint8_t LED_USER_2 = PIN_LED_2;
    const uint8_t LED_USER_3 = PIN_LED_3;
    const uint8_t LED_WAIT_ARM = PIN_LED_4;
    const uint8_t LED_WAIT_TRIGGER = PIN_LED_5;
    const uint8_t LED_WAIT_CLEAR = PIN_LED_6;

    uint8_t responded = 4;

    void display_responded()
    {

        digitalWrite(LED_USER_0, (0 == responded));
        digitalWrite(LED_USER_1, (1 == responded));
        digitalWrite(LED_USER_2, (2 == responded));
        digitalWrite(LED_USER_3, (3 == responded));
    }

public:
    void
    tick()
    {
        game_state_t next_state = current_state;
        switch (current_state)
        {
        case POWER_UP:
        {
            // Set all IO low
            digitalWrite(PIN_READY, LOW);
            analogWrite(PIN_LED_0, LED_OFF);
            analogWrite(PIN_LED_1, LED_OFF);
            analogWrite(PIN_LED_2, LED_OFF);
            analogWrite(PIN_LED_3, LED_OFF);
            analogWrite(PIN_LED_4, LED_OFF);
            analogWrite(PIN_LED_5, LED_OFF);
            analogWrite(PIN_LED_6, LED_OFF);
            analogWrite(PIN_LED_7, LED_OFF);
            next_state = POWER_UP;
            break;
        }
        case WAIT_ARM_0:
        {
            analogWrite(LED_WAIT_ARM, LED_ON);
            if (digitalRead(PIN_BUTTON))
            {
                next_state = WAIT_ARM_1;
            }
            break;
        }
        case WAIT_ARM_1:
        {
            analogWrite(LED_WAIT_ARM, LED_ON);
            if (!digitalRead(PIN_BUTTON))
            {
                analogWrite(LED_WAIT_ARM, LED_OFF);
                next_state = WAIT_TRIGGER_0;
            }
            break;
        }
        case WAIT_TRIGGER_0:
        {
            analogWrite(LED_WAIT_TRIGGER, LED_ON);
            // Look for a response in rapid succession
            // NOTE: Fair arbitration implemented below
            bool response_0 = digitalRead(PIN_TRIGGER_1);
            bool response_1 = digitalRead(PIN_TRIGGER_2);
            bool response_2 = digitalRead(PIN_TRIGGER_3);
            bool response_3 = digitalRead(PIN_TRIGGER_4);
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
                        this->responded = i;
                    }
                }
            }
            break;
        }
        case WAIT_TRIGGER_1:
        {
            analogWrite(LED_WAIT_TRIGGER, LED_ON);
            this->display_responded();

            // Look for a response in rapid succession
            // NOTE: Fair arbitration implemented below
            bool response_0 = digitalRead(PIN_TRIGGER_1);
            bool response_1 = digitalRead(PIN_TRIGGER_2);
            bool response_2 = digitalRead(PIN_TRIGGER_3);
            bool response_3 = digitalRead(PIN_TRIGGER_4);
            bool responded = response_0 | response_1 | response_2 | response_3;

            // Check if all the triggers are low
            if (!responded)
            {
                analogWrite(LED_WAIT_TRIGGER, LED_OFF);
                next_state = WAIT_CLEAR_0;
            }
            break;
        }
        case WAIT_CLEAR_0:
        {
            analogWrite(LED_WAIT_CLEAR, LED_ON);
            this->display_responded();
            if (digitalRead(PIN_BUTTON))
            {
                next_state = WAIT_CLEAR_1;
            }
            break;
        }
        case WAIT_CLEAR_1:
        {
            analogWrite(LED_WAIT_CLEAR, LED_ON);
            this->display_responded();
            if (!digitalRead(PIN_BUTTON))
            {
                analogWrite(LED_WAIT_CLEAR, LED_OFF);
                next_state = WAIT_ARM_0;
            }
            break;
        }
        default:
        {
            // ERROR CASE
            next_state = POWER_UP;
            break;
        }
        }
        current_state = next_state;
    }
};