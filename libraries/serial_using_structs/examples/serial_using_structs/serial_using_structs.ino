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

Pose pose;             /// object to send
tuw::ComMessage msg;   /// object to hande the serial communication

void setup() {
    init();
    Serial.begin ( 115200 );
    msg.try_sync();   /// sync time
    delay ( 1000 );
    pose.x = 140, pose.y = 0., pose.theta = 0.02;
}

void loop() {
    delay ( 1000 );              
    pose.x += 1.;               /// we are increasing the x for debugging reasons
    msg.update_time(millis());  /// update time stamp
    msg.set(pose).send();       /// sends pose message
    if(msg.receive()) {         /// check for messages
      switch(msg.type){         /// check message type
        case tuw::ComHeader::TYPE_TIME: /// case time sync message
          tuw::Time::setClock(msg.stamp, millis()); /// set clock 
          break;
        case Pose::TYPE:        /// case pose type
          msg.get(pose);        /// update pose    
          break;
      }
    }
}
