#include<iostream>
#include<vector>

#include<mpi.h>

#include "include/cuda_kernel.cu"
#include "include/VectorProcessor.hpp"

using namespace std;

int main(int argc, char** argv){
    MPI_Init(&argc, &argv);
    
    int rank, size;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int n=100000;

    int local=n/size;

    vector<float> A(local), B(local), C(local);

    VectorProcessor obj(local);

    obj.initialize(A, B);

    gpuVectorAdd(A.data(), B.data(), C.data(), local);

    float localsum=thrustsum(C.data(), local);

    float globalsum;

    MPI_Reduce(&localsum, &globalsum, 1, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);

    if(rank==0){
        cout<<"Global sum = "<<globalsum<<endl;
    }

    obj.verify(A, B, C);

    MPI_Finalize();

    return 0;
}