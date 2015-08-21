/**
 * @author Markus Bader <markus.bader@tuwien.ac.at>
 * @brief Simple servo driver for an arduino leonardo to controll 
 * three servos with timer 1
 * @file servos.h
 * @license Simplified BSD License
 */


#ifndef Servos_h
#define Servos_h

#include <Arduino.h>

extern "C" void TIMER1_COMPA_vect(void) __attribute__ ((signal));
extern "C" void TIMER1_COMPB_vect(void) __attribute__ ((signal));
extern "C" void TIMER1_COMPC_vect(void) __attribute__ ((signal));

/**
 * Class to simplify servo commands 
 * works on Arduino Leonardo
 * @author Markus Bader <markus.bader@tuwien.ac.at>
 **/
class Servos {
    /// 20ms on f_clk = 16Mhz and prescaler = 8 : ( 0.000001 / (1/16000000*8) )
    static const uint16_t counter_ticks_per_us = 2; 
    /// 1.5ms on f_clk = 16Mhz and prescaler = 8 : ( 0.0015 / (1/16000000*8) ) 
    static const uint16_t servo_null = 2000; 
    /// 0.005555 = 1ms/180 on f_clk = 16Mhz and prescaler = 8 : ( (0.001/180) / (1/16000000*8) ) 
    static const float servo_one_deg = 11.111; 
    /// 0.005555 = 1ms/180 on f_clk = 16Mhz and prescaler = 8 : ( (0.001/PI) / (1/16000000*8) ) 
    static const float servo_one_rad = 636.62; 
    
    static int output_pin[3];  /// output pin for servo 0, values < 0 means inactive
    static uint16_t high_time[3];   /// servo 0 position in counter ticks
    static uint16_t low_time[3];   /// servo 0 position in counter ticks
    
    static uint8_t prescaler_bits;   /// used to set the timer prescaler
    
	/**
	 * interrupt handler 
	 * @param ocr OutputCompareRegister
	 * @param servo id of the servo to set
	 **/
	static void updateComperatorAndOutput(volatile uint16_t *ocr, uint8_t servo);
	
public:
    Servos();
	/**
	 * initializes timer
	 * enables the interrupts and defines the output pins
	 * @param pin_0  output servo 0, values < 0 means inactive
	 * @param pin_1  output servo 1, values < 0 means inactive
     * @param pin_2  output servo 2, values < 0 means inactive
	 **/
	void setupServosOnTimer1(int pin_0, int pin_1 = -1, int pin_2 = -1);
	
	/** 
	 * @brief enables the timer
	 * @param value ture => enable, false => disable
	 */
	void enable(bool value);
	
	/**
	 * sets an angle on the selected servo in degrees
	 * @param deg angle in deg between 0 and 180
	 * @param servo id of the servo to set
	 **/
	void setDegrees(float deg, uint8_t servo);
	/**
	 * sets an angle on the selected servo in rad
	 * @param deg angle in deg between -pi/2 and +pi/2
	 * @param servo id of the servo to set
	 **/
	void setRad(float rad, uint8_t servo);
	/**
	 * set the high time for a servo in us
	 * @param v high time in us
	 * @param servo id of the servo to set
	 **/
	void setHighTime(uint16_t v, uint8_t servo);
	/**
	 * sets the low time per servo in us
	 * @param t low time 
	 * @param servo id of the servo to set
	 **/
	void setLowTime(uint16_t t, uint8_t servo);
	/**
	 * returns the low time in us
	 * @param servo id of the servo to set
	 * @return low time in us
	 **/
	uint16_t getLowTime(uint8_t servo);
	/**
	 * returns the high time in us
	 * @param servo id of the servo to set
	 * @return high time in us
	 **/
	uint16_t getHighTime(uint8_t servo);
	/**
	 * returns an angle value to the selected servo in degrees
	 * @param servo id of the servo to set
	 * @return angle in degrees
	 **/
	float getDegrees(uint8_t servo);
	/**
	 * returns an angle value to the selected servo in rad
	 * @param servo id of the servo to set
	 * @return angle in rad
	 **/
	float getRad(uint8_t servo);
	
    friend void TIMER1_COMPA_vect(void); /// friend to allow access
    friend void TIMER1_COMPB_vect(void); /// friend to allow access
    friend void TIMER1_COMPC_vect(void); /// friend to allow access
};


#endif
