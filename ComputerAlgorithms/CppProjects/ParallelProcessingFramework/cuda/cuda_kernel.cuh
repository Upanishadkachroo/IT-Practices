#ifndef CUDA_KERNEL_CUH
#define CUDA_KERNEL_CUH

void gpuVectorAdd(float *A, float *B, float *C, int n);

float thrustsum(float *C, int n);

#endif