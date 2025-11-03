#include "../include/MACD.h"
#include <gtest/gtest.h>
#include <stdexcept>
#include <vector>


static std::vector<double> data = {1.0, 1.0, 1.0, 100.0, 1.0};


TEST(TestMACD, GetsDefaultParameters) {
    std::vector<double> data(30, 10.0);
    MACD macd(data);

    EXPECT_EQ(macd.get_short_term(), 12);
    EXPECT_EQ(macd.get_long_term(), 26);
}


TEST(TestMACD, ThrowsInvalidArgument) {
    MACD macd(data, 1, 2);

    // day < long_term
    EXPECT_THROW(macd.indicator(1), std::invalid_argument);
    
    // day > size
    EXPECT_THROW(macd.indicator(6), std::invalid_argument);
}


TEST(TestMACD, IndicatorReturnsABool) {
    MACD macd(data, 2, 3);

    EXPECT_TRUE(macd.indicator(3));
    EXPECT_FALSE(macd.indicator(4));
}

