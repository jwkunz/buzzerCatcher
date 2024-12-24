#pragma once
#include <stdint.h>

class task_interface
{
public:
    typedef enum
    {
        SUCCESS,
        FAILURE,
        GENERIC_ERROR
    } status_code_t;

    virtual status_code_t tick(
        uint32_t micros_since_last_call) {};
};