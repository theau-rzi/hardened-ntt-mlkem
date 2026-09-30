#pragma once
#include <stdint.h>

/* Algorithm constants */
#define kyber_q 3329
#define kyber_n 256

/* Precalculed value of q^{-1} mod (+/-) 2^16 for Mongomery reduction */
/* It is signed to avoid overflow during butterflies */
#define q_inv -3327
