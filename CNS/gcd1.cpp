#include <bits/stdc++.h>
using namespace std;

int GCD(int a, int b, int& operations) {
    while (b != 0) {
        operations++;  // Increment operation count
        a = a % b;
        swap(a, b);
    }
    return a;
}

int main() {
    vector<int> digit_count;    // To store the number of digits for each run
    vector<int> operations_count; // To store the number of operations (iterations)

    // Vary number of digits in input numbers
    for (int digits = 1; digits <= 10; digits++) {  // Try from 1 to 10 digits
        int a = rand() % (int)pow(10, digits);   // Random number with 'digits' digits
        int b = rand() % (int)pow(10, digits);   // Another random number with 'digits' digits

        int operations = 0;
        GCD(a, b, operations);
        
        digit_count.push_back(digits);
        operations_count.push_back(operations);
    }

    // Print operation results
    cout << "Digits\tOperations\n";
    for (size_t i = 0; i < digit_count.size(); i++) {
        cout << digit_count[i] << "\t" << operations_count[i] << endl;
    }
}
