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
#include "fmt/format.h"

#include <cstring>

namespace ceammc {

CEAMMC_DEFINE_HASH(none);

static bool validOscTypeString(const char* str)
{
    const char* s = str;
    char c;
    while ((c = *s++)) {
        switch (c) {
        case LO_FLOAT:
        case LO_DOUBLE:
        case LO_INT32:
        case LO_INT64:
        case LO_TRUE:
        case LO_FALSE:
        case LO_MIDI:
        case LO_INFINITUM:
        case LO_NIL:
        case LO_CHAR:
        case LO_STRING:
        case LO_SYMBOL:
            continue;
        default:
            return strcmp(str, str_none) == 0;
        }
    }

    return true;
}

class OscAtomVisitor : public boost::static_visitor<> {
    AtomList& r_;

public:
    OscAtomVisitor(AtomList& res)
        : r_(res)
    {
    }

    void operator()(float f) const { r_.append(Atom(f)); }
    void operator()(double d) const { r_.append(Atom(d)); }
    void operator()(bool b) const { r_.append(b ? 1 : 0); }
    void operator()(int32_t i) const { r_.append(i); }
    void operator()(int64_t h) const { r_.append(h); }
    void operator()(const std::string& s) const { r_.append(gensym(s.c_str())); }
    void operator()(char c) const
    {
        char buf[2] = { c, '\0' };
        r_.append(gensym(buf));
    }
    void operator()(net::OscMessageSpec spec)
    {
        switch (spec) {
        case net::OscMessageSpec::INF:
            r_.append(gensym("inf"));
            break;
        case net::OscMessageSpec::NIL:
            r_.append(gensym("null"));
            break;
        default:
            break;
        }
    }
    void operator()(const net::OscMessageMidi& midi)
    {
        for (int i = 0; i < 4; i++)
            r_.append(midi.data[i]);
    }
};

namespace net {

    NetOscReceive::NetOscReceive(const PdArgs& args)
        : BaseObject(args)
        , server_(nullptr)
        , path_(nullptr)
        , types_(nullptr)
        , disp_(this)
    {
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

        bindReceive(gensym(OscServerList::DISPATCHER));
    }

    NetOscReceive::~NetOscReceive()
    {
        auto osc = OscServerList::instance().findByName(server_->value());
        if (osc)
            osc->unsubscribeAll(disp_.id());
    }

    void NetOscReceive::initDone()
    {
        auto osc = net::OscServerList::instance().findByName(server_->value());
        if (osc != nullptr && osc->isValid()) {
            const char* types = (crc32_hash(types_->value()) == hash_none) ? nullptr
                                                                           : types_->value()->s_name;
            osc->subscribeMethod(path_->value()->s_name, types, disp_.id(), &pipe_);
            LIB_LOG << fmt::format("[osc] subscribed to {} at \"{}\"", path_->value()->s_name, osc->name());
        } else
            LIB_LOG << fmt::format("[osc] can't subscribe to {} '{}'", path_->value()->s_name, server_->value()->s_name);
    }

    bool NetOscReceive::notify(NotifyEventType code)
    {
        switch (code) {
        case NOTIFY_UPDATE: {
            OscMessage msg;
            while (pipe_.try_dequeue(msg))
                processMessage(msg);
        } break;
        default:
            break;
        }
        return true;
    }

    void NetOscReceive::processMessage(const OscMessage& msg)
    {
        AtomList res;
        res.reserve(msg.size());

        OscAtomVisitor v(res);
        for (auto& a : msg)
            a.apply_visitor(v);

        if (msg.size() == 1 && msg[0].type() == typeid(OscMessageSpec)) {
            auto spec = boost::get<OscMessageSpec>(msg[0]);
            if (spec == OscMessageSpec::INF)
                anyTo(0, gensym("inf"), AtomListView());
            else if (spec == OscMessageSpec::NIL)
                anyTo(0, gensym("null"), AtomListView());
        } else if (msg.size() == 1 && msg[0].type() == typeid(OscMessageMidi)) {
            anyTo(0, gensym("midi"), res);
        } else
            outletAtomList(outletAt(0), res, true);
    }

    void NetOscReceive::updateServer(t_symbol* name, const AtomListView& lv)
    {
        if (lv != server_->value())
            return;

        auto osc = net::OscServerList::instance().findByName(server_->value());
        if (osc != nullptr && osc->isValid()) {
            const char* types = (crc32_hash(types_->value()) == hash_none) ? nullptr
                                                                           : types_->value()->s_name;
            osc->subscribeMethod(path_->value()->s_name, types, disp_.id(), &pipe_);
        }
    }
}
}

void setup_net_osc_receive()
{
    using namespace ceammc;

    ObjectFactory<net::NetOscReceive> obj("net.osc.receive");
    obj.addAlias("net.osc.r");

    obj.addMethod(net::OscServerList::METHOD_UPDATE, &net::NetOscReceive::updateServer);
}
