cmake_minimum_required(VERSION 3.18)

project(
HybridParallelProject
LANGUAGES CXX CUDA)

set(CMAKE_CXX_STANDARD 17)

find_package(MPI REQUIRED)
find_package(OpenMP REQUIRED)

include_directories(

include
cuda

${MPI_INCLUDE_PATH}

)

add_executable(

hybrid

main.cpp

src/VectorProcessor.cpp

src/cuda_kernel.cu

)

target_link_libraries(

hybrid

MPI::MPI_CXX

OpenMP::OpenMP_CXX

cuda

cudart

)