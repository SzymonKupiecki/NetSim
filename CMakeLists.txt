cmake_minimum_required(VERSION 3.20)
project(NetSim)

set(CMAKE_CXX_STANDARD 17)

add_compile_options(-Wall -Wextra -Werror -Wpedantic -pedantic-errors -Wconversion)

include_directories(include
                    test)

set(SOURCE_FILES
        src/factory.cpp
        src/helpers.cpp
        src/nodes.cpp
        src/package.cpp
        src/reports.cpp
        src/simulation.cpp
        src/storage_types.cpp
        src/types.cpp
        )


add_executable(own_test ${SOURCE_FILES} testy.cpp)
add_executable(main ${SOURCE_FILES} main.cpp)
add_executable(PackageQueue ${SOURCE_FILES} test/PackageQueue_tests.cpp)
