# tradingSim Makefile
# -------------------
# build, test and run tradingSim
# usage examples:
#   make all
#   make run TICKER=AAPL [PERIOD=5] [STOCKS=1000]
#   make test
#   make clean


# error message
ERR_MSG = Usage: make run TICKER=<ticker_symbol> [PERIOD={1,2,3,4,5}] [STOCKS=N]


# workflow: fetch data with Python, then execute C++ simulator.
# usage: make run TICKER=<symbol> [PERIOD=1..5] [STOCKS=N]

# suppress make's directory messages on recursive calls
MAKEFLAGS += --no-print-directory

# compiler configuration
CXX = g++
CXXFLAGS = -Wall -Wextra -Werror -Wpedantic -g -MMD -MP
TARGET_FLAGS = $(CXXFLAGS) -DDATA_DIR=\"$(PWD)/data/\"
TEST_FLAGS = $(CXXFLAGS) -DTEST_DATA_DIR=\"$(PWD)/test_data/\"


# default command-line arguments
STOCKS ?= 1000
PERIOD ?= 5


# targets and build paths
TARGET = ./bin/trading_sim
TEST_TARGET = ./bin/tests

BUILD_OBJS = ./build/strategy.o ./build/SMA.o ./build/MACD.o ./build/simulator.o ./build/util.o
MAIN_OBJ = ./build/main.o
OBJS = $(MAIN_OBJ) $(BUILD_OBJS)
TEST_OBJS = ./build/test_strategy.o ./build/test_sma.o ./build/test_macd.o ./build/test_util.o ./build/test_simulator.o

DEPS = $(OBJS:.o=.d) $(TEST_OBJS:.o=.d)


# phony targets
.PHONY: all clean run run-offline test rebuild info help install-deps


# default build rule
all: $(TARGET) $(TEST_TARGET)


# link trading_sim executable
$(TARGET): $(OBJS) | bin
	$(CXX) $(TARGET_FLAGS) $^ -o $@


# link tests executable and generate test csvs
$(TEST_TARGET): $(TEST_OBJS) $(BUILD_OBJS) | bin test_data
	@printf "10.2\n15\n3.887" > ./test_data/valid_data.csv
	@printf "Hello\n \n3.887" > ./test_data/invalid_data.csv
	$(CXX) $(TEST_FLAGS) $^ -o $@ -lgtest -lgtest_main -pthread


# compilation rules
./build/%.o: ./src/%.cpp | build
	$(CXX) $(TARGET_FLAGS) -c $< -o $@


./build/%.o: ./tests/%.cpp | build
	$(CXX) $(TEST_FLAGS) -c $< -o $@


-include $(DEPS)


# directory creation rules
bin:
	@mkdir -p bin


build:
	@mkdir -p build


test_data:
	@mkdir -p test_data


# utility targets
rebuild: clean all


test: $(TEST_TARGET)
	@echo "Running tests..."
	@$(TEST_TARGET)


info:
	@echo "CXX       = $(CXX)"
	@echo "CXXFLAGS  = $(CXXFLAGS)"
	@echo "TARGET    = $(TARGET)"
	@echo "TESTS     = $(TEST_TARGET)"


help:
	@echo "Available targets:"
	@echo "  all           - build main program and tests"
	@echo "  run           - run trading_sim (fetch data + simulate)"
	@echo "  run-offline   - run trading_sim on existing data"
	@echo "  test          - build and run tests"
	@echo "  clean         - remove build artifacts"
	@echo "  clean-data    - remove /data folder"
	@echo "  rebuild       - clean and rebuild everything"
	@echo "  info          - show compiler and flags"
	@echo "  install-deps  - install Python dependencies from requirements.txt"


clean:
	@rm -rf bin build test_data


clean-data:
	@rm -rf data


# run targets
# workflow: fetch data with Python, then execute C++ simulator.
# usage: make run TICKER=<symbol> [PERIOD=1..5] [STOCKS=N]
run: $(TARGET)
# enforce ticker_symbol to be explicitly passed by the user
	@if [ -z "$(TICKER)" ]; then \
		echo "Invalid arguments"; \
		echo "$(ERR_MSG)"; \
	else \
		echo "Fetching stock data for '$(TICKER)'..."; \
		status=0; \
		python3 ./scripts/fetch_ticker_data.py $(TICKER) -p=$(PERIOD) -x || status=$$?; \
		if [ $$status -eq 0 ]; then \
			$(MAKE) run-offline; \
			rm -f ./data/temp.csv; \
		fi; \
	fi


run-offline: $(TARGET)
	@if [ -z "$(TICKER)" ]; then \
		echo "$(ERR_MSG)"; \
	else \
		echo "Running trading_sim for '$(TICKER)'..."; \
		status=0; \
		$(TARGET) -t=$(TICKER) -sk=$(STOCKS) || status=$$?; \
		if [ $$status -eq 2 ]; then \
			echo "Invalid arguments"; \
			echo "$(ERR_MSG)"; \
		elif [ $$status -eq 3 ]; then \
			echo "$(ERR_MSG)"; \
		fi; \
	fi


# install dependencies (yfinance)
install-deps:
	@pip install -r requirements.txt

