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
#ifndef NET_OSC_SERVER_H
#define NET_OSC_SERVER_H

#include <cstdint>
#include <cstring>
#include <forward_list>
#include <list>
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include <boost/container/small_vector.hpp>
#include <boost/variant.hpp>
#include <lo/lo.h>

#include "ceammc_datatypes.h"
#include "ceammc_log.h"
#include "ceammc_object.h"
#include "ceammc_poll_dispatcher.h"
#include "ceammc_property.h"
#include "osc_property.h"
#include "readerwriterqueue.h"

namespace ceammc {
namespace net {

    enum class OscMessageSpec {
        NIL,
        INF
    };

    struct OscMessageMidi {
        uint8_t data[4];
        OscMessageMidi(const uint8_t* p)
        {
            memcpy(data, p, 4);
        }
    };

    struct OscMessageBlob {
        std::vector<char> data;

        OscMessageBlob(size_t size, const char* p)
            : data(size)
        {
            if (data.size() == size)
                memcpy(data.data(), p, size);
        }
    };

    using OscMessageAtom = boost::variant<bool, char, int32_t, int64_t, float, double, std::string, OscMessageMidi, OscMessageSpec, OscMessageBlob>;
    using OscMessage = boost::container::small_vector<OscMessageAtom, 8>;
    using OscMethodPipe = moodycamel::ReaderWriterQueue<OscMessage>;
    using OscMethodHash = std::uint32_t;

    class OscMethodSubscriber {
        SubscriberId id_;
        OscMethodPipe* pipe_;

    public:
        OscMethodSubscriber(SubscriberId id, OscMethodPipe* pipe);

        SubscriberId id() const { return id_; }
        OscMethodPipe* pipe() { return pipe_; }

        /**
         * notify all method subscribers
         * @note called from worker thread
         */
        void notify(const char* types, lo_arg** argv, int argc);
    };

    class OscServerSubscriberList {
    public:
        using Container = std::forward_list<OscMethodSubscriber>;

    private:
        Container subscribers_;
        std::mutex mutex_;

    public:
        OscServerSubscriberList();
        OscServerSubscriberList(SubscriberId id, OscMethodPipe* pipe);

        /**
         * Send to to all subscribers
         * @note called from worker thread
         */
        void notifyAll(const char* path, const char* types, lo_arg** argv, int argc);

        void subscribe(SubscriberId id, OscMethodPipe* pipe);
        void unsubscribe(SubscriberId id);

        void getSubscribers(std::unordered_set<SubscriberId>& s);
    };

    class OscServerLogger : public NotifiedObject {
        using LogString = BoostStaticString<80>;
        using LogEntry = std::pair<LogLevel, LogString>;
        using LogPipe = moodycamel::ReaderWriterQueue<LogEntry>;

        LogPipe err_pipe_;

        OscServerLogger();
        ~OscServerLogger();

        OscServerLogger(const OscServerLogger&) = delete;
        OscServerLogger(OscServerLogger&&) = delete;
        OscServerLogger& operator=(const OscServerLogger&) = delete;
        OscServerLogger& operator=(OscServerLogger&&) = delete;

    public:
        static OscServerLogger& instance();

        bool notify(NotifyEventType code) final;
        SubscriberId id() const { return reinterpret_cast<SubscriberId>(this); }
        void error(int errNo, const char* msg, const char* where);
        void print(const char* str);
    };

    class OscServer {
        using SubscriberListPtr = std::unique_ptr<OscServerSubscriberList>;
        using MethodSubscriberMap = std::unordered_map<OscMethodHash, SubscriberListPtr>;

    private:
        std::string name_;
        uint32_t name_hash_;
        MethodSubscriberMap subs_;
        lo_server_thread lo_;

        OscServer(const OscServer&) = delete;
        OscServer& operator=(const OscServer&) = delete;

    public:
        OscServer(const char* name, int port, OscProto proto = OSC_PROTO_UDP);
        OscServer(const char* name, const char* url);
        OscServer(OscServer&& srv);
        ~OscServer();

        const std::string& name() const { return name_; }
        uint32_t nameHash() const { return name_hash_; }

        void start(bool value);
        bool isValid() const;

        // called from worker thread
        void onMessage(const char* path, const char* types, lo_arg** argv, int argc);

        // called from main thread
        void subscribeMethod(const char* path, const char* types, SubscriberId id, OscMethodPipe* pipe);
        void unsubscribeMethod(const char* path, const char* types, SubscriberId id);
        void unsubscribeAll(SubscriberId id);

        std::string hostname() const;
        int port() const;

        void setDumpAll(bool value);

    private:
        static void errorHandler(int num, const char* msg, const char* where);
        static int logHandler(const char* path, const char* types, lo_arg** argv, int argc, void* data, void* user_data);
    };

    class OscServerList {
    public:
        using OscServerPtr = std::shared_ptr<OscServer>;
        using Entry = std::pair<OscServerPtr, int>;

    private:
        std::list<Entry> servers_;

        OscServerList();

    public:
        static OscServerList& instance();

        OscServerPtr findByName(t_symbol* name) { return findByName(name->s_name); }
        OscServerPtr findByName(const char* name);

        OscServerPtr createByUrl(const char* name, const char* url);
        OscServerPtr createByPortProto(const char* name, OscProto proto, int port);

        void start(const char* name, bool value);

        void addRef(const char* name);
        void unRef(const char* name);

        void dump();

    private:
        OscServerPtr addToList(const OscServerPtr& osc);

    public:
        static constexpr const char* DISPATCHER = "#osc";
        static constexpr const char* METHOD_UPDATE = ".update";
    };

    class NetOscServer : public BaseObject {
        SymbolProperty* name_;
        OscUrlProperty* url_;
        BoolProperty* dump_;
        BoolProperty* auto_start_;
        std::weak_ptr<OscServer> server_;

    public:
        NetOscServer(const PdArgs& args);
        ~NetOscServer();

        void dump() const override;

        void initDone() final;
        void m_start(t_symbol* s, const AtomListView& lv);
        void m_stop(t_symbol* s, const AtomListView& lv);
    };
}
}

void setup_net_osc_server();

#endif // NET_OSC_SERVER_H
