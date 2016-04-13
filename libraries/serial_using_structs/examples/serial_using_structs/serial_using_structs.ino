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

struct __attribute__ ( ( packed ) ) Text {
    char c[32];
    static const uint16_t TYPE = 200;
};

Pose pose;             /// object to send
Text text;             /// object to send
tuw::ComMessage msg;   /// object to hande the serial communication
int loop_count;        /// defines which message should be send

void setup() {
    init();
    Serial.begin ( 115200 );
    //msg.try_sync();   /// sync time
    delay ( 1000 );
    pose.x = 140, pose.y = 10., pose.theta = 0.2;
    sprintf(text.c,"Hello World!");
    loop_count = 0;
}

void loop() {
    delay ( 1000 );              
    pose.x += 1.;               /// we are increasing the x for debugging reasons
    msg.update_time(millis());  /// update time stamp
    msg.clear();
    //msg.push(pose);
    //msg.push(text);
    msg.push_sync_request();
    //if(loop_count % 2 == 0) msg.push(pose);       /// sends pose message
    //if(loop_count % 2 == 1) msg.push(text);       /// sends pose message
    msg.send();
    /*
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
    */
    loop_count++;
}
