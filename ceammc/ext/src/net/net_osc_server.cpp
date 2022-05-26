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
#include "fmt/format.h"

#include <algorithm>
#include <unordered_set>

using MutexLock = std::lock_guard<std::mutex>;

namespace ceammc {
namespace net {

    int generic_handler(const char* path, const char* types, lo_arg** argv,
        int argc, void* data, void* user_data)
    {
        printf("path: <%s>\n", path);
        for (int i = 0; i < argc; i++) {
            printf("arg %d '%c' ", i, types[i]);
            lo_arg_pp((lo_type)types[i], argv[i]);
            printf("\n");
        }
        printf("\n");
        fflush(stdout);

        return 1;
    }

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
                    atom = &argv[i]->s;
                    break;
                case LO_SYMBOL:
                    atom = &argv[i]->S;
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

        std::remove_if(
            subscribers_.begin(),
            subscribers_.end(),
            [id](const OscMethodSubscriber& m) { return m.id() == id; });
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

    OscServer::OscServer(const std::string& name, int port, int proto)
        : name_(name)
        , name_hash_(crc32_hash(name_))
        , lo_(lo_server_thread_new_with_proto(fmt::format("{}", port).c_str(), proto,
              [](int num, const char* msg, const char* where) {
                  std::cerr << fmt::format("[osc error] {}\n", msg);
              }))
    {
        LIB_LOG << fmt::format("OSC server created: \"{}\" {}://{}", name_, protoToStr(proto), port);

        if (lo_)
            lo_server_thread_add_method(lo_, NULL, NULL, generic_handler, NULL);
    }

    OscServer::~OscServer()
    {
        if (lo_)
            lo_server_thread_free(lo_);

        std::unordered_set<SubscriberId> id_set;

        for (auto& s : subs_)
            s.second->getSubscribers(id_set);

        subs_.clear();

        for (auto id : id_set)
            Dispatcher::instance().send({ id, NOTIFY_DONE });

        LIB_LOG << fmt::format("OSC server destroyed: \"{}\"", name_);
    }

    void OscServer::start()
    {
        if (!lo_)
            return;

        auto rc = lo_server_thread_start(lo_);
        LIB_LOG << fmt::format("OSC server started: \"{}\" [{}]", name_, rc);
    }

    void OscServer::stop()
    {
        if (!lo_)
            return;

        auto rc = lo_server_thread_stop(lo_);
        LIB_LOG << fmt::format("OSC server stopped: \"{}\"", name_);
    }

    bool OscServer::isValid() const
    {
        return lo_ != nullptr;
    }

    void OscServer::onMessage(const char* path, const char* types, lo_arg** argv, int argc)
    {
        const auto hash = crc32_hash(path);

        auto it = subs_.find(hash);
        if (it == subs_.end()) {
            std::cerr << "not found: " << path << "\n";
            return;
        }

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
            auto fn = [](const char* path, const char* types, lo_arg** argv, int argc, void* data, void* user_data) -> int {
                OscServer* s = static_cast<OscServer*>(user_data);
                if (s)
                    s->onMessage(path, types, argv, argc);
                return 0;
            };

            lo_server_thread_add_method(lo_, path, types, fn, this);
        }
    }

    OscServerList& OscServerList::instance()
    {
        static OscServerList m;
        return m;
    }

    OscServer* OscServerList::findByName(const char* name)
    {
        const auto hash = crc32_hash(name);

        for (auto& s : servers_) {
            if (s.nameHash() == hash)
                return &s;
        }

        return nullptr;
    }

    OscServer* OscServerList::createUdp(const char* name, int port)
    {
        const auto hash = crc32_hash(name);
        for (auto& s : servers_) {
            if (s.nameHash() == hash) {
                LIB_ERR << fmt::format("server already exists: \"{}\"", name);
                return nullptr;
            }
        }

        servers_.emplace_front(name, port, LO_UDP);
        return &servers_.front();
    }

}

}
