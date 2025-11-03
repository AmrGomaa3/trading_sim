#ifndef SIMULATOR_H
#define SIMULATOR_H


#include "./SMA.h"
#include "./MACD.h"
#include <optional>
#include <vector>


class Simulator {
private:
    std::optional<SMA> sma;
    std::optional<MACD> macd;
    const std::vector<double>& price;
    const int size;
    const int start_day;

    void indicator_sma(bool signal) const;
    void indicator_macd(bool signal) const;
    void backtest_strategy(bool is_sma, int stocks=1) const;
    void backtest_bnh(int stocks=1) const;
    void break_line() const;
    void print_profit(double profit, double percent) const;

public:
    Simulator(const SMA& s, const std::vector<double>& p);
    Simulator(const MACD& m, const std::vector<double>& p);
    Simulator(const SMA& s, const MACD& m, const std::vector<double>& p);

    void indicator() const;
    void backtest(int stocks=1) const;
};


#endif
