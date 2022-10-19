#ifndef PARSER_OSC_H
#define PARSER_OSC_H

#include "m_pd.h"

#include "osc_common.h"

namespace ceammc {
namespace parser {

    /*
     * Parse OSC address
     * @param str - osc address
     * @param proto - osc proto: tcp, udp or unix
     * @param host
     * @param port
     * @param unix_path - path to unix socket
     * @return true on success, false on error
     * @example
     * osc://:9000
     * osc.udp://:9000
     * osc.tcp://:9000
     * osc.unix:///var/tmp/socket
     * 9000
     * udp:9000
     * tcp:9000
     */

    enum OscAddress {
        OSC_ADDR_INVALID,
        OSC_ADDR_URL,
        OSC_ADDR_PROTO_PORT,
        OSC_ADDR_UNIX,
    };

    OscAddress parse_osc_url(const char* str, OscProto& proto, t_symbol*& host, int& port, t_symbol*& unix_path);

}
}

#endif // PARSER_OSC_H
