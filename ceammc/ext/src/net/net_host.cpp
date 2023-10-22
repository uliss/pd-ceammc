/*****************************************************************************
 * Copyright 2018 Serge Poltavsky. All rights reserved.
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
#include "net_host.h"
#include "ceammc_crc32.h"
#include "ceammc_factory.h"
#include "ceammc_platform.h"

#include <unordered_set>

namespace ceammc {

CEAMMC_DEFINE_HASH(ipv4);
CEAMMC_DEFINE_HASH(ipv6);

namespace {

    void startNetHostEvent(ceammc::NetHost* host, ceammc::SubscriberId id)
    {
        HostResult msg;
        while (host->inPipe().try_dequeue(msg)) {
            const auto type = msg.type;

            if (msg.data.empty()) {
                host->outPipe().try_enqueue({ HOST_RESOLVE_ERROR, type, "no host name specified" });
                continue;
            }

            platform::NetAddressList res;
            platform::PlatformError err;

            auto either = platform::hostnametoip(msg.data.c_str(), type);
            if (either.matchValue(res)) {
                // remove duplicates preserving initial order
                std::unordered_set<std::string> ip_set;

                for (auto& ip : res) {
                    if (ip_set.find(ip) == ip_set.end()) {
                        ip_set.insert(ip);
                        host->outPipe().try_enqueue({ HOST_GET_ADDR, type, ip });
                    }
                }

            } else if (either.matchError(err)) {
                host->outPipe().try_enqueue({ HOST_RESOLVE_ERROR, type, err.msg });
            } else {
                host->outPipe().try_enqueue({ HOST_RESOLVE_ERROR, type, "bug" });
                break;
            }
        }

        Dispatcher::instance().send({ id, 0 });
    }

    bool symToAddress(const char* str, platform::NetAddressType& type)
    {
        switch (crc32_hash(str)) {
        case hash_ipv4:
            type = platform::ADDR_IPV4;
            return true;
        case hash_ipv6:
            type = platform::ADDR_IPV6;
            return true;
        default:
            return false;
        }
    }

}

NetHost::NetHost(const PdArgs& args)
    : NetHostBase(args)
    , addr_type_(nullptr)
{
    createOutlet();

    addr_type_ = new SymbolEnumProperty("@type", { str_ipv4, str_ipv6 });
    addr_type_->setArgIndex(1);
    addProperty(addr_type_);

    addProperty(new SymbolEnumAlias("@ipv4", addr_type_, gensym(str_ipv4)));
    addProperty(new SymbolEnumAlias("@ipv6", addr_type_, gensym(str_ipv6)));
}

void NetHost::onSymbol(t_symbol* s)
{
    platform::NetAddressType type;
    if (!symToAddress(addr_type_->value()->s_name, type)) {
        OBJ_ERR << "unknown IP type: " << s;
        return;
    }

    if (!inPipe().try_enqueue({ HOST_RESOLVE_ADDR, type, s->s_name })) {
        OBJ_ERR << "can't send command to worker";
        return;
    }

    if (!runTask())
        OBJ_ERR << "can't run worker";
}

void NetHost::onAny(t_symbol* s, const AtomListView&)
{
    onSymbol(s);
}

void NetHost::processMessage(const HostResult& msg)
{
    switch (msg.cmd) {
    case HOST_GET_ADDR:
        result_.append(gensym(msg.data.c_str()));
        break;
    case HOST_RESOLVE_ERROR:
        OBJ_ERR << msg.data;
        break;
    default:
        OBJ_ERR << "invalid resolve answer: " << (int)msg.cmd;
        return;
    }
}

NetHost::Future NetHost::createTask()
{
    return std::async(std::launch::async, startNetHostEvent, this, subscriberId());
}

bool NetHost::notify(int event)
{
    result_.clear();

    if (!NetHostBase::notify(event))
        return false;

    if (result_.isSymbol())
        symbolTo(0, result_[0].asT<t_symbol*>());
    else if (!result_.empty())
        listTo(0, result_);

    result_.clear();
    return true;
}
}

void setup_net_host()
{
    using namespace ceammc;

    ObjectFactory<NetHost> obj("net.host2ip");
    obj.addAlias("net.host->ip");
}
