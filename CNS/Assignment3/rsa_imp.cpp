#include <bits/stdc++.h>
using namespace std;

long long modPow(long long base, long long exp, long long mod) {
    long long result = 1;
    base %= mod;
    while (exp > 0) {
        if (exp & 1) result = (result * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }
    return result;
}

int main() {
    long long p = 17, q = 11;
    long long n = p * q;          
    long long phi = (p - 1) * (q - 1); 
    long long e = 7, d = 23;       

    long long plaintext = 88;
    cout << "Plaintext: " << plaintext << endl;

    // Encryption
    long long ciphertext = modPow(plaintext, e, n);
    cout << "Encrypted Ciphertext: " << ciphertext << endl;

    // Decryption
    long long decrypted = modPow(ciphertext, d, n);
    cout << "Decrypted Plaintext: " << decrypted << endl;

    return 0;
}
