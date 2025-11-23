find_package(Boost REQUIRED COMPONENTS unit_test_framework)

project(UnitTest)

# Set all of the include directories so that you can include the necessary header files
include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(${PROJECT_NAME}
    Test_VWAPOrderExecutionRiskMetrics.cpp
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        RiskManagement
        Boost::unit_test_framework
)

enable_testing()
add_test(NAME BoostTests COMMAND ${PROJECT_NAME})

