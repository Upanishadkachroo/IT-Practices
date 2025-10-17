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

    // Scatter the data array to all processors
    int localN = N / 8;  // Each processor gets a chunk of 250 elements
    int localData[localN];

    MPI_Scatter(data, localN, MPI_INT, localData, localN, MPI_INT, 0, MPI_COMM_WORLD);

    // Each processor computes the operations on its local data
    localSum = 0;
    localProduct = 1;
    localMin = INT_MAX;
    localMax = INT_MIN;

    for (int i = 0; i < localN; i++) {
        localSum += localData[i];
        localProduct *= localData[i];
        if (localData[i] < localMin) localMin = localData[i];
        if (localData[i] > localMax) localMax = localData[i];
    }

    // Reduce results to rank 0
    long long globalSum, globalProduct;
    int globalMin, globalMax;

    MPI_Reduce(&localSum, &globalSum, 1, MPI_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(&localProduct, &globalProduct, 1, MPI_LONG_LONG, MPI_PROD, 0, MPI_COMM_WORLD);
    MPI_Reduce(&localMin, &globalMin, 1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);
    MPI_Reduce(&localMax, &globalMax, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

    // End timing
    double end_time = MPI_Wtime();

    if (rank == 0) {
        std::cout << "Results from Data Parallelism (All Operations on Each Processor):\n";
        std::cout << "Sum: " << globalSum << std::endl;
        std::cout << "Product: " << globalProduct << std::endl;
        std::cout << "Min: " << globalMin << std::endl;
        std::cout << "Max: " << globalMax << std::endl;
        std::cout << "Time taken: " << end_time - start_time << " seconds." << std::endl;
    }

    MPI_Finalize();
    return 0;
}
