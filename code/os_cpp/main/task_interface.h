#pragma once
#include <stdint.h>

class TaskInterface
{
public:
    typedef enum
    {
        SUCCESS,
        FAILURE,
        GENERIC_ERROR
    } StatusCodeType;

    typedef uint32_t MicrosTrackType;

    virtual StatusCodeType tick(
        MicrosTrackType micros_since_last_call,
        void *args) {};
};