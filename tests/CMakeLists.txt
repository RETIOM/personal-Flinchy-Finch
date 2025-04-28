enable_testing()

find_package(GTest REQUIRED)
find_package(Eigen3 REQUIRED)

add_executable(network_test
        genome_test.cpp
)

target_link_libraries(network_test
        PRIVATE
        flinchy_core
        GTest::gtest_main
        Eigen3::Eigen
        pthread
)

include(GoogleTest)
gtest_discover_tests(network_test)
