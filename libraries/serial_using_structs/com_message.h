/**
 * @author Markus Bader <markus.bader@tuwien.ac.at>
 * @license Simplified BSD License
 */


#ifndef COM_MESSAGE_H
#define COM_MESSAGE_H

#include <Arduino.h>
#include <com_header.h>

namespace tuw {


/**
 * Class to simplify the serial communication
 * @author Markus Bader <markus.bader@tuwien.ac.at>
 **/
class __attribute__ ( ( packed ) ) ComMessage : public ComHeader {
private:
    char buffer[0xFF];          /// buffer to receive messages increas if needed
    static uint16_t count_msg;  /// static variale to increase message count
	static uint16_t sync_count; /// number of sync attempts 
public:
    ComMessage();				/// constructor
    /**
     * clears the buffer 
     **/
    void reset();

    /**
     * sends the current object stored in the buffer
     * @pre set() use the set function to place a object into the buffer
     * @return number of total send bytes including header
     **/
    uint16_t send (); 
    /**
     * sends a sync message for time request
     * @return number of total send bytes including header
     **/
    uint16_t send_sync (); 
    /**
     * receives a object with header 
     * the type varible can be used to identify the object received
     * @see Header 
     * @post get() use the get function copy the object out of the buffer
     * @return number of total bytes received including header
     **/         
    uint16_t receive();

    /**
     * sets the buffer
     * @param src source to serialize object into the buffer
     * @post send()
     * @return a reference to the current class
     **/         
    template <class T> ComMessage& set ( const T& src ) {
      memcpy(buffer, (void*) &src, sizeof(T));
        this->size = sizeof ( T );
        this->type = T::TYPE;
	return *this;
    }
    /**
     * get clones the buffer into a object
     * @param des destination to serialize buffer to
     * @pre receive()
     **/        
    template <class T> void get ( T& des ) {
      memcpy((void*) &des, buffer, sizeof(T));
    }
    
    /**
     * tries a sync if needed
     **/     
    void try_sync();
};
};
#endif
