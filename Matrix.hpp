#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <vector>
#include <stdexcept>
#include <iostream>
#include <iomanip>
#include <cmath>
using namespace std;

class Matrix 
{
private:
    vector<vector<double>> data;
    size_t rows, cols;

public:
    // Default constructor
    Matrix() : rows(0), cols(0), data() {}

    // Constructor for zero matrix
    Matrix(size_t r, size_t c) : rows(r), cols(c), data(r, vector<double>(c, 0.0)) {}

    // Constructor with initial data
    Matrix(const vector<vector<double>>& d) : data(d), rows(d.size()), cols(d[0].size()) 
    {
        for (const auto& row : d)
            if (row.size() != cols)
                throw invalid_argument("All rows must have the same number of columns.");
    }

    size_t num_rows() const { return rows; }
    size_t num_cols() const { return cols; }

    // Element access
    double& operator()(size_t r, size_t c) 
    {
        return data[r][c];
    }

    double operator()(size_t r, size_t c) const 
    {
        return data[r][c];
    }

    friend ostream& operator<<(ostream& out, const Matrix& mat) 
    {
        for (size_t i = 0; i < mat.rows; ++i) 
        {
            for (size_t j = 0; j < mat.cols; ++j)
                out << setw(10) << mat(i,j) << " ";
            out << endl;
        }
        return out;
    }


    // Matrix transpose
    Matrix transpose() const 
    {
        Matrix result(cols, rows);
        for (size_t i = 0; i < rows; ++i)
            for (size_t j = 0; j < cols; ++j)
                result(j, i) = data[i][j];
        return result;
    }

    // Dot product (Matrix multiplication)
    Matrix dot(const Matrix& other) const 
    {
        if (cols != other.rows)
            throw invalid_argument("Matrix dimension mismatch for multiplication");

        Matrix result(rows, other.cols);
        for (size_t i = 0; i < rows; ++i)
            for (size_t j = 0; j < other.cols; ++j)
                for (size_t k = 0; k < cols; ++k)
                    result(i, j) += data[i][k] * other(k, j);

        return result;
    }

    // Scalar operations
    Matrix operator*(double scalar) const 
    {
        Matrix result(*this);
        for (size_t i = 0; i < rows; ++i)
            for (size_t j = 0; j < cols; ++j)
                result(i, j) *= scalar;

        return result;
    }

    // Row mean (column-wise average across rows)
    vector<double> mean_per_column() const 
    {
        vector<double> mean(cols, 0.0);
        for (size_t j = 0; j < cols; ++j)
            for (size_t i = 0; i < rows; ++i)
                mean[j] += data[i][j];

        for (double& m : mean)
            m /= rows;

        return mean;
    }

    // Covariance matrix (unbiased estimator)
    Matrix covariance_matrix() const 
    {
        vector<double> means = mean_per_column();
        Matrix cov(cols, cols);

        for (size_t i = 0; i < cols; ++i) 
        {
            for (size_t j = 0; j < cols; ++j) 
            {
                double sum = 0.0;
                for (size_t k = 0; k < rows; ++k)
                    sum += (data[k][i] - means[i]) * (data[k][j] - means[j]);
                cov(i, j) = sum / (rows - 1);
            }
        }
        return cov;
    }
};

#endif // MATRIX_HPP