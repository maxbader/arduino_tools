/**
 * @author Markus Bader <markus.bader@tuwien.ac.at>
 * @license Simplified BSD License
 */


#ifndef COM_MESSAGE_H
#define COM_MESSAGE_H

#include <Arduino.h>
#include <tuw/com_header.h>

namespace tuw {

class __attribute__ ( ( packed ) ) ComMessage : private ComHeader {
private:
    char buffer[0xFF];
    static uint16_t count_msg;
public:
    ComMessage();
    void reset();

    uint16_t send ();
    uint16_t receive();

    template <class T> ComMessage& set ( const T& src ) {
      memcpy(buffer, (void*) &src, sizeof(T));
        this->size = sizeof ( T );
        this->type = T::TYPE;
	return *this;
    }
    template <class T> void get ( T& des ) {
      memcpy((void*) &des, buffer, sizeof(T));
    }

};
};
#endif
