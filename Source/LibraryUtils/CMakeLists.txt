# Gather source files
file(GLOB SOURCE_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
)
list(REMOVE_ITEM SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/dllmain.cpp")

# Get library name from current directory
get_filename_component(library_name "${CMAKE_CURRENT_SOURCE_DIR}" NAME)

# Find packages
find_package(Boost REQUIRED COMPONENTS algorithm)

# Create the library
add_library(${library_name} SHARED ${SOURCE_FILES})

# Include directories
target_include_directories(${library_name}
PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/.."
    ${Boost_INCLUDE_DIRS}
)

# Link JSON
target_link_libraries(${library_name}
PRIVATE
    Boost::algorithm 
)

# Export symbols when compiling this library
target_compile_definitions(${library_name}
PRIVATE
    LIBRARYUTILS_EXPORTS
    BINAPI_HEADER_ONLY
)
