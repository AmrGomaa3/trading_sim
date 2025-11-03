# Trading Simulator

A command-line C++ application for live-data indicator, backtesting and analyzing stock trading strategies using technical indicators. The simulator implements Simple Moving Average (SMA) and Moving Average Convergence Divergence (MACD) strategies to generate trading signals and evaluate historical performance.

## Features

- **Multiple Trading Strategies**
  - Simple Moving Average (SMA) with configurable short/long-term periods (default: 50/200 days)
  - Moving Average Convergence Divergence (MACD) with configurable periods (default: 12/26 days)
  - Combined strategy analysis for comprehensive signals

- **Dual Operating Modes**
  - **Indicator Mode**: Real-time buy/sell signals based on latest data
  - **Backtest Mode**: Historical performance simulation with profit/loss calculations

- **Automated Data Pipeline**
  - Python-based data fetching via Yahoo Finance API
  - Configurable time periods (1-5 years or custom date ranges)
  - Automatic CSV export and processing

- **Performance Analysis**
  - Transaction tracking and profit/loss calculations
  - Percentage return comparisons
  - Buy-and-hold baseline comparison
  - Color-coded terminal output for signal visualization

## Technical Architecture

```
Strategy (abstract base)
├── SMA (Simple Moving Average)
└── MACD (Moving Average Convergence Divergence)

Simulator
├── Uses: std::optional<SMA>
├── Uses: std::optional<MACD>
└── Analyzes: std::vector<double> price data
```

### Key Implementation Details

- **MACD Optimization**: Pre-computes and stores Exponential Moving Averages (EMAs) during construction for O(1) indicator lookups
- **Memory Safety**: Uses references and `std::optional` instead of raw pointers
- **Error Handling**: Exception-based validation with specific error messages
- **Modern C++**: Leverages C++17 features (`std::optional`, `std::string_view`)

## Prerequisites

- **C++ Compiler**: g++ with C++17 support
- **Build Tools**: GNU Make
- **Python**: 3.6+ with pip
- **Testing Framework**: Google Test (for running unit tests)

## Installation

### 1. Install Python Dependencies

```bash
make install-deps

# or manually:
pip install -r requirements.txt
```

### 2. Build the Project

```bash
make all
```

This compiles the main executable (`bin/trading_sim`) and test suite (`bin/tests`).

## Usage

### Basic Workflow

The simulator follows a two-stage pipeline:

1. **Data Fetching and Cleaning** (Python): Downloads historical stock data from Yahoo Finance and saves the closing price data to `data/temp.csv`
2. **Analysis** (C++): Processes data and runs strategy simulations

### Quick Start

Run a complete analysis (fetch data + simulate):

```bash
make run TICKER=AAPL
```

### Command-Line Options

```bash
python3 fetch_ticker_data.py -h

usage: fetch_ticker_data.py [-h] [--period {1,2,3,4,5}] [--start START]
                            [--end END] [--export]
                            ticker

TradingSim: run backtesting or live-data indicators on stock data

positional arguments:
  ticker                Stock ticker symbol (i.e. AAPL)

options:
  -h, --help            show this help message and exit
  --period {1,2,3,4,5}, -p {1,2,3,4,5}
                        Backtest period in years (i.e. 1)
  --start START, -s START
                        Backtest start date (%Y-%m-%d)
  --end END, -e END     Backtest end period (%Y-%m-%d)
  --export, -x          Export data file to 'tradingSim/data/'
```


```bash
./bin/trading_sim -h

Usage:
  ./trading_sim [OPTIONS]

Description:
  A command-line trading simulator that analyzes stock data using
  Simple Moving Average (SMA) and Moving Average Convergence Divergence (MACD)
  strategies. Supports both indicator and backtesting modes.

Options:
  -t, --ticker=<symbol>         Specify the stock ticker symbol (1–5 characters).
                                Example: --ticker=AAPL

  -sk, --stocks=<n>             Set the number of stocks to simulate (integer > 0).
                                Default: 1

  -m, --mode=<type>             Select execution mode:
                                  indicator  Display indicator signals only
                                  backtest   Run historical performance simulation
                                Default: both modes run.

  -s, --strategy=<name>         Choose strategy:
                                  sma   Use Simple Moving Average
                                  macd  Use Moving Average Convergence Divergence
                                Default: both strategies used.

  -h, --help                    Show this help message and exit.

Notes:
  • Arguments are case-sensitive.
  • Conflicting or repeated options will cause an error.
  • Input data is read from: ../data/temp.csv

Examples:
  trading_sim --ticker=AAPL --stocks=10 --mode=backtest --strategy=sma
  trading_sim -t=TSLA -sk=5 -m=indicator -s=macd
```

### Usage Examples

**Example 1: Full analysis with custom stock quantity**
```bash
make run TICKER=TSLA STOCKS=100 PERIOD=3
```

**Example 2: Indicator signals only (latest data)**
```bash
./bin/trading_sim --ticker=MSFT --mode=indicator
```

**Example 3: Backtest with SMA strategy only**
```bash
./bin/trading_sim --ticker=AAPL --stocks=50 --mode=backtest --strategy=sma
```

**Example 4: Custom date range**
```bash
python3 ./scripts/fetch_ticker_data.py AAPL --start=2020-01-01 --end=2023-12-31 -x
make run-offline TICKER=AAPL STOCKS=500
```

### Sample Run

```bash
make run TICKER=TSLA STOCKS=500 PERIOD=2

Fetching stock data for 'TSLA'...
Data for 'TSLA' saved to 'data/TSLA_2023-11-04_to_2025-11-03.csv'
Running trading_sim for 'TSLA'...
** Running tests **

 Stock: TSLA
 Current Price: 456.56
 Days Analysed: 499 days
 Simulating for: 500 stocks

 [ Trading Signal ]

 Strategy: SMA (50/200)
 Signal: BUY
------------------------------
 Strategy: MACD (12/26)
 Signal: SELL
------------------------------
 Recommendation: MIXED SIGNAL

 [ Backtest Results ]

 Strategy: SMA
 No. of transactions: 6
 Profit: +$34375
 Percentage: +31.2046%
------------------------------
 Strategy: MACD
 No. of transactions: 10
 Profit: +$109630
 Percentage: +95.26%
------------------------------
 Strategy: Buy and hold
 Starting buy price: 219.27
 Final sell price: 456.56
 Profit: +$118645
 Percentage: +108.218%

Note: transaction fees and dividends have not been factored in the calculations
```

## Testing

### Run All Tests

```bash
make test

Running tests...
Running main() from ./googletest/src/gtest_main.cc
[==========] Running 14 tests from 5 test suites.
[----------] Global test environment set-up.
[----------] 3 tests from TestStrategyClass
[ RUN      ] TestStrategyClass.ThrowsInvalidArgument
[       OK ] TestStrategyClass.ThrowsInvalidArgument (0 ms)
[ RUN      ] TestStrategyClass.AcceptValidParameters
[       OK ] TestStrategyClass.AcceptValidParameters (0 ms)
[ RUN      ] TestStrategyClass.GetsLongTermAndShortTerm
[       OK ] TestStrategyClass.GetsLongTermAndShortTerm (0 ms)
[----------] 3 tests from TestStrategyClass (0 ms total)

[----------] 3 tests from TestSMA
[ RUN      ] TestSMA.GetsDefaultParameters
[       OK ] TestSMA.GetsDefaultParameters (0 ms)
[ RUN      ] TestSMA.ThrowsInvalidArgument
[       OK ] TestSMA.ThrowsInvalidArgument (0 ms)
[ RUN      ] TestSMA.IndicatorReturnsABool
[       OK ] TestSMA.IndicatorReturnsABool (0 ms)
[----------] 3 tests from TestSMA (0 ms total)

[----------] 3 tests from TestMACD
[ RUN      ] TestMACD.GetsDefaultParameters
[       OK ] TestMACD.GetsDefaultParameters (0 ms)
[ RUN      ] TestMACD.ThrowsInvalidArgument
[       OK ] TestMACD.ThrowsInvalidArgument (0 ms)
[ RUN      ] TestMACD.IndicatorReturnsABool
[       OK ] TestMACD.IndicatorReturnsABool (0 ms)
[----------] 3 tests from TestMACD (0 ms total)

[----------] 3 tests from TestUtil
[ RUN      ] TestUtil.FailToOpenFile
[       OK ] TestUtil.FailToOpenFile (0 ms)
[ RUN      ] TestUtil.CorruptDataError
[       OK ] TestUtil.CorruptDataError (0 ms)
[ RUN      ] TestUtil.ValidFileWithNoErrors
[       OK ] TestUtil.ValidFileWithNoErrors (0 ms)
[----------] 3 tests from TestUtil (0 ms total)

[----------] 2 tests from TestSimulator
[ RUN      ] TestSimulator.ConstructorBranching
[       OK ] TestSimulator.ConstructorBranching (0 ms)
[ RUN      ] TestSimulator.ThrowsInvalidArgument
[       OK ] TestSimulator.ThrowsInvalidArgument (0 ms)
[----------] 2 tests from TestSimulator (0 ms total)

[----------] Global test environment tear-down
[==========] 14 tests from 5 test suites ran. (0 ms total)
[  PASSED  ] 14 tests.
```

### Test Coverage

- **Strategy Base Class**: Parameter validation, getter methods
- **SMA Implementation**: Default parameters, signal generation, duration validation
- **MACD Implementation**: Default parameters, signal generation, duration validation
- **Simulator**: Constructor variants, backtest duration validation
- **Utilities**: File I/O operations, error handling for corrupt/missing data

### Test Framework

Unit tests implemented using Google Test framework. Test files located in `tests/` directory.

## Project Structure

```
tradingSim/
├── bin/                    # Compiled executables
│   ├── trading_sim         # Main application
│   └── tests               # Test suite
├── build/                  # Object files and dependencies
├── data/                   # Stock data CSV files
├── include/                # Header files
│   ├── strategy.h
│   ├── SMA.h
│   ├── MACD.h
│   ├── simulator.h
│   └── util.h
├── src/                    # Implementation files
│   ├── strategy.cpp
│   ├── SMA.cpp
│   ├── MACD.cpp
│   ├── simulator.cpp
│   ├── util.cpp
│   └── main.cpp
├── scripts/                # Python data fetching script
│   └── fetch_ticker_data.py
├── tests/                  # Unit tests
│   ├── test_strategy.cpp
│   ├── test_sma.cpp
│   ├── test_macd.cpp
│   ├── test_simulator.cpp
│   └── test_util.cpp
├── Makefile               # Build automation
├── requirements.txt       # Python dependencies (yfinance)
└── README.md
```

## Notes
- `fetch_ticker_data.py` and `ticker_symbol` can be run individually or automated via `make`
- `make run` by default has `-x` flag activated for `fetch_ticker_data.py`

## Makefile Targets

| Target | Description |
|--------|-------------|
| `make all` | Build main program and tests |
| `make run TICKER=<ticker_symbol> [STOCKS=N] [PERIOD={1,2,3,4,5}]` | Fetch data and run simulation |
| `make run-offline TICKER=<ticker_symbol> [STOCKS=N] [PERIOD={1,2,3,4,5}]` | Run on existing data |
| `make test` | Build and execute test suite |
| `make clean` | Remove build artifacts |
| `make clean-data` | Remove `data/` folder |
| `make rebuild` | Clean and rebuild everything |
| `make install-deps` | Install Python dependencies |
| `make help` | Display available targets |
| `make info` | Display compiler and flags |
> Note: `make run-offline` will fail without an existing `temp.csv` file in `data/`. It is recommended to either run `fetch_ticker_data.py` explicitly or have your own `temp.csv` file in `data/` before using `run-offline` Otherwise, avoid it.

## Limitations and Disclaimers

- **Transaction costs not included**: Brokerage fees and taxes are not factored into profit calculations
- **Dividends excluded**: Dividend payments are not considered in profits/losses
- **Slippage ignored**: Assumes perfect execution at closing prices
- **Data source limitations**: Dependent on Yahoo Finance API availability and accuracy
- **Simplified MACD strategy**: compared MACD-line with base `0` for Simplified calculations
- **Simplified make**: `make run` does not support all flags (`--mode`, `strategy`, `--start`, `--end`). To make use of these flags, you must explicitly run `fetch_ticker_data.py` and `trading_sim` with the intended flags
- **Educational purpose**: Not intended for real trading decisions or financial advice

## Future Enhancements

Potential improvements for extended development:

- Additional strategies (RSI, Bollinger Bands, Stochastic Oscillator)
- Multi-threaded backtesting for portfolio simulation
- Statistical analysis with visualization
- Database integration for persistent data storage
- Risk management features (stop-loss, position sizing)
- Add dividends and transaction fees in the analysis

## Build Configuration

**Compiler Flags:**
- `-Wall -Wextra -Werror -Wpedantic`: Strict warning enforcement
- `-g`: Debug symbols enabled
- `-MMD -MP`: Automatic dependency generation

## License

The project is licensed under the MIT License.
See the [LICENSE](./LICENSE) file for license information.

## Author

**Amr Gomaa**  
Self-taught systems/software developer with a background in engineering.  
GitHub: [AmrGomaa3](https://github.com/AmrGomaa3)

## Acknowledgments

- Yahoo Finance API via `yfinance` library
- Google Test framework for unit testing

