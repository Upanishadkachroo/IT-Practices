#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 1000

int main() {
    int data[N];
    long long int sum = 0, product = 1;
    int min, max;

    // Initialize the array with values from 1 to N
    for (int i = 0; i < N; i++) {
        data[i] = i + 1;
    }

    // Start the clock to measure execution time
    clock_t start_time = clock();

    // Perform the operations (sum, product, min, max)
    sum = 0;
    product = 1;
    min = data[0];
    max = data[0];

    for (int i = 0; i < N; i++) {
        sum += data[i];
        product *= data[i];
        if (data[i] < min) min = data[i];
        if (data[i] > max) max = data[i];
    }

    // Stop the clock after computation
    clock_t end_time = clock();
    double time_taken = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

    // Output results
    printf("Serial Program Results:\n");
    printf("Sum: %lld\n", sum);
    printf("Product: %lld\n", product);
    printf("Min: %d\n", min);
    printf("Max: %d\n", max);
    printf("Execution Time: %f seconds\n", time_taken);

    return 0;
}
