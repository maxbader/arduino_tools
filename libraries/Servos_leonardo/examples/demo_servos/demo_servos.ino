/**
 * @author Markus Bader <markus.bader@tuwien.ac.at>
 * @brief Simple servo driver for an arduino leonardo to controll 
 * three servos with timer 1
 * @file demo_servos.ino
 * @license Simplified BSD License
 */

#include <Servos_leonardo.h>

Servos servos;

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  servos.setupServosOnTimer1(9, 10, 11);
}

void loop() {
  if (Serial.available() ) {
    float angle0 = Serial.parseInt();    
    float angle1 = Serial.parseInt();
    float angle2 = Serial.parseInt();
      servos.setDegrees(angle0, 0);
      servos.setDegrees(angle1, 1);
      servos.setDegrees(angle2, 2);
    Serial.print(servos.getDegrees(0));
    Serial.print(" DEG, ");
    Serial.print(servos.getDegrees(1));
    Serial.print(" DEG, ");
    Serial.print(servos.getDegrees(2));
    Serial.println(" DEG");
  }
  delay(100);
}
