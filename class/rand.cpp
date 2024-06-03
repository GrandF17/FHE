#ifndef RAND_GEN
#define RAND_GEN

/**
 * This file represents methods of encryption/decryption
 * of numbers consisting in the R field.
 */

#include <chrono>
#include <cstdint>
#include <iostream>
#include <random>

#include "../libs/BigNumber.h"

using namespace std;

class Random {
   public:
    Random() {
        // Initialize seed for random number generator
        seed = chrono::system_clock::now().time_since_epoch().count();
        generator.seed(seed);
    }

    // Generate random uint64_t number with optional modulus
    uint64_t randomU64(uint64_t from = 0, uint64_t to = UINT64_MAX) {
        uniform_int_distribution<uint64_t> distribution(from, to);
        return distribution(generator);
    }

    // Generate random uint32_t number with optional modulus
    uint32_t randomU32(uint32_t from = 0, uint32_t to = UINT32_MAX) {
        return static_cast<uint32_t>(randomU64(from, to));
    }

    // Generate random uint16_t number with optional modulus
    uint16_t randomU16(uint16_t from = 0, uint16_t to = UINT16_MAX) {
        return static_cast<uint16_t>(randomU64(from, to));
    }

    // Generate random uint8_t number with optional modulus
    uint8_t randomU8(uint8_t from = 0, uint8_t to = UINT8_MAX) {
        return static_cast<uint8_t>(randomU64(from, to));
    }

   private:
    unsigned seed;
    mt19937 generator;
};

#endif  // RAND_GEN