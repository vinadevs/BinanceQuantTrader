# Trading Strategies Documentation

This document describes five sample strategies implemented or conceptualized in
the binance quantitative trading engine.

------------------------------------------------------------------------

# 1. VWAPStrategy

## Overview

`VWAPStrategy` implements a Volume Weighted Average Price (VWAP)
execution strategy.\
It receives **parent orders** and executes them as smaller **child
orders** over a trading period.

VWAP is used as a benchmark price for institutional trading.

## VWAP Formula

VWAP = sum(Price × Volume) / sum(Volume)

## Responsibilities

-   Accumulate trade data in real time
-   Compute VWAP continuously
-   Slice large orders into smaller executions
-   Control execution pace based on market volume
-   Reduce market impact
-   Enforce risk limits such as participation rate and price deviation

------------------------------------------------------------------------

# 2. SmartLongShortStrategy

## Overview

`SmartLongShortStrategy` manages **long and short positions in futures
markets**.

It processes trading hints and generates trading signals automatically.

## SMA Formula

The Simple Moving Average (SMA) is defined as:

$$
SMA_n = \frac{1}{n}\sum_{i=1}^{n} P_i
$$

Where:

- $P_i$ = price at period $i$
- $n$ = number of periods

## Features

-   Reacts to trading hints
-   Generates long or short signals
-   Subscribes to target symbols
-   Integrates with market data and trading rule modules

------------------------------------------------------------------------

# 3. MarketMonitorStrategy

## Overview

`MarketMonitorStrategy` is a **non-trading monitoring strategy**.

It observes market data in real time and provides analytics or
diagnostic information.

## Market Moving Detection

Fast SMA crosses above Slow SMA
This indicates a potential uptrend.

Fast SMA crosses below Slow SMA
This indicates a potential downtrend.

------------------------------------------------------------------------

# 4. DerivativesArbitrageStrategy

## Overview

`DerivativesArbitrageStrategy` exploits **spot vs futures pricing
inefficiencies** using a Cash & Carry arbitrage model.

The strategy aims to maintain **delta-neutral exposure**.

## Cash & Carry Logic

1.  Buy the asset in the spot market
2.  Short the futures contract
3.  Capture the price spread (basis)
4.  Hold until convergence

## Quantitative Model

Cash & Carry arbitrage exploits pricing differences between the spot market and the futures market.

The theoretical fair value of a futures contract is given by:

$$
F = S \cdot e^{(r - y)T}
$$

Where:

- $F$ = futures price  
- $S$ = spot price  
- $r$ = risk-free interest rate  
- $y$ = asset yield (or funding benefit)  
- $T$ = time to maturity (in years)

---

### Basis

The **basis** measures the difference between the futures price and the spot price.

$$
Basis = F - S
$$

---

### Arbitrage Opportunity

A cash and carry opportunity exists when:

$$
F > S \cdot e^{(r - y)T}
$$

In this case the trader can:

1. Buy the asset in the **spot market**
2. Short the **futures contract**
3. Hold until expiration

The locked profit is approximately:

$$
Profit \approx F - S \cdot e^{(r - y)T}
$$

---

### Reverse Cash & Carry

If the futures price is too low:

$$
F < S \cdot e^{(r - y)T}
$$

The trader can perform **reverse cash and carry**:

1. Short the asset in the **spot market**
2. Long the **futures contract**

---

### Delta Neutral Hedge

The position is typically delta-neutral:

$$
\Delta_{total} \approx 0
$$

Because:

- Long spot: $\Delta = +1$
- Short future: $\Delta = -1$

## Strategy Pipeline

MarketData → FairValueModel → GreeksModel → RiskModel →
CashCarryStrategy

## Risk Management With Greeks

Option Greeks measure the sensitivity of an option's price to different market variables.

---

## Delta (Δ)

Delta measures the sensitivity of the option price to changes in the underlying asset price.

$$
\Delta = \frac{\partial V}{\partial S}
$$

Where:

- $V$ = option price
- $S$ = underlying asset price

Interpretation:

- Call option: $0 < \Delta < 1$
- Put option: $-1 < \Delta < 0$

Example:

If $\Delta = 0.6$, then a $1 increase in the asset price increases the option price by approximately $0.60.

---

## Gamma (Γ)

Gamma measures the rate of change of Delta with respect to the underlying price.

$$
\Gamma = \frac{\partial^2 V}{\partial S^2}
$$

Interpretation:

- High Gamma means Delta changes quickly
- Important for **dynamic hedging**

---

## Theta (Θ)

Theta measures the sensitivity of the option price to the passage of time.

$$
\Theta = \frac{\partial V}{\partial t}
$$

Interpretation:

- Usually negative for long options
- Represents **time decay**

Example:

If $\Theta = -0.05$, the option loses about $0.05 per day due to time decay.

---

## Summary

| Greek | Measures | Formula |
|------|------|------|
| Delta | Price sensitivity to underlying | $\partial V / \partial S$ |
| Gamma | Change of delta | $\partial^2 V / \partial S^2$ |
| Theta | Time decay | $\partial V / \partial t$ |

------------------------------------------------------------------------

# 5. FOMOStrategy

## Overview

`FOMOStrategy` attempts to capture short-term price moves caused by
**Fear Of Missing Out (FOMO)** behavior.

When traders rush into a rapidly moving market, strong short-term
momentum can appear.

## Typical Pattern

Low volatility\
→ breakout move\
→ rapid increase in volume\
→ aggressive market buys\
→ short-term price spike

## Logic

If:

-   price momentum is strong
-   volume spike occurs
-   buy order flow dominates

Then:

enter long position and ride short-term momentum.

## Risks

-   fake breakouts
-   liquidity traps
-   momentum exhaustion

------------------------------------------------------------------------

# Summary

  --------------------------------------------------------------------------
  Strategy                       Type                  Purpose
  ------------------------------ --------------------- ---------------------
  VWAPStrategy                   Execution             Execute large orders
                                                       with minimal impact

  SmartLongShortStrategy         Directional           Automated long/short
                                                       trading

  MarketMonitorStrategy          Monitoring            Market observation

  DerivativesArbitrageStrategy   Arbitrage             Capture spot/futures
                                                       price discrepancies

  FOMOStrategy                   Behavioral            Exploit momentum
                                                       driven by trader
                                                       psychology
  --------------------------------------------------------------------------
