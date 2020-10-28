//
// File debug_value.cpp
// Author Francesco Mecatti
//
#include <msp430.h>
#include <Debug.h>

int main(void) {
  WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
  for (int i = 0; i < 10; i+=2) {
      // I want to know the value of i, but UART (or every other peripheral is not available)
      Debug::value(i);  // The "transmission" starts with red led blink and then
                        // the number of green led's blink represent num (passed arg)
  }
}
