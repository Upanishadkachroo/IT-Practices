#include <bits/stdc++.h>
using namespace std;

// Euclid's Algorithm with iteration counter
int GCD(int a, int b, int &count) {
    count++;
    if (a == 0) return b;
    return GCD(b % a, a, count);
}


double average_iterations_same_digits(int n, int trials) {
    int lower = pow(10, n - 1);
    int upper = pow(10, n) - 1;
    long long total = 0;

    for (int i = 0; i < trials; i++) {
        int a = lower + rand() % (upper - lower + 1);
        int b = lower + rand() % (upper - lower + 1);

        int count = 0;
        GCD(a, b, count);
        total += count;
    }
    return (double)total / trials;
}

int main() {
    srand(time(0));
    int trials = 1000;//number of test cases 

    cout << "Part (a): n-digit vs Average Iterations" << endl;
    cout << "Digits,AverageIterations" << endl;

    for (int n = 1; n <= 8; n++) { 
        double avg = average_iterations_same_digits(n, trials);
        cout << n << "," << avg << endl;
    }

    return 0;
}
