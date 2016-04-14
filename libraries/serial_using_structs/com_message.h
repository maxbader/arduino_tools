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
     * sends a singe object
     * @remark for simple messages with one object
     * @post send()
     * @return a ref to the message to call send() in one line
     **/
    template <class T> ComMessage& set_object ( const T& src ) {
		this->size = 0;
		int object_size = sizeof ( Type ) + sizeof ( T );
        if ( object_size <= MAX_BUFFER_SIZE ) {
            Type& type = * ( ( Type* ) buffer );
            type = T::TYPE;
            this->size += sizeof ( Type );
            memcpy ( buffer + this->size, ( void* ) &src, sizeof ( T ) );
            this->size += sizeof ( T );
        } 
        return *this;
    }
    
    /**
     * deserialize allways the frist object type after the header
     * @remark for simple messages with one object
     * @pre receive()
     * @return the object type 
     **/
    ComHeader::Type get_type ( Type &type );
    
    /**
     * deserialize allways the frist object after the header and object type
     * @remark for simple messages with one object
     * @pre get_type()
     * @return remaining bytes in the message to deserialize
     * if the return value < 0 you may got the wrong object type
     **/
    template <class T> int get_object ( T& des ) {
		int bytes_remaining = this->size - (sizeof ( Type ) + sizeof ( T ));
        if ( bytes_remaining >= 0 ) {
            memcpy ( ( void* ) &des, buffer + sizeof ( Type ), sizeof ( T ) );
        } 
        return bytes_remaining;
    }
    
    /**
     * sends the current objects stored in the buffer
     * @pre push_object() use the set function to place a objects into the buffer
     * @return number of total transmitted bytes including header
     **/
    int send ();
    
    /**
     * places a sync request into the message buffer
     * @post send()
     * @return true on success, false if message buffer was full
     **/
    bool push_sync_request ();
    
    /**
     * receives a message with header
     * @post pop_type() to check which objects are in the buffer
     * @return number of total bytes received including header
     **/
    int receive();

    /**
     * serializes a object into the message buffer
     * @post send()
     * @return remaining bytes in the message, 
     * if the return value < 0 the object was not placed into the message
     **/
    template <class T> int push_object ( const T& src ) {
        if ( this->size + sizeof ( Type ) + sizeof ( T ) <= MAX_BUFFER_SIZE ) {
            Type& type = * ( ( Type* ) ( buffer + this->size ) );
            type = T::TYPE;
            this->size += sizeof ( Type );
            memcpy ( buffer + this->size, ( void* ) &src, sizeof ( T ) );
            this->size += sizeof ( T );
        } 
        return MAX_BUFFER_SIZE - this->size;
    }

    /**
     * tries a sync if needed
     * blocking
     **/
    void try_sync();

    
    /**
     * deserialize and pop's the object type from the buffer
     * @post receive()
     * @return remaining bytes in the message to deserialize
     * if the return value < 0 there are no more bytes left to deserialize
     **/
    int pop_type ( Type &type );
    
    /**
     * deserialize and pop's a object from the buffer
     * @post pop_type()
     * @return remaining bytes in the message to deserialize
     * if the return value < 0 there are no more bytes left to deserialize
     **/
    template <class T> int pop_object ( T& des ) {
		int bytes_remaining = this->size - (stack_idx + sizeof ( T  ));
        if ( bytes_remaining >= 0 ) {
            memcpy ( ( void* ) &des, buffer + stack_idx, sizeof ( T ) );
        } 
        stack_idx += sizeof ( T );
        return bytes_remaining;
    }

};
};
#endif
