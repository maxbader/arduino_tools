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
    static const int MAX_BUFFER_SIZE = 32;
    char txt[MAX_BUFFER_SIZE];
    static const uint16_t TYPE = 200;
    void clear(){
      memset ( txt, '\0', MAX_BUFFER_SIZE );
    }
    int write(const char *msg) {
       int bytes_remaining = MAX_BUFFER_SIZE - strlen(msg);
       if(bytes_remaining > 0){
         strcpy( txt, msg);
       }
       return bytes_remaining;
    }
    int size(){
      return strlen(txt);
    }
};

Pose pose;             /// object to send
Text text;             /// object to send
tuw::ComMessage msg;   /// object to hande the serial communication
int loop_count;        /// defines which message should be send

void setup() {
    init();
    Serial.begin ( 115200 );			/// init serial
    msg.try_sync();   			        /// blocks until a sync message arrives and sends sync requests
    delay ( 1000 );
    pose.x = 140, pose.y = 10., pose.theta = 0.2;
    text.write("Hello World!" );
    loop_count = 0;
}

void loop() {
    delay ( 1000 );
    pose.x += 1.;               		/// we are increasing the x for debugging reasons
    msg.update_time ( millis() ); 		/// update time stamp
    msg.set_object ( text ).send;       /// send a object
    msg.set_object ( pose ).send;       /// send a object
    
    if ( msg.receive() ) {      		     /// check for messages
        tuw::ComHeader::Type type;
        if (  msg.get_type(type) >= 0) {	/// check for objects and there type
            switch ( type ) {
            case Pose::TYPE:        		/// case pose type
                msg.get_object ( pose );    /// update pose
                sprintf(text.txt, "pose <%i, %i, %i> received", (int) pose.x, (int) pose.y, (int) pose.theta);
                break;
            case tuw::ComHeader::TYPE_EMPTY: 	/// case empty object
            default:/// case unkown type
                text.write("Unknown type received");
            }
        } else {
            text.write("Nothing received");
        }
    }
    loop_count++;
}
