#include <limits.h>
#include <gtest/gtest.h>

#include "sqrt.hpp"
#include "sqrt.cpp"

using namespace std;

/**
 * @brief Test positive numbers in squareRoot function. 
 * 
 * @param SquareRootTest is test fixture name
 * @param PositiveNos is test name
 * 
 * @return 0 if all tests are successfull or 1 otherwise
 *  
 */

TEST(SquareRootTest, PositiveNos) { 
    ASSERT_EQ(6, squareRoot(36.0));
    ASSERT_EQ(18.0, squareRoot(324.0));
    ASSERT_EQ(25.4, squareRoot(645.16));
    ASSERT_EQ(0, squareRoot(0.0));
}

/**
 * @brief Test negative numbers in squareRoot function. 
 * 
 * @param SquareRootTest is test fixture name
 * @param NegativeNos is test name
 * 
 * @return 0 if all tests are successfull or 1 otherwise
 *  
 */

TEST(SquareRootTest, NegativeNos) {
    ASSERT_EQ(-1.0, squareRoot(-15.0));
    ASSERT_EQ(-1.0, squareRoot(-0.2));
}

/**
 * @brief initializes Google Tests and run all tests defined before.
 * 
 * @param argc indicates how many arguments were entered on the command line when the program starts
 * @param argv is an array of pointers to arrays of character objects
 * 
 * @return all test running
 *  
 */

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}




