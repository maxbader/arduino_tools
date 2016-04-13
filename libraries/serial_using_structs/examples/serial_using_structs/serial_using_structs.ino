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
    Serial.begin ( 115200 );			/// init serial
    msg.try_sync();   				    /// blocks until a sync message arrives
    delay ( 1000 );
    pose.x = 140, pose.y = 10., pose.theta = 0.2;
    sprintf ( text.c,"Hello World!" );
    loop_count = 0;
}

void loop() {
    delay ( 1000 );
    pose.x += 1.;               		/// we are increasing the x for debugging reasons
    msg.clear();				        /// removes all objects in message
    msg.update_time ( millis() ); 		/// update time stamp
    msg.push_object ( pose );			/// pushes a object into the message
    msg.push_object ( text );			/// pushes a object into the message
    msg.send();					        /// sends the message

    if ( msg.receive() ) {      		/// check for messages
        tuw::ComHeader::Type type;
        while (  msg.pop_type(type) ) {	/// check for objects and there type
            switch ( type ) {
            case Pose::TYPE:        		/// case pose type
                msg.pop_object ( pose );    	/// update pose
                break;
            case tuw::ComHeader::TYPE_SYNC: 	/// case time sync message
                tuw::Time::setClock ( msg.stamp, millis() ); /// set clock
                break;
            }
        }
    }

    loop_count++;
}
