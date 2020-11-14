//
// File Timer.h
// Author Francesco Mecatti
//
#ifndef TIMER_H  // Include guard
#define TIMER_H

#include <msp430.h>
#include <stdint.h>
#include <stdbool.h>


#define __await __attribute__((optimize("O0")))  // Use it if the function contains dead times

namespace TimerA {  // Timer_A is a 16-bit timer/counter with up to seven capture/compare registers.
    //  *** Structure ***
    //  Registers
    //      Fields
    //          Values

    // dec_reg CTL = TA0CTL;  // ConTroL register - for configuration
        // SSEL --> Source clock SELect field
            enum SourceSelect { TACLK = TASSEL__TACLK,
                                ACLK = TASSEL__ACLK,  // 33 kHz
                                SMCLK = TASSEL__SMCLK,  // 1 MHz
                                INCLK = TASSEL__INCLK };
        // ID --> Input Divider field
            enum InputDivider { DIV1 = ID__1,
                                DIV2 = ID__2,
                                DIV4 = ID__4,
                                DIV8 = ID__8 };
        // MC --> Mode Control field
            enum ModeControl {  STOP = MC__STOP,           // The timer is halted
                                UP = MC__UP,             // The timer repeatedly counts from zero to the value of TAxCCR0
                                CONTINUOUS = MC__CONTINOUS,     // The timer repeatedly counts from zero to 0FFFFh.
                                UPDOWN = MC__UPDOWN          // The timer repeatedly counts from zero up to the value of TAxCCR0 and back down to zero.
                                };
        // dec_field CLR = TACLR;  // CLeaR bit
        // dec_field IE = TAIE;    // Interrupt Enable
                                // 0b interrupt disabled
                                // 1b interrupt enabled
        // dec_field IFG = TAIFG;  // Interrupt Flag
                                // 0b no interrupt pending
                                // 1b interrupt pending
    // dec_reg R = TA0R;   // counteR - count of Timer_A
                        // Register that increments/decrements at each rising/falling edge of the clock signal
    // dec_reg CCR0 = TA0CCR0;     // Counter upper bound - when reached Timer_A ISR is executed
                                // If is equal to 0b the timer stops, otherwise it operates in MC mode

    typedef void (*isr_pointer)(void);  // ISR - Interrupt Service Routine - pointer


    class Timer {
    public:
        Timer();  // Creates timer in count mode - used to wait() or to keep track of elapsed time
        // Timer(const uint32_t millis, isr_pointer callback_function);  // Creates timer and attach callaback_function (called every millis [ms])
        uint32_t getElapsedTime(void) const;  // Getter of counter
        bool addCallback(const isr_pointer callback_function, const uint32_t millis);  // callback_function called every millis
        __await void wait(const uint32_t millis) const;  // Waits millis [ms] - do nothing for millis and then return
        void reset(void);
        uint32_t start(void) const;  // Run timer. return: current counter value (elapsed millis)
        uint32_t stop(void) const;  // Stop timer. return: current counter value (elapsed millis)
        ~Timer();  // Deallocate memory and decrease sTimerInstanceCount

    private:
        static uint8_t sTimerInstanceCount;  // Only one instance of this class can exist at a time
        bool unique;  // True if the instance was unique when constructed
        static uint32_t counter;  // Internal counter [ms] - it contains elapsed millis. Up to 2^32 milliseconds
        static uint32_t upperBound;  // When reached counter is reset
        static isr_pointer aCallbackFunctions[64];  // Multiple callback functions are supported (executed at different time intervals)
        static uint32_t aTimesMap[64];  // Location i contains the time [ms] at which aCallbackFunctions[i] must be executed
        static uint8_t mapDim;  // Number of callback functions inserted

        // void set(uint32_t reg, const uint32_t value);  // value --> bits to set at 1, others at 0
        // void unset(uint32_t reg, const uint32_t value);  // value --> bits to unset at 1, others at 0     
        void setSourceSelect(const SourceSelect ssel) const;
        void setInputDivider(const InputDivider id) const;
        void setModeControl(const ModeControl mc) const;
        void enableInterrupt(void) const;
        void disableInterrupt(void) const;
        void clearInterruptFlag(void) const;
        void setCounter(const uint32_t);
        uint32_t getCounter(void) const;
        void setUpperBound(const uint32_t);
        uint32_t getUpperBound(void) const;
        void increaseCounter(void);
        bool isUniqueInstance(void) const;
        __interrupt void interrupt_handler(void);

        bool isAttachedToISR(uint32_t millis) const;
        isr_pointer getISRAttachedTo(uint32_t millis) const;
    };
    
}

#endif  // TIMER_H