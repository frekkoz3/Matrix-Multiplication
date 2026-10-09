#include "Matrix.hpp"

// row-major
Matrix::Matrix(std::size_t rows, std::size_t cols)
    : rows_(rows),
      cols_(cols),
      data_(rows * cols, 0.0f)
{
}

std::size_t Matrix::rows() const
{
    return rows_;
}

std::size_t Matrix::cols() const
{
    return cols_;
}

float& Matrix::operator()(std::size_t row, std::size_t col)
{
    return data_[row * cols_ + col];
}

const float& Matrix::operator()(
    std::size_t row,
    std::size_t col
) const
{
    return data_[row * cols_ + col];
}

float* Matrix::data()
{
    return data_.data();
}

const float* Matrix::data() const
{
    return data_.data();
}