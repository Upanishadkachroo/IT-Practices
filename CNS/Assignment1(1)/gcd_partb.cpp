#include <bits/stdc++.h>
using namespace std;


int GCD(int a, int b, int &count) {
    count++;
    if (a == 0) return b;
    return GCD(b % a, a, count);
}


double average_iterations_fixed_first(int fixed, int digits, int trials) {
    int lower = pow(10, digits - 1);
    int upper = pow(10, digits) - 1;
    long long total = 0;

    for (int i = 0; i < trials; i++) {
        int b = lower + rand() % (upper - lower + 1);

        int count = 0;
        GCD(fixed, b, count);
        total += count;
    }
    return (double)total / trials;
}

int main() {
    srand(time(0));
    int trials = 1000;

    int fixed_num = 12345; // fixed 5-digit number
    int fixed_digits = 5;

    cout << "Part (b): Fixed First Number (" << fixed_num << "), Vary Second Number" << endl;
    cout << "SecondNumDigits,AverageIterations" << endl;

    for (int d = fixed_digits; d <= fixed_digits + 5; d++) { // vary from 5-digit to 10-digit
        double avg = average_iterations_fixed_first(fixed_num, d, trials);
        cout << d << "," << avg << endl;
    }

    return 0;
}
