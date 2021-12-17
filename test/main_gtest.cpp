#include "gtest/gtest.h"
#include "testing_tests.cpp" //dodawanie nieefektywne ale umozliwia niedodawanie niechcianych testow

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
