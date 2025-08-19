#include <bits/stdc++.h>
using namespace std;

// Function to find gcd of two numbers
int gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

int main() {
    cout << "Valid (a, b) pairs for Affine Cipher are:\n";

    for (int a = 1; a < 26; a++) {
        if (gcd(a, 26) == 1) {  // only coprime values of a
            for (int b = 0; b < 26; b++) {
                cout << "(" << a << ", " << b << ")\n";
            }
        }
    }

    return 0;
}
