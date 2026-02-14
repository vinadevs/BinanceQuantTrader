# Gather source files
file(GLOB SOURCE_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
)
list(REMOVE_ITEM SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/dllmain.cpp")

# Get library name from current directory
get_filename_component(library_name "${CMAKE_CURRENT_SOURCE_DIR}" NAME)

# Find packages
find_package(SQLite3 REQUIRED)

# Include directories
include_directories(
PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/.."
    ${SQLite3_INCLUDE_DIRS}
)

# Create the library
add_library(${library_name} SHARED ${SOURCE_FILES})

# Link libraries
target_link_libraries(${library_name}
PRIVATE
    LibraryUtils
    SettingNConfig
    SQLite::SQLite3
)

# Compile definitions
target_compile_definitions(${library_name}
PRIVATE
    HISTORICALDATA_EXPORTS
    BINAPI_HEADER_ONLY
)