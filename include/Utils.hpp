#pragma once

#include "Matrix.hpp"

Matrix randomMatrix(
    std::size_t rows,
    std::size_t cols
);

bool compareMatrices(
    const Matrix& A,
    const Matrix& B,
    float tolerance
);

void printMatrix(const Matrix& matrix);