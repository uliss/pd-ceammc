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
#include "ceammc_platform.h"

#include <mutex>

typedef int t_pipe_fd[2];

class HostTask : public thread::Task {
    mutable std::mutex lock_;
    t_symbol* name_;
    AtomList result_;

public:
    HostTask()
        : name_(&s_)
    {
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

        auto either = platform::hostnametoip(name_->s_name, platform::ADDR_IPV4);
        if (either.matchValue(res)) {
            std::lock_guard<std::mutex> lock(lock_);
            result_.clear();

            for (auto& addr : res)
                result_.append(gensym(addr.c_str()));

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

NetHost::NetHost(const PdArgs& args)
    : ThreadExternal(args, new HostTask())
{
    task()->setName(positionalSymbolArgument(0, &s_));
    createOutlet();
}

void NetHost::onSymbol(t_symbol* s)
{
    task()->setName(s);
    start();
}

void NetHost::onAny(t_symbol* s, const AtomList&)
{
    onSymbol(s);
}

void NetHost::onThreadExit(int rc)
{
    if (rc == 0)
        to_outlet(outletAt(0), task()->result(), true);
}

HostTask* NetHost::task()
{
    return static_cast<HostTask*>(task_.get());
}

void setup_net_host()
{
    ObjectFactory<NetHost> obj("net.host->ip");
}
