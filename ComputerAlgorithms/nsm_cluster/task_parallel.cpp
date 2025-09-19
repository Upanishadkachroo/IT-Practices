#include <iostream>
#include <mpi.h>
#include <climits>

#define N 1000

int main(int argc, char **argv) {
    int rank, size;
    int data[N];
    long long localSum = 0, localProduct = 1;
    int localMin, localMax;

    // Start measuring time
    double start_time = MPI_Wtime();

    // Initialize MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Only rank 0 initializes the array
    if (rank == 0) {
        for (int i = 0; i < N; i++) {
            data[i] = i + 1;  // Initialize array with numbers 1 to 1000
        }
    }

    // Broadcast the array to all processors
    MPI_Bcast(data, N, MPI_INT, 0, MPI_COMM_WORLD);

    // Perform the operation depending on the rank (0: sum, 1: product, 2: min, 3: max)
    if (rank == 0) {  // Sum operation
        localSum = 0;
        for (int i = 0; i < N; i++) {
            localSum += data[i];
        }
    } else if (rank == 1) {  // Product operation
        localProduct = 1;
        for (int i = 0; i < N; i++) {
            localProduct *= data[i];
        }
    } else if (rank == 2) {  // Min operation
        localMin = INT_MAX;
        for (int i = 0; i < N; i++) {
            if (data[i] < localMin) localMin = data[i];
        }
    } else if (rank == 3) {  // Max operation
        localMax = INT_MIN;
        for (int i = 0; i < N; i++) {
            if (data[i] > localMax) localMax = data[i];
        }
    }

    // Gather the results from all processors
    long long globalSum;
    long long globalProduct;
    int globalMin, globalMax;

    MPI_Gather(&localSum, 1, MPI_LONG_LONG, &globalSum, 1, MPI_LONG_LONG, 0, MPI_COMM_WORLD);
    MPI_Gather(&localProduct, 1, MPI_LONG_LONG, &globalProduct, 1, MPI_LONG_LONG, 0, MPI_COMM_WORLD);
    MPI_Gather(&localMin, 1, MPI_INT, &globalMin, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Gather(&localMax, 1, MPI_INT, &globalMax, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // End timing
    double end_time = MPI_Wtime();

    // Only rank 0 will print the results
    if (rank == 0) {
        std::cout << "Results from Task Parallelism (Operations Divided Across Processors):\n";
        std::cout << "Sum: " << globalSum << std::endl;
        std::cout << "Product: " << globalProduct << std::endl;
        std::cout << "Min: " << globalMin << std::endl;
        std::cout << "Max: " << globalMax << std::endl;
        std::cout << "Time taken: " << end_time - start_time << " seconds." << std::endl;
    }

    MPI_Finalize();
    return 0;
}
