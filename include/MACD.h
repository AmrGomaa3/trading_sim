#ifndef MACD_H
#define MACD_H


#include "strategy.h"
#include <vector>


class MACD : public Strategy {
private:
    std::vector<double> short_ema;
    std::vector<double> long_ema;

    double seed(int t) const;
    double ema(int t, int day=-1) const;

public:
    MACD(const std::vector<double>& p, int s=12, int l=26);
    bool indicator(int day=-1) const;
};


#endif
