#include <stddef.h>
#include <stdint.h>
#include <stdio.h> /* To run tests on an OS */

#include "core/subroutines.h"
#include "math/ntt.h"

/* To simulate a hardware reset upon fault detection */
void hardware_reset(void) {
  while (1) {
    __asm__ volatile("nop");
  }
}

/* To get random numbers from a hardware peripheral */
/* Using Linear Feedback Shift Register, to simulate RNG without an OS */
uint16_t random_uint16_from_hardware_peripheral(void) {
  static uint16_t lfsr = 0xACE1u;
  unsigned lsb = lfsr & 1;
  lfsr >>= 1;
  if (lsb == 1) {
    lfsr ^= 0xB400u;
    /* 0xB400u is X^16+X^14+X^13+X^11+1, a primitive polynomial over GF(2)[X] */
  }
  return lfsr;
}

int main(void) {
  /* Hardware initialization: */
  /* Clock Tree, MPU,  Peripherals, etc. */

  /* Statical memory allocation */
  static int16_t f[kyber_n];
  static int16_t g[kyber_n];
  static int16_t h_ntt[kyber_n];

  /* Random numbers generation */
  while (1) {
    static int counter = 0; /* For tests purposes */

    for (int i = 0; i < kyber_n; i++) {
      f[i] = (int16_t)(random_uint16_from_hardware_peripheral() % kyber_q);
      g[i] = (int16_t)(random_uint16_from_hardware_peripheral() % kyber_q);
    }

    /* NTT algorithms execution */
    NTT(f);
    NTT(g);
    NTTs_mul(h_ntt, f, g);
    NTT_inv(h_ntt);

    /* Here will lie the fault verification */

    /* Ensure coefficients remain within bounds */
    for (int i = 0; i < kyber_n; i++) {
      if (h_ntt[i] >= kyber_q || h_ntt[i] <= -kyber_q) {
        printf("FAULT ERROR."); /* Testing fault detection */
        hardware_reset();
      }
    }

    /* Simulated delay between events */
    for (volatile int delay = 0; delay < 10000; delay++);

    /* Testing whether the algorithm runs well */
    counter++;
    if ((counter % 10000) == 0) {
      printf("%d th turn completed.\n", counter);
    }
  }

  return 0;
}
