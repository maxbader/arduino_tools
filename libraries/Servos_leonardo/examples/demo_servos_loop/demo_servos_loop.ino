/**
 * @author Markus Bader <markus.bader@tuwien.ac.at>
 * @brief Simple servo driver for an arduino leonardo to controll 
 * three servos with timer 1
 * @file demo_servos.ino
 * @license Simplified BSD License
 */

#include <Servos_leonardo.h>

Servos servos;
int pinServo = 9;
float angle = 0;


void setup() {
  pinMode(pinServo, OUTPUT);
  Serial.begin(9600);
  // put your setup code here, to run once:
  
  servos.setupServosOnTimer1(pinServo);
  servos.setDegrees(angle, 0);
}

void loop() {
  delay(100);
  while(angle < 20.){
    angle = angle + 1.;
    servos.setDegrees(angle, 0);
    delay(200);
  }
  while(angle > -20.){
    angle = angle - 1.;
    servos.setDegrees(angle, 0);
    delay(200);
  }
}
