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

#include "args/argcheck2.h"
#include "ceammc_containers.h"
#include "ceammc_crc32.h"
#include "ceammc_factory.h"
#include "ceammc_format.h"
#include "datatype_dict.h"
#include "net_zeroconf.h"
#include "pd_rs.h"

#include "fmt/core.h"

using namespace ceammc::mdns;
using namespace ceammc::mdns::req;
using namespace ceammc::mdns::reply;

using MutexLock = std::lock_guard<std::mutex>;

static const std::string ALL_SERVICES = "_services._dns-sd._udp.local.";

CEAMMC_DEFINE_SYM(add)
CEAMMC_DEFINE_SYM(remove)
CEAMMC_DEFINE_SYM(resolve)
CEAMMC_DEFINE_SYM(service)

mdns::MdnsServiceInfo::MdnsServiceInfo(const ceammc_mdns_service_info& info)
    : ty_domain { info.fullname } // <service>.<domain>
//    /// See RFC6763 section 7.1 about "Subtypes":
//    /// <https://datatracker.ietf.org/doc/html/rfc6763#section-7.1>
//    std::string sub_domain; // <subservice>._sub.<service>.<domain>
// std::string fullname; // <instance>.<service>.<domain>
// std::string server; // fully qualified name for service host
// IpList addresses;
// TxtPropertyList props;
// std::uint32_t host_ttl { 0 }; // used for SRV and Address records
// std::uint32_t other_ttl { 0 }; // used for PTR and TXT records
// std::uint16_t port { 0 };
// std::uint16_t priority { 0 };
// std::uint16_t weight { 0 };
{
}

NetZeroconfImpl::NetZeroconfImpl()
{
    start();
}

NetZeroconfImpl::~NetZeroconfImpl()
{
    stop();
}

void NetZeroconfImpl::start()
{
    MutexLock lock(mtx_);
    if (!mdns_) {
        mdns_ = ceammc_mdns_create(
            {
                this,
                [](void* user, const char* msg) {
                    auto this_ = static_cast<NetZeroconfImpl*>(user);
                    if (this_ && this_->cb_err)
                        this_->cb_err(msg);
                },
            },
            {
                this,
                [](void* user, const char* type, const char* fullname, bool found) {
                    auto this_ = static_cast<NetZeroconfImpl*>(user);
                    if (this_ && this_->cb_service)
                        this_->cb_service(type, fullname, found);
                },
            },
            {
                this,
                [](void* user, const ceammc_mdns_service_info* info) {
                    auto this_ = static_cast<NetZeroconfImpl*>(user);
                    if (this_ && this_->cb_resolv) {
                        MdnsServiceInfo si(*info);
                        this_->cb_resolv(si);
                    }
                },
            });
    }
}

void NetZeroconfImpl::stop()
{
    MutexLock lock(mtx_);
    if (mdns_) {
        ceammc_mdns_free(mdns_);
        mdns_ = nullptr;
    }
}

void NetZeroconfImpl::subscribe(const char* service)
{
    MutexLock lock(mtx_);
    if (mdns_) {
        ceammc_mdns_subscribe(mdns_, service);
    }
}

void NetZeroconfImpl::unsubscribe(const char* service)
{
    MutexLock lock(mtx_);
    if (mdns_) {
        ceammc_mdns_unsubscribe(mdns_, service);
    }
}

void mdns::NetZeroconfImpl::process(const req::RegisterService& m)
{
    MutexLock lock(mtx_);
    if (mdns_) {
        // props
        std::vector<ceammc_mdns_txt_prop> txt;
        txt.reserve(m.props.size());
        for (auto& kv : m.props)
            txt.push_back({ kv.first.c_str(), kv.second.c_str() });

        ceammc_mdns_service_info info {
            m.type.c_str(),
            m.name.c_str(),
            m.hostname.c_str(),
            m.port,
            0,
            0,
            0,
            0,
            nullptr, 0,
            txt.data(),
            txt.size()
        };

        ceammc_mdns_register(mdns_, &info);
    }
}

void mdns::NetZeroconfImpl::process(const req::UnregisterService& m)
{
    MutexLock lock(mtx_);
    if (mdns_) {
        ceammc_mdns_unregister(mdns_, m.type.c_str(), 0);
    }
}

void NetZeroconfImpl::process_events()
{
    MutexLock lock(mtx_);
    if (mdns_) {
        ceammc_mdns_process_events(mdns_, 100);
    }
}

NetZeroconf::NetZeroconf(const PdArgs& args)
    : BaseZeroconf(args)
{
    timeout_ = new IntProperty("@t", 3);
    timeout_->checkClosedRange(1, 10);
    timeout_->setUnitsSec();
    addProperty(timeout_);

    ifaces_ = new ListProperty("@ifaces");
    addProperty(ifaces_);

    mdns_.reset(new NetZeroconfImpl);
    mdns_->cb_err = [this](const char* msg) { workerThreadError(msg); };
    mdns_->cb_service = [this](const char* type, const char* fullname, bool found) {
        if (found)
            addReply(ServiceAdded { type, fullname });
        else
            addReply(ServiceRemoved { type, fullname });
    };

    createOutlet();
}

void NetZeroconf::m_active(t_symbol* s, const AtomListView& lv)
{
    if (lv.boolAt(0, true))
        addRequest(req::Subscribe { ALL_SERVICES });
    else
        addRequest(req::Unsubscribe { ALL_SERVICES });
}

void NetZeroconf::m_subscribe(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("SERVICE:s ON:b?");
    auto type = lv.symbolAt(0, &s_)->s_name;
    auto on = lv.boolAt(1, true);

    if (on)
        addRequest(req::Subscribe { type });
    else
        addRequest(req::Unsubscribe { type });
}

void NetZeroconf::m_unsubscribe(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("SERVICE:s");
    auto type = lv.symbolAt(0, &s_)->s_name;

    addRequest(req::Unsubscribe { type });
}

void NetZeroconf::processRequest(const Request& req, ResultCallback fn)
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

void NetZeroconf::processResult(const Reply& r)
{
    if (processReplyT<ServiceAdded>(r)) {
    } else if (processReplyT<ServiceRemoved>(r)) {
    } else if (processReplyT<ServiceResolved>(r)) {
    } else {
        OBJ_ERR << "unknown reply type: " << r.type().name();
    }
}

void NetZeroconf::processEvents()
{
    if (!mdns_)
        return;

    mdns_->process_events();
}

void NetZeroconf::processReply(const mdns::reply::ServiceAdded& r)
{
    if (r.type == ALL_SERVICES) {
        anyTo(0, sym_service(), gensym(r.name.c_str()));
    } else {
        AtomArray<2> data { gensym(r.type.c_str()), gensym(r.name.c_str()) };
        anyTo(0, sym_add(), data.view());
    }
}

void NetZeroconf::processReply(const mdns::reply::ServiceRemoved& r)
{
    AtomArray<2> data { gensym(r.type.c_str()), gensym(r.name.c_str()) };
    anyTo(0, sym_remove(), data.view());
}

void NetZeroconf::processReply(const mdns::reply::ServiceResolved& r)
{
    DictAtom info;
    anyTo(0, sym_resolve(), info);
}

void setup_net_zeroconf()
{
    ObjectFactory<NetZeroconf> obj("net.zeroconf");
    obj.addMethod("active", &NetZeroconf::m_active);
    obj.addMethod("subscribe", &NetZeroconf::m_subscribe);
    obj.addMethod("unsubscribe", &NetZeroconf::m_unsubscribe);
}

#endif
