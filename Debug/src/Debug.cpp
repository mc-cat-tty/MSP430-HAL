//
// File Debug.cpp
// Author Francesco Mecatti
//
#include "Debug.h"

namespace {  // Anonymous namespace for private members
    void set(volatile unsigned char*, const short int&);
    void unset(volatile unsigned char*r, const short int&);
    void setupGreenLed(void);
    void setupRedLed(void);
    void turnOnGreenLed(void);
    void turnOffGreenLed(void);
    void turnOnRedLed(void);
    void turnOffRedLed(void);
    __await void blinkGreenLed(void);
    __await void blinkRedLed(void);

    void set(volatile unsigned char* reg, const short int& mask) {
        *reg |= mask;
    }

    void unset(volatile unsigned char* reg, const short int& mask) {
        *reg &= ~mask;
    }

    void setupGreenLed(void) {
        WDTCTL = WDTPW | WDTHOLD;  // It is always called at least once
        set(&P4DIR, BIT7);
        turnOffGreenLed();  // Initial clearing
    }
    
    void setupRedLed(void) {
        WDTCTL = WDTPW | WDTHOLD;  // It is always called at least once
        set(&P1DIR, BIT0);
        turnOffRedLed();  // Initial clearing
    }

    void turnOnGreenLed(void) {
        set(&P4OUT, BIT7);
    }

    void turnOnRedLed(void) {
        set(&P1OUT, BIT0);
    }

    void turnOffGreenLed(void) {
        unset(&P4OUT, BIT7);
    }

    void turnOffRedLed(void) {
        unset(&P1OUT, BIT0);
    }

    __await void blinkGreenLed(void) {
        __delay_cycles(WAIT_TIME);
        turnOnGreenLed();
        __delay_cycles(WAIT_TIME);
        turnOffGreenLed();
    }

    __await void blinkRedLed(void) {
        __delay_cycles(WAIT_TIME);
        turnOnRedLed();
        __delay_cycles(WAIT_TIME);
        turnOffRedLed();
    }
}

__await void Debug::reachedA(void) {
    setupGreenLed();  // During setup the led is turned off
    __delay_cycles(WAIT_TIME);  // Wait + turnOn ==> blink
    turnOnGreenLed();  // If already on blinks, otherwise nothing change
    __delay_cycles(WAIT_TIME);  // Maintain the current state
}

__await void Debug::reachedB(void) {
    setupRedLed();  // During setup the led is turned off
    __delay_cycles(WAIT_TIME*2);  // Wait + turnOn ==> blink
    turnOnRedLed();  // If already on blinks, otherwise nothing change
    __delay_cycles(WAIT_TIME);  // Maintain the current state
}

__await void Debug::value(const unsigned short int num) {
    setupGreenLed();
    setupRedLed();
    blinkRedLed();
    for (int i = 0; i < num; i++) {
        blinkGreenLed();
    }
}