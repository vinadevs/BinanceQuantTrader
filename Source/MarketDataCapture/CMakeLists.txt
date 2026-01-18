# Gather source files
file(GLOB SOURCE_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
)
# Get library name from current directory
get_filename_component(library_name "${CMAKE_CURRENT_SOURCE_DIR}" NAME)

# Find packages
find_package(Boost REQUIRED COMPONENTS multiprecision)
find_package(SQLite3 REQUIRED)

# Include directories
include_directories(
PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/.."
    ${Boost_INCLUDE_DIRS}
    ${SQLite3_INCLUDE_DIRS}
)

# Create executable
add_executable(${library_name} ${SOURCE_FILES})

# Link libraries
target_link_libraries(${library_name}
PRIVATE
    LibraryUtils
    MarketData
    SettingNConfig
    MiddlewareMQ
    PythonPlugin
    HistoricalData
    Boost::multiprecision
    SQLite::SQLite3
)

# Compile definitions
target_compile_definitions(${library_name}
PRIVATE
    BINAPI_HEADER_ONLY
)