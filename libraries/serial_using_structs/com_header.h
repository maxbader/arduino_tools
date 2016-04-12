/**
 * @author Markus Bader <markus.bader@tuwien.ac.at>
 * @license Simplified BSD License
 */


#ifndef COM_HEADER_H
#define COM_HEADER_H

namespace tuw{
	
class __attribute__ ( ( packed ) ) Time {
public:
    int32_t sec;      /// seconds (stamp_secs) since epoch
    int32_t nsec;     /// nanoseconds = 0.000000001 sec since stamp_secs 
    Time();
    Time(int32_t sec, int32_t nsec);
    Time(const Time &t);
    static void setClock(Time now, uint32_t millisecond);
    static Time now(uint32_t millisecond);
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
    uint16_t size;    /// size of following message
    uint16_t type;    /// message type
					  /// type = 0 means empty
					  /// type = 10 means sync request
					  /// type = 11 means time syncronization
    uint32_t seq;     /// consecutively increasing ID 
    Time stamp;       /// time since epoch
    void update_time(unsigned long millisecond); /// update time stamp
    static const uint16_t TYPE_NA   = 0;
    static const uint16_t TYPE_SYNC = 10;
    static const uint16_t TYPE_TIME = 11;
};
};


#endif
