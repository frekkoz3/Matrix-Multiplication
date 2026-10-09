#include "Arguments.hpp"
#include "Timer.hpp"
#include "Utils.hpp"
#include "cpu/matrix_mul.hpp"
#include "cuda/matrix_mul.cuh"

#include <iostream>

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

        Matrix A = randomMatrix(args.size, args.size);
        Matrix B = randomMatrix(args.size, args.size);

        // compute reference only once
        Matrix reference(args.size, args.size);

        if (args.verify)
        {
            matrixMultiply(A, B, reference);
        }
        
        // benchmarking
        Matrix C(args.size, args.size);

        Timer timer;

        double totalTime = 0.0;

        for (int iteration = 0; iteration < args.iterations; ++iteration)
        {
            timer.start();

            matrixMultiply(A, B, C);

            totalTime += timer.stopMilliseconds();
        }

        const double averageTime = totalTime / args.iterations;

        std::cout << "Average time: "
                  << averageTime
                  << " ms\n";
        
        const double n = static_cast<double>(args.size);
        const double operations = 2.0 * n * n * n;
        // this is the one core definition
        // on squared matrices
        const double gflops = operations / (averageTime * 1e6); // it should be *1e9 but time is in millisecond

        std::cout << "GFLOPS: "
                << gflops
                << '\n';

        // verification
        if (args.verify)
        {
            const bool correct = compareMatrices(C, reference, 1e-5f);

            std::cout << "Verification: "
                      << (correct ? "PASS" : "FAIL")
                      << '\n';
        }
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