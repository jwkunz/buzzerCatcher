#pragma once
#include "task_interface.h"
#include "piano_key_definitions.h"
#include "pin_debouncer.h"
#include "pin_map.h"

class BuzzerDriver : public TaskInterface
{
private:
    MicrosTrackType note_to_period(uint8_t note)
    {
        if (note >= 88)
        {
            return 0;
        }
        else
        {
            return KEY_PERIOD_MICROS[note];
        }
    }

public:
    // This is the data field that will be passed into the task
    // It contains all the IO resources this task needs
    typedef struct
    {
        PinDebouncer *pin_switch;
    } TickArgsType;

    // Set song array.  This array will be read only referenced and should be persistent (aka not on the stack)
    void set_song(const uint8_t *song, uint16_t n_song)
    {
        this->song = (uint8_t *)song;
        this->n_song = n_song;
        uint8_t bpm = song[0];
        this->micros_per_32note = 60000000 / (8 * bpm);
        this->restart_song();
    }
    // Use this to restart the at the beginning
    void restart_song()
    {
        this->current_index = 1;
        this->accumulated_buzz_time = 0;
        this->buzzer_state = false;
        this->next_buzz_mark = note_to_period(song[1]);
        this->next_note_mark = song[2] * this->micros_per_32note;
        this->finished_song = false;
    }
    // Toggle if to repeat or not
    void set_repeat(bool flag)
    {
        this->repeat_flag = flag;
    }
    // Set wheather to activate the buzzer
    void set_active(bool flag)
    {
        this->active_flag = flag;
    }
    // Get status if finished
    bool get_finished()
    {
        return this->finished_song;
    }

    // The main tick function gets called every cycle in the operating system
    void tick(MicrosTrackType micros_since_last_call,
              void *args)
    {
        // Check the sound switch
        bool sound_switch = ((TickArgsType *)args)->pin_switch->get_current_value();

        // Only drive if active and not finished and the sound is not turned off
        if ((this->active_flag == false) || (this->finished_song) || (sound_switch == false))
        {
            // Force pin low if not already
            if (this->buzzer_state)
            {
                this->buzzer_state = false;
                digitalWrite(PIN_BUZZER, this->buzzer_state);
            }
            return;
        }

        // Log time change
        this->accumulated_buzz_time += micros_since_last_call;
        this->accumulated_note_time += micros_since_last_call;

        // Check if time to buzz and not resting
        bool resting = this->next_buzz_mark == 0;
        if ((this->accumulated_buzz_time >= this->next_buzz_mark) && (!resting))
        {
            // Track time with some 'vibrato' from the remainder
            this->accumulated_buzz_time -= this->next_buzz_mark;
            // Alternate buzzer
            digitalWrite(PIN_BUZZER, this->buzzer_state);
            this->buzzer_state = !this->buzzer_state;
        }
        // Check if time to change notes
        if (this->accumulated_note_time >= this->next_note_mark)
        {
            // Track time with some 'rubato' from the remainder
            this->accumulated_note_time -= this->next_note_mark;
            // Advance
            this->current_index += 2;

            // Check if song is finished
            if (this->current_index >= this->n_song)
            {
                // If repeating
                if (this->repeat_flag)
                {
                    this->restart_song();
                }
                else // If not repeating
                {
                    this->finished_song = true;
                }
            }
            else
            {
                // Advance
                this->next_buzz_mark = note_to_period(song[this->current_index]);
                this->next_note_mark = song[this->current_index + 1] * this->micros_per_32note;
            }
        }
    }

private:
    uint8_t *song;
    uint16_t n_song;
    uint16_t current_index;
    MicrosTrackType micros_per_32note;
    MicrosTrackType accumulated_buzz_time;
    MicrosTrackType next_buzz_mark;
    MicrosTrackType accumulated_note_time;
    MicrosTrackType next_note_mark;
    bool active_flag;
    bool repeat_flag;
    bool buzzer_state;
    bool finished_song;
};