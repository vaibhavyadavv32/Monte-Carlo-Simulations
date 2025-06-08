#ifndef PORTFOLIO_HPP
#define PORTFOLIO_HPP

#include "matrix.hpp"
#include <vector>
#include <numeric> // std::accumulate
#include <cmath>
#include <stdexcept>
using namespace std;

class Portfolio 
{
private:
    Matrix returns;              // T x N matrix (T days, N assets)
    Matrix cov_matrix;           // N x N covariance matrix
    vector<double> means;        // mean return per asset
    size_t num_assets;

public:
    // Constructor: takes return matrix as input
    Portfolio(const Matrix& R) : returns(R)
    {
        if (R.num_rows() < 2)
            throw invalid_argument("Need at least 2 rows of returns.");

        num_assets = R.num_cols();
        means = R.mean_per_column();
        cov_matrix = R.covariance_matrix();
    }

    // Portfolio return = w^T * mean_returns
    double portfolio_return(const vector<double>& weights) const 
    {
        if (weights.size() != num_assets)
            throw invalid_argument("Weight vector size mismatch.");

        double ret = 0.0;
        for (size_t i = 0; i < num_assets; ++i)
            ret += weights[i] * means[i];
        return ret;
    }

    // Portfolio variance = w^T * cov * w
    double portfolio_variance(const std::vector<double>& weights) const 
    {
        if (weights.size() != num_assets)
            throw invalid_argument("Weight vector size mismatch.");

        double var = 0.0;
        for (size_t i = 0; i < num_assets; ++i)
            for (size_t j = 0; j < num_assets; ++j)
                var += weights[i] * weights[j] * cov_matrix(i, j);

        return var;
    }

    // Portfolio standard deviation (risk)
    double portfolio_risk(const vector<double>& weights) const 
    {
        return sqrt(portfolio_variance(weights));
    }

    // Sharpe Ratio = (Rp - Rf) / sigma
    double sharpe_ratio(const vector<double>& weights, double risk_free = 0.0) const 
    {
        double port_ret = portfolio_return(weights);
        double port_risk = portfolio_risk(weights);
        if (port_risk == 0.0) return 0.0; // avoid division by zero
        return (port_ret - risk_free) / port_risk;
    }
};

#endif // PORTFOLIO_HPP