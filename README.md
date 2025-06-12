
# Portfolio Optimization using Monte Carlo Simulation

This project implements a portfolio optimization tool using Monte Carlo simulations to find the optimal asset weights that maximize the Sharpe ratio. It includes C++ code for data processing and simulation, as well as Python code for interactive visualization of the efficient frontier.

---

## Project Structure

```
/ProjectRoot
│
├── /Sample Data
│    ├── stocks.csv                  # Raw stock price data
│    ├── stocks_with_DailyReturns.csv # Daily returns derived from stock prices
│    ├── MonteCarloResults.csv       # Monte Carlo simulation output
│    └── Visualisation.html           # Interactive visualization HTML file
│
├── src
│    ├── main.cpp                    # Main C++ driver program
│    ├── matrix.hpp                 # Matrix class for numerical operations
│    ├── portfolio.hpp              # Portfolio class for return, risk, Sharpe ratio
│    └── utils.hpp                  # Utility functions (random weights, dot product, etc.)
│
├── Visualisation.py                # Python script for interactive plot using Plotly
├── README.md                      # This file
```

---

## Features

- Load historical daily returns from CSV data.
- Calculate covariance matrix and mean returns of assets.
- Perform Monte Carlo simulations to generate random portfolios.
- Compute portfolio return, risk (standard deviation), and Sharpe ratio.
- Identify and output the portfolio with the highest Sharpe ratio.
- Save simulation results to CSV for further analysis.
- Generate interactive visualization of efficient frontier using Plotly.

---

## Prerequisites

- **C++ Compiler** supporting C++11 or higher (e.g., g++, clang++)
- **Python 3.6+** with the following packages:
  - pandas
  - plotly
  - yfinance

You can install the Python packages via:

```bash
pip install pandas plotly yfina
```

---

## Usage

### Step 1: Prepare data

- Use the sample data files in `Sample Data/` for a quick start.
- The file `stocks_with_DailyReturns.csv` contains precomputed daily returns for 10 assets.

### Step 2: Build and run C++ simulation

```bash
g++ -std=c++11 -O2 main.cpp -o portfolio_sim
./portfolio_sim
```

- This reads `stocks_with_DailyReturns.csv` from the current directory.
- Generates `MonteCarloResults.csv` with simulated portfolio metrics and weights.

### Step 3: Visualize results with Python

```bash
python Visualisation.py
```

- Loads `MonteCarloResults.csv` to produce an interactive scatter plot.
- Opens a browser window with the efficient frontier visualization.
- Saves `Visualisation.html` which can be opened standalone later.

---

## File Descriptions

### C++ Source

- **matrix.hpp**: Implements a basic Matrix class supporting operations like transpose, dot product, covariance, and mean calculation.
- **portfolio.hpp**: Defines a Portfolio class to compute portfolio return, variance, risk, and Sharpe ratio given weights.
- **utils.hpp**: Helper functions for random portfolio weight generation, dot products, and portfolio statistics.
- **main.cpp**: Reads data, runs Monte Carlo simulations, writes output, and reports best portfolio.

### Python Script

- **Visualisation.py**: Uses Plotly to create an interactive plot of portfolio risk vs return colored by Sharpe ratio. Highlights the optimal portfolio.

---

## Notes

- The portfolio weights are generated randomly such that they sum to 1.
- Annualized returns and risks assume 252 trading days per year.
- Sharpe ratio calculation assumes a risk-free rate of 0 by default (can be changed in code).
- Invalid or degenerate portfolios (zero risk or NaN Sharpe) are skipped.

---

## Contact

For questions, improvements, or collaboration, feel free to reach out!

---

*Built with ❤️ by Vaibhav — Engineering Student at IIT Madras*
