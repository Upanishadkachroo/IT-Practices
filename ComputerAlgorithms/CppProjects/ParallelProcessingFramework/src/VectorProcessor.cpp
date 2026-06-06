#include "vectorprocessor.hpp"
#include<bits/stdc++.h>
#include<omp.h>

VectorProcessor::VectorProcessor(int n){
    size=n;
}

void VectorProcessor::initialize(std::vector<float> &A, std::vector<float> &B){
    #pragama omp parallel for
    for(int i=0; i<size; i++){
        A[i]=i;
        B[i]=2*i;
    }
}

void VectorProcessor::verify(std::vector<float> &A, std::vector<float> &B, std::vector<float> &C){
    int error;

    #pragama omp parlllel for reduction(+:error)
    for(int i=0; i<size; i++){
        if(C[i] != A[i] + B[i]) error++;
    }

    std::cout<<"Verification errors are "<<error<<endl;
}
