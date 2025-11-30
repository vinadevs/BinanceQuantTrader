#include "pch.h"

#include "../SettingNConfig/tinyxml2.h"
#include "../LibraryUtils/Logger.h"

#include "StrategyParentOrderHttpService.h"
#include "StrategyRequestHttpService.h"
#include "ExternalController.h"

using namespace TradingStrategies;
using namespace LibraryUtils;

ExternalController::ExternalController(const tinyxml2::XMLElement* binanceRestAPIServerXmlCfg)
	: m_logger{ std::make_unique<LibraryUtils::Logger>("ExternalController") }
	, m_strategyRequestHttpService{ std::make_unique<StrategyRequestHttpService>() }
	, m_strategyParentOrderHttpService{ std::make_unique<StrategyParentOrderHttpService>() }
{
	assert(binanceRestAPIServerXmlCfg);
	const auto* connectionXml = binanceRestAPIServerXmlCfg->FirstChildElement("Connection");
	assert(connectionXml);
	m_logger = std::make_unique<LibraryUtils::Logger>("ExternalController");
	m_serverIpAddress = std::string(connectionXml->Attribute("ServerIpAddress"));
	m_serverPort = std::string(connectionXml->Attribute("ServerPort"));
	m_serverConnection = m_serverIpAddress + ":" + m_serverPort;
}

ExternalController::~ExternalController()
{
	m_logger->Info("Stopping ExternalController.");
	Stop();
	m_logger->Info("ExternalController stopped.");
}

void ExternalController::Start()
{
	m_grpcThread = std::thread(&ExternalController::WaitForIncomingMessage, this);
	m_grpcThread.detach();
}

void ExternalController::Stop()
{
	if (m_grpcServer)
	{
		m_grpcServer->Shutdown();
	}
	if (m_grpcThread.joinable())
	{
		m_grpcThread.join();
	}
}

void ExternalController::RegisterTargetStrategy(ExternalRequestReceiver* recevier)
{
	m_strategyParentOrderHttpService->AddTargetStrategy(recevier);
}

void ExternalController::RegisterTargetAdminRequestHandler(ExternalRequestReceiver* recevier)
{
	m_strategyRequestHttpService->AddTargetAdminRequestHandler(recevier);
}

void ExternalController::WaitForIncomingMessage()
{
	grpc::ServerBuilder builder;
	builder.AddListeningPort(m_serverConnection, grpc::InsecureServerCredentials());
	builder.RegisterService(m_strategyRequestHttpService.get());
	builder.RegisterService(m_strategyParentOrderHttpService.get());
	m_grpcServer = builder.BuildAndStart();
	m_logger->Info("Strategy is listening external parent order on " + m_serverConnection);
	m_grpcServer->Wait(); // wait call
}
