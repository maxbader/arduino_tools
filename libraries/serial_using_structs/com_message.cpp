/**
 * @author Markus Bader <markus.bader@tuwien.ac.at>
 * @license Simplified BSD License
 */

#include "com_message.h"

using namespace tuw;
uint16_t ComMessage::count_msg = 0;
uint16_t ComMessage::sync_count = 0;

ComMessage::ComMessage() {
    clear();
}

void ComMessage::clear() {
    size = 0;
    stamp.sec = 0;
    stamp.nsec = 0;
    memset ( buffer, '\0', MAX_BUFFER_SIZE );
}

uint16_t ComMessage::receive() {
    uint16_t rx_count = 0;
    if ( Serial.available() ) {
        uint16_t rx_expected = 0;
        char *c = ( char* ) this;
        for ( rx_expected = sizeof ( ComHeader ); rx_count < rx_expected; rx_count++ ) {
            if ( Serial.available() ) {
                *c++ = Serial.read();
            } else {
                continue;
            }
        }
        if ( rx_count == rx_expected ) {
            for ( rx_expected = sizeof ( ComHeader ) + this->size; rx_count < rx_expected; rx_count++ ) {
                if ( Serial.available() ) {
                    *c++ = Serial.read();
                } else {
                    continue;
                }
            }
        }
    }
    stack_idx = 0;
    return rx_count;
}


uint16_t ComMessage::send () {
    uint16_t total = 0;
    if ( Serial ) {
        char *c = ( char * ) this;
        uint16_t total = sizeof ( ComHeader ) + this->size;
        this->seq = count_msg++;
        for ( int i = 0; i < total; i++ ) {
            Serial.print ( *c++ );
        }
    }
    return total;
}

bool ComMessage::push_sync_request () {
    if ( this->size + sizeof ( Type ) <= MAX_BUFFER_SIZE ) {
        Type& type = * ( ( Type* ) ( buffer + this->size ) );
        type = TYPE_SYNC_REQUEST;
        this->size += sizeof ( Type );
        return true;
    } else {
        return false;
    }
}

void ComMessage::try_sync() {
    if ( tuw::Time::isSet() ) return;
    for ( int i = 0; tuw::Time::isSet() == false; i++ ) {
        if ( i%10 == 0 ) {
            clear();
            push_sync_request();
            send();
        };
        if ( receive() ) {      /// check for messages
            tuw::ComHeader::Type type;
            while ( pop_type ( type ) ) {
                if ( type == tuw::ComHeader::TYPE_SYNC ) { /// case time sync message
                    tuw::Time::setClock ( stamp, millis() ); /// set clock
                    break;
                }
            }
        }
        delay ( 10 );
    }
}

bool ComMessage::pop_type ( Type &type ) {
    if ( stack_idx + sizeof ( Type ) <= this->size ) {
        type = * ( ( Type* ) ( buffer + stack_idx ) );
        stack_idx += sizeof ( Type );
        return true;
    } else {
        type = TYPE_EMPTY;
        stack_idx += sizeof ( Type );
        return false;
    }
}
