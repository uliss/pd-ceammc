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
#include "ceammc_osc.h"
#include "ceammc_crc32.h"
#include "ceammc_format.h"
#include "ceammc_pd.h"
#include "ceammc_poll_dispatcher.h"
#include "fmt/core.h"

CEAMMC_DEFINE_STR(none)

#include <memory>
#include <mutex>
#include <type_traits>

#include <lo/lo_cpp.h>

using MutexLock = std::lock_guard<std::mutex>;

namespace {

using AddressPtr = std::unique_ptr<std::remove_pointer<lo_address>::type, typeof(&lo_address_free)>;
inline AddressPtr make_address(lo_address addr)
{
    return AddressPtr(addr, &lo_address_free);
}

using BlobPtr = std::unique_ptr<std::remove_pointer<lo_blob>::type, typeof(&lo_blob_free)>;
inline BlobPtr make_blob(std::int32_t size, const void* data)
{
    return BlobPtr(lo_blob_new(size, data), &lo_blob_free);
}

using ServerPtr = std::unique_ptr<std::remove_pointer<lo_server>::type, typeof(&lo_server_free)>;
inline ServerPtr make_server(lo_server srv)
{
    return ServerPtr(srv, &lo_server_free);
}

const char* to_string(ceammc::osc::OscProto p)
{
    using namespace ceammc::osc;
    switch (p) {
    case OSC_PROTO_UDP:
        return "udp";
    case OSC_PROTO_TCP:
        return "tcp";
    case OSC_PROTO_UNIX:
        return "unix";
    default:
        return "??";
    }
}

}

namespace ceammc {

static inline char atom_arg_type(const Atom& a)
{
    switch (a.type()) {
    case Atom::FLOAT:
        return 'f';
    case Atom::SYMBOL:
        return 's';
    default:
        return '?';
    }
}

namespace osc {
    static int oscProtoToLiblo(OscProto proto)
    {
        switch (proto) {
        case OSC_PROTO_UDP:
            return LO_UDP;
        case OSC_PROTO_TCP:
            return LO_TCP;
        case OSC_PROTO_UNIX:
            return LO_UNIX;
        default:
            return LO_DEFAULT;
        }
    }

    bool validOscTypeString(const char* str)
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

    OscMethodSubscriber::OscMethodSubscriber(SubscriberId id, OscMethodFn fn)
        : id_(id)
        , fn_(fn)
    {
    }

    void OscMethodSubscriber::notify(const char* path, const char* types, lo_arg** argv, int argc)
    {
        if (fn_) {
            OscRecvMessage msg;
            msg.setPath(path);

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
                    atom = static_cast<char>(argv[i]->c);
                    break;
                case LO_BLOB:
                    atom = OscMessageBlob(argv[i]->blob.size, &argv[i]->blob.data);
                    break;
                default:
                    fmt::print("[osc] [{}] unsupported OSC type: '{}'\n", __FUNCTION__, t);
                    break;
                }

                msg.add(atom);
            }

            fn_(msg);
            Dispatcher::instance().send({ id_, 0 });
        }
    }

    OscServerSubscriberList::OscServerSubscriberList()
    {
    }

    OscServerSubscriberList::OscServerSubscriberList(SubscriberId id, OscMethodFn fn)
    {
        MutexLock g(mutex_);
        subscribers_.emplace_front(id, fn);
    }

    void OscServerSubscriberList::notifyAll(const char* path, const char* types, lo_arg** argv, int argc)
    {
        MutexLock g(mutex_);

        for (auto& s : subscribers_)
            s.notify(path, types, argv, argc);
    }

    void OscServerSubscriberList::subscribe(SubscriberId id, OscMethodFn fn)
    {
        MutexLock g(mutex_);

        auto it = std::find_if(
            subscribers_.begin(),
            subscribers_.end(),
            [id](const OscMethodSubscriber& m) { return m.id() == id; });

        if (it != subscribers_.end()) {
            *it = { id, fn };
        } else {
            subscribers_.emplace_front(id, fn);
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

    bool OscServerLogger::notify(int /*event*/)
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
        Dispatcher::instance().send({ id(), 0 });
    }

    void OscServerLogger::print(const char* str)
    {
        err_pipe_.enqueue({ LOG_POST, str });
        Dispatcher::instance().send({ id(), 0 });
    }

    OscServer::OscServer(const char* name, int port, OscProto proto)
        : name_(name)
        , name_hash_(crc32_hash(name_))
        , lo_(nullptr)
    {
        auto lo_proto = oscProtoToLiblo(proto);

        if (port <= 0) {
            // create at free system port
            lo_ = lo_server_thread_new_with_proto(nullptr, lo_proto, errorHandler);
        } else {
            auto str_port = fmt::format("{}", port);
            lo_ = lo_server_thread_new_with_proto(str_port.c_str(), lo_proto, errorHandler);
        }

        OscServerLogger::instance().print(fmt::format("server created: \"{}\" at {}", name_, hostname()).c_str());
    }

    OscServer::OscServer(const char* name, const char* url)
        : name_(name)
        , name_hash_(crc32_hash(name_))
        , lo_(lo_server_thread_new_from_url(url, errorHandler))
    {
        if (lo_)
            OscServerLogger::instance().print(fmt::format("server created: \"{}\" at {}", name_, hostname()).c_str());
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

        LIB_LOG << fmt::format("[osc] server destroyed: \"{}\" at {}", name_, host);
    }

    bool OscServer::start(bool value)
    {
        if (!lo_)
            return false;

        if (value) {
            auto rc = lo_server_thread_start(lo_);
            LIB_LOG << fmt::format("[osc] server started: \"{}\" at {} [{}]", name_, hostname(), rc);
            if (rc == 0) {
                is_running_ = true;
                return true;
            } else
                return false;
        } else {
            auto rc = lo_server_thread_stop(lo_);
            LIB_LOG << fmt::format("[osc] server stopped: \"{}\" at {} [{}]", name_, hostname(), rc);
            if (rc == 0) {
                is_running_ = false;
                return true;
            } else
                return false;
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

    void OscServer::subscribeMethod(const char* path, const char* types, SubscriberId id, OscMethodFn fn)
    {
        const auto hash = crc32_hash(path);
        auto it = subs_.find(hash);

        if (it == subs_.end()) {
            subs_[hash].reset(new OscServerSubscriberList(id, fn));
        } else if (it->second) {
            it->second->subscribe(id, fn);
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

    OscServerList::OscServerPtr OscServerList::findByName(const char* name)
    {
        const auto hash = crc32_hash(name);

        // assuming that list of OSC servers is not long,
        // so doing linear search, it should be faster
        auto it = std::find_if(servers_.begin(), servers_.end(), [hash](const Entry& x) { return x.hash == hash; });
        if (it == servers_.end())
            return {};

        // remove expiried weak pointers
        if (it->wosc.expired()) {
            servers_.erase(it);
            return {};
        } else
            return it->wosc;
    }

    //    OscServerList::OscServerPtr OscServerList::addToList(const OscServerPtr& osc)
    //    {
    //        if (osc && osc->isValid()) {
    //            servers_.push_front({ osc, 0 });

    //            auto res = servers_.front().first;

    //            auto x = gensym(OSC_DISPATCHER);
    //            if (x->s_thing) {
    //                auto s = gensym(OSC_METHOD_UPDATE);
    //                Atom a(gensym(res->name().c_str()));
    //                pd::message_to(x->s_thing, s, a);
    //            }

    //            return res;
    //        } else
    //            return {};
    //    }

    bool OscServerList::registerServer(const char* name, const OscServerPtr& wptr)
    {
        if (wptr.expired())
            return false;

        const auto hash = crc32_hash(name);
        auto ptr = findByName(name);
        if (!ptr.expired()) // already registered with same name
            return false;

        servers_.push_back(Entry { wptr, hash, 0 });

        auto x = gensym(OSC_DISPATCHER);
        if (x->s_thing) {
            auto s = gensym(OSC_METHOD_UPDATE);
            Atom a(gensym(name));
            pd::message_to(x->s_thing, s, a);
        }

        return true;
    }

    bool OscServerList::unregisterServer(const char* name)
    {
        const auto hash = crc32_hash(name);
        auto it = std::find_if(servers_.begin(), servers_.end(), [hash](const Entry& x) { return x.hash == hash; });
        if (it == servers_.end()) {
            return false;
        } else {
            servers_.erase(it);
            return false;
        }
    }

    void OscServerList::dump()
    {
        LIB_POST << "OSC servers:";
        for (auto& s : servers_) {
            if (!s.wosc.expired()) {
                auto osc = s.wosc.lock();
                if (osc)
                    LIB_POST << fmt::format(" - '{}': {} [{}]", osc->name(), osc->hostname(), s.wosc.use_count());
            }
        }
    }

    LoOscMessage::LoOscMessage()
        : message(lo_message_new())
    {
        lo_message_incref(message);
    }

    LoOscMessage::LoOscMessage(const LoOscMessage& m) noexcept
        : message(m.message)
    {
        if (m.message)
            lo_message_incref(m.message);
    }

    LoOscMessage::~LoOscMessage() noexcept
    {
        if (message)
            lo_message_free(message);
    }

    LoOscMessage& LoOscMessage::operator=(const LoOscMessage& m) noexcept
    {
        message = m.message;
        if (message)
            lo_message_incref(message);

        return *this;
    }

    OscSendWorker::OscSendWorker()
        : pipe_(64)
        , quit_(false)
    {
        LIB_LOG << "[osc_send] launch OSC sender worker process";

        future_ = std::async(
            std::launch::async,
            [this]() {
                auto srv_udp = make_server(lo_server_new_with_proto(nullptr, LO_UDP, nullptr));
                if (!srv_udp) {
                    logger_.error("[osc_send] can't create send UDP socket");
                    return false;
                }

                auto srv_tcp = make_server(lo_server_new_with_proto(nullptr, LO_TCP, nullptr));
                if (!srv_tcp) {
                    logger_.error("[osc_send] can't create send TCP socket");
                    return false;
                }

                while (!quit_) {
                    try {
                        SendOscTask task;
                        while (pipe_.try_dequeue(task)) {
                            if (quit_)
                                return true;

                            int rc = -1;

                            auto addr = make_address(nullptr);

                            switch (task.proto) {
                            case ceammc::osc::OSC_PROTO_UDP:
                                addr.reset(lo_address_new_with_proto(LO_UDP,
                                    task.host.c_str(),
                                    fmt::format("{:d}", task.port).c_str()));
                                rc = lo_send_message_from(addr.get(), srv_udp.get(), task.path.c_str(), task.m.get());
                                break;
                            case ceammc::osc::OSC_PROTO_TCP:
                                addr.reset(lo_address_new_with_proto(LO_TCP,
                                    task.host.c_str(),
                                    fmt::format("{:d}", task.port).c_str()));
                                rc = lo_send_message_from(addr.get(), srv_tcp.get(), task.path.c_str(), task.m.get());
                                break;
                            case ceammc::osc::OSC_PROTO_UNIX:
                                addr.reset(lo_address_new_with_proto(LO_UNIX, nullptr, task.host.c_str()));
                                rc = lo_send_message(addr.get(), task.path.c_str(), task.m.get());
                                break;
                            default:
                                logger_.error(fmt::format("[osc_send] unsupported OSC protocol: {}", ::to_string(task.proto)));
                                break;
                            }

                            if (!addr || rc == -1) {
                                auto url = lo_address_get_url(addr.get());
                                logger_.error(fmt::format("[osc_send] {} - `{}`", lo_address_errstr(addr.get()), url));
                                free(url);
                            }
                        }

                        if (!quit_)
                            notify_.waitFor(100);

                    } catch (std::exception& e) {
                        std::cerr << "[osc_send]  exception: " << e.what();
                    }
                }

                return true;
            });
    }

    OscSendWorker::~OscSendWorker()
    {
        quit_ = true;
        if (!future_.valid())
            return;

        auto st = future_.wait_for(std::chrono::milliseconds(10));

        while (st != std::future_status::ready) {
            st = future_.wait_for(std::chrono::milliseconds(100));
            LIB_LOG << "waiting OscSendWorker thread to stop ...";
        }

        LIB_LOG << "[osc_send] OSC sender worker process done";
    }

    bool OscSendWorker::add(const SendOscTask& task)
    {
        auto ok = pipe_.enqueue(task);
        if (ok)
            notify_.notifyOne();

        return ok;
    }

    void OscSendWorker::setPointer(std::shared_ptr<OscSendWorker>& ptr)
    {
        static std::weak_ptr<OscSendWorker> shared;
        if (!shared.expired()) {
            ptr = shared.lock();
        } else {
            ptr.reset(new OscSendWorker);
            shared = ptr;
        }
    }

    void SendOscTask::addBool(bool v)
    {
        if (v)
            lo_message_add_true(m.get());
        else
            lo_message_add_false(m.get());
    }

    void SendOscTask::addChar(char x)
    {
        lo_message_add_char(m.get(), x);
    }

    void SendOscTask::addFloat(float f)
    {
        lo_message_add_float(m.get(), f);
    }

    void SendOscTask::addDouble(double x)
    {
        lo_message_add_double(m.get(), x);
    }

    void SendOscTask::addSymbol(const char* s)
    {
        lo_message_add_string(m.get(), s);
    }

    void SendOscTask::addString(const char* str)
    {
        lo_message_add_string(m.get(), str);
    }

    void SendOscTask::addInt32(std::int32_t v)
    {
        lo_message_add_int32(m.get(), v);
    }

    void SendOscTask::addInt64(std::int64_t v)
    {
        lo_message_add_int64(m.get(), v);
    }

    void SendOscTask::addNil()
    {
        lo_message_add_nil(m.get());
    }

    void SendOscTask::addInf()
    {
        lo_message_add_infinitum(m.get());
    }

    void SendOscTask::addMidi(std::uint8_t data[])
    {
        lo_message_add_midi(m.get(), data);
    }

    void SendOscTask::addBlob(const std::string& data)
    {
        auto b = make_blob(data.size(), data.data());
        lo_message_add_blob(m.get(), b.get());
    }

    bool SendOscTask::addAtom(const Atom& a, char t, std::string& err)
    {
        switch (t) {
        case LO_FLOAT:
            if (!a.isFloat()) {
                err = fmt::format("argument type mismatch: '{}'!='{}'", t, atom_arg_type(a));
                return false;
            } else
                addFloat(a.asT<t_float>());
            break;
        case LO_DOUBLE:
            if (!a.isFloat()) {
                err = fmt::format("argument type mismatch: '{}'!='{}'", t, atom_arg_type(a));
                return false;
            } else
                addDouble(a.asT<t_float>());
            break;
        case LO_INT32:
            if (!a.isFloat()) {
                err = fmt::format("argument type mismatch: '{}'!='{}'", t, atom_arg_type(a));
                return false;
            } else
                addInt32(a.asT<t_float>());
            break;
        case LO_INT64:
            if (!a.isFloat()) {
                err = fmt::format("argument type mismatch: '{}'!='{}'", t, atom_arg_type(a));
                return false;
            } else
                addInt64(a.asT<t_float>());
            break;
        case LO_STRING:
            if (!a.isSymbol())
                addString(to_string(a).c_str());
            else
                addString(a.asT<t_symbol*>()->s_name);
            break;
        case LO_SYMBOL:
            if (!a.isSymbol())
                addSymbol(to_string(a).c_str());
            else
                addSymbol(a.asT<t_symbol*>()->s_name);
            break;
        default:
            err = fmt::format("unknown argument type: '{}'", t);
            return false;
        }

        return true;
    }

    OscAtomVisitor::OscAtomVisitor(AtomList& res)
        : r_(res)
    {
    }

    void OscAtomVisitor::operator()(char c) const
    {
        char buf[2] = { c, '\0' };
        r_.append(gensym(buf));
    }

    void OscAtomVisitor::operator()(const OscMessageBlob& blob)
    {
        for (auto b : blob.data)
            r_.append(static_cast<int>(b));
    }

    void OscAtomVisitor::operator()(const OscMessageMidi& midi)
    {
        for (int i = 0; i < 4; i++)
            r_.append(midi.data[i]);
    }

    void OscAtomVisitor::operator()(OscMessageSpec spec)
    {
        switch (spec) {
        case osc::OscMessageSpec::INF:
            r_.append(gensym("inf"));
            break;
        case osc::OscMessageSpec::NIL:
            r_.append(gensym("null"));
            break;
        default:
            break;
        }
    }

    void dumpVersion()
    {
        char str[16];
        lo_version(str, sizeof(str), 0, 0, 0, 0, 0, 0, 0);
        LIB_DBG << "liblo version: " << str;
    }

}
}
