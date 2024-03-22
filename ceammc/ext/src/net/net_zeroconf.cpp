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
#include "ceammc_factory.h"
#include "net_zeroconf.h"
#include "pd_rs.h"

using namespace ceammc::mdns;
using namespace ceammc::mdns::req;
using namespace ceammc::mdns::reply;

using MutexLock = std::lock_guard<std::mutex>;

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
        mdns_ = ceammc_rs_mdns_create(
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
            });
    }
}

void NetZeroconfImpl::stop()
{
    MutexLock lock(mtx_);
    if (mdns_) {
        ceammc_rs_mdns_free(mdns_);
        mdns_ = nullptr;
    }
}

void NetZeroconfImpl::subscribe(const char* service)
{
    MutexLock lock(mtx_);
    if (mdns_) {
        ceammc_rs_mdns_subscribe(mdns_, service);
    }
}

void NetZeroconfImpl::unsubscribe(const char* service)
{
    MutexLock lock(mtx_);
    if (mdns_) {
        ceammc_rs_mdns_unsubscribe(mdns_, service);
    }
}

void NetZeroconfImpl::process_events()
{
    MutexLock lock(mtx_);
    if (mdns_) {
        ceammc_rs_mdns_process_events(mdns_, 100);
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
    }
}

void NetZeroconf::processResult(const Reply& r)
{
    if (processReplyT<ServiceAdded>(r)) {
    } else if (processReplyT<ServiceRemoved>(r)) {
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
    AtomArray<2> data { gensym(r.type.c_str()), gensym(r.name.c_str()) };
    anyTo(0, gensym("add"), data.view());
}

void NetZeroconf::processReply(const mdns::reply::ServiceRemoved& r)
{
    AtomArray<2> data { gensym(r.type.c_str()), gensym(r.name.c_str()) };
    anyTo(0, gensym("remove"), data.view());
}

void setup_net_zeroconf()
{
    ObjectFactory<NetZeroconf> obj("net.zeroconf");
    obj.addMethod("subscribe", &NetZeroconf::m_subscribe);
    obj.addMethod("unsubscribe", &NetZeroconf::m_unsubscribe);
}

#endif
