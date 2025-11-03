#include "../include/util.h"
#include <exception>
#include <string_view>
#include <sstream>
#include <string>
#include <vector>
#include <fstream>
#include <stdexcept>
#include <iostream>


// read .csv file
std::vector<double> read_file(std::string_view file_name) {
    // open file
    std::ifstream file(file_name.data());

    // check if file opened correctly
    if (!file.is_open()) {
        std::ostringstream oss;
        oss << "Failed to open file: '" << file_name << "'";

        throw std::runtime_error(oss.str());
    }

    std::string line;
    std::vector<double> closingPrices;

    // read data until EOF
    while (std::getline(file, line)) {
        try {
            closingPrices.push_back(std::stod(line));
        }
        catch (const std::exception&) {
            throw std::runtime_error("Error: potentially corrupt data");
        }
    }
    return closingPrices;
}


// print parser error message
void parser_error(std::string_view argv0) {
    std::cerr << "Usage: " << argv0 << " [--ticker=ticker_symbol] "
              << "[--stocks=N] "
              << "[--mode=backtest | --mode=indicator] " 
              << "[--strategy=macd | --strategy=sma]\n";
}


// print help message
void print_help() {
    std::cout << R"(Usage:
  ./trading_sim [OPTIONS]

Description:
  A command-line trading simulator that analyzes stock data using
  Simple Moving Average (SMA) and Moving Average Convergence Divergence (MACD)
  strategies. Supports both indicator and backtesting modes.

Options:
  -t, --ticker=<symbol>         Specify the stock ticker symbol (1–5 characters).
                                Example: --ticker=AAPL

  -sk, --stocks=<n>             Set the number of stocks to simulate (integer > 0).
                                Default: 1

  -m, --mode=<type>             Select execution mode:
                                  indicator  Display indicator signals only
                                  backtest   Run historical performance simulation
                                Default: both modes run.

  -s, --strategy=<name>         Choose strategy:
                                  sma   Use Simple Moving Average
                                  macd  Use Moving Average Convergence Divergence
                                Default: both strategies used.

  -h, --help                    Show this help message and exit.

Notes:
  • Arguments are case-sensitive.
  • Conflicting or repeated options will cause an error.
  • Input data is read from: ../data/temp.csv

Examples:
  trading_sim --ticker=AAPL --stocks=10 --mode=backtest --strategy=sma
  trading_sim -t=TSLA -sk=5 -m=indicator -s=macd)" << "\n";
}

