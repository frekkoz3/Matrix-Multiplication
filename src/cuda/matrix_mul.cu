#include "cuda/matrix_mul.cuh"

#include <iostream>
#include <cuda_runtime.h> // cudaMalloc, cudaMemcpy, cudaFree, cudaEvent (and relative methods)
#include <cublas_v2.h>    // cublasCreate, cublasSgemm, cublasDestroy

/*
    error checking: on failure print file, line and error, then exit
*/
#define CUDA_CHECK(call)                                                        \
    do {                                                                        \
        cudaError_t err = (call);                                               \
        if (err != cudaSuccess) {                                               \
            std::cerr << "CUDA error at " << __FILE__ << ":" << __LINE__        \
                      << ": " << cudaGetErrorString(err) << std::endl;          \
            std::exit(1);                                                       \
        }                                                                       \
    } while (0)

#define CUBLAS_CHECK(call)                                                      \
    do {                                                                        \
        cublasStatus_t status = (call);                                         \
        if (status != CUBLAS_STATUS_SUCCESS) {                                  \
            std::cerr << "cuBLAS error at " << __FILE__ << ":" << __LINE__      \
                      << ": " << cublasGetStatusString(status) << std::endl;    \
            std::exit(1);                                                       \
        }                                                                       \
    } while (0)


void cudaNaiveMatrixMultiply(
    const Matrix& A,
    const Matrix& B,
    Matrix& C,
    const size_t bytes,
    const size_t N,
    float& milliseconds
){
    C(0, 0) = A(0, 0) + B(0, 0); // placeholder
}

void cudaTilingMatrixMultiply(
    const Matrix& A,
    const Matrix& B,
    Matrix& C,
    const size_t bytes,
    const size_t N,
    float& milliseconds
){
    C(0, 0) = A(0, 0) + B(0, 0); // placeholder
}

void cudaCublasMatrixMultiply(
    const Matrix& A,
    const Matrix& B,
    Matrix& C,
    const size_t bytes,
    const size_t N,
    float& milliseconds
){  
    // device allocation
    float *d_A, *d_B, *d_C;
    CUDA_CHECK(cudaMalloc(&d_A, bytes));
    CUDA_CHECK(cudaMalloc(&d_B, bytes));
    CUDA_CHECK(cudaMalloc(&d_C, bytes));

    // cublas context, needed by every cublas call
    cublasHandle_t handle;
    CUBLAS_CHECK(cublasCreate(&handle));
    const float alpha = 1.0f, beta = 0.0f;

    // copy A and B host -> device
    CUDA_CHECK(cudaMemcpy(d_A, A.data(), bytes, cudaMemcpyHostToDevice));
    CUDA_CHECK(cudaMemcpy(d_B, B.data(), bytes, cudaMemcpyHostToDevice));

    // using cuda events
    cudaEvent_t start, stop;
    CUDA_CHECK(cudaEventCreate(&start));
    CUDA_CHECK(cudaEventCreate(&stop));

    // start
    CUDA_CHECK(cudaEventRecord(start)); 

    // C = A * B on the GPU.
    // cublas is column major: it reads our row major buffers as A^T and B^T.
    // Passing B first computes B^T * A^T = (A * B)^T in column major,
    // which in memory is exactly C = A * B in row major.
    // cublas takes sizes and leading dimensions as int: explicit cast (fine for N < 2^31)
    const int n = static_cast<int>(N);
    CUBLAS_CHECK(cublasSgemm(handle, CUBLAS_OP_N, CUBLAS_OP_N, n, n, n,  &alpha, d_B, n, d_A, n, &beta, d_C, n));
    
    // end
    CUDA_CHECK(cudaEventRecord(stop));
    CUDA_CHECK(cudaEventSynchronize(stop)); // synch

    // compute actual elapsed time in millisecond
    CUDA_CHECK(cudaEventElapsedTime(&milliseconds, start, stop));

    // destroy events
    CUDA_CHECK(cudaEventDestroy(start));
    CUDA_CHECK(cudaEventDestroy(stop));

    // copy C device -> host; this waits for the (asynchronous) cublas call to finish
    CUDA_CHECK(cudaMemcpy(C.data(), d_C, bytes, cudaMemcpyDeviceToHost));
    
    // free device memory and cublas context
    CUBLAS_CHECK(cublasDestroy(handle));
    CUDA_CHECK(cudaFree(d_A));
    CUDA_CHECK(cudaFree(d_B));
    CUDA_CHECK(cudaFree(d_C));
}