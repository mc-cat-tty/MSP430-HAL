//
// File Debug.h
// Author Francesco Mecatti
//
#ifndef DEBUG_H  // Include guard
#define DEBUG_H

#include <msp430.h>

// Use it if the function contains dead times
#define __wait __attribute__((optimize("O0")))
#define WAIT_TIME 800000

namespace Debug  {
    // When called turns on green led. Place where you want to valuate if that point is reached
    void reachedA(void);

    // When called turns on red led. Place where you want to valuate if that point is reached
    void reachedB(void);

    // Output the value of num by blinking green led that number of times. Red led blinks before a new "transmission"
    __wait void value(const unsigned short int num);
}

#endif  // DEBUG_H