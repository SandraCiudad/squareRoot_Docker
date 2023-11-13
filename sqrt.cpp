#include "sqrt.hpp"
#include <math.h>

/**
 * @brief calculates the square root of a number
 * 
 * @param a is the number whose square root is going to be calculated
 * 
 * @return the square root of a
 *  
 */

double squareRoot(const double a) {
    double b = sqrt(a);
    if(b != b) { 
        return -1.0; 
    }
    else { 
        return sqrt(a); 
    }
}