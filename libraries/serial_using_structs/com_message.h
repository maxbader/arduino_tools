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
	typedef uint16_t Type;
private:
    char buffer[0xFF];          /// buffer to receive messages increas if needed
    static uint16_t count_msg;  /// static variale to increase message count
	static uint16_t sync_count; /// number of sync attempts 
	static const uint16_t TYPE_TIME_REQUEST = 10;
	static const uint16_t TYPE_TIME = 10;
public:
    ComMessage();				/// constructor
    /**
     * clears the buffer 
     **/
    void clear();

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
    uint16_t push_sync_request (); 
    /**
     * receives a object with header 
     * the type varible can be used to identify the object received
     * @see Header 
     * @post get() use the get function copy the object out of the buffer
     * @return number of total bytes received including header
     **/         
    uint16_t receive();

    /**
     * adds a object to the
     * @param src source to serialize object into the buffer
     * @post send()
     * @return a reference to the current class
     **/         
    template <class T> ComMessage& push ( const T& src ) {
		Type& type = *((Type*) (buffer + this->size));
		type = T::TYPE;
		this->size += sizeof(Type);
		memcpy(buffer + this->size, (void*) &src, sizeof(T));
		this->size += sizeof(T);
		return *this;
    }
    /**
     * get clones the buffer into a object
     * @param des destination to serialize buffer to
     * @pre receive()
     **/        
    template <class T> uint16_t get ( T& des ) {
		int idx = this->size - sizeof(T) - sizeof(Type);
		memcpy((void*) &des, buffer + idx, sizeof(T));
		return getType();
    }
    
    /**
     * tries a sync if needed
     **/     
    void try_sync();
    /**
     * tries a sync if needed
     **/     
    uint16_t getType(){
		return *((Type*) buffer[this->size-sizeof(Type)]);
	}
    /**
     * tries a sync if needed
     **/     
    template <class T> bool pop ( T& des ) {
		if(this->size - sizeof(Type) > sizeof(ComHeader)){
			this->size -= sizeof(Type);
			this->size -= sizeof(T);
			memcpy((void*) &des, buffer + this->size, sizeof(T));
		}
		return des;
    }
	
};
};
#endif
