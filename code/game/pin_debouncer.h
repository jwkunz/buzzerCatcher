#pragma once
#include "arduino.h"
#include "pin_map.h"
#include "task_interface.h"

class PinDebouncer : public TaskInterface {
private:
  bool current_value;
  uint8_t pin_number;
  MicrosTrackType micros_for_stable;
  MicrosTrackType current_micros;

public:
  void set(uint8_t pin_number, MicrosTrackType micros_for_stable) {
    this->current_value = false;
    this->pin_number = pin_number;
    this->micros_for_stable = micros_for_stable;
    this->current_micros = 0;
  }

  // This is the debounced IO
  bool get_current_value() {
    return current_value;
  }

  void tick(MicrosTrackType micros_since_last_call,
            void *args) {
    // Sample
    bool current_read = digitalRead(this->pin_number);
    // If the value is not what is currently locked
    if (current_read != current_value) {
      // Count the time in a deviation
      this->current_micros += micros_since_last_call;
      // If deviated for long enough
      if (this->current_micros > this->micros_for_stable) {
#ifdef VERBOSE_MODE
        this->print("Pin changed values");
#endif
        // Update state
        this->current_value = current_read;
        this->current_micros = 0;
      }
    } else {
      this->current_micros = 0;
    }
  }
};