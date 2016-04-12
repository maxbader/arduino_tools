/*
 * serial_using_structs
 * demo which sends and receives a struct using the serial interface
 * the struct Pose will send to a PC (python program and returned)
 * @see pythpon program
 * @author Markus Bader
 */
 
#include <com_message.h>
struct __attribute__ ( ( packed ) ) Pose {
    float x;
    float y;
    float theta;
    static const uint16_t TYPE = 100;
};

struct __attribute__ ( ( packed ) ) PWMCmd {
    float motor;
    float servo;
    static const uint16_t TYPE = 101;
};

Pose pose;             /// object to send
tuw::ComMessage msg;   /// object to hande the serial communication

void setup() {
    init();
    Serial.begin ( 115200 );
    delay ( 1000 );
    pose.x = 140, pose.y = 0., pose.theta = 0.02;
}

void sync_time(){
  if(tuw::Time::isSet()) return;
  int request_count = 0;
  for(int i = 0; tuw::Time::isSet() == false; i++){
      if(i%10 == 0) msg.send_sync();
      if(msg.receive() && (msg.type == tuw::ComHeader::TYPE_TIME)) { 
        tuw::Time::setClock(msg.stamp, millis());  
      }   
      delay ( 10 );
   }
}

void loop() {
    pose.x += 1.;   /// we are increasing the x for debugging reasons
    delay ( 1000 );
    while(msg.try_sync())  delay ( 10 );
    //sync_time();
    msg.update_time(millis());
    msg.set(pose).send();
    if(msg.receive()) {      
      if(msg.type == tuw::ComHeader::TYPE_TIME){
        tuw::Time::setClock(msg.stamp, millis());
      }
      if(msg.type == Pose::TYPE){
        msg.get(pose);
      }      
    }
}
