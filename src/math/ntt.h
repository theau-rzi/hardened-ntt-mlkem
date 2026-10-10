#pragma once
#include "parameters.h"
void NTT(int16_t f[static kyber_n]);
void NTT_inv(int16_t f[static kyber_n]);
void NTTs_mul(int16_t h_cap[static kyber_n], int16_t f_cap[static kyber_n],
              int16_t g_cap[static kyber_n]);
