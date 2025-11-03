#include "../include/SMA.h"
#include <gtest/gtest.h>
#include <stdexcept>
#include <vector>


static std::vector<double> data = {10.0, 20.0, 30.0, 20.0, 10.0};


TEST(TestSMA, GetsDefaultParameters) {
    std::vector<double> data(250, 10.0);
    SMA sma(data);

    EXPECT_EQ(sma.get_short_term(), 50);
    EXPECT_EQ(sma.get_long_term(), 200);
}


TEST(TestSMA, ThrowsInvalidArgument) {
    SMA sma(data, 1, 2);

    // day < long_term
    EXPECT_THROW(sma.indicator(1), std::invalid_argument);
    
    // day > size
    EXPECT_THROW(sma.indicator(6), std::invalid_argument);
}


TEST(TestSMA, IndicatorReturnsABool) {
    SMA sma(data, 2, 3);

    EXPECT_TRUE(sma.indicator(3));
    EXPECT_FALSE(sma.indicator(5));
}

