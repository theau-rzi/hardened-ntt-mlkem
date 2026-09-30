#pragma once
#include <stdint.h>

/* Precalculed tables in Montgomery domain */
/* zeta^{2BitRev_7(i)} for NTT and NTT^-1 with twiddle factors */
extern const int16_t md_zetas[128];

/* zeta^{2BitRev_7(i)+1} for MultiplyNTTs with twiddle factors */
extern const int16_t md_zetas_mul[128];
