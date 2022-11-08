#ifndef OSC_COMMON_H
#define OSC_COMMON_H

#include <cstdint>

namespace ceammc {

enum OscProto : uint8_t {
    OSC_PROTO_UDP,
    OSC_PROTO_TCP,
    OSC_PROTO_UNIX,
    OSC_PROTO_DEFAULT = OSC_PROTO_UDP,
    OSC_PROTO_INVALID,
};

}

#endif // OSC_COMMON_H
