/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#include "pch.h"

#include "UserTradeProfileManager.h"
#include "UserTradeProfileService.h"
#include "UserTradeProfile.h"

#include "../LibraryUtils/Logger.h"

using namespace ExchangeSimulator;

UserTradeProfileService::UserTradeProfileService(UserTradeProfileManager* userTradeProfileManager)
	: m_logger{ std::make_unique<LibraryUtils::Logger>("UserTradeProfileService") },
	m_userTradeProfileManager(userTradeProfileManager)
{
}

UserTradeProfileService::~UserTradeProfileService() {}

grpc::Status UserTradeProfileService::UpdateUserTradeProfile(
	grpc::ServerContext* context,
	const usertradeprofile::UpdateUserTradeProfileRequest* request,
	usertradeprofile::UpdateUserTradeProfileResponse* response)
{
	try
	{
		const std::string& userId = request->user_account_id();
		m_logger->Info("Received UpdateUserTradeProfile request for User ID=" + userId);

		auto& userTradeProfile = m_userTradeProfileManager->LookupUserTradeProfile(userId);
		const double leverage = request->leverage();

		if (leverage <= 0.0 || leverage > 100.0) {
			response->set_success(false);
			response->set_message("Invalid leverage rate.");
			return grpc::Status::OK;
		}

		// Update the leverage for the user trade profile
		userTradeProfile.SetLeverageRate(leverage);

		// Success (stubbed logic)
		response->set_success(true);
		response->set_message("Leverage updated successfully for user " + userId);
		return grpc::Status::OK;
	}
	catch (const std::exception& e)
	{
		response->set_success(false);
		response->set_message("Leverage updating failed for user ID=" 
			+ request->user_account_id() + ", error=" + e.what());
		return grpc::Status::OK;
	}

}

