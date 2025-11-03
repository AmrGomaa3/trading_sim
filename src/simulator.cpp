#include "../include/simulator.h"
#include <stdexcept>
#include <optional>
#include <iostream>
#include <cmath>
#include <algorithm>


Simulator::Simulator(const SMA& s, const std::vector<double>& p) 
    : sma(s), macd(std::nullopt), price(p), size(p.size()), start_day(s.get_long_term()) {}

Simulator::Simulator(const MACD& m, const std::vector<double>& p) 
    : sma(std::nullopt), macd(m), price(p), size(p.size()), start_day(m.get_long_term()) {}

Simulator::Simulator(const SMA& s, const MACD& m, const std::vector<double>& p) 
    : sma(s), macd(m), price(p), size(p.size()),
      start_day(std::max(s.get_long_term(), m.get_long_term())) {}

void Simulator::break_line() const {
    std::cout << "------------------------------" << "\n";
}

void Simulator::indicator_sma(bool signal) const {
    std::cout << " Strategy: SMA (" << sma->get_short_term() 
              << "/" << sma->get_long_term() << ")" << "\n"
              << " Signal: ";

    // conditional signal and coloring
    std::cout << ((signal) ? "\033[32mBUY\033[0m" : "\033[31mSELL\033[0m") << "\n";

    break_line();
}

void Simulator::indicator_macd(bool signal) const {
    std::cout << " Strategy: MACD (" << macd->get_short_term() 
              << "/" << macd->get_long_term() << ")" << "\n"
              << " Signal: ";

    // consditional signal and coloring
    std::cout << ((signal) ? "\033[32mBUY\033[0m" : "\033[31mSELL\033[0m") << "\n";

    break_line();
}

// give live-data indicator for buy/sell
void Simulator::indicator() const {
    if (!sma && !macd) return;

    int recommendation {};

    std::cout << " [ Trading Signal ]" << "\n\n";

    if (sma) {
        bool sma_signal {sma->indicator()};

        indicator_sma(sma_signal);

        if (sma_signal) recommendation++;
        else recommendation--;
    }    

    if (macd) {
        bool macd_signal {macd->indicator()};

        indicator_macd(macd_signal);

        if (macd_signal) recommendation++;
        else recommendation--;
    }

    // give final recommendation
    if (recommendation > 1) std::cout << " Recommendation: \033[32mSTRONG BUY\033[0m" << "\n";
    if (recommendation < -1) std::cout << " Recommendation: \033[31mSTRONG SELL\033[0m" << "\n";
    if (recommendation == 0) std::cout << " Recommendation: MIXED SIGNAL" << "\n";
}

// conditional naming and coloring
void Simulator::print_profit(double profit, double percent) const {
    if (profit > 0) {
        std::cout << "\033[32m Profit: +$" << profit << "\n"
                  << " Percentage: +" << percent << "%\033[0m\n";
    } else if (profit < 0) {
        std::cout << "\033[31m Losses: -$" << std::abs(profit) << "\n"
                  << " Percentage: -" << std::abs(percent) << "%\033[0m\n";
    } else {
        std::cout << " Profit: ---" << "\n" << " Percentage: ---" << "\n";
    }
}

// simulating a trading strategy
void Simulator::backtest_strategy(bool is_sma, int stocks) const {
    int transactions {};
    double profit {};
    double percent {};
    bool bought {false};
    double buy_price {};
    double sell_price {};
    double initial_buy {};
    bool is_initial_buy {false};
    bool decision {};

    for (int i = start_day; i < size; i++) {
        decision = (is_sma) ? sma->indicator(i) : macd->indicator(i);

        // detect buy move
        if (decision && !bought) {
            transactions++;
            buy_price = price.at(i);
            bought = true;

            if (!is_initial_buy) {
                is_initial_buy = true;
                initial_buy = buy_price;
            }
        }

        // detect sell move
        if ((!decision && bought) || ((i == size - 1) && bought)) {
            transactions++;
            sell_price = price.at(i);
            profit += (sell_price - buy_price) * stocks;
            bought = false;
        }
    }

    percent = ((profit / stocks) / initial_buy) * 100;

    std::cout << " Strategy: " << ((is_sma) ? "SMA" : "MACD") << "\n"
              << " No. of transactions: " << transactions << "\n";

    print_profit(profit, percent);

    break_line();
}

// simulating buy and hold strategy
void Simulator::backtest_bnh(int stocks) const {
    double buy_price {price.at(0)};
    double sell_price {price.at(price.size() - 1)};
    double profit {(sell_price - buy_price) * stocks};
    double percent {((profit / stocks) / buy_price) * 100};

    std::cout << " Strategy: Buy and hold" << "\n"
              << " Starting buy price: " << buy_price << "\n"
              << " Final sell price: " << sell_price << "\n";

    print_profit(profit, percent);
}

// simulating historical backtest and compare to buy and hold
void Simulator::backtest(int stocks) const {
    if (stocks <= 0) throw std::invalid_argument("Error: Invalid number of stocks");
    
    if (!sma && !macd) return;

    std::cout << " [ Backtest Results ]" << "\n\n";

    if (sma) backtest_strategy(true, stocks);

    if (macd) backtest_strategy(false, stocks);

    backtest_bnh(stocks);
}

