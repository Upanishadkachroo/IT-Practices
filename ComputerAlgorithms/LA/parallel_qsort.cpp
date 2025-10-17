#include <mpi.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>

void sequential_quicksort(std::vector<int>& arr) {
    std::sort(arr.begin(), arr.end());
}

// Partition array and return pivot index
int partition(std::vector<int>& arr, int low, int high) {
    int pivot = arr[high];
    int i = low - 1;
    for(int j=low; j<high; j++){
        if(arr[j]<=pivot){
            i++;
            std::swap(arr[i], arr[j]);
        }
    }
    std::swap(arr[i+1], arr[high]);
    return i+1;
}

// Parallel quicksort
void parallel_quicksort(std::vector<int>& arr, int rank, int size, MPI_Comm comm) {
    int n = arr.size();
    if(size == 1 || n <= 1000) { // base case: small array or single process
        sequential_quicksort(arr);
        return;
    }

    // Choose pivot (here: last element)
    int pivot = arr[n-1];

    // Partition locally
    std::vector<int> less, greater;
    for(int i=0; i<n-1; i++){
        if(arr[i] <= pivot) less.push_back(arr[i]);
        else greater.push_back(arr[i]);
    }
    less.push_back(pivot);

    int partner = rank ^ (size/2); // XOR trick to choose partner
    int half = size / 2;

    // Send greater to partner process if rank < half
    if(rank < half) {
        int send_count = greater.size();
        MPI_Send(&send_count, 1, MPI_INT, partner, 0, comm);
        MPI_Send(greater.data(), send_count, MPI_INT, partner, 1, comm);
        parallel_quicksort(less, rank, half, comm);
        // Receive sorted greater back
        int recv_count;
        MPI_Recv(&recv_count, 1, MPI_INT, partner, 2, comm, MPI_STATUS_IGNORE);
        std::vector<int> sorted_greater(recv_count);
        MPI_Recv(sorted_greater.data(), recv_count, MPI_INT, partner, 3, comm, MPI_STATUS_IGNORE);
        arr = less;
        arr.insert(arr.end(), sorted_greater.begin(), sorted_greater.end());
    } else {
        // Receive subarray to sort
        int recv_count;
        MPI_Recv(&recv_count, 1, MPI_INT, partner, 0, comm, MPI_STATUS_IGNORE);
        std::vector<int> recv_array(recv_count);
        MPI_Recv(recv_array.data(), recv_count, MPI_INT, partner, 1, comm, MPI_STATUS_IGNORE);
        parallel_quicksort(recv_array, 0, half, comm); // recursive on local half
        // Send back
        int send_count = recv_array.size();
        MPI_Send(&send_count, 1, MPI_INT, partner, 2, comm);
        MPI_Send(recv_array.data(), send_count, MPI_INT, partner, 3, comm);
    }
}

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    std::vector<int> arr;
    int N = 1000000;

    if(rank==0) {
        srand(time(0));
        arr.resize(N);
        for(int i=0;i<N;i++) arr[i] = rand();
    }

    double start_time = MPI_Wtime();

    parallel_quicksort(arr, rank, size, MPI_COMM_WORLD);

    double end_time = MPI_Wtime();

    if(rank==0) {
        std::cout << "Parallel Quicksort completed in " << end_time-start_time << " seconds.\n";

        // Optional: compare with sequential quicksort
        std::vector<int> arr_seq = arr;
        start_time = MPI_Wtime();
        sequential_quicksort(arr_seq);
        end_time = MPI_Wtime();
        std::cout << "Sequential Quicksort completed in " << end_time-start_time << " seconds.\n";
    }

    MPI_Finalize();
    return 0;
}

