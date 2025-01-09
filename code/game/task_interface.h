/*
    This is the interface for adding a task to the round_robin_os.h

    Copyright January 2025 Dr. Jakob W. Kunzler
    This file is licensed under the terms of the MIT LICENSE
*/

#pragma once
#include <arduino.h>
#include <stdint.h>

class TaskInterface {

public:

  // This wraps the tick function for a task with timekeeping between calls
  void call_tick(void *args) {
    MicrosTrackType now = micros();
    MicrosTrackType delta = now - this->last_time_stamp;
    this->last_time_stamp = now;
    return this->tick(delta, args);
  }  
  
  // Get the human readable name for the tasks
  String get_name() {
    return name;
  }

  // Setup the error handler
  void set_error_handler_function(void (*error_handler)(String &)) {
    this->error_handler = error_handler;
  }

  // Setup the print handler
  void set_print_handler_function(void (*print_handler)(String &)) {
    this->print_handler = print_handler;
  }

  // Setup the name
  void set_name(String name) {
    this->name = name;
  }

protected:
  // Time tracking
  typedef uint32_t MicrosTrackType;
  MicrosTrackType last_time_stamp;
  // Name
  String name = "";
  // Handlers
  void (*print_handler)(String &) = NULL;
  void (*error_handler)(String &) = NULL;

  // This wraps the print with a task name
  void print(String message) {
    if (this->print_handler != NULL) {
      (*print_handler)("Task \'" + name + "\' says: \'" + message + "\'");
    }
  }

  // This wraps the error call with a task name
  void error(String message) {
    if (this->error_handler != NULL) {
      (*error_handler)("Task \'" + name + "\' has error \'" + message + "\'");
      ;
    }
  }

  // The user writes their task to this
  virtual void tick(
    MicrosTrackType,
    void *){};
};