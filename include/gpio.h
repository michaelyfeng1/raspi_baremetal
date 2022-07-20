#pragma once

#include "aux.h"

#define GPIO_FUNC_NUM_MAX       6

struct GpioPinData
{
    reg32 reserved;
    reg32 data[2];
};

struct GpioRegs
{
    reg32 func_select[GPIO_FUNC_NUM_MAX];
    struct GpioPinData  output_set;
    struct GpioPinData  output_clear;
};



/**
 * @brief GPIO Notes
 * 
 */