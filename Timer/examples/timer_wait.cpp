//
// File timer_wait.cpp
// Author Francesco Mecatti
//
#include <msp430.h>
#include <Timer.h>

void setup(void);
void toggle_green(void);

int main(void) {
  WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
  setup();
  TimerA::Timer t;
  t.start();
  while (1) {
      t.wait(2000);
      toggle_green();
  }
}

void setup (void) {
  // *** Green Led configuration ***
  P4DIR |= BIT7;  // Green Led output direction
  P4OUT &= ~BIT7;  // Clear Green Led output
}

void toggle_green (void) {
  P4OUT ^= BIT7;  // Toggle Green Led
}