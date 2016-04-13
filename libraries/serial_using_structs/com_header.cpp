/**
 * @author Markus Bader <markus.bader@tuwien.ac.at>
 * @license Simplified BSD License
 */

#include <Arduino.h>
#include "com_header.h"

using namespace tuw;
Time Time::OFFSET ( 0,0 );
bool Time::CLOCK_SYNC ( false );
Time::Time() : sec ( 0 ), nsec ( 0 ) {}
Time::Time ( int32_t sec, int32_t nsec ) :sec ( sec ), nsec ( nsec ) {}
Time::Time ( const Time &t ) :sec ( t.sec ), nsec ( t.nsec ) {}
void Time::setClock ( Time now, uint32_t millisecond ) {
    int32_t s = ( millisecond / 1000 );
    int32_t n = ( millisecond % 1000UL ) * 1000UL * 1000UL;
    OFFSET.sec = now.sec - s;
    OFFSET.nsec = now.nsec - n;
    if ( OFFSET.nsec < 0 ) {
        OFFSET.nsec = ( 1000UL * 1000UL * 1000UL ) + OFFSET.nsec;
        OFFSET.sec--;
    }
    CLOCK_SYNC = true;
}
Time Time::now ( uint32_t millisecond ) {
    Time t;
    int32_t s = millisecond / 1000;
    int32_t m = millisecond % 1000;
    int32_t n = OFFSET.nsec + m * ( 1000UL * 1000UL );
    t.sec = OFFSET.sec + s + n / ( 1000UL * 1000UL * 1000UL );
    t.nsec = n % ( 1000UL * 1000UL * 1000UL );
    return t;
}
Time Time::offest() {
    return OFFSET;
}
bool Time::isSet() {
    return CLOCK_SYNC;
}

void ComHeader::update_time ( unsigned long millisecond ) {
    stamp = Time::now ( millisecond );
}
