/**
 * @author Markus Bader
 * @brief this program shows how to use the TCC timer with interrupts on an Arduino Zero board
 * @email markus.bader@tuwien.ac.at
 */
 
int pin_ovf_led = 13; // debug pin for overflow led 
int pin_mc0_led = 5;  // debug pin for compare led 
unsigned int loop_count = 0;
unsigned int irq_ovf_count = 0;

void setup() {

  pinMode(pin_ovf_led, OUTPUT);   // for debug leds
  digitalWrite(pin_ovf_led, LOW); // for debug leds
  pinMode(pin_mc0_led, OUTPUT);   // for debug leds
  digitalWrite(pin_mc0_led, LOW); // for debug leds

  
  // Enable clock for TC 
  REG_GCLK_CLKCTRL = (uint16_t) (GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN_GCLK0 | GCLK_CLKCTRL_ID_TCC0_TCC1) ;
  while ( GCLK->STATUS.bit.SYNCBUSY == 1 ); // wait for sync 


  // The type cast must fit with the selected timer 
  Tcc* TC = (Tcc*) TCC0; // get timer struct
  
  TC->CTRLA.reg &= ~TCC_CTRLA_ENABLE;   // Disable TC
  while (TC->SYNCBUSY.bit.ENABLE == 1); // wait for sync 


  TC->CTRLA.reg |= TCC_CTRLA_PRESCALER_DIV256;   // Set perscaler


  TC->WAVE.reg |= TCC_WAVE_WAVEGEN_NFRQ;   // Set wave form configuration 
  while (TC->SYNCBUSY.bit.WAVE == 1); // wait for sync 

  TC->PER.reg = 0xFFFF;              // Set counter Top using the PER register  
  while (TC->SYNCBUSY.bit.PER == 1); // wait for sync 

  TC->CC[0].reg = 0xFFF;
  while (TC->SYNCBUSY.bit.CC0 == 1); // wait for sync 
  
  // Interrupts 
  TC->INTENSET.reg = 0;                 // disable all interrupts
  TC->INTENSET.bit.OVF = 1;          // enable overfollow
  TC->INTENSET.bit.MC0 = 1;          // enable compare match to CC0

  // Enable InterruptVector
  NVIC_EnableIRQ(TCC0_IRQn);

  // Enable TC
  TC->CTRLA.reg |= TCC_CTRLA_ENABLE ;
  while (TC->SYNCBUSY.bit.ENABLE == 1); // wait for sync 

}

void loop() {
  // dummy
  delay(250);
}

void TCC0_Handler()
{
  Tcc* TC = (Tcc*) TCC0;       // get timer struct
  if (TC->INTFLAG.bit.OVF == 1) {  // A overflow caused the interrupt
    digitalWrite(pin_ovf_led, irq_ovf_count % 2); // for debug leds
    digitalWrite(pin_mc0_led, HIGH); // for debug leds
    TC->INTFLAG.bit.OVF = 1;    // writing a one clears the flag ovf flag
    irq_ovf_count++;                 // for debug leds
  }
  
  if (TC->INTFLAG.bit.MC0 == 1) {  // A compare to cc0 caused the interrupt
    digitalWrite(pin_mc0_led, LOW);  // for debug leds
    TC->INTFLAG.bit.MC0 = 1;    // writing a one clears the flag ovf flag
  }
}


