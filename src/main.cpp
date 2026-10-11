#include "Arguments.hpp"
#include "Timer.hpp"
#include "Utils.hpp"
#include "cpu/matrix_mul.hpp"
#include "cuda/matrix_mul.cuh"

#include <iostream>
#include <string>

int main(int argc, char* argv[])
{
    try
    {
        const Arguments args = parseArguments(argc, argv);

        std::cout << "Matrix Multiplication\n";
        std::cout << "---------------------\n";
        std::cout << "Matrix size: " << args.size << " x "
                  << args.size << '\n';
        std::cout << "Iterations:  " << args.iterations << '\n';
        std::cout << "Method:  " << args.method << '\n';

        
        size_t N = args.size;
        // this is all on the HOST
        Matrix A = randomMatrix(N, N);
        Matrix B = randomMatrix(N, N);
    
        /*
        size_t N = 2;

        Matrix A (N, N);
        Matrix B (N, N);

        A(0, 0) = 1;
        A(0, 1) = 2;
        A(1, 0) = 3;
        A(1, 1) = 4;

        B(0, 0) = 5;
        B(0, 1) = 6;
        B(1, 0) = 7;
        B(1, 1) = 8;
        */
        // this is need for device allocation 
        size_t bytes = N * N * sizeof(float);
        
        // benchmarking
        Matrix C(N, N);

        Timer timer;
        timer.start();

        float totalTime = 0.0;

        if (args.method == "cpu")
        {
            for (int iteration = 0; iteration < args.iterations; ++iteration)
            {   
                timer.start();
                cpuMatrixMultiply(A, B, C);
                totalTime += timer.stopMilliseconds(); 
            }
        }
        else if (args.method == "cuda_native")
        {
            // WARMUP
            for (int iteration = 0; iteration < args.iterations; ++iteration)
            {
                float gpuTime = 0.0f;
                cudaNaiveMatrixMultiply(A, B, C, bytes, N, gpuTime);
                totalTime += gpuTime;
            }
        }
        else if (args.method == "cuda_tiling")
        {
            // WARMUP
            for (int iteration = 0; iteration < args.iterations; ++iteration)
            {
                float gpuTime = 0.0f;
                cudaTilingMatrixMultiply(A, B, C, bytes, N, gpuTime);
                totalTime += gpuTime;
            }
        }
        else if (args.method == "cuda_cublas")
        {
            // WARMUP -> this is not really measured
            float gpuTime = 0.0;
            cudaCublasMatrixMultiply(A, B, C, bytes, N, gpuTime);
            for (int iteration = 0; iteration < args.iterations; ++iteration)
            {
                gpuTime = 0.0;
                cudaCublasMatrixMultiply(A, B, C, bytes, N, gpuTime);
                totalTime += gpuTime;
            }
        }

        const double averageTime = totalTime / args.iterations;

        std::cout << "Average time: "
                  << averageTime
                  << " ms\n";
        
        const double n = static_cast<double>(N);
        const double operations = 2.0 * n * n * n;
        // this is the one core definition
        // on squared matrices
        const double gflops = operations / (averageTime * 1e6); // it should be *1e9 but time is in millisecond

        std::cout << "GFLOPS: "
                << gflops
                << '\n';

        // verification
        // simplifying to speed things up
        // we can call this a sanity check
        double reference = 0.0;

        for (size_t k = 0; k < N; ++k)
        {
            reference += double(A(0, k)) * double(B(k, 0));
        }

        const double actual = static_cast<double>(C(0, 0));
        const double tolerance = 1e-4;

        std::cout << "C[0][0] GPU = " << actual
                << ", CPU = " << reference << '\n';

        const bool correct = std::abs(actual - reference) < tolerance;

        std::cout << "Verification: "
                << (correct ? "PASS" : "FAIL")
                << '\n';
                
    }
    catch (const std::exception& exception)
    {
        std::cerr << "Error: "
                  << exception.what()
                  << '\n';

        return 1;
    }

    return 0;
}