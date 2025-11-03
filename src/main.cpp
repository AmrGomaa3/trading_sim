#ifndef DATA_DIR
#define DATA_DIR "../data/"
#endif


#include "../include/util.h"
#include "../include/MACD.h"
#include "../include/SMA.h"
#include "../include/simulator.h"
#include <cstddef>
#include <vector>
#include <string>
#include <string_view>
#include <stdexcept>
#include <optional>
#include <iostream>


constexpr int MAX_ARGS {5};


int main(int argc, char* argv[]) {

    if (argc > MAX_ARGS) {
        parser_error(argv[0]);
        return 2;
    }

    // check duplicate arguments
    // brute-force check (O(n²)), chosen deliberately over std::unordered_set
    // since MAX_ARGS = 5, this is faster, simpler, and avoids unnecessary hashing and allocations
    for (int i = 1; i < argc; i++) {
        std::string_view arg {argv[i]};

        for (int j = i + 1; j < argc; j++) {
            std::string_view arg_check {argv[j]};

            if (arg == arg_check) {
                std::cerr << "Error: duplicate argument '" << arg << "'\n";
                parser_error(argv[0]);
                return 2;
            }
        }
    }

    // default values in case of no flags used
    int no_of_stocks {1};
    std::string ticker_symbol {"---"};
    bool backtest_mode {true};
    bool indicator_mode {true};
    bool sma_on {true};
    bool macd_on {true};

    // check valid arguments
    for (int i = 1; i < argc; i++) {
        std::string arg {argv[i]};
        
        if (arg == "--help" || arg == "-h") {
            print_help();
            return 0;
        } else if (arg.rfind("--stocks=", 0) == 0 || arg.rfind("-sk=", 0) == 0) {
            std::size_t splitter = arg.find("=");
            std::string value = arg.substr(splitter + 1);

            // turn the string into an integer
            try {
                no_of_stocks = std::stoi(value);
            }
            catch (const std::invalid_argument& e) {
                std::cerr << "Error: '" << arg << "' is not a valid integer\n";
                return 2;
            }
            catch (const std::out_of_range& e) {
                std::cerr << "Error: '" << arg << "' is out of range for int\n";
                return 2;
            }
        } else if (arg.rfind("--ticker=", 0) == 0 || arg.rfind("-t=", 0) == 0) {
            std::size_t splitter = arg.find("=");
            ticker_symbol = arg.substr(splitter + 1);
        } else if (arg == "--mode=backtest" || arg == "-m=backtest") indicator_mode = false;
        else if (arg == "--mode=indicator" || arg == "-m=indicator") backtest_mode = false;
        else if (arg == "--strategy=macd" || arg == "-s=macd") sma_on = false;
        else if (arg == "--strategy=sma" || arg == "-s=sma") macd_on = false;
        else {
            parser_error(argv[0]);
            return 2;
        }
    }

    if (!backtest_mode && !indicator_mode) {
        std::cerr << "Error: conflicting modes specified\n"
                  << " --mode=backtest --mode=indicator\n";
        
        return 2;
    }

    if (!macd_on && !sma_on) {
        std::cerr << "Error: conflicting strategies specified\n"
                  << " --strategy=macd --strategy=sma\n";
        
        return 2;
    }

    std::vector<double> stock_data;

    try {
        stock_data = read_file(std::string(DATA_DIR) + "temp.csv"); // read data file
    }
    catch (std::runtime_error& e) {
        std::cerr << "Error: " << e.what() << "\n";
        
        return 3;
    }

    int days_analysed = stock_data.size();

    MACD macd(stock_data);
    SMA sma(stock_data);

    // optional TradingSim object to be defined later
    std::optional<Simulator> sim;

    // conditional construction of sim object (based on user flags)
    if (!macd_on) sim.emplace(sma, stock_data);
    else if (!sma_on) sim.emplace(macd, stock_data);
    else sim.emplace(sma, macd, stock_data);

    std::cout << "** Running tests **" << "\n\n"
              << " Stock: " << ticker_symbol << "\n"
              << " Current Price: " << stock_data.at(days_analysed - 1) << "\n"
              << " Days Analysed: " << days_analysed << " days\n"
              << " Simulating for: " << no_of_stocks << " stocks\n\n";
    
    if (indicator_mode) sim->indicator();

    std::cout << "\n";

    if (backtest_mode) sim->backtest(no_of_stocks);

    std::cout << "\nNote: transaction fees and dividends have not been factored in the calculations\n";

    return 0;
}

