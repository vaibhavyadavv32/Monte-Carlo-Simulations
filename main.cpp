#include "matrix.hpp"
#include "utils.hpp"
#include "portfolio.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#include <iostream>
#include <iomanip> // for std::setprecision
using namespace std;

int main() 
{
    // === Load daily return data from CSV ===
    ifstream fin("stocks_with_DailyReturns.csv");
    if (!fin.is_open()) 
    {
        cerr << "Failed to open CSV file." << endl;
        return 1;
    }

    string line;
    getline(fin, line); // skip header

    vector<vector<double>> return_data;
    while (getline(fin, line)) 
    {
        stringstream ss(line);
        string cell;
        vector<double> row;

        getline(ss, cell, ','); // skip date column

        // Skip price columns (10)
        for (int i = 0; i < 10; ++i) getline(ss, cell, ',');

        // Read 10 daily return columns
        for (int i = 0; i < 10; ++i) 
        {
            getline(ss, cell, ',');
            try {
                row.push_back(stod(cell));
            } catch (...) {
                row.push_back(0.0); // fallback in case of bad data
            }
        }

        return_data.push_back(row);
    }
    fin.close();

    // === Construct matrix and portfolio statistics ===
    Matrix returns(return_data);
    Matrix cov = returns.covariance_matrix();
    vector<double> mean_returns = returns.mean_per_column();
    Portfolio port(returns);

    const int trading_days = 252;
    const int simulations = 200'000;

    // === Random generator ===
    random_device rd;
    mt19937 gen(rd());  // seed with a random device for true randomness

    // === Simulation result variables ===
    double best_sharpe = -1e9;
    vector<double> best_weights;
    double best_ret = 0.0, best_risk = 0.0;

    // === Open file to store simulation results ===
    ofstream sim_out("MonteCarloResults.csv");
    if (!sim_out.is_open()) 
    {
        cerr << "Failed to open MonteCarloResults.csv for writing." << endl;
        return 1;
    }

    // === Write CSV header ===
    sim_out << "Return,Risk,Sharpe";
    for (size_t i = 0; i < mean_returns.size(); ++i)
        sim_out << ",W" << i;
    sim_out << "\n";

    // === Run Monte Carlo simulations ===
    for (int i = 0; i < simulations; ++i) 
    {
        vector<double> weights = random_weights(mean_returns.size(), gen);
        double port_ret = dot_product(weights, mean_returns);
        double port_var = portfolio_variance(weights, cov);
        double port_std = sqrt(port_var);

        double annual_ret = port_ret * trading_days;
        double annual_std = port_std * sqrt(trading_days);
        double sharpe = sharpe_ratio(annual_ret, annual_std);

        // Skip invalid or degenerate portfolios
        if (std::isnan(sharpe) || annual_std < 1e-6) continue;

        // Write simulation to CSV
        sim_out << annual_ret << "," << annual_std << "," << sharpe;
        for (double w : weights) sim_out << "," << w;
        sim_out << "\n";

        // Track the best Sharpe ratio
        if (sharpe > best_sharpe) 
        {
            best_sharpe = sharpe;
            best_weights = weights;
            best_ret = annual_ret;
            best_risk = annual_std;
        }

        // Optional: Progress display
        if (i % 100000 == 0 && i > 0)
            cerr << "Simulated " << i << " portfolios...\n";
    }

    sim_out.close();

    // === Final Output ===
    cout << fixed << setprecision(6);
    cout << "\nBest Annualized Sharpe Ratio: " << best_sharpe << endl;
    cout << "Best Annualized Return      : " << best_ret << endl;
    cout << "Best Annualized Risk (Std)  : " << best_risk << endl;

    cout << "Optimal Portfolio Weights:\n";
    for (size_t i = 0; i < best_weights.size(); ++i)
        cout << "  Asset " << i << ": " << best_weights[i] << endl;

    return 0;
}
