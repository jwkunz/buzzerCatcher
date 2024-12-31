#pragma once
#include <arduino.h>
#include <stdint.h>

class TaskInterface {

public:

  void call_tick(void *args) {
    MicrosTrackType now = micros();
    MicrosTrackType delta = now - this->last_time_stamp;
    this->last_time_stamp = now;
    return this->tick(delta, args);
  }

  String get_name() {
    return name;
  }

  void set_error_handler_function(void (*error_handler)(String &)) {
    this->error_handler = error_handler;
  }
  void set_print_handler_function(void (*print_handler)(String &)) {
    this->print_handler = print_handler;
  }

  void set_name(String name) {
    this->name = name;
  }

protected:
  typedef uint32_t MicrosTrackType;
  MicrosTrackType last_time_stamp;
  String name = "";
  void (*print_handler)(String &) = NULL;
  void (*error_handler)(String &) = NULL;

  void print(String message) {
    if (this->print_handler != NULL) {
      (*print_handler)("Task \'" + name + "\' says: \'" + message + "\'");
    }
  }

  void error(String message) {
    if (this->error_handler != NULL) {
      (*error_handler)("Task \'" + name + "\' has error \'" + message + "\'");
      ;
    }
  }

  virtual void tick(
    MicrosTrackType,
    void *){};
};