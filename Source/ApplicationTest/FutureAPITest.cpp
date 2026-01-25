// ApplicationTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include "CurlAPI/BinanceFutureApiGateway.h"

using namespace CurlAPI;

int main()
{
	// Initialize the Binance Future API Gateway
	BinanceFutureApiGateway::GetInstance().InitiateAPI(
		"fapi.binance.com",
		"test",
		"test");
	// Set leverage rate
	BinanceFutureApiGateway::GetInstance().SetLeverageRate(
		"BTCUSDT",
		20);
	// Send a test order
	BinanceFutureApiGateway::GetInstance().SendTestOrder(
		"BTCUSDT",
		"BUY",
		"LIMIT",
		"0.01",
		"testOrderId");
	// Cancel an order
	BinanceFutureApiGateway::GetInstance().CancelOrder(
		"BTCUSDT",
		"testOrderId");
	// Query an order
	BinanceFutureApiGateway::GetInstance().QueryOrder(
		"BTCUSDT",
		"testOrderId");

	// Get leverage bracket for a symbol
	auto leverageBracket = BinanceFutureApiGateway::GetInstance().GetLeverageBracket("BTCUSDT");
	std::cout << "Leverage Bracket for BTCUSDT:\n" << leverageBracket << std::endl;
    return EXIT_SUCCESS;
}
