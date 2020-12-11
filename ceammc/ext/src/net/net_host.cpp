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
#include "ceammc_factory.h"
#include "ceammc_fn_list.h"
#include "ceammc_output.h"
#include "ceammc_platform.h"

#include <mutex>

typedef int t_pipe_fd[2];

class HostTask : public thread::Task {
    mutable std::mutex lock_;
    t_symbol* name_;
    AtomList result_;
    platform::NetAddressType type_;

public:
    HostTask(NetHost* caller)
        : Task(caller)
        , name_(&s_)
        , type_(platform::ADDR_IPV4)
    {
    }

    void setType(platform::NetAddressType t)
    {
        type_ = t;
    }

    void setName(t_symbol* n)
    {
        std::lock_guard<std::mutex> lock(lock_);
        name_ = n;
    }

    AtomList result() const
    {
        std::lock_guard<std::mutex> lock(lock_);
        return result_;
    }

    int run() override
    {
        if (name_ == &s_) {
            writeError("no host name specified");
            return 1;
        }

        platform::NetAddressList res;
        platform::PlatformError err;

        auto either = platform::hostnametoip(name_->s_name, type_);
        if (either.matchValue(res)) {
            std::lock_guard<std::mutex> lock(lock_);
            result_.clear();

            for (auto& addr : res)
                result_.append(gensym(addr.c_str()));

            result_ = list::uniqueStable(result_);

            return 0;

        } else if (either.matchError(err)) {
            writeError(err.msg.c_str());
            return 1;
        } else {
            writeError("bug");
            return 2;
        }
    }
};

static t_symbol* SYM_IPV4;
static t_symbol* SYM_IPV6;

NetHost::NetHost(const PdArgs& args)
    : ThreadExternal(args, new HostTask(this))
    , addr_type_(nullptr)
{
    task()->setName(parsedPosArgs().symbolAt(0, &s_));
    createOutlet();

    addr_type_ = new SymbolEnumProperty("@type", { SYM_IPV4, SYM_IPV6 });
    addr_type_->setArgIndex(1);
    addProperty(addr_type_);

    addProperty(new SymbolEnumAlias("@ipv4", addr_type_, SYM_IPV4));
    addProperty(new SymbolEnumAlias("@ipv6", addr_type_, SYM_IPV6));
}

void NetHost::onSymbol(t_symbol* s)
{
    task()->setName(s);
    task()->setType(addr_type_->value() == SYM_IPV4 ? platform::ADDR_IPV4 : platform::ADDR_IPV6);
    start();
}

void NetHost::onAny(t_symbol* s, const AtomListView&)
{
    onSymbol(s);
}

void NetHost::onThreadDone(int rc)
{
    if (rc == 0)
        outletAtomList(outletAt(0), task()->result(), true);
}

HostTask* NetHost::task()
{
    return static_cast<HostTask*>(task_.get());
}

void setup_net_host()
{
    SYM_IPV4 = gensym("ipv4");
    SYM_IPV6 = gensym("ipv6");

    ObjectFactory<NetHost> obj("net.host2ip");
    obj.addAlias("net.host->ip");
}
