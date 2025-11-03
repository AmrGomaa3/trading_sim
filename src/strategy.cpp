#include "../include/strategy.h"
#include <vector>
#include <stdexcept>


Strategy::Strategy(const std::vector<double> &p, int s, int l) 
    : price(p), short_term(s), long_term(l), size(p.size()) {
    if (l <= 0 || s <= 0) throw std::invalid_argument("Long-term and short-term periods must be positive");
    if (l <= s) throw std::invalid_argument("Long-term period must be larger than short-term period");
    if (l > size) throw std::invalid_argument("Long-term period cannot be larger than data size");
}
