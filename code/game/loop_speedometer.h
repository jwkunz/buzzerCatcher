/*
    The task can be used to track the loop time and provide estimates of the runtime speed

    Copyright January 2025 Dr. Jakob W. Kunzler
    This file is licensed under the terms of the MIT LICENSE
*/

#pragma once
#include "task_interface.h"
 
class LoopSpeedometer : public TaskInterface {
private:
  uint32_t current_value = 0;
  uint32_t limit = 100000;
  MicrosTrackType accumulated_time = 0;
  MicrosTrackType speed = 0;

public:
  // Access the last speed measurement
  MicrosTrackType get_last_speed(){
    return speed;
  }
  // Set the time limit between measurements
  void set_limit(uint32_t limit) {
    this->limit = limit;
  }
  // Reset the time 
  void reset() {
    this->current_value = 0;
    this->accumulated_time = 0;
  }
  // Tick API for OS
  void tick(MicrosTrackType micros_since_last_call,
            void *args) {
    // Count until the limit is reached, accumulating the time 
    this->current_value += 1;
    this->accumulated_time += micros_since_last_call;
    if (this->current_value >= this->limit) {
      // After hitting the limit, calculate the average speed
      this->speed = this->accumulated_time / this->limit;
      #ifdef DEBUG_MODE
      String message("Hit limit of " + String(this->limit) + " in " + String(this->accumulated_time) + " us which is " + String(speed) + " us per loop");
      this->print(message);
      #endif
      this->current_value = 0;
      this->accumulated_time = 0;
    }
  }
};
