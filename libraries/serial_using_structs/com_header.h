/**
 * @author Markus Bader <markus.bader@tuwien.ac.at>
 * @license Simplified BSD License
 */


#ifndef COM_HEADER_H
#define COM_HEADER_H

namespace tuw {

class __attribute__ ( ( packed ) ) Time {
public:
    int32_t sec;      /// seconds (stamp_secs) since epoch
    int32_t nsec;     /// nanoseconds = 0.000000001 sec since stamp_secs
    Time();
    Time ( int32_t sec, int32_t nsec );
    Time ( const Time &t );
    static void setClock ( Time now, uint32_t millisecond );
    static Time now ( uint32_t millisecond );
    static bool isSet();
    static Time offest();
private:
    static Time OFFSET;
    static bool CLOCK_SYNC;
};
/**
 * Header for serial communication
 * @author Markus Bader <markus.bader@tuwien.ac.at>
 **/
class __attribute__ ( ( packed ) ) ComHeader {
public:
    typedef uint16_t Type;
    static const Type TYPE_EMPTY = 0;
    static const Type TYPE_SYNC_REQUEST = 10;
    static const Type TYPE_SYNC = 11;
    uint16_t size;    /// total message size if size
    uint32_t seq;     /// consecutively increasing ID
    Time stamp;       /// time since epoch
    void update_time ( unsigned long millisecond ); /// update time stamp
};
};


#endif
