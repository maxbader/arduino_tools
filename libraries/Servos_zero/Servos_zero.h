/**
 * @author Markus Bader <markus.bader@tuwien.ac.at>
 * @brief Simple servo driver for an arduino zero to controll
 * three servos with timer 1
 * @file Servos_zero.h
 * @license Simplified BSD License
 */


#ifndef Servos_h
#define Servos_h

#include <Arduino.h>

extern "C" void TCC1_Handler ( void ) __attribute__ ( ( signal ) );

/**
 * Class to simplify servo commands
 * works on Arduino Zero
 * @author Markus Bader <markus.bader@tuwien.ac.at>
 **/
class Servos {
    static Tcc* TC; /// Tcc timer to use
    static unsigned int timer_irq_ovf_count;
    static unsigned int timer_irq_mc0_count;
    static unsigned int timer_irq_mc1_count;
    static unsigned int timer_irq_mc2_count;

    static const int32_t counter_ticks_per_micro_secounds;      /// ticks per micro (0.000001) second
    static int pin_output[3];       /// output pin for servo 0, values < 0 means inactive
    static int32_t servo_compare_value[3];       /// servo neutral pose
    static int32_t servo_neutral_signal_micro_secounds[3];   /// servo neutral pose
    static float servo_micro_secounds_per_rad[3];


    /**
     * interrupt handler
     * @param sets all active pins ot high
     **/
    static void set_compare_register ( uint8_t servo );

public:
    Servos();
    /**
     * initializes timer
     * enables the interrupts and defines the output pins
     * @param pin_0  output servo 0, values < 0 means inactive
     * @param pin_1  output servo 1, values < 0 means inactive
     * @param pin_2  output servo 2, values < 0 means inactive
     **/
    static void setupServosOnTimer1 ( int pin_0, int pin_1 = -1, int pin_2 = -1 );

    /**
     * @brief sets the scale for setDeg and setRad
     * @param micro_secounds time per rad in micro_secounds (us)
     * @param servo id of the servo to set
     */
    void setMicroSecoundsPerRad ( float micro_secounds, uint8_t servo );
    /**
     * @brief returns the scale of setDeg and setRad
     * @param servo id of the servo to set
     * @return micro_secounds time per rad in micro_secounds (us)
     */
    float getMicroSecoundsPerRad ( uint8_t servo );
    /**
     * @brief enables the timer
     * @param value ture => enable, false => disable
     */
    static void enable ( bool value );
    /**
     * sets the neutral pose
     * @param micro_secounds high time in micro_secounds (us)
     * @param servo id of the servo to set
     **/
    static void setNeutral ( int32_t micro_secounds, uint8_t servo );
    /**
     * sets the neutral pose
     * @param servo id of the servo to set
     **/
    static int32_t getNeutral ( uint8_t servo );
    /**
     * sets an angle on the selected servo in degrees
     * @param deg angle 0 means neutral pose
     * @param servo id of the servo to set
     **/
    static void setDegrees ( float deg, uint8_t servo );
    /**
     * sets an angle on the selected servo in rad
     * @param deg angle 0 means neutral pose
     * @param servo id of the servo to set
     **/
    static void setRad ( float rad, uint8_t servo );
    /**
     * set the high time for a servo in us
     * @param micro_secounds high time in micro_secounds (us)
     * @param servo id of the servo to set
     **/
    static void setHighTime ( int32_t micro_secounds, uint8_t servo );
    /**
     * returns the high time in us
     * @param servo id of the servo to set
     * @return high time in us
     **/
    static int32_t getHighTime ( uint8_t servo );
    /**
     * returns an angle value to the selected servo in degrees
     * @param servo id of the servo to set
     * @return angle in degrees
     **/
    static float getDegrees ( uint8_t servo );
    /**
     * returns an angle value to the selected servo in rad
     * @param servo id of the servo to set
     * @return angle in rad
     **/
    static float getRad ( uint8_t servo );

    friend void TCC1_Handler ( void ); /// friend to allow access
};


#endif
