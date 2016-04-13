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
    static const int MAX_BUFFER_SIZE = 0xFF;
private:
    char buffer[MAX_BUFFER_SIZE];	/// buffer to receive messages increas if needed
    static uint16_t count_msg;  	/// static variale to increase message count
    static uint16_t sync_count; 	/// number of sync attempts
    int stack_idx;			/// index for message decoding
public:
    ComMessage();			/// constructor
    /**
     * clears the buffer
     **/
    void clear();

    /**
     * sends the current objects stored in the buffer
     * @pre push_object() use the set function to place a objects into the buffer
     * @return number of total transmitted bytes including header
     **/
    uint16_t send ();
    /**
     * places a sync request into the message buffer
     * @return true on success, false if message buffer was full
     **/
    bool push_sync_request ();
    /**
     * receives a message with header
     * @post pop_type() to check which objects are in the buffer
     * @return number of total bytes received including header
     **/
    uint16_t receive();

    /**
     * places a object  into the message buffer
     * @post send()
     * @return true on success, false if message buffer was full
     **/
    template <class T> bool push_object ( const T& src ) {
        if ( this->size + sizeof ( Type ) + sizeof ( T ) <= MAX_BUFFER_SIZE ) {
            Type& type = * ( ( Type* ) ( buffer + this->size ) );
            type = T::TYPE;
            this->size += sizeof ( Type );
            memcpy ( buffer + this->size, ( void* ) &src, sizeof ( T ) );
            this->size += sizeof ( T );
            return true;
        } else {
            return false;
        }
    }

    /**
     * tries a sync if needed
     * blocking
     **/
    void try_sync();

    /**
     * return the next object type
     * @post receive()
     * @return true on success, false if message holds no more objects
     **/
    bool pop_type ( Type &type );
    
    /**
     * return the next object
     * @post pop_type()
     * @return true on success, false if message holds no more objects
     **/
    template <class T> bool pop_object ( T& des ) {
        if ( stack_idx + sizeof ( T ) <= this->size ) {
            memcpy ( ( void* ) &des, buffer + stack_idx, sizeof ( T ) );
            stack_idx += sizeof ( T );
            return true;
        } else {
            stack_idx += sizeof ( T );
            return false;
        }
    }

};
};
#endif
