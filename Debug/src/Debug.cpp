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
    __wait void blinkGreenLed(void);
    __wait void blinkRedLed(void);

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

    __wait void blinkGreenLed(void) {
        __delay_cycles(WAIT_TIME);
        turnOnGreenLed();
        __delay_cycles(WAIT_TIME);
        turnOffGreenLed();
    }

    __wait void blinkRedLed(void) {
        __delay_cycles(WAIT_TIME);
        turnOnRedLed();
        __delay_cycles(WAIT_TIME);
        turnOffRedLed();
    }
}

void Debug::reachedA(void) {
    setupGreenLed();
    turnOnGreenLed();
}

void Debug::reachedB(void) {
    setupRedLed();
    turnOnRedLed();
}

__wait void Debug::value(const unsigned short int num) {
    setupGreenLed();
    setupRedLed();
    blinkRedLed();
    for (int i = 0; i < num; i++) {
        blinkGreenLed();
    }
}