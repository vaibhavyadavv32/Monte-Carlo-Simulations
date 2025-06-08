#include <iostream>           // For standard I/O (cout, cerr)
#include <fstream>            // For file I/O (ifstream, ofstream)
#include <sstream>            // For string parsing (stringstream)
#include <vector>             // For dynamic arrays
#include <string>             // For string operations
#include <cmath>              // For log() function
using namespace std;

int main() 
{
    // Open the CSV file containing prices (stocks.csv)
    ifstream fin("stocks.csv");
    if (!fin.is_open()) 
    {
        cerr << "Failed to open input file." << endl;
        return 1;  // Exit if failure in opening (stocks.csv)
    }

    // Make a .csv file to accomodate daily returns along with prices (stocks_with_DailyReturns.csv)
    ofstream fout("stocks_with_DailyReturns.csv");
    if (!fout.is_open()) 
    {
        cerr << "Failed to open output file." << endl;
        return 1;   // Exit if failure in creation of (stocks_with_DailyReturns.csv)
    }

    string line;

    // =========== READ HEADER LINE (Date,AAPL,MSFT,...) ===========
    getline(fin, line);          // Read first line from CSV
    stringstream ss(line);       // Convert line into stream for splitting

    vector<string> headers;      // Vector to store header of input file (Date,AAPL,MSFT,...)
    string token;

    // Split by commas and store column names
    while (getline(ss, token, ',')) 
    {
        headers.push_back(token);
    }

    // =========== WRITE OUTPUT HEADER (INPUT HEADERS + RETURN COLUMNS) (Date,AAPL,MSFT,...,AAPL_DailyReturn,MSFT_DailyReturn,...)===========
    for (size_t i = 0; i < headers.size(); ++i) 
    {
        fout << headers[i];     // Copying input headers
        if (i != headers.size() - 1) fout << ",";
    }

    // Adding ...._DailyReturn Columns (one for each stock)
    for (size_t i = 1; i < headers.size(); ++i)  // Skip "Date" at index 0
    {
        fout << "," << headers[i] << "_DailyReturn";
    }
    fout << endl;  // End header

    // =========== PREPARE fOR PROCESSING DATA LINES =========== 
    vector<double> prev_prices(headers.size() - 1, 0.0); // Vector to store Previous day's prices
    bool first_line = true; // Day 1 has no previous day

    // =========== PROCESS EACH LINE OF PRICES =========== 
    while (getline(fin, line))      // Until the end of file
    {
        stringstream ss_line(line);   // Create a string stream of each line
        vector<string> row_data;      // To store current row's data
        string val;

        // Split line by commas and store each cell in row_data
        while (getline(ss_line, val, ',')) 
        {
            row_data.push_back(val);
        }

        // =========== ADDING ORIGINAL DATA TO OUTPUT FILE =========== 
        for (size_t i = 0; i < row_data.size(); ++i) 
        {
            fout << row_data[i];
            if (i != row_data.size() - 1) fout << ",";
        }

        // =========== CALCULATING AND ADDING LOG RETURNS =========== 
        for (size_t i = 1; i < row_data.size(); ++i)    // Skip "Date" column
        {
            double curr_price = 0.0;
            try 
            {
                curr_price = stod(row_data[i]);     // Price string to double
            } 
            catch (...)     // Handling Exceptions
            {
                curr_price = 0.0;
            }

            if (first_line) 
            {
                fout << ",0";
                prev_prices[i - 1] = curr_price;    // Store current price into previous
            } 
            else
            {
                if (prev_prices[i - 1] > 0 && curr_price > 0) 
                {
                    double log_return = log(curr_price / prev_prices[i - 1]);
                    fout << "," << log_return;      // Add log returns
                } 
                else 
                {
                    fout << ",0";   // Handling Unexpected Data
                }
                prev_prices[i - 1] = curr_price;    // Store current price into previous
            }
        }

        fout << endl;   // End current row
        first_line = false; // To disable first line after first iteration
    }

    // =========== CLEANUP =========== 
    fin.close();
    fout.close();

    cout << "Saved CSV with daily log returns: stocks_with_DailyReturns.csv\n";
    return 0;
}