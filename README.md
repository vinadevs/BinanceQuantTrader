# Binance Quantitative Trader Framework

A **complete, extensible, and low-latency quantitative trading framework for cryptocurrency** built on **Modern C++**, designed for researchers and developers to implement and deploy custom crypto trading strategies on the **Binance Exchange**.

This framework handles all core components of a production trading system—including real-time market data, execution, risk management, and backtesting—allowing developers to focus solely on building strategy logic.

---

## 📈 Why This Framework?
- Ideal for developers and learners who want to study quantitative trading, market microstructure, and algo/HFT system design through a real, production-style framework.
- Built for **speed**, **stability**, and **extensibility**.
- Abstracts away exchange complexity and system plumbing.
- Lets developers focus 100% on **alpha generation**.
- Suitable for research, live trading, and HFT experimentation.

---

## 🚀 Key Features

### **1. Modular Strategy Plug-In Architecture**
- Clean, event-driven interface for adding custom strategies.
- Developers only write trading logic; infrastructure is fully handled.
- Suitable for rapid prototyping and deployment of low-latency trading algos.

### **2. High-Performance Low-Latency Core**
- Millisecond-level processing speeds.
- Optimized concurrency model for high-frequency trading.
- Efficient handling of order book updates, execution events, and tick data.

### **3. Real-Time Market Data Integration**
- Full integration with **Binance WebSocket streams** (depth, trades, klines, system events).
- Normalized data models to ensure consistency across all strategy modules.

### **4. Backtesting & Simulation Engine**
- Integrates an advanced Exchange simulator (Matching Engine) which provides liquidity for strategy's order.
- Supports historical simulations with realistic fill modeling.
- Models latency, queue position, slippage, and market microstructure.
- Provides Sharpe ratio, drawdown, win rate, PnL curves, volatility metrics, and more.

### **5. Robust Risk Management Layer**
- Position limits, stop-loss rules, leverage caps, and exposure constraints.
- Real-time capital protection and algorithm oversight.

### **6. Machine Learning & Analytics Integration**
- Optional ML modules for prediction, trend detection, and volatility forecasting.
- Supports offline training and online adaptation.

### **7. Production-Ready Infrastructure**
- Built-in logging, error recovery, reconnect logic, and monitoring.
- Designed for 24/7 crypto trading operations.
- Clear separation of concerns: framework handles core mechanics; developers handle strategy logic.

---

## 🧰 Tech Stack & Low-Latency Engineering

### **Modern C++ Foundation**
- Written in **C++17/20**.

---

## 🧪 Strategy Development
Implement your strategy by inheriting and overriding core virtual callbacks:

```cpp
class SmartLongShortStrategy :
    public TradingStrategyBase,
    public MarketData::MarketDataObserver
{
public:
    explicit SmartLongShortStrategy(const std::string& cfgPath,
                                    MarketData::RealTimeMarketData* marketData,
                                    UserAccount::Trader* trader,
                                    ComplianceNRegulatory::BinanceTradingRules* rules);

    void StartLive() override;
    void StopLive() override;

private:
    std::vector<std::string> m_targetSymbols;
    std::unique_ptr<QuantitativeModel::MarketDataAnalyzer> m_analyzer;
};
```

---

## 📊 Backtesting Example
```cpp
BQTBacktestEngine bt;
bt.loadHistoricalData("BTCUSDT");
bt.setStrategy(std::make_unique<SmartLongShortStrategy>());
bt.run();
bt.report();
```

---

## 📜 License
 Apache-2.0 license



