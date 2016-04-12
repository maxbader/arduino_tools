/**
 * @author Markus Bader <markus.bader@tuwien.ac.at>
 * @file bldc.cpp
 * @license Simplified BSD License
 */

#include "com_message.h"

using namespace tuw;
uint16_t ComMessage::count_msg = 0;

ComMessage::ComMessage() {
    reset();
}

void ComMessage::reset() {
    memset ( buffer, '\0', 0xFF );
    type = 0;
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
