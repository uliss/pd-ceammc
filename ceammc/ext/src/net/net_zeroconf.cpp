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
#ifndef WITH_ZEROCONF
#include "ceammc_stub.h"
CONTROL_OBJECT_STUB(NetZeroconf, 1, 1, "compiled without zeroconf support");
OBJECT_STUB_SETUP(NetZeroconf, net_zeroconf, "net.zeroconf");
#else

#include "net_zeroconf.h"
#include "ceammc_factory.h"

#include "datatype_dict.h"
#include "fmt/core.h"
#include "pd_rs.h"

NetZeroconf::NetZeroconf(const PdArgs& args)
    : BaseZeroconf(args)
{
    timeout_ = new IntProperty("@t", 3);
    timeout_->checkClosedRange(1, 10);
    timeout_->setUnitsSec();
    addProperty(timeout_);

    ifaces_ = new ListProperty("@ifaces");
    addProperty(ifaces_);

    createOutlet();
}

void NetZeroconf::onSymbol(t_symbol* s)
{
    std::vector<t_symbol*> ifaces;
    for (auto& s : ifaces_->value()) {
        if (s.isSymbol())
            ifaces.push_back(s.asT<t_symbol*>());
    }

    addRequest({ s->s_name, ifaces, static_cast<uint16_t>(timeout_->value()) });
}

static void mdns_callback(void* user_data, const ceammc_rs_mdns_service_info* info)
{
    auto cb = static_cast<std::function<void(const MDNSReply&)>*>(user_data);
    if (!cb || !*cb)
        return;

    MDNSReply rep;
    rep.port = info->port;
    rep.name = info->fullname;
    rep.hostname = info->hostname;
    rep.type = info->rtype;
    for (size_t i = 0; i < info->ip_count; i++)
        rep.ip.push_back(info->ip[i]);

    for (size_t i = 0; i < info->txt_prop_count; i++)
        rep.txt_props.emplace_back(info->txt_props[i].key, info->txt_props[i].value);

    (*cb)(rep);
}

void NetZeroconf::processRequest(const MDNSRequest& req, ResultCallback fn)
{
    auto srv = ceammc_rs_mdns_create();
    if (!srv) {
        workerThreadError("can't start MDNS service");
        return;
    }

    for (auto& iface : req.ifaces) {
        auto rc = ceammc_rs_mdns_enable_iface(srv, iface->s_name);
        if (rc != ceammc_rs_mdns_rc::Ok)
            workerThreadDebug(fmt::format("can't enable iface: '{}'", iface->s_name));

        workerThreadDebug(fmt::format("enable iface: '{}'", iface->s_name));
    }

    auto rc = ceammc_rs_mdns_browse(srv, req.service.c_str(), req.timeout, &mdns_callback, &fn);
    if (rc != ceammc_rs_mdns_rc::Ok)
        workerThreadError(ceammc_rs_mdns_strerr(rc));

    ceammc_rs_mdns_free(srv);
}

void NetZeroconf::processResult(const MDNSReply& res)
{
    auto sym_host = gensym("hostname");
    auto sym_type = gensym("type");
    auto sym_name = gensym("name");
    auto sym_port = gensym("port");
    auto sym_ip = gensym("ip");
    auto sym_txt = gensym("txt");

    AtomList ip;

    DictAtom dict;
    dict->insert(sym_host, gensym(res.hostname.c_str()));
    dict->insert(sym_name, gensym(res.name.c_str()));
    dict->insert(sym_type, gensym(res.type.c_str()));
    dict->insert(sym_port, Atom(static_cast<int>(res.port)));

    // ip
    ip.clear();
    for (auto& b : res.ip) {
        ip.append(gensym(b.c_str()));
    }
    dict->insert(sym_ip, ip);

    // txt
    DictAtom txt;
    for (auto& kv : res.txt_props) {
        txt->insert(gensym(kv.first.c_str()), gensym(kv.second.c_str()));
    }
    dict->insert(sym_txt, txt);
    atomTo(0, dict);
}

void setup_net_zeroconf()
{
    ObjectFactory<NetZeroconf> obj("net.zeroconf");
}

#endif
