#include "Utils.hpp"

#include <cmath>
#include <iomanip>
#include <iostream>
#include <random>
#include <stdexcept>

Matrix randomMatrix(
    std::size_t rows,
    std::size_t cols
)
{
    Matrix matrix(rows, cols);

    std::mt19937 generator(37); // lucky number
    std::uniform_real_distribution<float> distribution(-1.0f, 1.0f);

    for (std::size_t i = 0; i < rows; ++i)
    {
        for (std::size_t j = 0; j < cols; ++j)
        {
            matrix(i, j) = distribution(generator);
        }
    }

    return matrix;
}

bool compareMatrices(
    const Matrix& A,
    const Matrix& B,
    float tolerance
)
{
    if (A.rows() != B.rows() || A.cols() != B.cols())
    {
        return false;
    }

    for (std::size_t i = 0; i < A.rows(); ++i)
    {
        for (std::size_t j = 0; j < A.cols(); ++j)
        {
            if (std::fabs(A(i, j) - B(i, j)) > tolerance)
            {
                std::cerr << "Mismatch at (" << i << ", " << j << ")"
                << ": actual = " << A(i, j)
                << ", expected = " << B(i, j)
                << ", difference = "
                << std::abs(A(i, j) - B(i, j))
                << '\n';

                return false;
            }
        }
    }

    return true;
}

void printMatrix(const Matrix& matrix)
{
    for (std::size_t i = 0; i < matrix.rows(); ++i)
    {
        for (std::size_t j = 0; j < matrix.cols(); ++j)
        {
            std::cout
                << std::setw(10)
                << matrix(i, j)
                << ' ';
        }

        std::cout << '\n';
    }
}