#include "subroutines.h"

/* montgom_red(A) = (A - tq)/R */
int16_t montgomery_mul_3329(int16_t a, int16_t br) {
  /* Initializing the value to reduce mod q*/
  int32_t A = (int32_t)a * br;

  /* Calculating the multiplier t = (A mod R) * q_inv mod R = 2^16*/
  /* Casting in int16_t applies signed mod R */
  int16_t t = (int16_t)A * q_inv;

  int32_t tq = t * kyber_q;
  int32_t difference = A - tq;

  /* Dividing by R = 2^16 */
  difference = difference >> 16;

  return (int16_t)difference;
}

/* Barrett's 16-bit reduction that computes a mod 3329. */
/* BAR_M(A) = A - 3329 * \lfloor (A * M_bar / 3329) \rceil*/
/* Where \lfloor (A * M_bar / 3329) \rceil = (A * M_bar + 2^{k-1}) >> k.*/
int16_t barrett_reduction_3329(int16_t a) {
  /* M_bar = \lfloor 2^k / 3329 \rceil. We want M_bar as big as possible to */
  /* maximize accuracy. k=27 gives M_bar=40318 which is too much for an int16.*/
  /* We thus take k=26. M_bar = \lfloor 2^16 / 3329 \rceil = 20159 */
  const int16_t M_bar = 20159;

  /* Product is done as int32_t to avoid overflow*/
  /* 2^{k-1} = 1 << k-1*/
  int16_t t = (int16_t)((((int32_t)M_bar * a) + (1 << 25)) >> 26);

  a = (int16_t)(a - 3329 * t);
  return a;
}

/* Returns (a0+a1X)*(b0+b1X)[X^2-i] = c0+c1X [X^2-i]*/
/* First 16 bits correspond to c0, others to c1*/
int32_t base_case_multiply(int16_t a0, int16_t a1, int16_t b0, int16_t b1,
                           int16_t i) {
  int16_t c0;
  int16_t c1;

  /*c0=a0*b0+a1*b1*i and c1=a0*b1+a1*b0. */
  int16_t a0_b0 = montgomery_mul_3329(b0, a0);
  int16_t a1_i = montgomery_mul_3329(a1, i);
  int16_t a1_b1 = montgomery_mul_3329(b1, a1_i);
  int16_t a0_b1 = montgomery_mul_3329(b1, a0);
  int16_t a1_b0 = montgomery_mul_3329(b0, a1);

  c0 = a0_b0 + a1_b1;
  c1 = a0_b1 + a1_b0;

  /* Concatenation of c0 and c1.*/
  /* c1 is treated as unsigned to preserve its signed bit-representation.*/
  int32_t result = ((int32_t)c0 << 16) | (uint16_t)c1;

  return result;
}
