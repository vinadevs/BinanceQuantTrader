# Binance Quantitative Trader Framework

A **complete, extensible, cross-platform, low-latency quantitative trading engine/framework/library for cryptocurrency**, built with **modern C++**, designed for researchers, developers, and quants to implement and deploy custom crypto trading strategies on the **Binance Exchange**.

This framework handles all core components of a production trading system—including real-time market data pipelines, order execution, quantitative modeling, risk management, historical replay, and backtesting simulators—allowing developers to focus solely on building strategy logic.

---

## 📈 Why This Framework?

- Ideal for those who want to study quantitative finance, asset trading, market microstructure, and algo/HFT system design through a real, production-style framework.
- Built for **speed**, **stability**, and **extensibility**.
- Abstracts away exchange complexity and system plumbing. It is runnable end-to-end—from order upstream (OMS) to downstream exchange acknowledgments.
- Allows developers to focus 100% on **alpha generation**.
- Suitable for study, research, live trading, and algorithmic experimentation.

---

## 🚀 Key Features

### **1. Modular Strategy Plug-In Architecture**
- Clean, event-driven interface for adding custom strategies (Auto, Full-Auto, Semi-Auto).
- Developers only write trading logic while infrastructure is fully handled by the engine.
- Suitable for rapid prototyping and deployment of beta trading algorithms.

### **2. High-Performance Low-Latency Core**
- Millisecond-level processing speeds with modern C++ standards.
- Optimized concurrency model with separated trading tasks.
- Applies HFT techniques such as lock-free structures, compile-time dispatch, and `constexpr`.

### **3. Real-Time Market Data Integration**
- Full integration with **Binance WebSocket streams** (order book depth, aggregate trades, candlesticks, funding rates).
- Normalized data models to ensure consistency across all strategy modules.
- Low-latency tick-by-tick data feed.

### **4. Backtesting & Simulation Engine**
- Integrates an Exchange Simulator (matching engine) that provides liquidity for testing strategy orders.
- Supports historical simulations with realistic execution modeling.
- Includes best-price matching, queue position modeling, and market microstructure simulation.
- Supports historical market data storage and replay.

### **5. Quantitative Models & Libraries**
- Integrates QuantLib and basic quantitative components.
- Supports pricing models for spot, futures, and options trading.

### **6. Robust Risk Management Layer**
- Exchange compliance rule bypass mechanisms.
- Position limits, stop-loss rules, leverage caps, and exposure constraints.
- Real-time algorithm oversight via a trading guard.
- Risk analytics including Sharpe ratio, drawdown, win rate, PnL curves, volatility metrics, and more.

### **7. Machine Learning & Analytics Integration**
- Optional ML modules for prediction, trend detection, and volatility forecasting.
- Supports offline training and online adaptation.

### **8. Full Strategy Samples**
- A collection of strategies across spot and futures markets.
- Covers multiple strategy types including execution (VWAP), market monitoring (trend following), derivatives (price arbitrage), and more.

### **9. Python Plug-In System**
- Allows Python developers to receive normalized market data via socket connections.
- Supports external order execution using Python protobuf messages (e.g., VWAP, TWAP, POV strategies).

### **10. Production-Ready Infrastructure**
- Built-in logging, error recovery, failure handling, and monitoring.
- Designed for 24/7 crypto trading operations.
- Clear separation of concerns: the engine handles core mechanics; developers/quants focus on strategy logic.

---

## 🧰 Tech Stack & Low-Latency Engineering

### **Modern C++ Foundation**
- Written in **C++17/20**.

### **Python as a Plug-In**
- Used for alpha research and strategy prototyping.

---

## 🧪 Example how a strategy will be created
Implement your strategy by inheriting and overriding core virtual callbacks:

```cpp
class SmartLongShortStrategy : // your strategy
    public TradingStrategyBase, // strategy core by the engine
    public MarketData::MarketDataObserver // market data updater
{
public:
    explicit SmartLongShortStrategy(const std::string& cfgPath, // strategy config file
                                    MarketData::RealTimeMarketData* marketData, // market data
                                    UserAccount::Trader* trader, // order sender
                                    ComplianceNRegulatory::BinanceTradingRules* rules); // exchange rule bypass engine
    // Future order book depth event
    bool OnBookDataFutureChange(MarketData::MarketDataSubject* marketData, const std::string& symbol) override;
    // Future trade data update event
    bool OnTradeDataFutureChange(MarketData::MarketDataSubject* marketData, const std::string& symbol) override;
    // Start strategy and send order
    void StartTrade() override;
    // Leave the market and calculate PNL
    void StopTrade() override;
private:
    // Quantitative models
    std::unique_ptr<QuantitativeModel::MarketDataAnalyzer> m_analyzer;
};
```

---

## 📊 Backtesting Example
```cpp
auto simulator = std::make_unique<BinanceExchangeSimulator>(configSimulatorXml.get());
simulator->Run();
```

---

## 📊 Historical Market Data Capture Example
```cpp
auto marketDataService = std::make_unique<MarketDataService>(config_market_data_path);
marketDataService->SubscribeTargetSymbols();
marketDataService->Run();
```

## 📊 Trading Simulation Between AlgoEngine and Simulator Example

<img width="2548" height="636" alt="image" src="https://github.com/user-attachments/assets/4d66ff23-85bb-4662-b435-c955cbf50181" />

---

## 📜 License
 Apache-2.0 license.



