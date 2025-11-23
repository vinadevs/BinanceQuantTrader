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

# Set default runtime output folder for all targets
set(OUTPUT_DIR ${CMAKE_BINARY_DIR}/Debug)
set(OUTPUT_DIR_RELEASE ${CMAKE_BINARY_DIR}/Release)

# For multi-config generators (Visual Studio)
foreach(CONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${CONFIG} CONFIG_UPPER)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${CONFIG_UPPER} ${CMAKE_BINARY_DIR}/${CONFIG})
endforeach()

# For single-config generators (Linux Makefiles, Ninja)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Debug)

# Add subdirectories
add_subdirectory(Source/RiskManagement)
add_subdirectory(Source/UnitTest)
add_subdirectory(Source/LibraryUtils)
add_subdirectory(Source/SettingNConfig)
add_subdirectory(Source/KernelTrading)
add_subdirectory(Source/RealTimeSocket)
add_subdirectory(Source/MarketData)
add_subdirectory(Source/MarketDataCapture)
