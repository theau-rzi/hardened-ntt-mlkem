#include "ntt.h"

#include "subroutines.h"

/* Computes the NTT representation f_cap of the given polynomial f in R_q.*/
void NTT(int16_t f[static kyber_n]) {
  uint16_t i = 1;

  for (uint8_t len = 128; len >= 2; len >>= 1) {
    for (uint16_t start = 0; start < 256; start += (uint16_t)(2 * len)) {
      int16_t zeta = md_zetas[i];
      i++;
      for (uint16_t j = start; j < start + len; j++) {
        int16_t t = montgomery_mul_3329(zeta, f[j + len]);
        int16_t u = f[j];

        /* No need for reduction for the following operations */
        /* as |f| < q for len=128, < 2q for len=64...*/
        /* and |f| < 8q = 26632 after 7th iteration.*/
        f[j + len] = u - t;
        f[j] = u + t;
      }
    }
  }
}

/* Returns the product h_cap = f_cap * g_cap * R^{-1} in NTT domain.*/
/* This R^{-1} parasite will be removed at the end of NTT_inv.*/
void NTTs_mul(int16_t h_cap[static kyber_n], int16_t f_cap[static kyber_n],
              int16_t g_cap[static kyber_n]) {
  for (uint16_t i = 0; i < 128; i++) {
    /* In practice, coefficients are h_cap[i]*R^{-1}.*/
    int32_t c = base_case_multiply(f_cap[2 * i], f_cap[2 * i + 1], g_cap[2 * i],
                                   g_cap[2 * i + 1], md_zetas_mul[i]);
    h_cap[2 * i] = (int16_t)(c >> 16);
    h_cap[2 * i + 1] = (int16_t)c;
  }
}

/* !!! Is not the inverse stricto senso. As base_case_multiply returns*/
/* f_cap[i]*g_cap[i]*R^{-1}, the final multiplication factors that in !!!*/
void NTT_inv(int16_t h_cap[static kyber_n]) {
  uint16_t i = 127;
  for (uint16_t len = 2; len <= 128; (len <<= 1)) {
    for (uint16_t start = 0; start < 256; start += (uint16_t)(2 * len)) {
      int16_t zeta = md_zetas[i];
      i--;
      for (uint16_t j = start; j < start + len; j++) {
        int16_t t = h_cap[j];
        h_cap[j] = barrett_reduction_3329(t + h_cap[j + len]);
        h_cap[j + len] = montgomery_mul_3329(zeta, h_cap[j + len] - t);
      }
    }
  }
  for (uint16_t k = 0; k < 256; k++) {
    /* In the NIST documentation, entries should be multiplied by 3303.*/
    /* However, montgomery_mul_3329 is adapted to twiddle factors.*/
    /* On top of that, goal is to multiply in NTT domain. The multiplication*/
    /* We thus calculate (3303*2^32) % 3329 = 1441 mod q.*/
    h_cap[k] = montgomery_mul_3329(h_cap[k], 1441);
  }
}
