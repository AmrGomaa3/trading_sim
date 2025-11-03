#include "../include/simulator.h"
#include <gtest/gtest.h>
#include <stdexcept>
#include <vector>


static std::vector<double> data = {1.0, 1.0, 1.0, 100.0, 1.0};
static SMA sma(data, 1, 2);
static MACD macd(data, 1, 2);


TEST(TestSimulator, ConstructorBranching) {
    EXPECT_NO_THROW(Simulator(sma, data));
    EXPECT_NO_THROW(Simulator(macd, data));
    EXPECT_NO_THROW(Simulator(sma, macd, data));
}


TEST(TestSimulator, ThrowsInvalidArgument) {
    Simulator sim(sma, data);

    // stocks <= 0
    EXPECT_THROW(sim.backtest(-1), std::invalid_argument);
}

