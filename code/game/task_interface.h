/*
    This is the interface for adding a task to the round_robin_os.h

    Copyright January 2025 Dr. Jakob W. Kunzler
    This file is licensed under the terms of the MIT LICENSE
*/

#pragma once
#include <arduino.h>
#include <stdint.h>
#include <string.h>

class TaskInterface
{

public:
  // Safety limits
  static const uint8_t MAX_SIZE_NAME_BUFFER = 32;
  static const uint8_t MAX_PRINT_STRING_LENGTH = 128;

  // This wraps the tick function for a task with timekeeping between calls
  void call_tick(void *args)
  {
    MicrosTrackType now = micros();
    MicrosTrackType delta = now - this->last_time_stamp;
    this->last_time_stamp = now;
    return this->tick(delta, args);
  }

  // Get the human readable name for the tasks
  const char *get_name()
  {
    return name;
  }

  // Setup the error handler
  void set_error_handler_function(void (*error_handler)(const char *))
  {
    this->error_handler = error_handler;
  }

  // Setup the print handler
  void set_print_handler_function(void (*print_handler)(const char *))
  {
    this->print_handler = print_handler;
  }

  // Setup the name
  void set_name(const char name[MAX_SIZE_NAME_BUFFER])
  {
    strncpy(this->name, name, MAX_SIZE_NAME_BUFFER);
  }

protected:
  // Time tracking
  typedef uint32_t MicrosTrackType;
  MicrosTrackType last_time_stamp;
  // Name
  char name[MAX_SIZE_NAME_BUFFER];
  // Handlers
  void (*print_handler)(const char *) = NULL;
  void (*error_handler)(const char *) = NULL;

  // This wraps the print with a task name
  void print(const char *message)
  {
    if (this->print_handler != NULL)
    {
      uint8_t n_buffer = min(strlen(message) + strlen(name) + 24, MAX_PRINT_STRING_LENGTH);
      char *print_buffer = (char *)malloc(n_buffer);
      if (print_buffer == NULL)
      {
        (this->error_handler)("Out of memory");
      }
      snprintf(print_buffer, n_buffer, "Task \'%s\' says \'%s\'", name, message);
      (this->print_handler)(print_buffer);
      free(print_buffer);
    }
  }

  // This wraps the error call with a task name
  void error(const char *message)
  {
    if (this->error_handler != NULL)
    {
      uint8_t n_buffer = min(strlen(message) + strlen(name) + 24, MAX_PRINT_STRING_LENGTH);
      char *print_buffer = (char *)malloc(n_buffer);
      if (print_buffer == NULL)
      {
        (this->error_handler)("Out of memory");
      }
      snprintf(print_buffer, n_buffer, "Task \'%s\' has error \'%s\'", name, message);
      (this->error_handler)(print_buffer);
      free(print_buffer);
    }
  }

  // The user writes their task to this
  virtual void tick(
      MicrosTrackType,
      void *) {};
};