#include <mpi.h>
#include <iostream>
#include <cmath>

// Example function to integrate
double f(double x) {
    return x*x; // change this to test other functions
}

// Compute Simpson rule over [local_a, local_b] with local_n intervals
double simpson(double local_a, double local_b, int local_n) {
    double h = (local_b - local_a) / local_n;
    double sum = f(local_a) + f(local_b);

    for (int i = 1; i < local_n; i++) {
        double x = local_a + i*h;
        if (i % 2 == 0)
            sum += 2*f(x);
        else
            sum += 4*f(x);
    }

    return sum * h / 3.0;
}

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Integration limits and intervals
    double a = 0.0, b = 1.0;        // integrate over [a,b]
    int n = 1000000;                // total number of intervals (even)
    if (n % 2 != 0) n++;            // ensure n is even

    // Divide intervals among processes
    int local_n = n / size;
    double h = (b - a) / n;
    double local_a = a + rank * local_n * h;
    double local_b = local_a + local_n * h;

    double start_time = MPI_Wtime();
    double local_integral = simpson(local_a, local_b, local_n);

    double total_integral = 0.0;
    MPI_Reduce(&local_integral, &total_integral, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    double end_time = MPI_Wtime();

    if (rank == 0) {
        std::cout.precision(15);
        std::cout << "Parallel Simpson Integral = " << total_integral << std::endl;
        std::cout << "Time elapsed (s) = " << end_time - start_time << std::endl;

        // Optional: compare with sequential computation
        start_time = MPI_Wtime();
        double seq_integral = simpson(a, b, n);
        end_time = MPI_Wtime();
        std::cout << "Sequential Simpson Integral = " << seq_integral << std::endl;
        std::cout << "Sequential Time (s) = " << end_time - start_time << std::endl;
    }

    MPI_Finalize();
    return 0;
}

//mpic++ -O2 -o parallel_simpson_mpi parallel_simpson_mpi.cpp
//mpirun -np 4 ./parallel_simpson_mpi
