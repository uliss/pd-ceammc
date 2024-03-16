/*****************************************************************************
 * Copyright 2023 Serge Poltavski. All rights reserved.
 *
 * This file may be distributed under the terms of GNU Public License version
 * 3 (GPL v3) as defined by the Free Software Foundation (FSF). A copy of the
 * license should have been included with this file, or the project in which
 * this file belongs to. You may also find the details of GPL v3 at:
 * http://www.gnu.org/licenses/gpl-3.0.txt
 *
 * If you have any questions regarding the use of this file, feel free to
 * contact the author of this file, or the owner of the project in which
 * this file belongs to.
 *****************************************************************************/
#ifndef NET_ZEROCONF_H
#define NET_ZEROCONF_H

#include "ceammc_pollthread_spsc.h"
using namespace ceammc;

struct MDNSRequest {
    std::string service;
    std::vector<t_symbol*> ifaces;
    std::uint16_t timeout;
};

struct MDNSReply {
    std::vector<std::string> ip;
    std::vector<std::pair<std::string, std::string>> txt_props;
    std::string hostname;
    std::string name;
    std::string type;
    std::uint16_t port;
};


using BaseZeroconf = FixedSPSCObject<MDNSRequest, MDNSReply>;

class NetZeroconf : public BaseZeroconf {
    IntProperty* timeout_ { nullptr };
    ListProperty* ifaces_ { nullptr };

public:
    NetZeroconf(const PdArgs& args);

    void onSymbol(t_symbol* s) final;

    void processRequest(const MDNSRequest& req, ResultCallback fn) final;
    void processResult(const MDNSReply& res) final;
};

void setup_net_zeroconf();

#endif // NET_ZEROCONF_H
