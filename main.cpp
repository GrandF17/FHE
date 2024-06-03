#include <iostream>

#include "./class/real.cpp"

using namespace std;

int main() {
    FHE fhe(64);  // p in [2^63; 2^64)
    BigNumber encryptedA = fhe.encrypt(11);
    BigNumber encryptedB = fhe.encrypt(2);
    BigNumber decryptedA = fhe.decrypt(encryptedA);
    BigNumber decryptedB = fhe.decrypt(encryptedB);

    cout << "Encrypted a: " << encryptedA << endl;
    cout << "Encrypted b: " << encryptedB << endl;
    cout << "Decrypted a: " << decryptedA << endl;
    cout << "Decrypted b: " << decryptedB << endl;

    cout << "Multiplication of encrypted A and B: " << fhe.decrypt(encryptedA * encryptedB) << endl;
    cout << "Addition of encrypted A and B: " << fhe.decrypt(encryptedA + encryptedB) << endl;

    cout << encryptedA * encryptedB << endl;
}