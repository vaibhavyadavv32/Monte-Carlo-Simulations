import yfinance as yf
import pandas as pd

tickers = [
    "AAPL", "MSFT", "GOOGL", "AMZN", "TSLA",
    "AMD", "NFLX", "NVDA", "JPM", "DIS"
]

start_date = "2011-01-01"
end_date = "2025-05-31"

try:
    data = yf.download(tickers, start=start_date, end=end_date)["Close"]
    data.ffill(inplace=True)
    data.to_csv("stocks.csv")
    print("Downloaded and saved close prices to stocks.csv")
except Exception as e:
    print(f"Error downloading data: {e}")
