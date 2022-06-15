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
#include "net_osc_server.h"
#include "ceammc_crc32.h"
#include "ceammc_factory.h"
#include "ceammc_format.h"
#include "fmt/format.h"

#include <algorithm>
#include <unordered_set>

using MutexLock = std::lock_guard<std::mutex>;

namespace ceammc {
namespace net {

    OscMethodSubscriber::OscMethodSubscriber(SubscriberId id, OscMethodPipe* pipe)
        : id_(id)
        , pipe_(pipe)
    {
    }

    void OscMethodSubscriber::notify(const char* types, lo_arg** argv, int argc)
    {
        if (pipe_) {
            OscMessage msg;
            for (int i = 0; i < argc; i++) {
                OscMessageAtom atom;
                const auto t = types[i];

                switch (t) {
                case LO_FLOAT:
                    atom = argv[i]->f;
                    break;
                case LO_DOUBLE:
                    atom = argv[i]->d;
                    break;
                case LO_TRUE:
                    atom = true;
                    break;
                case LO_FALSE:
                    atom = false;
                    break;
                case LO_INT32:
                    atom = argv[i]->i32;
                    break;
                case LO_INT64:
                    atom = argv[i]->i64;
                    break;
                case LO_STRING:
                    atom = std::string(&argv[i]->s);
                    break;
                case LO_SYMBOL:
                    atom = std::string(&argv[i]->S);
                    break;
                case LO_MIDI:
                    atom = OscMessageMidi { argv[i]->m };
                    break;
                case LO_NIL:
                    atom = OscMessageSpec::NIL;
                    break;
                case LO_INFINITUM:
                    atom = OscMessageSpec::INF;
                    break;
                case LO_CHAR:
                    atom = argv[i]->c;
                    break;
                }

                msg.push_back(atom);
            }

            pipe_->try_enqueue(msg);
        }

        Dispatcher::instance().send({ id_, NOTIFY_UPDATE });
    }

    OscServerSubscriberList::OscServerSubscriberList()
    {
    }

    OscServerSubscriberList::OscServerSubscriberList(SubscriberId id, OscMethodPipe* pipe)
    {
        MutexLock g(mutex_);
        subscribers_.emplace_front(id, pipe);
    }

    void OscServerSubscriberList::notifyAll(const char* path, const char* types, lo_arg** argv, int argc)
    {
        MutexLock g(mutex_);

        for (auto& s : subscribers_)
            s.notify(types, argv, argc);
    }

    void OscServerSubscriberList::subscribe(SubscriberId id, OscMethodPipe* pipe)
    {
        MutexLock g(mutex_);

        auto it = std::find_if(
            subscribers_.begin(),
            subscribers_.end(),
            [id](const OscMethodSubscriber& m) { return m.id() == id; });

        if (it != subscribers_.end()) {
            *it = { id, pipe };
        } else {
            subscribers_.emplace_front(id, pipe);
        }
    }

    void OscServerSubscriberList::unsubscribe(SubscriberId id)
    {
        MutexLock g(mutex_);

        subscribers_.remove_if([id](const OscMethodSubscriber& m) { return m.id() == id; });
    }

    void OscServerSubscriberList::getSubscribers(std::unordered_set<SubscriberId>& s)
    {
        MutexLock g(mutex_);

        for (auto& x : subscribers_)
            s.insert(x.id());
    }

    static const char* protoToStr(int proto)
    {
        switch (proto) {
        case LO_UDP:
            return "udp";
        case LO_TCP:
            return "tcp";
        case LO_UNIX:
            return "unix";
        default:
            return "??";
        }
    }

    OscServerLogger::OscServerLogger()
    {
        Dispatcher::instance().subscribe(this, id());
    }

    OscServerLogger::~OscServerLogger()
    {
        Dispatcher::instance().unsubscribe(this);
    }

    OscServerLogger& OscServerLogger::instance()
    {
        static OscServerLogger log;
        return log;
    }

    bool OscServerLogger::notify(NotifyEventType /*code*/)
    {
        LogEntry log_msg;

        while (err_pipe_.try_dequeue(log_msg)) {
            switch (log_msg.first) {
            case LOG_POST:
                LIB_POST << "[osc] " << log_msg.second.c_str();
                break;
            case LOG_ERROR:
            default:
                LIB_ERR << "[osc error] " << log_msg.second.c_str();
                break;
            }
        }

        return true;
    }

    void OscServerLogger::error(int errNo, const char* msg, const char* where)
    {
        LogString str = (where == nullptr)
            ? fmt::format("error #{}: '{}'", errNo, msg).c_str()
            : fmt::format("error #{}: '{}' at {}", errNo, msg, where).c_str();
        err_pipe_.enqueue({ LOG_ERROR, str });
        Dispatcher::instance().send({ id(), NOTIFY_UPDATE });
    }

    void OscServerLogger::print(const char* str)
    {
        err_pipe_.enqueue({ LOG_POST, str });
        Dispatcher::instance().send({ id(), NOTIFY_UPDATE });
    }

    OscServer::OscServer(const char* name, int port)
        : name_(name)
        , name_hash_(crc32_hash(name_))
        , lo_(nullptr)
    {
        if (port <= 0) {
            // create at free system port
            lo_ = lo_server_thread_new(nullptr, errorHandler);
        } else
            lo_ = lo_server_thread_new(fmt::format("{}", port).c_str(), errorHandler);

        if (lo_)
            OscServerLogger::instance().print(fmt::format("Server created: \"{}\" at {}", name_, hostname()).c_str());
    }

    OscServer::OscServer(const char* name, const char* url)
        : name_(name)
        , name_hash_(crc32_hash(name_))
        , lo_(lo_server_thread_new_from_url(url, errorHandler))
    {
        if (lo_)
            OscServerLogger::instance().print(fmt::format("Server created: \"{}\" at {}", name_, hostname()).c_str());
    }

    OscServer::OscServer(OscServer&& srv)
        : name_(std::move(srv.name_))
        , name_hash_(std::move(srv.name_hash_))
        , lo_(std::move(srv.lo_))
    {
    }

    OscServer::~OscServer()
    {
        const auto host = hostname();

        if (lo_) {
            lo_server_thread_free(lo_);
            lo_ = nullptr;
        }

        LIB_LOG << fmt::format("[osc] Server destroyed: \"{}\" at {}", name_, host);
    }

    void OscServer::start(bool value)
    {
        if (!lo_)
            return;

        if (value) {
            auto rc = lo_server_thread_start(lo_);
            LIB_LOG << fmt::format("OSC server started: \"{}\" [{}]", name_, rc);
        } else {
            auto rc = lo_server_thread_stop(lo_);
            LIB_LOG << fmt::format("OSC server stopped: \"{}\" [{}]", name_, rc);
        }
    }

    bool OscServer::isValid() const
    {
        return lo_ != nullptr;
    }

    void OscServer::onMessage(const char* path, const char* types, lo_arg** argv, int argc)
    {
        const auto hash = crc32_hash(path);

        auto it = subs_.find(hash);
        if (it == subs_.end())
            return;

        if (it->second)
            it->second->notifyAll(path, types, argv, argc);
    }

    void OscServer::unsubscribeMethod(const char* path, const char* types, SubscriberId id)
    {
        if (lo_)
            lo_server_thread_del_method(lo_, path, types);

        const auto hash = crc32_hash(path);
        auto it = subs_.find(hash);

        if (it == subs_.end())
            return;

        if (it->second)
            it->second->unsubscribe(id);
    }

    void OscServer::unsubscribeAll(SubscriberId id)
    {
        for (auto& s : subs_) {
            if (s.second)
                s.second->unsubscribe(id);
        }
    }

    std::string OscServer::hostname() const
    {
        if (lo_) {
            auto url = lo_server_thread_get_url(lo_);
            std::string res(url);
            free(url);
            return res;
        } else
            return {};
    }

    int OscServer::port() const
    {
        if (lo_)
            return lo_server_thread_get_port(lo_);
        else
            return -1;
    }

    void OscServer::setDumpAll(bool value)
    {
        if (!lo_)
            return;

        if (value)
            lo_server_thread_add_method(lo_, nullptr, nullptr, &logHandler, nullptr);
        else
            lo_server_thread_del_method(lo_, nullptr, nullptr);
    }

    void OscServer::errorHandler(int num, const char* msg, const char* where)
    {
        OscServerLogger::instance().error(num, msg, where);
    }

    int OscServer::logHandler(const char* path, const char* types, lo_arg** argv,
        int argc, void* data, void* user_data)
    {
        std::string str = fmt::format("{}", path);
        for (int i = 0; i < argc; i++) {
            auto a = argv[i];
            switch (types[i]) {
            case LO_FLOAT:
                str += fmt::format(" {}_f", a->f);
                break;
            case LO_DOUBLE:
                str += fmt::format(" {}_d", a->d);
                break;
            case LO_STRING:
                str += fmt::format(" \"{}\"", &a->s);
                break;
            case LO_CHAR:
                str += fmt::format(" '{}'", a->c);
                break;
            case LO_SYMBOL:
                str += fmt::format(" \"{}\"", &a->S);
                break;
            case LO_INT32:
                str += fmt::format(" {}_i", a->i);
                break;
            case LO_INT64:
                str += fmt::format(" {}_l", a->h);
                break;
            case LO_TRUE:
                str += " #true";
                break;
            case LO_FALSE:
                str += " #false";
                break;
            case LO_INFINITUM:
                str += " #inf";
                break;
            case LO_NIL:
                str += " #null";
                break;
            case LO_MIDI:
                str += fmt::format(" midi({:02X} {:02X} {:02X} {:02X})", a->m[0], a->m[1], a->m[2], a->m[3]);
                break;
            default:
                str += " ??";
                break;
            }
        }

        OscServerLogger::instance().print(str.c_str());

        return 1;
    }

    void OscServer::subscribeMethod(const char* path, const char* types, SubscriberId id, OscMethodPipe* pipe)
    {
        const auto hash = crc32_hash(path);
        auto it = subs_.find(hash);

        if (it == subs_.end()) {
            subs_.insert(std::make_pair(hash, new OscServerSubscriberList(id, pipe)));
        } else if (it->second) {
            it->second->subscribe(id, pipe);
        }

        if (lo_) {
            auto fn = [](const char* path, const char* types, lo_arg** argv, int argc, void* /*data*/, void* user_data) -> int {
                OscServer* s = static_cast<OscServer*>(user_data);
                if (s)
                    s->onMessage(path, types, argv, argc);
                return 0;
            };

            lo_server_thread_add_method(lo_, path, types, fn, this);
        }
    }

    OscServerList::OscServerList()
    {
    }

    OscServerList& OscServerList::instance()
    {
        static OscServerList m;
        return m;
    }

    OscServer* OscServerList::findByName(const char* name)
    {
        const auto hash = crc32_hash(name);

        // assuming that list of OSC servers if not long,
        // so doing linear search
        for (auto& s : servers_) {
            if (s.first && s.first->nameHash() == hash)
                return s.first.get();
        }

        return nullptr;
    }

    OscServer* OscServerList::addToList(OscServerPtr&& osc)
    {
        if (osc && osc->isValid()) {
            servers_.push_front({ std::move(osc), 0 });

            auto res = servers_.front().first.get();

            auto x = gensym(DISPATCHER);
            auto s = gensym(METHOD_UPDATE);
            if (x->s_thing) {
                t_atom a;
                SETSYMBOL(&a, gensym(res->name().c_str()));
                pd_typedmess(x->s_thing, s, 1, &a);
            }

            return res;
        } else
            return nullptr;
    }

    OscServer* OscServerList::createByUrl(const char* name, const char* url)
    {
        if (findByName(name)) {
            LIB_ERR << fmt::format("server already exists: \"{}\"", name);
            return nullptr;
        }

        return addToList(OscServerPtr { new OscServer(name, url) });
    }

    OscServer* OscServerList::createByPort(const char* name, int port)
    {
        if (findByName(name)) {
            LIB_ERR << fmt::format("server already exists: \"{}\"", name);
            return nullptr;
        }

        return addToList(OscServerPtr { new OscServer(name, port) });
    }

    void OscServerList::start(const char* name, bool value)
    {
        auto osc = findByName(name);
        if (osc)
            osc->start(value);
    }

    void OscServerList::addRef(const char* name)
    {
        const auto hash = crc32_hash(name);
        for (auto& s : servers_) {
            if (s.first->nameHash() == hash) {
                s.second++;
                break;
            }
        }
    }

    void OscServerList::unRef(const char* name)
    {
        const auto hash = crc32_hash(name);
        for (auto it = servers_.begin(); it != servers_.end(); ++it) {
            if (it->first->nameHash() == hash) {
                if (--(it->second) <= 0)
                    servers_.erase(it);

                return;
            }
        }
    }

    NetOscServer::NetOscServer(const PdArgs& args)
        : BaseObject(args)
        , name_(nullptr)
        , url_(nullptr)
        , dump_(nullptr)
    {
        createOutlet();

        name_ = new SymbolProperty("@name", gensym("default"), PropValueAccess::INITONLY);
        name_->setArgIndex(0);
        addProperty(name_);

        url_ = new OscUrlProperty("@url", &s_, PropValueAccess::INITONLY);
        url_->setArgIndex(1);
        addProperty(url_);

        dump_ = new BoolProperty("@dump", false);
        dump_->setSuccessFn([this](Property*) {
            auto osc = OscServerList::instance().findByName(name_->value());
            if (osc)
                osc->setDumpAll(dump_->value());
        });
        addProperty(dump_);
    }

    NetOscServer::~NetOscServer()
    {
        OscServerList::instance().unRef(name_->value()->s_name);
    }

    void NetOscServer::initDone()
    {
        auto name = name_->value()->s_name;
        auto url = url_->value();

        auto& srv_list = OscServerList::instance();

        auto osc = srv_list.findByName(name);
        if (osc == nullptr) {
            t_float port = 0;
            t_symbol* str_url = &s_;

            if (url.getFloat(&port))
                osc = srv_list.createByPort(name, port);
            else if (url.getSymbol(&str_url))
                osc = srv_list.createByUrl(name, str_url->s_name);
            else
                osc = srv_list.createByPort(name, 0);
        }

        if (!osc || !osc->isValid())
            LIB_ERR << fmt::format("can't create server '{}': {}", name, to_string(url));
        else {
            osc->setDumpAll(dump_->value());
            srv_list.addRef(name);
        }
    }

    void NetOscServer::m_start(t_symbol* s, const AtomListView& lv)
    {
        bool value = false;

        if (lv.empty())
            value = true;
        else if (lv.isBool())
            value = lv[0].asT<bool>();
        else {
            METHOD_ERR(s) << "bool value expected, got: " << lv;
            return;
        }

        OscServerList::instance().start(name_->value()->s_name, value);
    }

    void NetOscServer::m_stop(t_symbol* s, const AtomListView& lv)
    {
        bool value = false;

        if (lv.empty())
            value = true;
        else if (lv.isBool())
            value = lv[0].asT<bool>();
        else {
            METHOD_ERR(s) << "bool value expected, got: " << lv;
            return;
        }

        OscServerList::instance().start(name_->value()->s_name, !value);
    }

    void NetOscServer::dump() const
    {
        BaseObject::dump();
    }

    void OscUrlProperty::parseUrl(const Atom& url)
    {
        if (url.isSymbol()) {
            auto s = url.asT<t_symbol*>()->s_name;
            auto host = lo_url_get_hostname(s);
            if (host) {
                host_ = gensym(host);
                free(host);
            } else
                host_ = &s_;

            auto port = lo_url_get_port(s);
            if (port) {
                port_ = gensym(port);
                free(port);
            } else
                port_ = &s_;

            auto proto = lo_url_get_protocol(s);
            if (proto) {
                proto_ = gensym(proto);
                free(proto);
            } else
                proto_ = &s_;

            if(port_ == host_)
                port_ = &s_;
        } else if (url.isInteger()) {
            constexpr int MIN_PORT = 1024;
            constexpr int MAX_PORT = std::numeric_limits<std::int16_t>::max();
            const int port = url.asT<int>();
            if (port <= MIN_PORT || port > MAX_PORT) {
                LIB_ERR << fmt::format("[@{}] invalid port value: {}, expected to be in {}..{} range", name()->s_name, port, MIN_PORT, MAX_PORT);
                return;
            }

            port_ = gensym(fmt::format("{:d}", port).c_str());
            proto_ = &s_;
            host_ = &s_;
        } else
            LIB_ERR << "OSC url or port number expected";
    }

    OscUrlProperty::OscUrlProperty(const std::string& name, t_symbol* def, PropValueAccess ro)
        : AtomProperty(name, def, ro)
        , host_(&s_)
        , port_(&s_)
        , proto_(&s_)
    {
        parseUrl(def);
        setSuccessFn([this](Property*) { parseUrl(value()); });
    }
}
}

void setup_net_osc_server()
{
    using namespace ceammc;

    ObjectFactory<net::NetOscServer> obj("net.osc.server");
    obj.addAlias("net.osc");

    obj.parseArgsMode(PdArgs::PARSE_UNQUOTE);
    obj.parsePropsMode(PdArgs::PARSE_UNQUOTE);

    obj.addMethod("start", &net::NetOscServer::m_start);
    obj.addMethod("stop", &net::NetOscServer::m_stop);
}
