# Gather source files
file(GLOB SOURCE_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
)

# TODO: Add proto generation steps here
# # Proto library
# add_library(ProtoExample src/proto/Example.proto)
# target_link_libraries(ProtoExample PUBLIC gRPC::grpc++)

# protobuf_generate(TARGET ProtoExample)
# protobuf_generate(
#   TARGET ProtoExample
#   LANGUAGE grpc
#   PLUGIN protoc-gen-grpc=$<TARGET_FILE:gRPC::grpc_cpp_plugin>
#   PLUGIN_OPTIONS generate_mock_code=true
#   GENERATE_EXTENSIONS .grpc.pb.h .grpc.pb.cc
# )

file(GLOB PROTO_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/protobuf/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/protobuf/*.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/protobuf/*.h"
)

list(REMOVE_ITEM SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/dllmain.cpp")

# Add to global formatting list
set(PROJECT_FORMAT_FILES
    ${PROJECT_FORMAT_FILES}
    ${SOURCE_FILES}
    CACHE INTERNAL "All files to format" FORCE
)

# Get library name from current directory
get_filename_component(library_name "${CMAKE_CURRENT_SOURCE_DIR}" NAME)

# Find packages
find_package(Boost REQUIRED COMPONENTS multiprecision)
find_package(protobuf REQUIRED)
find_package(gRPC CONFIG REQUIRED)
find_package(absl CONFIG REQUIRED)

# Create the library
add_library(${library_name} SHARED ${SOURCE_FILES} ${PROTO_FILES})

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
    UserAccount
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
