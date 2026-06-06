#ifndef VECRORPROCESSOR_HPP
#define VECTORPROCESSOR_HPP

#include<vector>

class VectorProcessor{
    private:
    int size;

    public:
    VectorProcessor(int n);

    void initialize(std::vector<float> &A, std::vector<float> &B);

    void verify(std::vector<float> &A, std::vector<float> &B, std::vector<float> &C);
};

#endif
