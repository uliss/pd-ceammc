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
#ifndef CEAMMC_OSC_H
#define CEAMMC_OSC_H

#include "ceammc_datatypes.h"
#include "ceammc_log.h"
#include "ceammc_poll_dispatcher.h"
#include "readerwriterqueue.h"

#include <cstdint>
#include <cstring>
#include <forward_list>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <boost/container/small_vector.hpp>
#include <boost/variant.hpp>

#ifdef WITH_LIBLO
#include <lo/lo.h>
#endif

namespace ceammc {
namespace osc {

    enum OscProto : uint8_t {
        OSC_PROTO_UDP,
        OSC_PROTO_TCP,
        OSC_PROTO_UNIX,
        OSC_PROTO_DEFAULT = OSC_PROTO_UDP,
        OSC_PROTO_INVALID,
    };

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

#ifdef WITH_LIBLO
        /**
         * notify all method subscribers
         * @note called from worker thread
         */
        void notify(const char* types, lo_arg** argv, int argc);
#endif
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

#ifdef WITH_LIBLO
        /**
         * Send to to all subscribers
         * @note called from worker thread
         */
        void notifyAll(const char* path, const char* types, lo_arg** argv, int argc);
#endif

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

        bool notify(int event) final;
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
#ifdef WITH_LIBLO
        lo_server_thread lo_;
#endif

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

#ifdef WITH_LIBLO
        // called from worker thread
        void onMessage(const char* path, const char* types, lo_arg** argv, int argc);
#endif

        // called from main thread
        void subscribeMethod(const char* path, const char* types, SubscriberId id, OscMethodPipe* pipe);
        void unsubscribeMethod(const char* path, const char* types, SubscriberId id);
        void unsubscribeAll(SubscriberId id);

        std::string hostname() const;
        int port() const;

        void setDumpAll(bool value);

    private:
        static void errorHandler(int num, const char* msg, const char* where);

#ifdef WITH_LIBLO
        static int logHandler(const char* path, const char* types, lo_arg** argv, int argc, void* data, void* user_data);
#endif
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

    bool validOscTypeString(const char* str);
}
}
#endif // CEAMMC_OSC_H
