#include "cpu/matrix_mul.hpp"

#include <stdexcept>

void cpuMatrixMultiply(
    const Matrix& A,
    const Matrix& B,
    Matrix& C
)
{
    if (A.cols() != B.rows())
    {
        throw std::invalid_argument(
            "Invalid matrix dimensions for multiplication"
        );
    }

    if (C.rows() != A.rows() || C.cols() != B.cols())
    {
        throw std::invalid_argument(
            "Output matrix has incorrect dimensions"
        );
    }

    for (std::size_t i = 0; i < A.rows(); ++i)
    {
        for (std::size_t j = 0; j < B.cols(); ++j)
        {
            float sum = 0.0f;

            for (std::size_t k = 0; k < A.cols(); ++k)
            {
                sum += A(i, k) * B(k, j);
            }

            C(i, j) = sum;
        }
    }
}