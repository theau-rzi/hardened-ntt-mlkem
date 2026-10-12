# Hardened Number Theoretic Transform (NTT) for ML-KEM

This repository contains a constant-time, branchless C implementation of the Number Theoretic Transform (NTT) specifically designed for the ML-KEM (Kyber) post-quantum cryptographic scheme.

This software implementation serves as the foundational algorithmic layer for a broader research project targeting aerospace hardware security. The ultimate goal is to transition this logic into radiation-hardened, fault-tolerant silicon (VHDL/ASIC).

## Current Features

*   **Algorithmic Constant-Time Execution:** The core NTT loops (`ntt.c`) rely solely on deterministic counters (`len`, `start`). Memory accesses (`f[j]`, `md_zetas[i]`) are decoupled from the secret data being processed.
*   **Branchless Arithmetic:** The implementation strictly avoids conditional branching in sensitive operations (e.g., modular reductions) to mitigate software-based timing and cache side-channel attacks.
*   **ML-KEM (Kyber) Compliance:** Parameters ($q=3329$, $n=256$) and structures are aligned with the NIST FIPS 203 draft standard.

## Hardware Security Context & Future Work

While this C implementation is resilient against software-level side channels, it is explicitly **not** hardened against physical hardware attacks, which are the primary threat vector in embedded and aerospace environments.

Future phases of this project (targeting ARM Cortex-M bare-metal, FPGA, and ASIC deployments) will address the following physical vulnerabilities:

### 1. Power Analysis (CPA/DPA) Leakage
The current arithmetic operations, particularly the Montgomery butterfly (`t = montgomery_mul_3329(zeta, f[j + len])`), directly manipulate intermediate values related to secret data. This directly leaks the Hamming weight via power consumption.
*   **Future Mitigation:** Implementation of Boolean-to-Arithmetic masking or exploration of physically uncloneable functions (PUFs) at the silicon level.

### 2. Differential Fault Analysis (DFA) & Fault Injection
The implementation is highly vulnerable to voltage or electromagnetic glitching. A targeted fault during a butterfly operation or while loading a twiddle factor (`md_zetas`) will silently corrupt the ciphertext, enabling key recovery.
*   **Future Mitigation (Topological Approach):** Integration of Algorithm-Based Fault Tolerance (ABFT) utilizing Grothendieck topology and scheme theory to mathematically verify the "gluing" (recollement) of local geometric invariants during the NTT execution, creating a fail-fast mechanism without the overhead of Triple Modular Redundancy (TMR).

### 3. Compiler-Induced Vulnerabilities in Bare-Metal
When compiled for specific microarchitectures (e.g., `arm-none-eabi-gcc` for Cortex-M4), standard C operations like signed bit-shifts (`>>` in `barrett_reduction_3329`) might be compiled into conditional branches or variable-time instructions depending on the compiler's optimization flags and standard interpretation.
*   **Future Mitigation:** Strict inspection of the generated assembly and implementation of core operations in pure ARM assembly if necessary to guarantee constant-time execution on silicon.
*   
## References

* **Bo-Yin Yang**, *Notes on Short-Limb Modular Multiplication Techniques: Barrett, Montgomery, Plantard, and the Explicit CRT*, Cryptology ePrint Archive, Paper 2026/1743, 2026. [https://eprint.iacr.org/2026/1743](https://eprint.iacr.org/2026/1743)
* **National Institute of Standards and Technology** (2024), *Module-Lattice-Based Key-Encapsulation Mechanism Standard*. (Department of Commerce, Washington, D.C.), Federal Information Processing Standards Publication (FIPS) NIST FIPS 203. [https://doi.org/10.6028/NIST.FIPS.203](https://doi.org/10.6028/NIST.FIPS.203)
