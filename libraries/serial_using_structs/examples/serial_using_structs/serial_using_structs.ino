/*
  sketch Demo
  @author Markus Bader
 */
#include <com_message.h>
struct __attribute__ ( ( packed ) ) Pose {
    float x;
    float y;
    float theta;
    static const uint16_t TYPE = 1;
};

struct __attribute__ ( ( packed ) ) PWMCmd {
    float motor;
    float servo;
    static const uint16_t TYPE = 2;
};

Pose pose;
tuw::ComMessage msg;
const int ledPin =  13;      // the number of the LED pin
const int buttonPin = 12;     // the number of the pushbutton pin
int buttonState = 0;         // variable for reading the pushbutton status

void setup() {
    init();
    Serial.begin ( 115200 );
    delay ( 1000 );
    pose.x = 140, pose.y = 0., pose.theta = 0.02;
    //header.seq = 0;
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, LOW);
    pinMode(buttonPin, INPUT);
}

// the loop function runs over and over again forever
void loop() {
    pose.x += 1.;
    delay ( 100 );
    buttonState = digitalRead(buttonPin);
    if (buttonState == HIGH) {// turn LED on:
      digitalWrite(ledPin, HIGH);
    } else {
      // turn LED off:
      digitalWrite(ledPin, LOW);
      msg.set(pose).send();
      if(msg.receive()) msg.get(pose);
      
    }
}

