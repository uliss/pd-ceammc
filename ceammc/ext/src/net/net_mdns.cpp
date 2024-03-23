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
#include "lex/parser_strings.h"
#include "net_mdns.h"
#include "pd_rs.h"

#include "fmt/core.h"

using namespace ceammc::mdns;
using namespace ceammc::mdns::req;
using namespace ceammc::mdns::reply;

using MutexLock = std::lock_guard<std::mutex>;

CEAMMC_DEFINE_SYM(add)
CEAMMC_DEFINE_SYM(remove)
CEAMMC_DEFINE_SYM(resolve)
CEAMMC_DEFINE_SYM(service)

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
        addresses.push_back(info.ip[i]);

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
            nullptr, 0,
            txt.data(),
            txt.size()
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

void MdnsImpl::process_events()
{
    MutexLock lock(mtx_);
    if (mdns_) {
        ceammc_mdns_process_events(mdns_, 100);
    }
}

NetMdns::NetMdns(const PdArgs& args)
    : BaseMdns(args)
{
    fullname_ = new BoolProperty("@fullname", true);
    addProperty(fullname_);

    mdns_.reset(new MdnsImpl);
    mdns_->cb_err = [this](const char* msg) { workerThreadError(msg); };
    mdns_->cb_service = [this](const char* type, const char* fullname, bool found) {
        if (found)
            addReply(ServiceAdded { type, fullname });
        else
            addReply(ServiceRemoved { type, fullname });
    };
    mdns_->cb_resolv = [this](const MdnsServiceInfo& info) { addReply({ ServiceResolved { info } }); };

    createOutlet();
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

    TxtPropertyList props;
    list::foreachProperty(lv, [this, &host, &props](t_symbol* k, const AtomListView& v) {
        switch (crc32_hash(k)) {
        case "@host"_hash:
            host = to_string(v);
            break;
        case "@ip"_hash:
            break;
        case "@ipv4"_hash:
            break;
        case "@ipv6"_hash:
            break;
        case "@local"_hash:
            break;
        default:
            props.push_back({ std::string(k->s_name + 1), to_string(v) });
            break;
        }
    });

    addRequest(RegisterService { name, service, host, props, port });
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
    } else {
        workerThreadError(fmt::format("unknown request type: {}", type.name()));
    }
}

void NetMdns::processResult(const Reply& r)
{
    if (processReplyT<ServiceAdded>(r)) {
    } else if (processReplyT<ServiceRemoved>(r)) {
    } else if (processReplyT<ServiceResolved>(r)) {
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

    auto ip_count = si.addresses.size();
    if (ip_count > 0) {
        res.append(gensym(si.addresses.front().c_str()));

        if (ip_count > 1) {
            res.append(gensym("@ip"));
            for (auto& i : si.addresses)
                res.append(gensym(i.c_str()));
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

t_symbol* NetMdns::instanceName(const std::string& name) const
{
    return gensym(
        (fullname_->value()
                ? name
                : string::mdns_instance_name_from_service(name.c_str()) //
            )
            .c_str());
}

void setup_net_mdns()
{
    ObjectFactory<NetMdns> obj("net.mdns");
    obj.addMethod("subscribe", &NetMdns::m_subscribe);
    obj.addMethod("unsubscribe", &NetMdns::m_unsubscribe);
    obj.addMethod("register", &NetMdns::m_register);
    obj.addMethod("unregister", &NetMdns::m_unregister);
}

#endif
