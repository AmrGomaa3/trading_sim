#include "../include/strategy.h"
#include <gtest/gtest.h>
#include <vector>
#include <stdexcept>


std::vector<double> data = {2.1, 3.5, 8.0};


class TestStrategy : public Strategy {
public:
    TestStrategy(const std::vector<double>& p, int s, int l) 
        : Strategy(p, s, l) {}
    bool indicator(int /*day*/) const override { return true; }
};


TEST(TestStrategyClass, ThrowsInvalidArgument) {
    EXPECT_THROW(TestStrategy(data, 1, 4), std::invalid_argument);
    EXPECT_THROW(TestStrategy(data, 2, 1), std::invalid_argument);
    EXPECT_THROW(TestStrategy(data, 0, 2), std::invalid_argument);
}


TEST(TestStrategyClass, AcceptValidParameters) {
    EXPECT_NO_THROW(TestStrategy(data, 1, 3));
}


TEST(TestStrategyClass, GetsLongTermAndShortTerm) {
    TestStrategy testStrategy(data, 1, 2);

    EXPECT_EQ(testStrategy.get_short_term(), 1);
    EXPECT_EQ(testStrategy.get_long_term(), 2);
}

