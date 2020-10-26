#include "Timer.h"

using namespace TimerA;


#define dec_reg auto&  // Declare Register
#define dec_field const auto&  // Declare Field

dec_reg CTL = TA0CTL;  // ConTroL register - for configuration
    dec_field CLR = TACLR;  // CLeaR bit
    dec_field IE = TAIE;    // Interrupt Enable
    dec_field IFG = TAIFG;  // Interrupt Flag
dec_reg R = TA0R;   // counteR - count of Timer_A
dec_reg CCR0 = TA0CCR0;     // Counter upper bound - when reached Timer_A ISR is executed

#undef dec_reg
#undef dec_field

uint8_t Timer::sTimerInstanceCount = 0;
isr_pointer Timer::aCallbackFunctions[64];
uint32_t Timer::aTimesMap[64];
uint8_t Timer::mapDim;
uint32_t Timer::counter;
bool Timer::unique;
uint32_t Timer::upper_bound;


Timer::Timer() {
    this->sTimerInstanceCount++;

    if (this->isUniqueInstance()) unique = true;
    else unique = false;
    
    if (not this->unique) return;
    

    this->reset();
    this->upper_bound = 0;
    this->mapDim = 0;

    // *** Timer configuration ***
    this->stop();
    setSourceSelect(ACLK);
    CCR0 = 33;  // Interrupt every millisecond
    this->enableInterrupt();
    this->clearInterruptFlag();

    __enable_interrupt();  // GIE = 1
}

/*
Timer::Timer(const uint32_t millis, isr_pointer callback_function) : Timer() {
    if (not this->unique) return;
    this->addCallback(callback_function, millis);
}
*/

Timer::~Timer() {
    this->sTimerInstanceCount--;
}

uint32_t Timer::getElapsedTime(void) const {
    if (not this->unique) return 0;
    return this->counter;
}

bool Timer::addCallback(const isr_pointer callback_function, const uint32_t millis) {
    if (not this->unique) return false;
    bool isOverlapping = false;  // Checks if overlapping function exist
    for (int i = 0; i < this->mapDim; i++) {
        if (millis%this->aTimesMap[i] == 0 || this->aTimesMap[i]%millis == 0 ) {
            isOverlapping = true;
            break;
        }
    }
    if (this->mapDim < 64 && not isOverlapping) {
        if (millis > this->upper_bound) this->upper_bound = millis;  // Updating upper bound
        this->aTimesMap[this->mapDim] = millis;
        this->aCallbackFunctions[this->mapDim] = callback_function;
        this->mapDim++;
        return true;
    }
    return false;  // There is no more space
}

void Timer::reset(void) {
    if (not this->unique) return;
    this->counter = 0;
}

uint32_t Timer::start(void) const{
    if (not this->unique) return 0;
    uint32_t elapsed_time = this->getElapsedTime();
    this->setModeControl(UP);
    return elapsed_time;
}

uint32_t Timer::stop(void) const{
    if (not this->unique) return 0;
    this->setModeControl(STOP);
    return this->getElapsedTime();
}

void Timer::setSourceSelect(const SourceSelect ssel) const {
    if (not this->unique) return;
    CTL |= ssel;
}

void Timer::setInputDivider(const InputDivider id) const {
    if (not this->unique) return;
    CTL |= id;
}

void Timer::setModeControl(const ModeControl mc) const {
    if (not this->unique) return;
    CTL |= mc;
}

void Timer::enableInterrupt(void) const {
    if (not this->unique) return;
    CTL |= IE;
}

void Timer::disableInterrupt(void) const {
    if (not this->unique) return;
    CTL &= ~IE;
}

void Timer::clearInterruptFlag(void) const {
    if (not this->unique) return;
    CTL &= ~IFG;
}

void Timer::setCounter(const uint32_t counter) {
    if (not this->unique) return;
    this->counter = counter;  // No overflow check
}

uint32_t Timer::getCounter(void) const {
    if (not this->unique) return 0;
    return this->counter;
}

void Timer::setUpperBound(const uint32_t upper_bound) {
    if (not this->unique) return;
    this->upper_bound = upper_bound;
}

uint32_t Timer::getUpperBound(void) const {
    if (not this->unique) return 0;
    return this->upper_bound;
}

void Timer::increaseCounter(void) {
    if (not this->unique) return;
    this->counter++;
}

bool Timer::isUniqueInstance(void) const {
    return (this->sTimerInstanceCount == 1);
}

isr_pointer Timer::getISRAttachedTo(uint32_t millis) const {
    if (not this->unique) return nullptr;
    for (int i = 0; i < mapDim; i++) {
        if (millis != 0 && millis%(this->aTimesMap[i]/2) == 0)
            return this->aCallbackFunctions[i];
    }
    return nullptr;
}

bool Timer::isAttachedToISR(uint32_t millis) const{
    if (not this->unique) return false;
    return (this->getISRAttachedTo(millis) != nullptr);
}

#pragma vector = TIMER0_A1_VECTOR
__interrupt void Timer::interrupt_handler(void) {
    this->increaseCounter();
    if (this->isAttachedToISR(this->counter)) this->getISRAttachedTo(this->counter)();
    if (this->getCounter() > this->upper_bound) this->reset();
    this->clearInterruptFlag();
}