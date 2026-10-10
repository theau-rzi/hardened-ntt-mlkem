#pragma once

#include "parameters.h"
#include "tables.h"

int16_t montgomery_mul_3329(int16_t a, int16_t br);
int16_t barrett_reduction_3329(int16_t a);
int32_t base_case_multiply(int16_t a0, int16_t a1, int16_t b0, int16_t b1,
                           int16_t i);
