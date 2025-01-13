/*
    This is a multitasking round robin operating system that can run code the conforms to the task_interface.h

    Copyright January 2025 Dr. Jakob W. Kunzler
    This file is licensed under the terms of the MIT LICENSE
*/

#pragma once
#include "task_interface.h"

// Define the allocation for ROUND_ROBIN_MAX_TASKS at compile time
template<unsigned int ROUND_ROBIN_MAX_TASKS> class RoundRobinOS {
  
private:
  // This is how tasks are enteredn_active_tasks
  typedef struct {
    TaskInterface* task_object; // An object that follows the API
    void* task_args; // A raw pointer to a binary blob containing arguments for the task
  } TaskEntry;
  // All the tasks are stored here
  TaskEntry tasks[ROUND_ROBIN_MAX_TASKS];
  uint8_t n_active_tasks = 0;

public:
  uint8_t get_n_active_tasks(){
      return this->n_active_tasks;
  }

  // The tick API is called periodically
  void tick() {
    for (uint8_t i = 0; i < n_active_tasks; ++i) {
      tasks[i].task_object->call_tick(tasks[i].task_args);
    }
  }

  // Add a task to the back of the tasks stack
  bool push_task(TaskInterface* task_object, void* task_args) {
    if (n_active_tasks >= ROUND_ROBIN_MAX_TASKS) {
      return false;
    }
    this->tasks[this->n_active_tasks] = { task_object, task_args };
    this->n_active_tasks += 1;
    return true;
  }

  // Pop a task from the back of the tasks stack
  bool pop_task(TaskInterface* task_object, void* task_args) {
    if (n_active_tasks == 0) {
      return false;
    }
    this->n_active_tasks -= 1;
  }

  // Use this to hook up an error handler during configuration
  void set_error_handler_functions(void (*error_handler)(const char*)) {
    for (uint8_t i = 0; i < n_active_tasks; ++i) {
      tasks[i].task_object->set_error_handler_function(error_handler);
    }
  }

  // Use this to hook up an print handler during configuration
  void set_print_handler_functions(void (*print_handler)(const char*)) {
    for (uint8_t i = 0; i < n_active_tasks; ++i) {
      tasks[i].task_object->set_print_handler_function(print_handler);
    }
  }
};