#include <mpi.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (argc < 2) {
        if (rank == 0)
            std::cerr << "Usage: mpirun -np <num_procs> ./monte_carlo_pi_mpi <num_points>\n";
        MPI_Finalize();
        return 1;
    }

    long long num_points_total = atoll(argv[1]);
    long long num_points_per_proc = num_points_total / size;

    // Initialize random seed differently for each process
    std::srand(static_cast<unsigned>(time(0)) + rank);

    long long local_count = 0;
    for (long long i = 0; i < num_points_per_proc; ++i) {
        double x = static_cast<double>(std::rand()) / RAND_MAX;
        double y = static_cast<double>(std::rand()) / RAND_MAX;
        if (x*x + y*y <= 1.0)
            local_count++;
    }

    long long global_count = 0;
    MPI_Reduce(&local_count, &global_count, 1, MPI_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        double pi_estimate = 4.0 * static_cast<double>(global_count) / num_points_total;
        std::cout << "Estimated Pi = " << pi_estimate << std::endl;
    }

    MPI_Finalize();
    return 0;
}

//mpic++ -O2 -o monte_carlo_pi_mpi monte_carlo_pi_mpi.cpp
//mpirun -np 4 ./monte_carlo_pi_mpi 10000000
