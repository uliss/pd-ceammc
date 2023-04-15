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
#include "ceammc_crc32.h"
#include "ceammc_factory.h"
#include "ceammc_output.h"
#include "fmt/core.h"

#include <cstring>

namespace ceammc {

using namespace ceammc::osc;

CEAMMC_DEFINE_HASH(none);

namespace net {

    NetOscReceive::NetOscReceive(const PdArgs& args)
        : DispatchedObject<BaseObject>(args)
        , server_(nullptr)
        , path_(nullptr)
        , types_(nullptr)
    {
        createInlet();
        createOutlet();

        server_ = new SymbolProperty("@server", gensym("default"));
        server_->setArgIndex(1);
        addProperty(server_);

        path_ = new SymbolProperty("@path", &s_);
        path_->setArgIndex(0);
        addProperty(path_);

        types_ = new SymbolProperty("@types", gensym(str_none));
        types_->setArgIndex(2);
        types_->setSymbolCheckFn([this](t_symbol* s) -> bool {
            return validOscTypeString(s->s_name);
        },
            "invalid type string");
        addProperty(types_);

        bindReceive(gensym(OSC_DISPATCHER));
    }

    NetOscReceive::~NetOscReceive()
    {
        auto osc = OscServerList::instance().findByName(server_->value());
        if (osc)
            osc->unsubscribeAll(subscriberId());
    }

    const char* NetOscReceive::types() const
    {
        return (crc32_hash(types_->value()) == hash_none) ? nullptr
                                                          : types_->value()->s_name;
    }

    bool NetOscReceive::subscribe(const OscServerList::OscServerPtr& osc, t_symbol* path)
    {
        if (osc && osc->isValid() && path != &s_) {
            osc->subscribeMethod(path->s_name, types(), subscriberId(),
                [this](const OscRecvMessage& m) -> bool {
                    return pipe_.try_enqueue(m);
                });

            LIB_LOG << fmt::format("[osc] subscribed to {} at \"{}\"", path->s_name, osc->name());
            return true;
        } else if (path != &s_) {
            LIB_LOG << fmt::format("[osc] can't subscribe to {} '{}'", path->s_name, server_->value()->s_name);
            return false;
        } else
            return true;
    }

    bool NetOscReceive::unsubscribe(const OscServerList::OscServerPtr& osc, t_symbol* path)
    {
        if (osc && osc->isValid() && path != &s_) {
            osc->unsubscribeMethod(path->s_name, types(), subscriberId());
            LIB_LOG << fmt::format("[osc] unsubscribed from {} at \"{}\"", path->s_name, osc->name());
            return true;
        } else if (path != &s_) {
            LIB_LOG << fmt::format("[osc] can't unsubscribe from {} '{}'", path->s_name, server_->value()->s_name);
            return false;
        } else
            return true;
    }

    void NetOscReceive::initDone()
    {
        subscribe(OscServerList::instance().findByName(server_->value()), path_->value());

        path_->setSymbolCheckFn([this](t_symbol* new_path) -> bool {
            auto osc = OscServerList::instance().findByName(server_->value());
            if (!unsubscribe(osc, path_->value()))
                return false;

            return subscribe(osc, new_path);
        });
    }

    bool NetOscReceive::notify(int code)
    {
        OscRecvMessage msg;
        while (pipe_.try_dequeue(msg))
            processMessage(msg);

        return true;
    }

    void NetOscReceive::processMessage(const OscRecvMessage& msg)
    {
        AtomList res;
        res.reserve(msg.size());

        OscAtomVisitor v(res);
        for (auto& a : msg.atoms())
            a.apply_visitor(v);

        if (msg.isSpec()) {
            auto spec = boost::get<OscMessageSpec>(msg[0]);
            if (spec == OscMessageSpec::INF)
                anyTo(0, gensym("inf"), AtomListView());
            else if (spec == OscMessageSpec::NIL)
                anyTo(0, gensym("null"), AtomListView());
        } else if (msg.isMidi()) {
            anyTo(0, gensym("midi"), res);
        } else if (msg.isBlob()) {
            anyTo(0, gensym("blob"), res);
        } else
            outletAtomList(outletAt(0), res, true);
    }

    void NetOscReceive::onInlet(size_t n, const AtomListView& lv)
    {
        path_->set(lv);
    }

    void NetOscReceive::updateServer(t_symbol* name, const AtomListView& lv)
    {
        if (lv != server_->value())
            return;

        auto osc = OscServerList::instance().findByName(server_->value());

        if (!unsubscribe(osc, path_->value()))
            return;

        subscribe(osc, path_->value());
    }
}
}

void setup_net_osc_receive()
{
    using namespace ceammc;

    ObjectFactory<net::NetOscReceive> obj("net.osc.receive");
    obj.addAlias("net.osc.r");

    obj.addMethod(OSC_METHOD_UPDATE, &net::NetOscReceive::updateServer);
}
