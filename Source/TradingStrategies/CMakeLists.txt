# Gather source files
file(GLOB SOURCE_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/protobuf/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/protobuf/*.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/protobuf/*.h"
)
list(REMOVE_ITEM SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/dllmain.cpp")

# Get library name from current directory
get_filename_component(library_name "${CMAKE_CURRENT_SOURCE_DIR}" NAME)

# Find packages
find_package(Boost REQUIRED COMPONENTS multiprecision)
find_package(protobuf REQUIRED)
find_package(gRPC CONFIG REQUIRED)
find_package(absl CONFIG REQUIRED)

# Create the library
add_library(${library_name} SHARED ${SOURCE_FILES})

# Include directories
target_include_directories(${library_name}
PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/.."
)

# Link libraries
target_link_libraries(${library_name}
PRIVATE
    Boost::multiprecision
    LibraryUtils
    KernelTrading
    MiddlewareMQ
    MarketData
    OrderManagement
    OrderRouting
    PortfolioManager
    QuantitativeModel
    ComplianceNRegulatory
    RiskManagement
    IndicatorNSignals
    RestAPI
    SettingNConfig
    protobuf::libprotobuf
    gRPC::grpc++
    absl::log_internal_message
    absl::log_internal_check_op
)

# Export symbols when compiling this library
target_compile_definitions(${library_name}
PRIVATE
    TRADINGSTRATEGIES_EXPORTS
    BINAPI_HEADER_ONLY
)
