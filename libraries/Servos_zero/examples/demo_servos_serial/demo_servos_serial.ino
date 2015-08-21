/**
 * @author Markus Bader <markus.bader@tuwien.ac.at>
 * @brief Simple servo driver for an arduino zero to controll
 * three servos with timer 1
 * @file demo_servos_serial.ino
 * @license Simplified BSD License
 */

#include <Servos_zero.h>

Servos servos;
uint8_t pinServo0 = 9;
uint8_t pinServo1 = 10;

void setup() {
  Serial.begin(9600);
  servos.setupServosOnTimer1(pinServo0, pinServo1);
}
void loop() {

  if (Serial.available() ) {
    float angle0 = Serial.parseInt();
    servos.setDegrees(angle0, 0);
    servos.setDegrees(angle0, 1);
    Serial.print(servos.getDegrees(0));
    Serial.println(" DEG");
  }
  delay(100);
}
