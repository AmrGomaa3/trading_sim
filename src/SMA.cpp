#include "../include/SMA.h"
#include <vector>
#include <stdexcept>


SMA::SMA(const std::vector<double>& p, int s, int l)
    : Strategy(p, s, l) {}

double SMA::short_term_avg(int day) const {
    double sum {0};

    for (int i = day - short_term; i < day; i++) sum += price.at(i);

    return sum / short_term;
}

double SMA::long_term_avg(int day) const {
    double sum {0};

    for (int i = day - long_term; i < day; i++) sum += price.at(i);

    return sum / long_term;
}

// give buy/sell indication based on the comparison between short_term and long-term averages
bool SMA::indicator(int day) const {
    if (day == -1) day = size;
    if (day > size) throw std::invalid_argument("day cannot exceed available data size");
    if (day < long_term) throw std::invalid_argument("SMA indicator requested for day earlier than long-term period");

    return short_term_avg(day) > long_term_avg(day);
}

