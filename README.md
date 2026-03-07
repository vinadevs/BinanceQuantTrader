# Binance Quantitative Trader Framework

A **complete, extensible, cross platforms and low-latency quantitative trading engine/framework/library for cryptocurrency** built on **Modern C++**, designed for researchers, developers, quants to implement and deploy custom crypto trading strategies on the **Binance Exchange**.

This framework handles all core components of a production trading system—including real-time market data pine line, order execution, quantitative modeling, risk management, historical replay and backtesting simulators, allowing developers to focus solely on building strategy logic.

---

## 📈 Why This Framework?
- Ideal for people who want to study quantitative finance, asset trading, market microstructure, and algo/HFT system design through a real, production-style framework.
- Built for **speed**, **stability**, and **extensibility**.
- Abstracts away exchange complexity and system plumbing, it is runnable from order upstream (OMS) to getting ack by downstream (Exchange).
- Lets developers focus 100% on **alpha generation**.
- Suitable for study, research, live trading, and algo experimentation.

---

## 🚀 Key Features

### **1. Modular Strategy Plug-In Architecture**
- Clean, event-driven interface for adding custom strategies (Auto, Full-Auto, Semi-Auto)
- Developers only write trading logic while infrastructure is fully handled by the engine.
- Suitable for rapid prototyping and deployment of beta trading algorithms.

### **2. High-Performance Low-Latency Core**
- Millisecond-level processing speeds with mordern C++ standards.
- Optimized concurrency model by separated trading tasks.
- Applied HFT techniques like lock free, compile-time dispatch, constexpr.

### **3. Real-Time Market Data Integration**
- Full integration with **Binance WebSocket streams** (book depth, aggregate trade, candle line, funding rate).
- Normalized data models to ensure consistency across all strategy modules.
- Low-latency tick by tick feeding system.

### **4. Backtesting & Simulation Engine**
- Integrates an Exchange Simulator (Matching Engine) which provides liquidity to test strategy's orders.
- Supports historical simulations with realistic execution modeling.
- Best price matching, queue position and market microstructure.
- Storing historical market data and replay systems.

### **5. Quantitative models & libraries**
- Integrates quantlib library and basic quantitative components.
- Pricing models for cash, future and option trading.

### **6. Robust Risk Management Layer**
- Exchange compliance rule bypass.
- Position limits, stop-loss rules, leverage caps, and exposure constraints.
- Real-time algorithm oversight, trading guard.
- Risk models with sharpe ratio, drawdown, win rate, PnL curves, volatility metrics, and more.

### **7. Machine Learning & Analytics Integration**
- Optional ML modules for prediction, trend detection, and volatility forecasting.
- Supports offline training and online adaptation.

### **8. Full strategy samples**
- A set of strategy/algorithm from spot to future markets.
- Supports many strategy types from excecution (VWAP), market monitoring (trend following), derivatives (price arbitrage) and more.

### **9. Python plug-in system**
- Allows python developers receive normalized market data through socket connection.
- Supports external large order using python protobuf message for execution strategy like VWAP/TWAP/PV.

### **10. Production-Ready Infrastructure**
- Built-in logging, error recovery, failure handling, and monitoring.
- Designed for 24/7 crypto trading operations.
- Clear separation of concerns: engine handles core mechanics; developers/quants handle strategy logic.

---

## 🧰 Tech Stack & Low-Latency Engineering

### **Modern C++ Foundation**
- Written in **C++17/20**.

### **Python As Plugin**
- Using python for alpha reseaching and strategy prototype.

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

## 📜 License
 Apache-2.0 license.



