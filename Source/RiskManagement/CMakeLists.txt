# Gather source files
file(GLOB SOURCE_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/VWAPOrderExecutionRiskMetrics.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/VWAPOrderExecutionRiskMetrics.h"
)

# Get library name from current directory
get_filename_component(library_name "${CMAKE_CURRENT_SOURCE_DIR}" NAME)

# Create the library
add_library(${library_name} SHARED ${SOURCE_FILES})

# Include directories
target_include_directories(${library_name}
PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/.."
)

# Export symbols when compiling this library
target_compile_definitions(${library_name}
PRIVATE
    RISKMANAGEMENT_EXPORTS
    BINAPI_HEADER_ONLY
)
