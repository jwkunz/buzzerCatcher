#pragma once
#include "task_interface.h"

template<unsigned int ROUND_ROBIN_MAX_TASKS> class RoundRobinOS {
private:

  typedef struct {
    TaskInterface* task_object;
    void* task_args;
  } TaskEntry;

  TaskEntry tasks[ROUND_ROBIN_MAX_TASKS];
  uint8_t n_tasks = 0;

public:
  void tick() {
    for (uint8_t i = 0; i < n_tasks; ++i) {
      tasks[i].task_object->call_tick(tasks[i].task_args);
    }
  }
  bool push_task(TaskInterface* task_object, void* task_args) {
    if (n_tasks >= ROUND_ROBIN_MAX_TASKS) {
      return false;
    }
    this->tasks[this->n_tasks] = { task_object, task_args };
    this->n_tasks += 1;
    return true;
  }
  bool pop_task(TaskInterface* task_object, void* task_args) {
    if (n_tasks == 0) {
      return false;
    }
    this->n_tasks -= 1;
  }
  void set_error_handler_functions(void (*error_handler)(String&)) {
    for (uint8_t i = 0; i < n_tasks; ++i) {
      tasks[i].task_object->set_error_handler_function(error_handler);
    }
  }
  void set_print_handler_functions(void (*print_handler)(String&)) {
    for (uint8_t i = 0; i < n_tasks; ++i) {
      tasks[i].task_object->set_print_handler_function(print_handler);
    }
  }
};