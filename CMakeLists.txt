# Copy from cpp-best-practices/cmake_template repository

cmake_minimum_required(VERSION 3.30)

# Define cxx_standard if it is not defined
if(NOT DEFINED CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 17)
endif()

# strongly encouraged to enable this globally to avoid conflicts between
# -Wpedantic being enabled and -std=c++20 and -std=gnu++20 for example
# when compiling with PCH enabled
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Lib)


# Suppress developer warnings
set(CMAKE_SUPPRESS_DEVELOPER_WARNINGS ON CACHE BOOL "" FORCE)

# Define project name and version
project(
    BinanceQuantTrader
    VERSION 0.0.1
    DESCRIPTION ""
    LANGUAGES CXX C)

set(GIT_SHA
    "Unknown"
    CACHE STRING "SHA this build was generated from")
string(
  SUBSTRING "${GIT_SHA}"
            0
            8
            GIT_SHORT_SHA)

if (MSVC)
    add_compile_options(/bigobj)
endif()

# Add subdirectories
# Core utilities (no dependencies)
add_subdirectory(Source/LibraryUtils)

# Base libraries (depend on LibraryUtils)
add_subdirectory(Source/SettingNConfig)
add_subdirectory(Source/KernelTrading)

# Network and API libraries
add_subdirectory(Source/RealTimeSocket)
add_subdirectory(Source/CurlAPI)
add_subdirectory(Source/RestAPI)
add_subdirectory(Source/MiddlewareMQ)

# Data libraries
add_subdirectory(Source/SqlDatabase)
add_subdirectory(Source/HistoricalData)
add_subdirectory(Source/MarketData)
add_subdirectory(Source/MarketDataCapture)

# Core trading libraries
add_subdirectory(Source/StaticData)
add_subdirectory(Source/ComplianceNRegulatory)
add_subdirectory(Source/PortfolioManager)
add_subdirectory(Source/OrderManagement)
add_subdirectory(Source/RiskManagement)
add_subdirectory(Source/QuantitativeModel)
add_subdirectory(Source/IndicatorNSignals)
add_subdirectory(Source/MacroData)

# Connectivity libraries
add_subdirectory(Source/ExchangeConnectivity)
add_subdirectory(Source/ExchangeSimulator)
add_subdirectory(Source/OrderRouting)

# Account and strategies
add_subdirectory(Source/UserAccount)
add_subdirectory(Source/TradingStrategies)

# Python integration
add_subdirectory(Source/PythonPlugin)

# Executables
add_subdirectory(Source/ApplicationTest)
add_subdirectory(Source/BackTesting)
add_subdirectory(Source/BinanceQuantTrader)
add_subdirectory(Source/MessageHubServer)

# Tests
add_subdirectory(Source/UnitTest)