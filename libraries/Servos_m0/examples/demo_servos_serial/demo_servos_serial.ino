/**
 * @author Markus Bader <markus.bader@tuwien.ac.at>
 * @brief Simple servo driver for an arduino m0 to controll
 * three servos with timer 1
 * @info you have to use the arduino.org framework
 * @file demo_servos_serial.ino
 * @license Simplified BSD License
 */

#include <Servos_m0.h>

Servos servos;
uint8_t pinServo0 = 9;
uint8_t pinServo1 = 10;

void setup() {
  Serial.begin(9600);
  servos.setupServosOnTimer1(pinServo0, pinServo1);
  print();
  Serial.print("Type a angle value: ");
}

void print(){
    Serial.print(servos.getHighTime(0));
    Serial.print(" us, ");
    Serial.print(servos.getRad(0));
    Serial.print(" rad, ");
    Serial.print(servos.getDegrees(0));
    Serial.print(" deg");
    Serial.print(" -- micro secounds per rad ");
    Serial.print(servos.getMicroSecoundsPerRad(0));
    Serial.println(" us, ");
}

void loop() {

  if (Serial.available() ) {
    float angle0 = Serial.parseInt();
    servos.setDegrees(angle0, 0);
    servos.setDegrees(angle0, 1);
    print();
  }
  delay(100);
}
