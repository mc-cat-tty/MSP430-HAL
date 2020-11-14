//
// File no_multiple_timers_demo1.cpp
// Author Francesco Mecatti
//
#include <msp430.h>
#include <Timer.h>


void setup(void);
void toggle_green(void);

int main(void) {
  WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
  setup();
  TimerA::Timer t1;  // Multiple timers are not allowed
  t1.start();  // Instead, use multiple callback functions (attached to one timer)
  TimerA::Timer t2;  // This timer is not configured, because Timer t2 already exists
  for (int i = 0; i < 10; i++) {
      t1.wait(2000);  // This method is executed because t1 works
      toggle_green();
  }
  t2.start();  // This method is not executed
  for (int i = 0; i < 3; i++) {
      t2.wait(2000);  // This method is NOT executed
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