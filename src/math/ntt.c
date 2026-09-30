#include "ntt.h"

#include "reduce.h"
#include "tables.h"

/* Computes the NTT representation f_cap of the given polynomial f in R_q.*/
void NTT(int16_t f[static kyber_n]) {
  uint16_t i = 1;

  for (uint8_t len = 128; len >= 2; len >>= 1) {
    for (uint16_t start = 0; start < 256; start += 2 * len) {
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

void NTT_inv(int16_t f_cap[static kyber_n]) {
  uint16_t i = 127;
  for (uint16_t len = 2; len <= 128; (len <<= 1)) {
    for (uint16_t start = 0; start < 256; start += 2 * len) {
      int16_t zeta = md_zetas[i];
      i--;
      for (uint16_t j = start; j < start + len; j++) {
        int16_t t = f_cap[j];
        f_cap[j] = barrett_reduction_3329(t + f_cap[j + len]);
        f_cap[j + len] = montgomery_mul_3329(zeta, f_cap[j + len] - t);
      }
    }
  }
  for (uint16_t k = 0; k < 256; k++) {
    /* In the NIST documentation, entries should be multiplied by 3303.*/
    /* However, montgomery_mul_3329 is adapted to twiddle factors.*/
    /* We thus calculate (3303R mod q) = (128^{-1}*2^{16} mod q) = 512 mod q.*/
    f_cap[k] = montgomery_mul_3329(f_cap[k], 512);
  }
}
