#ifndef UTILS_HPP
#define UTILS_HPP

#include <vector>
#include <random>
#include <numeric>
#include <algorithm>
#include <stdexcept>
#include <cmath>
#include "Matrix.hpp"
using namespace std;

// Generate a random weight vector that sums to 1
vector<double> random_weights(size_t n) 
{
    if (n == 0) throw invalid_argument("Size of weight vector must be non-zero.");

    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(0.0, 1.0);

    vector<double> weights(n);
    for (size_t i = 0; i < n; ++i)
        weights[i] = dis(gen);

    double sum = accumulate(weights.begin(), weights.end(), 0.0);
    for (double& w : weights) w /= sum;

    return weights;
}

// Overload: takes in a generator to avoid reseeding every time
vector<double> random_weights(size_t n, mt19937& gen) {
    if (n == 0) throw invalid_argument("Size of weight vector must be non-zero.");

    uniform_real_distribution<> dis(0.0, 1.0);
    vector<double> weights(n);
    for (size_t i = 0; i < n; ++i)
        weights[i] = dis(gen);

    double sum = accumulate(weights.begin(), weights.end(), 0.0);
    for (double& w : weights) w /= sum;

    return weights;
}

// Compute dot product of two vectors
inline double dot_product(const vector<double>& a, const vector<double>& b) {
    if (a.size() != b.size()) throw invalid_argument("Vector sizes must match for dot product.");
    double result = 0.0;
    for (size_t i = 0; i < a.size(); ++i)
        result += a[i] * b[i];
    return result;
}

// Compute portfolio return given weights and mean returns
inline double portfolio_return(const vector<double>& weights, const vector<double>& mean_returns) 
{
    return dot_product(weights, mean_returns);
}

// Compute portfolio variance: w^T * Cov * w
inline double portfolio_variance(const vector<double>& weights, const Matrix& cov) 
{
    if (weights.size() != cov.num_rows() || cov.num_rows() != cov.num_cols())
        throw invalid_argument("Covariance matrix must be square and match weight vector size.");

    double var = 0.0;
    for (size_t i = 0; i < weights.size(); ++i)
        for (size_t j = 0; j < weights.size(); ++j)
            var += weights[i] * cov(i, j) * weights[j];
    return var;
}

// Compute Sharpe Ratio (risk-free rate default = 0)
inline double sharpe_ratio(double port_return, double port_stddev, double risk_free = 0.0) 
{
    if (port_stddev == 0.0) return 0.0;
    return (port_return - risk_free) / port_stddev;
}

#endif // UTILS_HPP
