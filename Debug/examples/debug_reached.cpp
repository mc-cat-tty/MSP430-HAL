//
// File debug_reached.cpp
// Author Francesco Mecatti
//
#include <msp430.h>
#include <Debug.h>

void test(void);

int main(void) {
  WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
  int x = 0;
  while (x++ < 10) ;;  // Do nothing
  for (int i = 0; i < 100; i++) {
      if (i == 50) {
          // I want to know if this code will ever be reached
          Debug::reachedA();  // So I place the reachedA() function
      }
      else if (i == 60) {
          Debug::reachedA();  // Point A is already reached, so it blinks the led
      }
      else if (i == 70) {
          Debug::reachedA();
      }
      else if (i==80) {
          test();
      }
  }
}

void test(void) {
    // I want to know if this code will ever be reached
    Debug::reachedB();  // So I place the reachedB() function
}
