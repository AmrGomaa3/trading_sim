#ifndef STRATEGY_H
#define STRATEGY_H

#include <vector>


class Strategy {
protected:
    const std::vector<double>& price;
    int short_term;
    int long_term;
    int size;

public:
    Strategy(const std::vector<double>& p, int s, int l);
    int get_short_term() const { return short_term; }
    int get_long_term() const { return long_term; }
    virtual bool indicator(int day=-1) const = 0;
    virtual ~Strategy() = default;
};

#endif
