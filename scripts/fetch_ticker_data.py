import yfinance as yf
import sys
import os
import argparse
from datetime import datetime, timedelta
from yfinance.exceptions import YFTzMissingError
import logging


# disable yfinance logging
logging.getLogger("yfinance").setLevel(logging.CRITICAL)


# create the directory (if it does not exist)
script_dir = os.path.dirname(os.path.abspath(__file__))
data_dir = os.path.join(script_dir, "..", "data")
os.makedirs(data_dir, exist_ok=True)


# create a parser
parser = argparse.ArgumentParser(description="TradingSim: run backtesting or live-data indicators on stock data")


# add arguments
parser.add_argument(
    "ticker",
    help="Stock ticker symbol (i.e. AAPL)"
)

parser.add_argument(
    "--period", "-p",
    choices=[i for i in range(1, 6)],
    type=int,
    help="Backtest period in years (i.e. 1)"
)

parser.add_argument(
    "--start", "-s",
    help="Backtest start date (%%Y-%%m-%%d)"
)

parser.add_argument(
    "--end", "-e",
    help="Backtest end period (%%Y-%%m-%%d)"
)

parser.add_argument(
    "--export", "-x",
    action="store_true",
    help="Export data file to 'tradingSim/data/'"
)

args = parser.parse_args()

# argv check
if args.period and (args.start or args.end):
    parser.error("use either --period or --start/--end")

if (args.start and not args.end) or (args.end and not args.start):
    parser.error("use either --period or --start/--end")


# getting user-defined dates from --start/--end
if args.start and args.end:
    try:
        start_date = datetime.strptime(args.start, "%Y-%m-%d")
        end_date = datetime.strptime(args.end, "%Y-%m-%d")
    except ValueError:
        parser.error("date must be in format '%Y-%m-%d'")

    # check date range
    if start_date >= end_date:
        sys.exit("Error: invalid date range")

    today = datetime.today()

    if end_date > today:
        sys.exit("Error: invalid end_date")

else:
    end_date = datetime.today()

    # getting user-defined period
    if args.period:
        chosen_period = args.period
    else:
        # if no --period then default to 1 year
        chosen_period = 1

    start_date = end_date - timedelta(days=(365*chosen_period))


# get data for ticker
try:
    data = yf.Ticker(args.ticker).history(
            start=start_date,
            end=end_date,
            auto_adjust=True,
            raise_errors=True)

# check valid ticker
except YFTzMissingError:
    sys.exit(f"Error: could not find data for '{args.ticker}'")

# check network connection
except Exception as e:
    sys.exit("Network Error: could not connect to https://finance.yahoo.com/")


# check valid data
if data.empty:
    sys.exit(f"No data available for '{args.ticker}' in the given date range")


# write data to '/data' folder
save_dir = os.path.join(data_dir, "temp.csv")

try:
    data[["Close"]].to_csv(save_dir, index=False, header=False)

    if args.export:
        start_date_str = start_date.date()
        end_date_str = end_date.date()
        data.to_csv(f"{data_dir}/{args.ticker}_{start_date_str}_to_{end_date_str}.csv")
        print(f"Data for '{args.ticker}' saved to 'data/{args.ticker}_{start_date_str}_to_{end_date_str}.csv'")

# check writing errors
except Exception as e:
    sys.exit(f"Error: could not save data to disk: {e}")
