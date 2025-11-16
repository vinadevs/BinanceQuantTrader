# Gather source files
file(GLOB SOURCE_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
)
# Get library name from current directory
get_filename_component(library_name "${CMAKE_CURRENT_SOURCE_DIR}" NAME)

# Find packages
find_package(Boost REQUIRED COMPONENTS multiprecision)
<<<<<<< HEAD
=======
find_package(SQLite3 REQUIRED)
>>>>>>> 7cdb2301259315d9ab1535b231be77e5bddaf606

# Include directories
include_directories(
PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/.."
    ${Boost_INCLUDE_DIRS}
<<<<<<< HEAD
=======
    ${SQLite3_INCLUDE_DIRS}
>>>>>>> 7cdb2301259315d9ab1535b231be77e5bddaf606
)

# Create executable
add_executable(${library_name} ${SOURCE_FILES})

# Link libraries
target_link_libraries(${library_name}
PRIVATE
    LibraryUtils
    MarketData
    SettingNConfig
    Boost::multiprecision
<<<<<<< HEAD
=======
    SQLite::SQLite3
>>>>>>> 7cdb2301259315d9ab1535b231be77e5bddaf606
)