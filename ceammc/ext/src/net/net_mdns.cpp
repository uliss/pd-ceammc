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
CONTROL_OBJECT_STUB(NetMdns, 1, 1, "compiled without MDNS support");
OBJECT_STUB_SETUP(NetMdns, net_mdns, "net.mdns");
#else

#include "args/argcheck2.h"
#include "ceammc_containers.h"
#include "ceammc_crc32.h"
#include "ceammc_factory.h"
#include "ceammc_fn_list.h"
#include "ceammc_format.h"
#include "datatype_dict.h"
#include "lex/parser_strings.h"
#include "net_mdns.h"

#include "fmt/core.h"

using namespace ceammc::mdns;
using namespace ceammc::mdns::req;
using namespace ceammc::mdns::reply;

using MutexLock = std::lock_guard<std::mutex>;

CEAMMC_DEFINE_SYM(add)
CEAMMC_DEFINE_SYM(remove)
CEAMMC_DEFINE_SYM(resolve)
CEAMMC_DEFINE_SYM(service)

CEAMMC_DEFINE_HASH(any)
CEAMMC_DEFINE_HASH(v4)
CEAMMC_DEFINE_HASH(v6)

static const std::string& try_service_alias(const std::string& str)
{
    static std::unordered_map<std::string, std::string> map {
        { "#all", "_services._dns-sd._udp" },
        { "#osc", "_osc._udp" },
        { "#http", "_http._tcp" },
        { "#daap", "_daap._tcp" },
        { "#ftp", "_ftp._tcp" },
        { "#printer", "_printer._tcp" },
        { "#scanner", "_scanner._tcp" },
        { "#sftp", "_sftp-ssh._tcp" },
        { "#smb", "_smb._tcp" },
        { "#ssh", "_ssh._tcp" },
        { "#telnet", "_telnet._tcp" },
        { "#ipp", "_ipp._tcp" }
    };

    auto it = map.find(str);
    return it == map.cend() ? str : it->second;
}

mdns::MdnsServiceInfo::MdnsServiceInfo(const ceammc_mdns_service_info& info)
    : service { info.stype }
    , fullname { info.fullname }
    , hostname { info.hostname }
    , host_ttl { info.host_ttl }
    , other_ttl { info.other_ttl }
    , port { info.port }
    , priority { info.priority }
    , weight { info.weight }
{
    for (size_t i = 0; i < info.ip_len; i++)
        addresses.emplace_back(IpAddr { info.ip[i].addr, info.ip[i].is_ipv4 });

    for (size_t i = 0; i < info.txt_len; i++)
        props.push_back({ info.txt[i].key, info.txt[i].value });
}

MdnsImpl::MdnsImpl()
{
    start();
}

MdnsImpl::~MdnsImpl()
{
    stop();
}

void MdnsImpl::start()
{
    MutexLock lock(mtx_);
    if (!mdns_) {
        mdns_ = ceammc_mdns_create(
            {
                this,
                [](void* user, const char* msg) {
                    auto this_ = static_cast<MdnsImpl*>(user);
                    if (this_ && this_->cb_err)
                        this_->cb_err(msg);
                },
            },
            {
                this,
                [](void* user, const char* type, const char* fullname, bool found) {
                    auto this_ = static_cast<MdnsImpl*>(user);
                    if (this_ && this_->cb_service)
                        this_->cb_service(type, fullname, found);
                },
            },
            {
                this,
                [](void* user, const ceammc_mdns_service_info* info) {
                    auto this_ = static_cast<MdnsImpl*>(user);
                    if (this_ && this_->cb_resolv) {
                        MdnsServiceInfo si(*info);
                        this_->cb_resolv(si);
                    }
                },
            });
    }
}

void MdnsImpl::stop()
{
    MutexLock lock(mtx_);
    if (mdns_) {
        ceammc_mdns_free(mdns_);
        mdns_ = nullptr;
    }
}

void MdnsImpl::subscribe(const std::string& service)
{
    MutexLock lock(mtx_);
    if (mdns_)
        ceammc_mdns_subscribe(mdns_, try_service_alias(service).c_str());
}

void MdnsImpl::unsubscribe(const std::string& service)
{
    MutexLock lock(mtx_);
    if (mdns_)
        ceammc_mdns_unsubscribe(mdns_, try_service_alias(service).c_str());
}

void mdns::MdnsImpl::process(const req::RegisterService& m)
{
    MutexLock lock(mtx_);
    if (mdns_) {
        // props
        std::vector<ceammc_mdns_txt_prop> txt;
        txt.reserve(m.props.size());
        for (auto& kv : m.props)
            txt.push_back({ kv.first.c_str(), kv.second.c_str() });

        ceammc_mdns_service_info_register info {
            try_service_alias(m.service).c_str(),
            m.name.c_str(),
            m.host.c_str(),
            m.port,
            txt.data(),
            txt.size(),
            m.iface
        };

        ceammc_mdns_register(mdns_, &info);
    }
}

void mdns::MdnsImpl::process(const req::UnregisterService& m)
{
    MutexLock lock(mtx_);
    if (mdns_)
        ceammc_mdns_unregister(mdns_, m.name.c_str(), try_service_alias(m.service).c_str(), 10);
}

void MdnsImpl::process(const req::EnableIface& m)
{
    MutexLock lock(mtx_);
    if (mdns_)
        ceammc_mdns_enable_iface(mdns_, m.name.c_str());
}

void MdnsImpl::process(const ListIfaces& /*m*/)
{
    auto ifl = ceammc_net_list_interfaces(
        { this,
            [](void* user, const char* msg) {
                auto this_ = static_cast<MdnsImpl*>(user);
                if (this_ && this_->cb_err)
                    this_->cb_err(msg);
            } });

    if (ifl) {
        ceammc_net_foreach_interfaces(ifl, this, [](void* user, const ceammc_net_iface* iface) {
            auto this_ = static_cast<MdnsImpl*>(user);
            if (this_ && this_->cb_err) {
                auto& fn = this_->cb_err;
                this_->cb_iface(net::Iface(*iface));
            }
        });
        ceammc_net_free_interfaces(ifl);
    }
}

void MdnsImpl::process_events()
{
    MutexLock lock(mtx_);
    if (mdns_)
        ceammc_mdns_process_events(mdns_, 100);
}

NetMdns::NetMdns(const PdArgs& args)
    : BaseMdns(args)
{
    fullname_ = new BoolProperty("@fullname", true);
    addProperty(fullname_);

    ip_ = new SymbolEnumProperty("@ip", { str_v4, str_v6, str_any });
    addProperty(ip_);

    mdns_.reset(new MdnsImpl);
    mdns_->cb_err = [this](const char* msg) { workerThreadError(msg); };
    mdns_->cb_service = [this](const char* type, const char* fullname, bool found) {
        if (found)
            addReply(ServiceAdded { type, fullname });
        else
            addReply(ServiceRemoved { type, fullname });
    };
    mdns_->cb_resolv = [this](const MdnsServiceInfo& info) { addReply({ ServiceResolved { info } }); };
    mdns_->cb_iface = [this](const net::Iface& iface) { addReply(iface); };

    createOutlet();
}

void NetMdns::m_ifaces(t_symbol* s, const AtomListView&)
{
    addRequest(req::ListIfaces {});
}

void NetMdns::m_set_iface(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("IFACE:s");
    if (!chk.check(lv, this))
        return;

    addRequest(req::EnableIface { lv.asSymbol()->s_name });
}

void NetMdns::m_subscribe(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("SERVICE:s ON:b?");
    auto type = lv.symbolAt(0, &s_)->s_name;
    auto on = lv.boolAt(1, true);

    if (on)
        addRequest(req::Subscribe { type });
    else
        addRequest(req::Unsubscribe { type });
}

void NetMdns::m_unsubscribe(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("SERVICE:s");
    auto type = lv.symbolAt(0, &s_)->s_name;

    addRequest(req::Unsubscribe { type });
}

void NetMdns::m_register(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("SERVICE:s NAME:s PORT:i PROPS:a*");
    if (!chk.check(lv, this))
        return;

    auto service = lv.symbolAt(0, &s_)->s_name;
    auto name = lv.symbolAt(1, &s_)->s_name;
    std::string host = name;
    std::uint16_t port = lv.intAt(2, 0);

    ceammc_mdns_iface if_type = ceammc_mdns_iface::ANY;
    switch (crc32_hash(ip_->value())) {
    case hash_v4:
        if_type = ceammc_mdns_iface::V4;
        break;
    case hash_v6:
        if_type = ceammc_mdns_iface::V6;
        break;
    default:
        if_type = ceammc_mdns_iface::ANY;
        break;
    }

    TxtPropertyList props;
    list::foreachProperty(lv, [this, &host, &props, &if_type](t_symbol* k, const AtomListView& v) {
        switch (crc32_hash(k)) {
        case "@host"_hash:
            host = to_string(v);
            break;
        case "@ipv4"_hash:
            if_type = ceammc_mdns_iface::V4;
            break;
        case "@ipv6"_hash:
            if_type = ceammc_mdns_iface::V6;
            break;
        default:
            props.push_back({ std::string(k->s_name + 1), to_string(v) });
            break;
        }
    });

    addRequest(RegisterService { name, service, host, props, port, if_type });
}

void NetMdns::m_unregister(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("SERVICE:s NAME:s");
    if (!chk.check(lv, this))
        return;

    auto service = lv.symbolAt(0, &s_)->s_name;
    auto name = lv.symbolAt(1, &s_)->s_name;

    addRequest({ req::UnregisterService { name, service } });
}

void NetMdns::processRequest(const Request& req, ResultCallback fn)
{
    if (!mdns_)
        return;

    auto& type = req.type();
    if (type == typeid(Subscribe)) {
        mdns_->subscribe(boost::get<Subscribe>(req).type.c_str());
    } else if (type == typeid(Unsubscribe)) {
        mdns_->unsubscribe(boost::get<Unsubscribe>(req).type.c_str());
    } else if (type == typeid(RegisterService)) {
        mdns_->process(boost::get<RegisterService>(req));
    } else if (type == typeid(UnregisterService)) {
        mdns_->process(boost::get<UnregisterService>(req));
    } else if (type == typeid(EnableIface)) {
        mdns_->process(boost::get<EnableIface>(req));
    } else if (type == typeid(ListIfaces)) {
        mdns_->process(boost::get<ListIfaces>(req));
    } else {
        workerThreadError(fmt::format("unknown request type: {}", type.name()));
    }
}

void NetMdns::processResult(const Reply& r)
{
    if (processReplyT<ServiceAdded>(r)) {
    } else if (processReplyT<ServiceRemoved>(r)) {
    } else if (processReplyT<ServiceResolved>(r)) {
    } else if (processReplyT<net::Iface>(r)) {
    } else {
        OBJ_ERR << "unknown reply type: " << r.type().name();
    }
}

void NetMdns::processEvents()
{
    if (!mdns_)
        return;

    mdns_->process_events();
}

void NetMdns::processReply(const mdns::reply::ServiceAdded& r)
{
    static const std::string ALL_SERVICES = "_services._dns-sd._udp.local.";

    if (r.type == ALL_SERVICES) {
        AtomArray<2> data { sym_add(), gensym(r.name.c_str()) };
        anyTo(0, sym_service(), data.view());
    } else {
        AtomArray<2> data { gensym(r.type.c_str()), instanceName(r.name) };
        anyTo(0, sym_add(), data.view());
    }
}

void NetMdns::processReply(const mdns::reply::ServiceRemoved& r)
{
    static const std::string ALL_SERVICES = "_services._dns-sd._udp.local.";

    if (r.type == ALL_SERVICES) {
        AtomArray<2> data { sym_remove(), gensym(r.name.c_str()) };
        anyTo(0, sym_service(), data.view());
    } else {
        AtomArray<2> data { gensym(r.type.c_str()), instanceName(r.name) };
        anyTo(0, sym_remove(), data.view());
    }
}

void NetMdns::processReply(const mdns::reply::ServiceResolved& r)
{
    auto& si = r.info;
    AtomList res;
    res.reserve(10);

    res.append(gensym(r.info.service.c_str()));
    res.append(instanceName(r.info.fullname));
    res.append(gensym(r.info.hostname.c_str()));
    res.append(Atom(r.info.port));

    auto ip = filterIpAddr(si.addresses);
    if (ip.size() > 0) {
        res.append(gensym(ip.front().addr.c_str()));

        if (ip.size() > 1) {
            res.append(gensym("@ip"));
            for (auto& i : ip)
                res.append(gensym(i.addr.c_str()));
        }
    } else {
        res.append(gensym("?.?.?.?"));
    }

    for (auto& kv : si.props) {
        char key[64];
        *fmt::format_to(key, "@{}", kv.first) = '\0';

        res.append(gensym(key));
        res.append(AtomList::parseString(kv.second.c_str()));
    }

    anyTo(0, sym_resolve(), res);
}

void NetMdns::processReply(const net::Iface& iface)
{
    DictAtom info;
    info->insert("name", gensym(iface.name.c_str()));
    info->insert("mac", gensym(iface.mac.c_str()));
    info->insert("index", iface.index);
    info->insert("ip", AtomList {});

    DictAtom ip;
    net::IfaceAddrVisitor v {
        [this, &ip](const net::IfaceAddrV4& v4) {
            ip->insert("ip", gensym(v4.ip.str.c_str()));

            ip->insert("is_loopback", v4.ip.bits.is_loopback);
            ip->insert("is_multicast", v4.ip.bits.is_multicast);

            if (v4.bcast)
                ip->insert("broadcast", gensym(v4.bcast->str.c_str()));

            if (v4.mask)
                ip->insert("mask", gensym(v4.mask->str.c_str()));
        },
        [this, &ip](const net::IfaceAddrV6& v6) {
            ip->insert("ip", gensym(v6.ip.str.c_str()));

            ip->insert("is_loopback", v6.ip.bits.is_loopback);
            ip->insert("is_multicast", v6.ip.bits.is_multicast);

            if (v6.bcast)
                ip->insert("broadcast", gensym(v6.bcast->str.c_str()));

            if (v6.mask)
                ip->insert("mask", gensym(v6.mask->str.c_str()));
        }
    };

    for (auto& a : iface.addrs) {
        a.apply_visitor(v);
        info->at("ip").append(ip->clone());
    }

    AtomArray<2> pair { gensym(iface.name.c_str()), info };
    anyTo(0, gensym("iface"), pair.view());
}

t_symbol* NetMdns::instanceName(const std::string& name) const
{
    return gensym(
        (fullname_->value()
                ? name
                : string::mdns_instance_name_from_service(name.c_str()) //
            )
            .c_str());
}

IpList NetMdns::filterIpAddr(const mdns::IpList& ips) const
{
    IpList res;
    auto hash = crc32_hash(ip_->value());

    std::copy_if(ips.begin(), ips.end(), std::back_inserter(res), [this, hash](const IpAddr& ip) {
        switch (hash) {
        case hash_v4:
            return ip.is_ipv4;
        case hash_v6:
            return !ip.is_ipv4;
        case hash_any:
        default:
            return true;
        }
    });

    return res;
}

void setup_net_mdns()
{
    ObjectFactory<NetMdns> obj("net.mdns");
    obj.addMethod("ifaces", &NetMdns::m_ifaces);
    obj.addMethod("register", &NetMdns::m_register);
    obj.addMethod("set_iface", &NetMdns::m_set_iface);
    obj.addMethod("subscribe", &NetMdns::m_subscribe);
    obj.addMethod("unregister", &NetMdns::m_unregister);
    obj.addMethod("unsubscribe", &NetMdns::m_unsubscribe);
}

#endif
