/**
 * @author Markus Bader <markus.bader@tuwien.ac.at>
 * @brief Simple servo driver for an arduino zero to controll
 * three servos with timer 1
 * @file Servos_zero.cpp
 * @license Simplified BSD License
 */


#include <Servos_zero.h>



unsigned int Servos::timer_irq_ovf_count = 0;
unsigned int Servos::timer_irq_mc0_count = 0;
unsigned int Servos::timer_irq_mc1_count = 0;
unsigned int Servos::timer_irq_mc2_count = 0;

/// 1us on f_clk = 48Mhz and prescaler = 16 : ( 0.000001 / (1.0/48000000*16) )
const int32_t Servos::counter_ticks_per_micro_secounds = 3;

int Servos::pin_output[3];       /// output pin for servo 0, values < 0 means inactive
int32_t Servos::servo_compare_value[3];
int32_t Servos::servo_neutral_signal_micro_secounds[3];   /// servo neutral pose
float Servos::servo_counter_ticks_per_rad[3];

/// Timer
Tcc* Servos::TC = ( Tcc* ) TCC1;

Servos::Servos() {
    for ( unsigned int i = 0; i < 3; i++ ) {
        pin_output[i] = -1;
        servo_neutral_signal_micro_secounds[i] = 1500;
        servo_counter_ticks_per_rad[i] = ( 48000000/1000 ) / ( 16.0 * M_PI );
        servo_compare_value[i] = servo_neutral_signal_micro_secounds[i] * counter_ticks_per_micro_secounds;
    }
}
void Servos::setupServosOnTimer1 ( int pin_0, int pin_1, int pin_2 ) {

    //set pins as outputs
    pin_output[0] = pin_0;
    pin_output[1] = pin_1;
    pin_output[2] = pin_2;

    REG_GCLK_CLKCTRL = ( GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN_GCLK0 | GCLK_CLKCTRL_ID_TCC0_TCC1 ); // Enable clock for Timer
    while ( GCLK->STATUS.bit.SYNCBUSY == 1 ); // wait for sync

    TC->CTRLA.reg &= ~TCC_CTRLA_ENABLE;   // Disable TC
    while ( TC->SYNCBUSY.bit.ENABLE == 1 ); // wait for sync

    TC->CTRLA.reg |= TCC_CTRLA_PRESCALER_DIV16;   // Set perscaler


    TC->WAVE.reg |= TCC_WAVE_WAVEGEN_NFRQ;   // Set wave form configuration
    while ( TC->SYNCBUSY.bit.WAVE == 1 ); // wait for sync

    TC->PER.reg = 60000;                 // Set counter Top using the PER register
    while ( TC->SYNCBUSY.bit.PER == 1 ); // wait for sync

    // Interrupts
    TC->INTENSET.reg = 0;                 // disable all interrupts
    TC->INTENSET.bit.OVF = 1;             // enable overfollow

    if ( pin_output[0] > 0 ) {
        pinMode ( pin_output[0], OUTPUT );
        digitalWrite ( pin_output[0], LOW );
        TC->CC[0].reg = servo_compare_value[0];
        while ( TC->SYNCBUSY.bit.CC0 == 1 ); // wait for sync
        TC->INTENSET.bit.MC0 = 1;          // enable compare match to CC0
    }
    if ( pin_output[1] > 0 ) {
        pinMode ( pin_output[1], OUTPUT );
        digitalWrite ( pin_output[1], LOW );
        TC->CC[1].reg = ( uint32_t ) servo_compare_value[1];
        while ( TC->SYNCBUSY.bit.CC1 == 1 ); // wait for sync
        TC->INTENSET.bit.MC1 = 1;          // enable compare match to CC1
    }
    if ( pin_output[2] > 0 ) {
        pinMode ( pin_output[2], OUTPUT );
        digitalWrite ( pin_output[2], LOW );
        TC->CC[2].reg = ( uint32_t ) servo_compare_value[2];
        while ( TC->SYNCBUSY.bit.CC2 == 1 ); // wait for sync
        TC->INTENSET.bit.MC2 = 1;          // enable compare match to CC2
    }

    NVIC_EnableIRQ ( TCC1_IRQn ); // Enable InterruptVector

    // Enable TC
    TC->CTRLA.reg |= TCC_CTRLA_ENABLE ;
    while ( TC->SYNCBUSY.bit.ENABLE == 1 ); // wait for sync
};

void Servos::setNeutral ( int32_t micro_secounds, uint8_t servo ) {
    servo_neutral_signal_micro_secounds[servo] = micro_secounds;
}
int32_t Servos::getNeutral ( int32_t micro_secounds, uint8_t servo ) {
    return servo_neutral_signal_micro_secounds[servo];
}
void Servos::setHighTime ( int32_t micro_secounds, uint8_t servo ) {
    servo_compare_value[servo] = micro_secounds * counter_ticks_per_micro_secounds;
    set_compare_register ( servo );
}
void Servos::setRad ( float rad, uint8_t servo ) {
    int32_t offset = servo_counter_ticks_per_rad[servo] * rad;
    int32_t base = servo_neutral_signal_micro_secounds[servo] * counter_ticks_per_micro_secounds;
    servo_compare_value[servo] = offset + base;
    set_compare_register ( servo );
}
void Servos::setDegrees ( float deg, uint8_t servo ) {
    setRad ( ( deg * M_PI ) / 180.0, servo );
}
int32_t Servos::getHighTime ( uint8_t servo ) {
    return servo_compare_value[servo] / counter_ticks_per_micro_secounds;
}
float Servos::getRad ( uint8_t servo ) {
    int32_t base = servo_neutral_signal_micro_secounds[servo] * counter_ticks_per_micro_secounds;
    float offset = servo_compare_value[servo] - base;
    return offset / servo_counter_ticks_per_rad[servo];
}
float Servos::getDegrees ( uint8_t servo ) {
    return getRad ( servo ) * ( 180.0 / M_PI );
}

void Servos::set_compare_register ( uint8_t servo ) {
    TC->CC[servo].reg = servo_compare_value[servo];
    if ( servo == 0 ) while ( TC->SYNCBUSY.bit.CC0 == 1 ); // wait for sync
    if ( servo == 1 ) while ( TC->SYNCBUSY.bit.CC1 == 1 ); // wait for sync
    if ( servo == 2 ) while ( TC->SYNCBUSY.bit.CC2 == 1 ); // wait for sync
}

void TCC1_Handler() {
    uint32_t clear_flags = 0;
    if ( Servos::TC->INTFLAG.bit.OVF == 1 ) { // A overflow caused the interrupt
        if ( Servos::pin_output[0] > 0 ) digitalWrite ( Servos::pin_output[0], HIGH );
        if ( Servos::pin_output[1] > 0 ) digitalWrite ( Servos::pin_output[1], HIGH );
        if ( Servos::pin_output[2] > 0 ) digitalWrite ( Servos::pin_output[2], HIGH );
        clear_flags |= TCC_INTENCLR_OVF;
    }
    if ( Servos::TC->INTFLAG.bit.MC0 == 1 ) {  // A compare to cc0 caused the interrupt
        digitalWrite ( Servos::pin_output[0], LOW );
        clear_flags |= TCC_INTENCLR_MC0;
    }
    if ( Servos::TC->INTFLAG.bit.MC1 == 1 ) {  // A compare to cc1 caused the interrupt
        digitalWrite ( Servos::pin_output[1], LOW );
        clear_flags |= TCC_INTENCLR_MC1;
    }
    if ( Servos::TC->INTFLAG.bit.MC2 == 1 ) {  // A compare to cc2 caused the interrupt
        digitalWrite ( Servos::pin_output[2], LOW );
        clear_flags |= TCC_INTENCLR_MC2;
    }
    Servos::TC->INTFLAG.reg = clear_flags;
}



