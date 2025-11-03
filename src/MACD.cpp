#include "../include/MACD.h"
#include <vector>
#include <stdexcept>


MACD::MACD(const std::vector<double> &p, int s, int l)
    : Strategy(p, s, l)
{
    // store EMAs for efficiency usage later
    double EMA {seed(short_term)};
    double k {2.0 / (short_term + 1)};

    for (int i = short_term; i < size; i++) {
        EMA = (price.at(i) * k) + (EMA * (1 - k));
        short_ema.push_back(EMA);
    }

    EMA = {seed(long_term)};
    k = 2.0 / (long_term + 1);

    for (int i = long_term; i < size; i++) {
        EMA = (price.at(i) * k) + (EMA * (1 - k));
        long_ema.push_back(EMA);
    }
}

double MACD::seed(int t) const {
    if (t <= 0) throw std::invalid_argument("t must be positive");
    
    double sum {0};
    
    for (int i = 0; i < t; i++) sum += price.at(i);
    
    return sum / t;
}

bool MACD::indicator(int day) const {
    if (day == -1) day = size;
    if (day > size) throw std::invalid_argument("day cannot exceed available data size");
    if (day < long_term) throw std::invalid_argument("MACD indicator requested for day earlier than long-term period");
    
    // MACD calculation now O(1) because EMAs are pre-stored
    // MACD calculation done with no signal line for simplicity
    return (short_ema[day - short_term] - long_ema[day - long_term]) > 0;
}
