cmake_minimum_required(VERSION 3.20)
project(NetSim)

set(CMAKE_CXX_STANDARD 17)

add_compile_options(-Wall -Wextra -Werror -Wpedantic -pedantic-errors -Wconversion)

include_directories(include)

set(SOURCE_FILES
        src/factory.cpp
        src/helpers.cpp
        src/nodes.cpp
        src/package.cpp
        src/reports.cpp
        src/simulation.cpp
        src/storage_types.cpp
        )

add_executable(own_tests ${SOURCE_FILES} testy.cpp)
add_executable(main ${SOURCE_FILES} main.cpp)

set(SOURCE_FILES_TESTS
        test/testing_tests.cpp
        test/test_package.cpp
        test/test_storage_types.cpp
        )

add_executable(NetSim_test ${SOURCE_FILES} ${SOURCE_FILES_TESTS} test/main_gtest.cpp)

target_include_directories(NetSim_test PUBLIC
        googletest-master/googlemock/include
        googletest-master/googletest/include
        mocks
        )

add_subdirectory(googletest-master)

target_link_libraries(NetSim_test gmock)
