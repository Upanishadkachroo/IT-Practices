#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <cmath>
using namespace std;

int gcd(int a, int b, int &iterations) {
    iterations = 0;
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
        iterations++;
    }
    return a;
}

int random_number(int digits) {
    int lower = pow(10, digits - 1);
    int upper = pow(10, digits) - 1;
    return lower + rand() % (upper - lower + 1);
}

void generate_same_digits_csv(const string &filename) {
    ofstream file(filename);
    file << "Digits,AverageIterations\n";
    int samples = 100;
    for (int digits = 1; digits <= 7; digits++) {
        int total = 0;
        for (int i = 0; i < samples; i++) {
            int a = random_number(digits);
            int b = random_number(digits);
            int it;
            gcd(a, b, it);
            total += it;
        }
        file << digits << "," << (double)total / samples << "\n";
    }
    file.close();
}

void generate_varying_b_csv(const string &filename, int fixed_digits) {
    ofstream file(filename);
    file << "Digits_b,AverageIterations\n";
    int samples = 100;
    for (int digits_b = fixed_digits; digits_b <= fixed_digits + 4; digits_b++) {
        int total = 0;
        for (int i = 0; i < samples; i++) {
            int a = random_number(fixed_digits);
            int b = random_number(digits_b);
            int it;
            gcd(a, b, it);
            total += it;
        }
        file << digits_b << "," << (double)total / samples << "\n";
    }
    file.close();
}

int main() {
    srand(time(0));
    generate_same_digits_csv("same_digits.csv");
    generate_varying_b_csv("varying_b.csv", 4); // fix a to 4 digits
    cout << "CSV files generated.\n";
    return 0;
}

