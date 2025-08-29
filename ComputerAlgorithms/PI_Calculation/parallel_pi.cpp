#include <iostream>
#include <cmath>
#include <iomanip>
#include <mpi.h> // Include OpenMPI header

using namespace std;

// Function to estimate pi for a chunk of the range [start, end]
double estimate_pi_chunk(int start, int end, int n) {
    double total_area = 0.0;
    double delta_x = 1.0 / n;

    for (int i = start; i <= end; ++i) {
        double x_i = i / static_cast<double>(n);
        double y_i = std::sqrt(1.0 - x_i * x_i);
        total_area += y_i * delta_x;
    }
    return total_area;
}

int main(int argc, char* argv[]) {
    // Initialize MPI
    MPI_Init(&argc, &argv);

    // Get the number of processes and rank
    int num_processes, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Enforce 8 processors
    if (num_processes != 8) {
        if (rank == 0) {
            cout << "This program must be run with exactly 8 processors." << endl;
        }
        MPI_Finalize();
        return 1;
    }

    int n = 1000000; // Number of strips (subintervals)
    
    // Root process asks for the number of strips
    if (rank == 0) {
        cout << "Enter the number of strips you want to create between [0,1]: ";
        cin >> n;
    }

    // Broadcast the value of n to all processes
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Calculate the range of the interval each process will handle
    int chunk_size = n / num_processes;
    int start = rank * chunk_size + 1;
    int end = (rank == num_processes - 1) ? n : (rank + 1) * chunk_size;

    // Record the starting time for the computation
    double start_time = MPI_Wtime();

    // Each process computes its portion of the total area
    double local_area = estimate_pi_chunk(start, end, n);

    // Gather the results from all processes in the root process
    double total_area = 0.0;
    MPI_Reduce(&local_area, &total_area, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    // Record the ending time for the computation
    double end_time = MPI_Wtime();

    // Root process computes the final pi estimate
    if (rank == 0) {
        double pi_estimate = 4 * total_area;
        double pi_actual = M_PI; // Actual value of Pi
        double error = std::fabs(pi_estimate - pi_actual);

        // Output the results
        cout << "Estimated value of Pi: " << std::setprecision(15) << pi_estimate << endl;
        cout << "Actual value of Pi: " << std::setprecision(15) << pi_actual << endl;
        cout << "Error from actual value of Pi: " << std::setprecision(15) << error << endl;

        // Output the time taken for the computation in milliseconds
        double time_taken_ms = (end_time - start_time) * 1000.0;
        cout << "Time taken for computation: " << time_taken_ms << " milliseconds." << endl;
    }

    // Finalize MPI
    MPI_Finalize();
    return 0;
}
