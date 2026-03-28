## 🐍 Python Market Data Integration

You can use Python to receive **normalized market data updates** from the C++ engine.

---

### Step 1: Start Message Broker Application

<img width="905" height="1007" alt="image" src="https://github.com/user-attachments/assets/4b4b8657-76c7-444f-b050-6ec89f5a8ff1" />

---

### Step 2: Start Market Data Capture (Python Server Mode)

1. Update the configuration file:

Enable **Python Server Mode**:

<img width="1510" height="1162" alt="image" src="https://github.com/user-attachments/assets/7f5eaae0-747d-4515-9277-378b0388af7a" />

---

#### Run on Windows (Visual Studio)

Start the **Market Data Capture Application** from Visual Studio:

<img width="807" height="1003" alt="image" src="https://github.com/user-attachments/assets/cd2b8d42-93de-44d6-814a-b38fd3de966c" />

---

#### Run on Linux / macOS (Terminal)

./MarketDataCapture.exe --config_market_data_path="Configurations/BinanceMarketDataCfg.xml"

### Step 3: Start Python Sample Application

cd Source\AlphaResearch\

python market_data_feed.py
