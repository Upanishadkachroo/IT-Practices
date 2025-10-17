#include <mpi.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>

using namespace std;

// Multiply two local blocks
void local_matrix_mult(vector<double>& A, vector<double>& B, vector<double>& C, int block_size) {
    for (int i = 0; i < block_size; i++)
        for (int j = 0; j < block_size; j++)
            for (int k = 0; k < block_size; k++)
                C[i*block_size + j] += A[i*block_size + k] * B[k*block_size + j];
}

// Initialize random matrix
void random_matrix(vector<double>& M, int N) {
    for (int i = 0; i < N*N; i++)
        M[i] = rand() % 10;
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int N = 8; // global matrix size
    int q = sqrt(size);
    if (q*q != size) {
        if(rank==0) cout << "Number of processes must be a perfect square!" << endl;
        MPI_Finalize();
        return 0;
    }

    int block_size = N / q;
    vector<double> A_block(block_size*block_size);
    vector<double> B_block(block_size*block_size);
    vector<double> C_block(block_size*block_size, 0);

    // Initialize blocks (for demo, random)
    srand(rank + time(0));
    random_matrix(A_block, block_size);
    random_matrix(B_block, block_size);

    // TODO: Implement Cannon algorithm: initial alignment + q steps of multiply+shift

    // For now just compute local multiplication (for testing)
    local_matrix_mult(A_block, B_block, C_block, block_size);

    // Print local C_block for verification
    for(int i=0;i<block_size;i++){
        for(int j=0;j<block_size;j++)
            cout<<C_block[i*block_size+j]<<" ";
        cout<<endl;
    }

    MPI_Finalize();
    return 0;
}

//mpic++ -O2 -o cannon_matrix cannon_matrix.cpp
//mpirun -np 4 ./cannon_matrix
//mpirun -np 9 ./cannon_matrix
