#include <iostream>
using namespace std;

// Simple function to calculate (a^b) % n
long long modExp(long long a, long long b, long long n) {
    long long result = 1;

    // Repeat until b becomes 0
    while (b > 0) {
        // If b is odd, multiply result with a
        if (b % 2 == 1) {
            result = (result * a) % n;
        }

        // Square the base
        a = (a * a) % n;

        // Divide exponent by 2
        b = b / 2;
    }

    return result;
}

int main() {
    long long a, b, n;
    
    cout << "Enter number a: ";
    cin >> a;
    
    cout << "Enter power b: ";
    cin >> b;
    
    cout << "Enter modulo n: ";
    cin >> n;

    long long ans = modExp(a, b, n);
    cout << a << "^" << b << " mod " << n << " = " << ans << endl;

    return 0;
}
