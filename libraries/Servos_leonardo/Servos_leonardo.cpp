/**
 * @author Markus Bader <markus.bader@tuwien.ac.at>
 * @brief Simple servo driver for an arduino leonardo to controll 
 * three servos with timer 1
 * @file servos.cpp
 * @license Simplified BSD License
 */


#include <Servos_leonardo.h>

int Servos::output_pin[3];
uint16_t Servos::high_time[3];
uint16_t Servos::low_time[3];
uint8_t Servos::prescaler_bits;

Servos::Servos(){
}
void Servos::setupServosOnTimer1(int pin_0, int pin_1, int pin_2){
	  //set pins as outputs
	  output_pin[0] = pin_0;
	  output_pin[1] = pin_1;
	  output_pin[2] = pin_2;
	  
	  //set the timer prescaler
	  prescaler_bits = (0 << CS12) | (1 << CS11) | (0 << CS10);
	
	  TCCR1A = 0;// set entire TCCR1A register to 0
	  TCCR1B = 0;// same for TCCR1B
	  // turn on normal mode
	  TCCR1A |= (0 << WGM11) | (0 << WGM10); 
	  TCCR1B |= (0 << WGM13) | (0 << WGM12); 
	  // stop 
	  TCCR1B |= (0 << CS12) | (0 << CS11) | (0 << CS10);
	  // enable timer interrupts
	  TIMSK1 = 0;
	  if(output_pin[0] > 0) {
		  pinMode(output_pin[0], OUTPUT);
		  setLowTime(20000, 0);
		  setDegrees(90, 0);
		  TIMSK1 |= (1 << OCIE1A);
	  }
	  if(output_pin[1] > 0) {
		  pinMode(output_pin[1], OUTPUT);
		  setLowTime(20000, 1);
		  setDegrees(90, 1);
		  TIMSK1 |= (1 << OCIE1B);
	  }
    if ( output_pin[2] > 0 ) {
        pinMode ( output_pin[2], OUTPUT );
        setLowTime ( 20000, 2 );
        setDegrees ( 90, 2 );
        TIMSK1 |= ( 1 << OCIE1C );
    }
	  // initialize counter value to 0
	  TCNT1  = 0;
	  // Set prescaler to 8 and start
	  TCCR1B |= prescaler_bits;
};

void Servos::updateComperatorAndOutput(volatile uint16_t *ocr, uint8_t servo){
  if (digitalRead(output_pin[servo]) == LOW) {
    digitalWrite(output_pin[servo], HIGH);
    if((*ocr) > (0xFFFF - high_time[servo]) ) (*ocr) = high_time[servo] - (0xFFFF - (*ocr));
    else (*ocr) += high_time[servo];
  }
  else {
    digitalWrite(output_pin[servo], LOW);
    if((*ocr) > (0xFFFF - low_time[servo]) ) (*ocr) = low_time[servo] - (0xFFFF - (*ocr));
    else  (*ocr) += low_time[servo];
  }
}

void Servos::enable(bool value){
	uint8_t mask = 0xFF ^ ((1 << CS12) | (1 << CS11) | (1 << CS10));
	uint8_t ttcr1b = TCCR1B & mask;
	if(value) {
	  TCCR1B = ttcr1b & prescaler_bits;
	} else {
	  TCCR1B = ttcr1b;
	}
}

void Servos::setHighTime(uint16_t v, uint8_t servo){
      high_time[servo] = v*counter_ticks_per_us;	
}
void Servos::setDegrees(float deg, uint8_t servo){
      high_time[servo] = servo_null + deg * servo_one_deg;	
}
void Servos::setRad(float rad, uint8_t servo){
      high_time[servo] = servo_null + rad * servo_one_rad;	
}
void Servos::setLowTime(uint16_t t, uint8_t servo){
      low_time[servo] = counter_ticks_per_us * t;
}
uint16_t Servos::getLowTime(uint8_t servo){
	return low_time[servo] / counter_ticks_per_us;
}
uint16_t Servos::getHighTime(uint8_t servo){
	return high_time[servo] / counter_ticks_per_us;
}
float Servos::getDegrees(uint8_t servo){
	return (high_time[servo] - servo_null) / servo_one_deg;
}
float Servos::getRad(uint8_t servo){
	return (high_time[servo] - servo_null) / servo_one_rad;
}
void TIMER1_COMPA_vect(void) { 
	Servos::updateComperatorAndOutput(&OCR1A, 0);
}
void TIMER1_COMPB_vect(void) { 
	Servos::updateComperatorAndOutput(&OCR1B, 1);
}
void TIMER1_COMPC_vect(void)  { 
	Servos::updateComperatorAndOutput(&OCR1C, 2);
}

