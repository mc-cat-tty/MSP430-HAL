//
// File Debug.h
// Author Francesco Mecatti
//
#ifndef DEBUG_H  // Include guard
#define DEBUG_H

#include <msp430.h>

// Use it if the function contains dead times
#define __await __attribute__((optimize("O0")))  // Avoid optimization that would remove delays
#define WAIT_TIME 800000

namespace Debug  {
    // When called turns on green led (if already on blinks and leaves the led on). Place where you want to valuate if that point is reached
    __await void reachedA(void);

    // When called turns on red led (if already on blinks and leaves the led on). Place where you want to valuate if that point is reached
    __await void reachedB(void);

    // Output the value of num by blinking green led that number of times. Red led blinks before a new "transmission"
    __await void value(const unsigned short int num);
}

#endif  // DEBUG_H