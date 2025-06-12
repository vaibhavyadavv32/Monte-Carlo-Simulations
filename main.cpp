#include "matrix.hpp"
#include "utils.hpp"
#include "portfolio.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#include <iostream>
#include <iomanip> // for std::setprecision
#include <vector>  // for std::vector
#include <random>  // for std::random_device, std::mt19937

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

    string header_line;
    if (!getline(fin, header_line)) 
    {
        cerr << "Failed to read header line from CSV." << endl;
        return 1;
    }
    
    // === Dynamically determine the number of stocks ===
    stringstream ss_header(header_line);
    string token;
    int total_columns = 0;
    while (getline(ss_header, token, ',')) 
    {
        total_columns++;
    }

    // The formula: (Total Columns - 1 for Date) / 2 (for prices and returns)
    if (total_columns <= 1 || (total_columns - 1) % 2 != 0) {
        cerr << "Error: Invalid CSV format. Expected Date column, N price columns, and N return columns." << endl;
        return 1;
    }
    const int num_stocks = (total_columns - 1) / 2;
    cout << "Detected " << num_stocks << " stocks from the CSV file." << endl;


    // === Read the rest of the data ===
    vector<vector<double>> return_data;
    string line;
    while (getline(fin, line)) 
    {
        stringstream ss(line);
        string cell;
        vector<double> row;

        // Skip date column
        getline(ss, cell, ','); 

        // Skip price columns using the dynamic count
        for (int i = 0; i < num_stocks; ++i) 
        {
            if (!getline(ss, cell, ',')) break; // Safety break
        }

        // Read daily return columns using the dynamic count
        for (int i = 0; i < num_stocks; ++i) 
        {
            if (!getline(ss, cell, ',')) break; // Safety break
            try {
                row.push_back(stod(cell));
            } catch (...) {
                row.push_back(nan("")); // Fallback for bad data
            }
        }
        
        // Ensure the row has the correct number of returns before adding
        if(row.size() == num_stocks) {
            return_data.push_back(row);
        }
    }
    fin.close();

    // === Construct matrix and portfolio statistics ===
    Matrix returns(return_data);
    Matrix cov = returns.covariance_matrix();
    vector<double> mean_returns = returns.mean_per_column();
    
    // Check if data was loaded correctly
    if(mean_returns.empty()){
        cerr << "No valid return data was loaded. Exiting." << endl;
        return 1;
    }

    for (size_t i = 0; i < mean_returns.size(); ++i)
        cout << "Mean return of asset " << i << " = " << mean_returns[i] << endl;

    const int trading_days = 252;
    const int simulations = 200000;

    // === Random generator ===
    random_device rd;
    mt19937 gen(rd());

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

        if (annual_std < 1e-3 || sharpe > 10) continue;

        sim_out << annual_ret << "," << annual_std << "," << sharpe;
        for (double w : weights) sim_out << "," << w;
        sim_out << "\n";

        if (sharpe > best_sharpe) 
        {
            best_sharpe = sharpe;
            best_weights = weights;
            best_ret = annual_ret;
            best_risk = annual_std;
        }

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
        cout << "   Asset " << i << ": " << best_weights[i] << endl;

    return 0;
}
