/**
 * @author Markus Bader <markus.bader@tuwien.ac.at>
 * @license Simplified BSD License
 */


#ifndef COM_HEADER_H
#define COM_HEADER_H

namespace tuw{
struct __attribute__ ( ( packed ) ) ComHeader {
    uint16_t size;    /// size of following message
    uint16_t type;    /// message type
    uint32_t seq;     /// consecutively increasing ID 
    uint32_t sec;     /// seconds (stamp_secs) since epoch
    uint32_t nsec;    /// nanoseconds since stamp_secs
};
};

#endif