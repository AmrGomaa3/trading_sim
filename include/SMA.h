#ifndef SMA_H
#define SMA_H


#include "strategy.h"
#include <vector>


class SMA : public Strategy {
private:
    double short_term_avg(int day=-1) const;
    double long_term_avg(int day=-1) const;

public:
    SMA(const std::vector<double>& p, int s=50, int l=200);
    bool indicator(int day=-1) const;
};

#endif
