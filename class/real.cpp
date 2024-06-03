#ifndef FHE_REAL
#define FHE_REAL

/**
 * This file represents methods of encryption/decryption
 * of numbers consisting in the R field.
 */

#include <cstdint>
#include <iostream>

#include "../libs/BigNumber.h"
#include "./rand.cpp"

using namespace std;

class FHE {
   public:
    FHE(const uint8_t& n) {
        /**
         * 2*r should be smaller than p/2!!!
         */
        Random gen;
        r = gen.randomU32(UINT16_MAX, UINT32_MAX);
        q = gen.randomU64(UINT32_MAX, UINT64_MAX);
        p = genKeys(n);

        cout << "p: " << p << endl;
        cout << "q: " << q << endl;
        cout << "r: " << r << endl;
    }

    /**
     * @return [p, r] array, where:
     * p - private key in [2^{n - 1}; 2^n), n <= 256
     */
    BigNumber genKeys(const uint8_t& n) {
        Random gen;
        BigNumber k = gen.randomU64(pow(2, n - 1), pow(2, n));
        return k * 2 - 1;  // to make key 100% odd number in range [2^{n - 1}; 2^n)
    }

    /**
     * Encryption of uint64_t represented as BigNumber:
     * @returns 2r + num + pq (BigNumber)
     * @param num - number to encrypt (BigNumber)
     * @param q - random integer (BigNumber)
     * @param r - random number (uint64_t)
     * @param p - key (BigNumber)
     */
    BigNumber encrypt(BigNumber num) {
        BigNumber z = (BigNumber(r) * 2) + num;
        return p * q + z;
    }

    /**
     * Decryption of BigNumber:
     */
    BigNumber decrypt(BigNumber num) {
        return (num % p) % BigNumber(r);
    }

   private:
    BigNumber p;  // key
    BigNumber q;  // q - random big number
    uint32_t r;   // r - random little number
};

#endif  // FHE_REAL
