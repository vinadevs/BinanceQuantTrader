# Gather source files
file(GLOB SOURCE_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
)
list(REMOVE_ITEM SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/dllmain.cpp")
# reports.cpp is from external binapi library (niXman/binapi), not part of this project
list(REMOVE_ITEM SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/reports.cpp")

# Get library name from current directory
get_filename_component(library_name "${CMAKE_CURRENT_SOURCE_DIR}" NAME)

# Find packages
find_package(CURL CONFIG REQUIRED)
find_package(Boost REQUIRED COMPONENTS asio multiprecision)

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
    Boost::asio
    Boost::multiprecision
    LibraryUtils
    KernelTrading
    CurlAPI
)

# Export symbols when compiling this library
target_compile_definitions(${library_name}
PRIVATE
    RESTAPI_EXPORTS
    BINAPI_HEADER_ONLY
)
