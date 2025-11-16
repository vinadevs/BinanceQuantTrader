# Binance Quant Trader Project  
## C++ Coding Style and Convention Guideline

---

### 🧭 General Philosophy

All code must be:

- **Readable** — clarity over cleverness.  
- **Consistent** — same style across the entire project.  
- **Safe & maintainable** — use RAII, smart pointers, and thread safety patterns.  
- **Modern C++** — follow C++17/20 best practices.

---

## 1. File Structure

### 1.1 Header File Template

```cpp
/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C)
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#pragma once
```

### 1.2 File Naming

| Type | Convention | Example |
|------|-------------|----------|
| Header files | `PascalCase.h` | `SmartLongShortStrategy.h` |
| Source files | `PascalCase.cpp` | `SmartLongShortStrategy.cpp` |
| Namespaces | lowerCamelCase | `namespace tradingStrategies` |
| Macros | ALL_CAPS_WITH_UNDERSCORES | `DLL_CLASS_TRADING_STRATEGIES_EXPORTS` |

---

## 2. Naming Conventions

| Element | Style | Example |
|----------|--------|----------|
| Classes / Structs / Enums | PascalCase | `SmartLongShortStrategy` |
| Functions / Methods | PascalCase | `InitializeParameters()` |
| Variables | lowerCamelCase | `targetSymbols` |
| Member Variables | prefix with `m_` | `m_targetFutureTradeSymbols` |
| Static Variables | prefix with `s_` | `s_instance` |
| Constants / Enums | ALL_CAPS | `MAX_ORDER_SIZE` |
| Namespaces | lowerCamelCase | `marketData`, `tradingStrategies` |

---

## 3. Class Design

### 3.1 Structure Order

```cpp
class ExampleClass : public BaseClass, public InterfaceA
{
public:
    // 1. Constructor / Destructor
    ExampleClass();
    virtual ~ExampleClass();

    // 2. Public interface
    void Initialize();
    void Execute();

protected:
    // 3. Protected helpers
    void Setup();

private:
    // 4. Private helpers
    void CreateProfile();
    void SubscribeSymbols();

    // 5. Member variables
    std::unique_ptr<Component> m_component;
    std::vector<std::string> m_symbols;
};
```

### 3.2 Inheritance Rules
- Use **virtual destructors** in polymorphic base classes.  
- Use **`override`** keyword for all overridden virtual methods.  
- Prefer **composition over inheritance** unless there’s a clear “is-a” relationship.

---

## 4. Function Guidelines

### 4.1 Declaration

- Use PascalCase for all function names.  
- Parameters should be `const &` where applicable.  
- Default parameters only in declarations.

```cpp
void InitializeParameters(const std::string& configPath);
bool OnBookTickerChange(MarketData::MarketDataSubject* data, const std::string& symbol);
```

### 4.2 Implementation Example

```cpp
void SmartLongShortStrategy::InitializeParameters(const std::string& configPath)
{
    // Initialize strategy parameters from configuration file
}
```

### 4.3 Inline and constexpr
- Use `constexpr` for compile-time constants.  
- Use `inline` only when performance justifies it.

---

## 5. Memory Management

- Use **`std::unique_ptr`** or **`std::shared_ptr`** instead of raw pointers.  
- Use raw pointers only for **non-owning references**.  
- Always **initialize members in constructor initializer list**.

```cpp
SmartLongShortStrategy::SmartLongShortStrategy(const std::string& cfgPath,
                                               MarketData::RealTimeMarketData* marketData,
                                               UserAccount::Trader* trader,
                                               ComplianceNRegulatory::BinanceTradingRules* rules)
    : m_marketDataAnalyzer(std::make_unique<QuantitativeModel::MarketDataAnalyzer>())
{
    // Setup logic
}
```

---

## 6. Thread Safety & Concurrency

- Use `std::mutex` or `std::shared_mutex` for shared data.  
- Protect critical sections using `std::lock_guard` or `std::unique_lock`.  
- Use `std::condition_variable` for event signaling.  
- Prefer thread-safe event loops or thread pools over raw `std::thread`.

---

## 7. Namespaces

- Group logically related modules under namespaces.  
- Avoid nesting beyond **two levels** (e.g., `TradingStrategies::SmartLongShortStrategy`).  
- Close namespaces with comments:

```cpp
} // namespace TradingStrategies
```

---

## 8. Includes & Forward Declarations

- Include order:
  1. Local header (`"SmartLongShortStrategy.h"`)
  2. Project headers
  3. Standard library headers
- Use **forward declarations** to reduce compile times.

---

## 9. Documentation & Comments

### 9.1 Function Documentation (Doxygen Style)

```cpp
/// Initializes all strategy parameters from a configuration file.
/// @param strategyCfgPath Path to the strategy configuration JSON file.
void InitializeParameters(const std::string& strategyCfgPath) override;
```

### 9.2 Class Documentation

```cpp
/// @class SmartLongShortStrategy
/// @brief Automated long-short futures trading strategy.
/// 
/// This strategy subscribes to futures market data, generates buy/sell signals,
/// and executes trades according to quantitative models and compliance rules.
```

### 9.3 Inline Comments

Use inline comments **only for non-obvious logic**.

```cpp
// Trigger alarm to send scheduled orders
OnAlarmTriggered();
```

---

## 10. Error Handling

- Use **exceptions** for runtime errors, not control flow.  
- Throw **specific standard exceptions** (`std::invalid_argument`, `std::runtime_error`, etc.).  
- Avoid using return codes or “magic values” for error handling.

---

## 11. Code Formatting

| Rule | Description |
|------|--------------|
| Indentation | 4 spaces (no tabs) |
| Line length | ≤ 120 characters |
| Braces | Always on new line for functions/classes |
| Spacing | One space after `if`, `for`, `while`, etc. |
| Member init list | Vertically aligned |

**Example:**
```cpp
SmartLongShortStrategy::SmartLongShortStrategy()
    : m_targetFutureTradeSymbols(),
      m_marketDataAnalyzer(nullptr)
{
}
```

---

## 12. Build & Linking

- Use **CMake** as the primary build system.  
- Each logical module should have its own target (`add_library` / `add_executable`).  
- Use DLL export macros consistently (`DLL_CLASS_TRADING_STRATEGIES_EXPORTS`).

---

## 13. Unit Testing

- Every new public API must have corresponding **unit tests**.  
- Use **GoogleTest** or **Catch2**.  
- Use **mock classes** to simulate dependencies (e.g., market data feeds, traders).

---

## 14. Project Module Naming

| Module | Namespace | Example Class |
|---------|------------|----------------|
| Market Data | `MarketData` | `MarketDataSubject`, `RealTimeMarketData` |
| Trading Strategies | `TradingStrategies` | `SmartLongShortStrategy`, `VWAPStrategy` |
| Compliance | `ComplianceNRegulatory` | `BinanceTradingRules` |
| Quantitative Models | `QuantitativeModel` | `MarketDataAnalyzer`, `SignalGenerator` |
| User Account | `UserAccount` | `Trader`, `PortfolioManager` |

---

## 15. Summary

✅ Follow **consistent naming** and **clear hierarchy**.  
✅ Document all **classes and functions** with Doxygen.  
✅ Prefer **composition, RAII, smart pointers**, and **modern C++ idioms**.  
✅ Ensure **thread safety** and **test coverage** for all trading logic.
