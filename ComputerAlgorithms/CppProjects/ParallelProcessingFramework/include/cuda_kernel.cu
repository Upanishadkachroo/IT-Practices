#include "../cuda/cuda_kernel.cuh"
#include<cuda_runtime.h>
#include<thrust/device_vector.h>
#include<thrust/reduce.h>

__global__ void addkernel(float *A, float *B, float *C, int n){
    int id=blockIdx.x * blockDIm.x + threadIdx.x;

    if(id<N){
        c[id] = A[id] + B[id];
    }
}

void gpuVectorAdd(float *A, float *B, float *C, int n){
    float *dA, *dB, *dC;

    cudaMalloc(&dA, N*sizeof(float));
    cudaMalloc(&dB, N*sizeof(float));
    cudaMalloc(&dB, N*sizeof(float));

    cudaMemcpy(dA, A, N*sizeof(float), cudaMemcpyHostToDevice);

    cudaMemcpy(dB, B, N*sizeof(float), cudaMemcpyHostToDevice);

    int threads=256;
    int block=(N + 255)/256;

    addkernel<<<blocks, threads>>>(dA, dB, dC);

    cudaMemcpy(C, dC, N*sizeof(float), cudaMemcpyDeviceToHost);

    cudaFree(dA);
    cudaFree(dB);
    cudaFree(dC);
}

float thrustsum(float *C, int n){
    thrust::device_vector<float> D(C, C+N);

    return thrust::reduce(D.begin(), D.end(), 0.0f, thrust::plus<float>());
}