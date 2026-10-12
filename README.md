# Matrix Multiplication (CUDA)

This is an educational project to learn a bit about the CUDA framework, in particular by implementing a CUDA Kernel to do matrix multiplication, trying to improve it to achieve a performance as close as possible to the Cubla's one.

## Structure

```bash
matrix-multiplication/
│
├── include/                    # headers file
│   ├── cuda/
│   │    └── matrix_mul.cuh
│   ├── cpu/
│   │   └── matrix_mul.hpp
│   ├── Matrix.hpp
│   ├── Arguments.hpp
│   ├── Timer.hpp
│   └── Utils.hpp
│
├── results/                    # results files
│
├── src/                        # code base
│   ├── cpu/                    
│   │   └── matrix_mul.cpp      
│   ├── cuda/
│   │   └── matrix_mul.cu
│   ├── main.cpp
│   ├── Arguments.cpp
│   ├── Matrix.cpp
│   ├── Timer.cpp
│   └── Utils.cpp
│
├── .gitignore
├── Makefile
└── README.md
```

## Problem formulation

Given two matrices $A$ and $B$, the goal is to compute their product

$$
C = A \cdot B
$$

Each element of the resulting matrix C is computed as the dot product between a row of A and a column of B, ergo

$$
C_i^j = A_i \cdot B^j = \sum_k A_i^kb_k^j
$$

This problem is is particularly well suited to GPUs because it consists of a large number of similar, independent operations that can be executed in parallel.

The project aims is then to explore how to exploit that parallelism using CUDA and how different memory-access patterns and optimization techniques affect performance.

In the end the focus is to achieve the best relative perfomance with relative to the matmul implementation by cuBLAS.

## Implementation

The project includes multiple implementations of matrix multiplication for comparison:

1. Serial CPU, a straightforward sequential implementation
2. CUDA, GPU implementations with progressively more advanced optimizations
3. cuBLAS, used as a reference point for GPU performance

> **Warmup** As a best practice for gpu benchmarking, a single matrix multiplication is performed without being measured on GPU. This is the warmup phase and it is necessary to set up everything before measuring the actual computational performances.

## Key metrics tracked

The metrics that will be tracked are **total time** and **GFLOPS**.\
The first one is $t_{ms}$ and it is computed using inner-profiling expressed in millisecond.\
The second one is $GFLOPS$ and it is computed in the following way (with the assumption of the total workload required for the matrix multiplication of two squared matrices $2n^3$):
$$
GFLOPS = \frac{2n^3}{t_{ms} 10^6}
$$
All the hyperparameters used for each specific implementation will be tracked as well.

## CLI Interface

The command line interface have really simple flags:

```bash
--help          # Helper
--size          # Dimension of the square matrix
--iteration     # Number of iterations to do
--method        # Implementation to use
```
