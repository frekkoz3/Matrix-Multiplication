#pragma once

#include <Matrix.hpp>

#define TILE_SIZE 16
#define BLOCK_SIZE 16

void cudaNaiveMatrixMultiply(
    const Matrix& A,
    const Matrix& B,
    Matrix& C,
    const size_t bytes,
    const size_t N,
    float& milliseconds
);

void cudaTilingMatrixMultiply(
    const Matrix& A,
    const Matrix& B,
    Matrix& C,
    const size_t bytes,
    const size_t N,
    float& milliseconds
);

void cudaCublasMatrixMultiply(
    const Matrix& A,
    const Matrix& B,
    Matrix& C,
    const size_t bytes,
    const size_t N,
    float& milliseconds
);