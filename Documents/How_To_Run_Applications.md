# Running the Application

## 1. Windows (Visual Studio)

If you are using **Windows OS with Visual Studio IDE**:

From **Solution Explorer**, right-click and start each project in the following order (Debug mode):

<img width="1277" height="90" alt="image" src="https://github.com/user-attachments/assets/8e2a442c-e129-4507-ae2f-75f06c0ef542" />

### Step 1: Start Message Broker Application

<img width="905" height="1007" alt="image" src="https://github.com/user-attachments/assets/4b4b8657-76c7-444f-b050-6ec89f5a8ff1" />

### Step 2: Start Exchange Simulator Application

<img width="905" height="863" alt="image" src="https://github.com/user-attachments/assets/9e31819a-29a8-436a-8e03-2d6d5736713f" />

### Step 3: Start Binance Quant Trader Application

<img width="901" height="826" alt="image" src="https://github.com/user-attachments/assets/b4a270cf-e674-4b1f-ab7c-923c4970001b" />

After a short wait, the trading simulation should look like this:

<img width="2548" height="636" alt="image" src="https://github.com/user-attachments/assets/baf3df1b-4b40-4a47-ad1f-941904603dce" />


---

## 2. Linux / macOS (Terminal)

If you are using **Linux or macOS**, run the applications from the terminal:

```bash
cd build/ # directory where all binaries are built (we need to open 3 terminals)

MessageHubServer.exe --config_message_hub_server_path="Configurations\MessageBrokerCfg.xml" # Start Message Broker Application first

BackTesting.exe --config_exchange_simulator_path="Configurations\BinanceExchangeSimulatorCfg.xml" # Next start Exchange Simulator Application

BinanceQuantTrader.exe --config_binance_quant_trader_path="Configurations\BinanceQuantTraderCfg.xml" --config_binance_access_key_path="Configurations\BinanceAKCfg.xml" # Final start Binance Quant Trader Application

