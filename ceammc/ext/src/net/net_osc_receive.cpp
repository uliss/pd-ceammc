/*****************************************************************************
 * Copyright 2022 Serge Poltavsky. All rights reserved.
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
#include "net_osc_receive.h"
#include "ceammc_factory.h"
#include "fmt/format.h"

namespace ceammc {
namespace net {

    NetOscReceive::NetOscReceive(const PdArgs& args)
        : NotifiedObject(args)
        , server_(nullptr)
        , path_(nullptr)
        , server_ptr_(nullptr)
        , disp_(this)
    {
        createOutlet();

        server_ = new SymbolProperty("@server", gensym("default"));
        addProperty(server_);

        path_ = new SymbolProperty("@path", &s_);
        path_->setArgIndex(0);
        addProperty(path_);
    }

    NetOscReceive::~NetOscReceive()
    {
        if (server_ptr_)
            server_ptr_->unsubscribeAll(disp_.id());
    }

    void NetOscReceive::initDone()
    {
        auto p = net::OscServerList::instance().findByName("default");
        if (p != nullptr && p->isValid()) {
            server_ptr_ = p;
            server_ptr_->subscribeMethod(path_->value()->s_name, nullptr, disp_.id(), &pipe_);
            LIB_LOG << fmt::format("subscribed to {} at \"{}\"", path_->value()->s_name, server_ptr_->name());
        }
    }

    bool NetOscReceive::notify(NotifyEventType code)
    {
        switch (code) {
        case NOTIFY_SOURCE_REMOVED:
            OBJ_LOG << "server removed";
            server_ptr_ = nullptr;
            break;
        case NOTIFY_UPDATE: {
            OscMessage msg;
            while (pipe_.try_dequeue(msg)) {
                OBJ_LOG << "new message readed";
            }
        } break;
        default:
            break;
        }
        return true;
    }

}
}

void setup_net_osc_receive()
{
    using namespace ceammc;

    ObjectFactory<net::NetOscReceive> obj("net.osc.receive");
    obj.addAlias("net.osc.r");

    auto osc = net::OscServerList::instance().createUdp("default", 7001);
    if (osc && osc->isValid())
        osc->start();
    else
        LIB_LOG << "can't start server";
}
