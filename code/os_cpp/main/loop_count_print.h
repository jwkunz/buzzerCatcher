#pragma once
#include "task_interface.h"

class LoopCountPrint : public TaskInterface {
private:
  uint32_t current_value = 0;
  uint32_t limit = 10000;
  MicrosTrackType accumulated_time = 0;

public:
  void set_limit(uint32_t limit) {
    this->limit = limit;
  }
  void reset(uint32_t limit) {
    this->current_value = 0;
    this->accumulated_time = 0;
  }
  void tick(MicrosTrackType micros_since_last_call,
            void *args) {
    this->current_value += 1;
    this->accumulated_time += micros_since_last_call;
    if (this->current_value >= this->limit) {
      MicrosTrackType speed = this->accumulated_time/this->limit;
      String message("Hit limit of " + String(this->limit) + " in " + String(this->accumulated_time) + " us which is " + String(speed) + " us per loop");
      this->print(message);
      this->current_value = 0;
      this->accumulated_time = 0;
    }
  }
};