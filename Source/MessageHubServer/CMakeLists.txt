# Gather source files
file(GLOB SOURCE_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
)

# Get executable name from current directory
get_filename_component(executable_name "${CMAKE_CURRENT_SOURCE_DIR}" NAME)

# Find packages

# Create the executable
add_executable(${executable_name} ${SOURCE_FILES})

# Include directories
target_include_directories(${executable_name}
PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/.."
)

# Link libraries
target_link_libraries(${executable_name}
PRIVATE
    LibraryUtils
    MiddlewareMQ
    SettingNConfig
)

# Compile definitions
target_compile_definitions(${executable_name}
PRIVATE
    BINAPI_HEADER_ONLY
)
