/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#include "pch.h"

#include "VWAPStrategy.h"
#include "VWAPVolumeProfile.h"

#include "../SettingNConfig/tinyxml2.h"
#include "../LibraryUtils/StringUtils.h"
#include "../MarketData/RealTimeMarketData.h"
#include "../MarketData/SynchronousMarketData.h"
#include "../StaticData/StaticDataManager.h"
#include "../UserAccount/BinanceTrader.h"
#include "../ComplianceNRegulatory/BinanceTradingRules.h"
#include "../ComplianceNRegulatory/BinanceExchangeProfile.h"
#include "../QuantitativeModel/QuantOrderParammeter.h"
#include "../QuantitativeModel/OrderParammeterGenerator.h"
#include "../QuantitativeModel/MarketDataAnalyzer.h"
#include "../QuantitativeModel/QuantMarketDataAnalyzer.h"
#include "../LibraryUtils/PathUtils.h"
#include "../LibraryUtils/FileUtils.h"
#include "../LibraryUtils/StringUtils.h"
#include "../OrderManagement/ExternalParentOrder.h"
#include "../RiskManagement/VWAPOrderExecutionRiskMetrics.h"

using namespace TradingStrategies;
using namespace QuantitativeModel;
using namespace MarketData;
using namespace UserAccount;
using namespace ComplianceNRegulatory;
using namespace LibraryUtils;
using namespace tinyxml2;

#undef max

VWAPStrategy::VWAPStrategy(
	const std::string& strategyCfgPath,
	RealTimeMarketData* marketData,
	Trader* trader,
	BinanceTradingRules* tradingRules)
	: TradingStrategyBase("VWAPStrategy", "Slicing big order into child orders as VWAP benchmark...",
		strategyCfgPath, marketData, trader, tradingRules),
	  AlarmSystem(LibraryUtils::DefaultAlarmInterval, AlarmSystem::AlarmMode::REPEAT)
{
	START_STRATEGY_INITIALIZATION_SECTION

	SetStrategyType(StrategyType::FULL_AUTO);
	InitializeParameters(strategyCfgPath);
	m_logger->Info("Completed initialization for the strategy.");

	END_STRATEGY_INITIALIZATION_SECTION
}

VWAPStrategy::~VWAPStrategy()
{
	m_marketData->UnRegisterDataListener(this); // I earn enough money, leave the market now!
}

bool VWAPStrategy::OnIndividualBookTickerChange(MarketDataSubject* marketData, const std::string& symbol)
{
	if (const auto* syncedData = marketData->GetSynchronousMarketData(symbol))
	{
		if (auto* analyzer = m_marketDataAnalyzer->GetQuantMarketDataAnalyzer(symbol))
		{
			analyzer->AnalysisIndividualBookTicker(syncedData->m_individualBookTickerData);
			return true;
		}
	}
	else
	{
		m_logger->Warning("Could not found synchronized market data for symbol=" + symbol);
	}
	return false;
}

bool VWAPStrategy::OnTradeChange(MarketData::MarketDataSubject* marketData, const std::string& symbol)
{
	if (const auto* syncedData = marketData->GetSynchronousMarketData(symbol))
	{
		if (auto* analyzer = m_marketDataAnalyzer->GetQuantMarketDataAnalyzer(symbol))
		{
			analyzer->AnalysisTrade(syncedData->m_tradeData);
		}
		const auto price = syncedData->GetSingleFeed(TradeID::PRICE)->GetDoubleData();
		const auto volume = syncedData->GetSingleFeed(TradeID::QUANTITY)->GetDoubleData();
		m_currentExchangeTimePointMs = syncedData->GetSingleFeed(TradeID::TRADE_TIME)->GetUnsignedIntData();
		// Update VWAP calculation for the symbol
		std::scoped_lock lock(m_mutex);
		for (auto& order : m_activeVWAPOrders)
		{
			if (order.second.GetParentOrder()->GetSymbol() == symbol)
			{
				m_logger->Info("Updating VWAP calculation for symbol=" + symbol
					+ ", price=" + std::to_string(price)
					+ ", volume=" + std::to_string(volume)
					+ ", time=" + std::to_string(m_currentExchangeTimePointMs));
				order.second.GetVWAPComponents().m_vwapVolumeProfilier->AddNewBucketVolume(volume, m_currentExchangeTimePointMs); // update volume profile
				order.second.GetVWAPComponents().m_cumPriceVolume += price * volume; // calculate cumulative value
				order.second.GetVWAPComponents().m_totalMarketVolume += volume; // calculate total market traded volume
				break;
			}
		}
		return true;
	}
	else
	{
		m_logger->Warning("Could not found synchronized market data for symbol=" + symbol);
	}
	return false;
}

double VWAPStrategy::CalculateCurrentVWAP(const ParentOrderId& parentOrderId)
{
	double vwap = 0.0;
	for (auto& order : m_activeVWAPOrders)
	{
		if (order.second.GetParentOrder()->GetParentOrderId() == parentOrderId)
		{
			const auto& vwapComponents = order.second.GetVWAPComponents();
			if (vwapComponents.m_totalMarketVolume > 0.0)
			{
				vwap = vwapComponents.m_cumPriceVolume / vwapComponents.m_totalMarketVolume;
			}
			break;
		}
	}
	return vwap;
}

double VWAPStrategy::GetOrderSizeForCurrentBucket(const ParentOrderId& parentOrderId, const std::size_t ts)
{
	const auto bucketId = GetBucketVWAPId(ts);
	auto it = m_activeVWAPOrders.find(parentOrderId);
	if (it == m_activeVWAPOrders.end())
	{
		return -1; // parent order not found
	}
	auto& vwapComponents = it->second.GetVWAPComponents();
	if (vwapComponents.m_vwapVolumeProfilier->HasBucketVolumnes() == false)
	{
		return -1; // no volume profile available
	}
	const auto& profile = vwapComponents.m_vwapVolumeProfilier->GetVolumeProfiles();

	double pct = 0.0;
	for (auto& p : profile)
	{
		if (p.first == bucketId)
		{
			pct = p.second;
			break;
		}
	}

	const double targetForBucket = vwapComponents.m_targetVWAPAmount * pct;
	const double alreadyTraded = vwapComponents.m_executedVolume[bucketId];
	return std::max(0.0, targetForBucket - alreadyTraded);
}

void VWAPStrategy::RecordTradeExecution(const ParentOrderId& parentOrderId, const double volume, const std::size_t ts)
{
	const auto bucketId = GetBucketVWAPId(ts);
	auto it = m_activeVWAPOrders.find(parentOrderId);
	if (it == m_activeVWAPOrders.end())
	{
		return; // parent order not found
	}
	auto& vwapComponents = it->second.GetVWAPComponents();
	vwapComponents.m_executedVolume[bucketId] += volume;
}

size_t VWAPStrategy::GetBucketVWAPId(const std::size_t ts) const
{
	auto epochSec = TimeUtils::EpochToTimePoint(ts,
		TimeUtils::TimeUnit::Milliseconds).time_since_epoch().count();
	return epochSec / m_profileBucketMs;
}

void VWAPStrategy::ReportTradeResults(const std::string& symbol)
{
	m_spotTrader->ReportTradeResults(symbol);
}

void VWAPStrategy::InitializeParameters(const std::string& strategyCfgPath)
{
	m_strategyCfgXml = std::make_unique<XMLDocument>();
	const auto errLoadFileXml = m_strategyCfgXml->LoadFile(strategyCfgPath.c_str());
	if (errLoadFileXml != XML_SUCCESS)
	{
		throw std::runtime_error("VWAPStrategy: Load file Xml error="
			+ std::string(XMLDocument::ErrorIDToName(errLoadFileXml)) + ", error path:" + strategyCfgPath);
	}
	SetupStrategyLifeTime(m_strategyCfgXml.get());
	// when we use alarm system, we need to set up the order scheduler
	SetupOrderScheduler();
	// load VWAP profile bucket seconds
	SetupVWAPProfileBucketSeconds();
	// setup VWAP risk management metrics
	SetupVWAPRiskManagements();
}

void VWAPStrategy::InitializeMarketDataAnalyzer()
{
	m_marketDataAnalyzer = std::make_unique<QuantitativeModel::MarketDataAnalyzer>(m_logger.get());
}

void VWAPStrategy::SetupOrderScheduler()
{
	m_logger->Info("Setting up alarm system for order sending interval.");
	const XMLElement* generalConfigXml = m_strategyCfgXml->FirstChildElement("OrderScheduler");
	assert(generalConfigXml);
	const XMLElement* orderSendingInvervalXml = generalConfigXml->FirstChildElement("OrderSendingInverval");
	assert(orderSendingInvervalXml);
	const int64_t alarmIntervalSecond = orderSendingInvervalXml->Int64Attribute("AlarmIntervalSecond");
	if (alarmIntervalSecond <= 0)
	{
		throw std::runtime_error("VWAPStrategy: Invalid alarm interval second="
			+ std::to_string(alarmIntervalSecond) + ", must be greater than 0.");
	}
	AlarmSystem::SetRepeatInterval(alarmIntervalSecond);
	m_strategyOrderScheduler = StrategyOrderScheduler::ALARM_BASED;
}

void VWAPStrategy::SetupVWAPProfileBucketSeconds()
{
	m_logger->Info("Load VWAP profile bucket milliseconds.");
	const XMLElement* vwapVolumeProfileXml = m_strategyCfgXml->FirstChildElement("VWAPVolumeProfile");
	assert(vwapVolumeProfileXml);
	const auto* bucketTimeXml = vwapVolumeProfileXml->FirstChildElement("BucketTime");
	assert(bucketTimeXml);
	const int ms = bucketTimeXml->IntAttribute("Milliseconds");
	if (ms <= 0)
	{
		throw std::runtime_error("VWAPStrategy: Invalid bucket ms="
			+ std::to_string(ms) + ", must be greater than 0.");
	}
	m_profileBucketMs = ms;
}

void VWAPStrategy::SetupVWAPRiskManagements()
{
	m_logger->Info("Setup VWAP risk management metrics.");
	const XMLElement* vwapRiskManagementXml = m_strategyCfgXml->FirstChildElement("RiskManagement");
	assert(vwapRiskManagementXml);
	const XMLElement* riskMetricsXml = vwapRiskManagementXml->FirstChildElement("Metrics");
	assert(riskMetricsXml);
	m_averageSlippageTolerance = riskMetricsXml->DoubleAttribute("AverageSlippageTolerance");
	if (m_averageSlippageTolerance < 0.0)
	{
		throw std::runtime_error("VWAPStrategy: Invalid average slippage tolerance="
			+ std::to_string(m_averageSlippageTolerance) + ", must be non-negative.");
	}
	m_maxDrawdownTolerance = riskMetricsXml->DoubleAttribute("MaxDrawdownTolerance");
	if (m_maxDrawdownTolerance < 0.0)
	{
		throw std::runtime_error("VWAPStrategy: Invalid max drawdown tolerance="
			+ std::to_string(m_maxDrawdownTolerance) + ", must be non-negative.");
	}
	m_maxParticipationRate = riskMetricsXml->DoubleAttribute("ParticipationRate");
	if (m_maxParticipationRate < 0.0 || m_maxParticipationRate > 1.0)
	{
		throw std::runtime_error("VWAPStrategy: Invalid max participation rate="
			+ std::to_string(m_maxParticipationRate) + ", must be between 0.0 and 1.0.");
	}
}

void VWAPStrategy::StartTrade()
{
	try
	{
		// Change Strategy state to live
		m_strategyRunStatus.store(StrategyRunStatus::LIVE, std::memory_order_release);
		// Create Market Data Analyzer
		m_logger->Info("Create market data analyzer.");
		InitializeMarketDataAnalyzer();
		// Initialize market data
		m_logger->Info("Initialize market data.");
		InitMarketData();
		// Create order parammeter generator
		CreateOrderParameterGenerator();
		// Start alarm system to send orders
		m_logger->Info("Starting live and trade.");
		AlarmSystem::Start();
	}
	catch (const std::exception& e)
	{
		m_logger->Exception(std::string(e.what()));
	}
	catch (...)
	{
		m_logger->Exception("Unknown exception occurred.");
	}
}

void VWAPStrategy::StopTrade()
{
	m_strategyRunStatus.store(StrategyRunStatus::STOP, std::memory_order_release);
	// Unsubscribe target symbols to stop receiving real time market data
	m_logger->Info("Unsubscribe target symbols.");
	UnsubscribeTargetSymbols();
}

void VWAPStrategy::OnAlarmTriggered(const int passToDerived)
{
	BEGIN_STRATEGY_ORDER_SENDING_ACTIVITY

		m_logger->Info("Alarm triggered, start sending child orders based on Volume Profile...");

	if (m_strategyRunStatus.load(std::memory_order_acquire) != StrategyRunStatus::LIVE)
	{
		m_logger->Warning("Strategy is not live now, stop sending orders.");
		return;
	}

	std::scoped_lock lock(m_mutex); // only lock here and dont add any blocking code inside child functions
	if (m_activeVWAPOrders.empty())
	{
		m_logger->Warning("No active VWAP orders to process, skip this alarm trigger.");
		return;
	}
	for (auto& order : m_activeVWAPOrders)
	{
		auto* parentOrder = order.second.GetParentOrder();
		const ParentOrderId& parentOrderId = parentOrder->GetParentOrderId();
		const std::string& symbol = parentOrder->GetSymbol();
		auto& vwapComponents = order.second.GetVWAPComponents();

		if (m_marketData->IsSubscribedSymbol(symbol) == false)
		{
			m_logger->Warning("Market data is not subscribed for symbol=" + symbol
				+ ", skip sending VWAP child order.");
			continue;
		}

		// Calculate next VWAP volume size at current time bucket
		const double orderSize = GetOrderSizeForCurrentBucket(parentOrderId, m_currentExchangeTimePointMs);
		const double marketVWAP = CalculateCurrentVWAP(parentOrderId);

		if (orderSize < 0.0)
		{
			m_logger->Warning("No VWAP volume profile available for parentOrderId =" + parentOrderId
				+ ", skip sending VWAP child order.");
			continue;
		}

		if (orderSize > 0.0) // still not finished the target volume
		{
			if (auto* marketDataAnalyzer = m_marketDataAnalyzer->GetQuantMarketDataAnalyzer(symbol))
			{
				std::unique_lock<std::mutex> lock(marketDataAnalyzer->m_mutex);

				const double limitPrice = parentOrder->GetPrice();
				SendVWAPChildOrderToExchange(parentOrderId, symbol, orderSize, limitPrice, parentOrder->GetSide());
				RecordTradeExecution(symbol, orderSize, m_currentExchangeTimePointMs);
				vwapComponents.m_executedPrices = m_spotTrader->GetOrderExecutedPrices(symbol);
				vwapComponents.m_slippageSeries = m_spotTrader->GetOrderExecutedSlippagePrices(symbol);
				vwapComponents.m_pnlSeries = m_spotTrader->GetPnLSeries(symbol);
				vwapComponents.m_vwapPrices.emplace_back(marketVWAP);
			}
			else
			{
				m_logger->Warning("No market data analyzer available for symbol=" + symbol
					+ ", skip sending VWAP child order.");
			}
		}
		else
		{
			m_logger->Info("VWAP target volume reached for symbol=" + symbol
				+ ", total executed volume=" + std::to_string(vwapComponents.m_targetVWAPAmount)
				+ ", stop sending child orders.");
			return;
		}

		// Risk management checks
		if (vwapComponents.m_executedPrices.size() > 5)
		{
			const double avgSlippage
				= RiskManagement::VWAPOrderExecutionRiskMetrics::computeAverageSlippage(vwapComponents.m_executedPrices, vwapComponents.m_vwapPrices);
			const double volSlippage
				= RiskManagement::VWAPOrderExecutionRiskMetrics::computeStdDevSlippage(vwapComponents.m_executedPrices, vwapComponents.m_vwapPrices);
			const double maxDrawdown
				= RiskManagement::VWAPOrderExecutionRiskMetrics::computeMaxDrawdown(vwapComponents.m_pnlSeries);
			const double skew
				= RiskManagement::VWAPOrderExecutionRiskMetrics::computeSkewness(vwapComponents.m_slippageSeries);

			if (avgSlippage > m_averageSlippageTolerance || maxDrawdown > m_maxDrawdownTolerance)
			{
				HaltExecution();
			}
		}
	}

	END_STRATEGY_ORDER_SENDING_NO_RETURN
}

void VWAPStrategy::SendVWAPChildOrderToExchange(
	const ParentOrderId& parentOrderId,
	const std::string& symbol,
	const double orderSize,
	const double limitPrice,
	const OrderManagement::ParentOrderSide side)
{
	const binapi::e_side sideChildOrder = (side == OrderManagement::ParentOrderSide::BUY)
		? binapi::e_side::buy
		: binapi::e_side::sell;
	const auto vwapChildOrder = m_orderParammeterGenerator->GenerateVWAPChildOrder(
		parentOrderId,
		symbol,
		limitPrice,
		orderSize,
		sideChildOrder);

	if (vwapChildOrder.has_value()) {
		const auto newOrder = m_spotTrader->CreateNewPosition(vwapChildOrder.value());
		if (newOrder.first)
		{
			m_logger->Info("Created a new VWAP child order for symbol=" + symbol);
			IncreaseComplianceRestAPIRequestCounter(BinanceTradingRules::SINGLE_REQUEST); // register a sent http request to ComplianceNRegulatory
		}
		else
		{
			m_logger->Error("Failed to create a new VWAP child order for symbol="
				+ symbol);
		}
	}
}

void VWAPStrategy::HaltExecution()
{
	m_logger->Info("Halting strategy execution as risk check.");
	m_strategyRunStatus.store(StrategyRunStatus::STOP, std::memory_order_release);
}

void VWAPStrategy::CreateBinanceExchangeProfile(const std::string& symbol)
{
	m_tradingRules->GetExchangeProfileMgr()->UpdateRemoteExchangeProfiles(symbol, true);
	IncreaseComplianceRestAPIRequestCounter(BinanceTradingRules::SINGLE_REQUEST); // register a sent http request to ComplianceNRegulatory
}

void VWAPStrategy::CreatePortfolioManagement(const std::string& symbol)
{
	m_spotTrader->CreatePortfolioManagement(symbol);
	IncreaseComplianceRestAPIRequestCounter(BinanceTradingRules::SINGLE_REQUEST); // register a sent http request to ComplianceNRegulatory
}

void VWAPStrategy::CreateOrderParameterGenerator()
{
	m_orderParammeterGenerator = std::make_unique<QuantitativeModel::OrderParammeterGenerator>(
		m_tradingRules,
		m_spotTrader->GetPortfolio(),
		m_spotTrader->GetRiskManager(),
		m_spotTrader->GetPositionManager(),
		m_logger.get());
}

void VWAPStrategy::SubscribeMarketData(const std::string& symbol)
{
	if (symbol.empty())
	{
		throw std::runtime_error("VWAPStrategy: Cannot subscribe empty symbol market data.");
	}
	m_marketData->SubscribeSymbol(symbol);
}

void VWAPStrategy::InitMarketData()
{
	// register this class with market data to receive real time data
	m_marketData->RegisterDataListener(this);
}

void VWAPStrategy::UnsubscribeTargetSymbols()
{
	std::scoped_lock lock(m_mutex);
	for (const auto& order : m_activeVWAPOrders)
	{
		m_marketData->UnsubscribeSymbol(order.second.GetParentOrder()->GetSymbol());
	}
}

// external callback for parent order event handling

void VWAPStrategy::OnNewExternalParentOrder(OrderManagement::NewExternalParentOrder* newOrder)
{
	VWAPParentOrder vwapParentOrder;
	vwapParentOrder.SetParentOrder(newOrder);
	vwapParentOrder.GetVWAPComponents().m_targetVWAPAmount = newOrder->GetQuantity();
	vwapParentOrder.GetVWAPComponents().m_vwapVolumeProfilier =
		std::make_unique<VWAPVolumeProfile>(m_profileBucketMs);
	std::scoped_lock lock(m_mutex);
	const auto parentOrderId = vwapParentOrder.GetParentOrder()->GetParentOrderId();
	m_activeVWAPOrders.emplace(parentOrderId, std::move(vwapParentOrder));
	// subscribe market data for the symbol from parent order
	SubscribeMarketData(newOrder->GetSymbol());
	// create market analyzer for the symbol
	m_marketDataAnalyzer->AddQuantMarketDataAnalyzer(newOrder->GetSymbol());
}

void VWAPStrategy::OnCancelExternalParentOrder(OrderManagement::CancelExternalParentOrder* cancelOrder)
{
	m_cancellingParentOrders.emplace_back(
		std::make_shared<OrderManagement::CancelExternalParentOrder>(*cancelOrder));
	std::scoped_lock lock(m_mutex);
	for (auto it = m_activeVWAPOrders.begin(); it != m_activeVWAPOrders.end(); )
	{
		if (it->second.GetParentOrder()->GetParentOrderId() == cancelOrder->GetOriginalParentOrderId())
		{
			m_logger->Info("Cancelling VWAP parent order for symbol="
				+ it->second.GetParentOrder()->GetSymbol());
			it = m_activeVWAPOrders.erase(it);
		}
		else
		{
			++it;
		}
	}
}

void VWAPStrategy::OnAmendExternalParentOrder(OrderManagement::AmendExternalParentOrder* amendOrder)
{
	m_amendingParentOrders.emplace_back( 
		std::make_shared<OrderManagement::AmendExternalParentOrder>(*amendOrder));
	std::scoped_lock lock(m_mutex);
	for (auto it = m_activeVWAPOrders.begin(); it != m_activeVWAPOrders.end(); )
	{
		if (it->second.GetParentOrder()->GetParentOrderId() == amendOrder->GetOriginalParentOrderId())
		{
			m_logger->Info("Amending VWAP parent order for symbol="
				+ it->second.GetParentOrder()->GetSymbol());
			// currently we only support amending target VWAP amount
			it->second.GetParentOrder()->SetQuantity(amendOrder->GetQuantity());
			++it;
		}
		else
		{
			++it;
		}
	}
}