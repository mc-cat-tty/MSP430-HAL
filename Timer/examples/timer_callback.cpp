//
// File timer_callback.cpp
// Author Francesco Mecatti
//
#include <msp430.h>
#include <Timer.h>

void setup(void);
void isr_green(void);
void isr_red(void);

int main(void) {
  WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
  setup();
  TimerA::Timer t;
  t.addCallback(isr_green, 1000);
  t.addCallback(isr_red, 2000);  // Not attached because it would overlap isr_green
  t.addCallback(isr_red, 9999);  // Attached because no overlapping functions exist
  t.start();
  while (1) ;;
}

void setup (void) {
  // *** Green Led configuration ***
  P4DIR |= BIT7;  // Green Led output direction
  P4OUT &= ~BIT7;  // Clear Green Led output

  // *** Red Led configuration ***
  P1DIR |= BIT0;  // Red Led output direction
  P1OUT &= ~BIT0;  // Clear Red Led output
}

void isr_green (void) {
  P4OUT ^= BIT7;  // Toggle Green Led
}

void isr_red(void) {
  P1OUT ^= BIT0;  // Toggle Red Led
}