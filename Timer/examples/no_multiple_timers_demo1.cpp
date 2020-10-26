//
// File no_multiple_timers_demo1.cpp
// Author Francesco Mecatti
//
#include <msp430.h>
#include <Timer.h>

// TODO: this demo doesn't work (probable problem: instance counter doesn't increment; vector for timer is overwritten)

void setup(void);
void toggle_green(void);

int main(void) {
  WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
  setup();
  TimerA::Timer t1;  // Multiple timers are not allowed
  t1.start();  // Instead, use multiple callback functions (attached to one timer)
  TimerA::Timer t2;  // This timer is configured
  // t2.stop();  // This method is not executed
  while (1) {
      t1.wait(2000);
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