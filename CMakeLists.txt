cmake_minimum_required(VERSION 3.24)

project(MatrixMultiplication
    LANGUAGES CXX CUDA
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CUDA_STANDARD 20)
set(CMAKE_CUDA_STANDARD_REQUIRED ON)

add_executable(matmul
    src/main.cpp
    src/Matrix.cpp
    src/Arguments.cpp
    src/Timer.cpp
    src/Utils.cpp
    src/cpu/matrix_mul.cpp
    src/cuda/matrix_mul.cu
)

target_include_directories(matmul
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)