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

#include "ceammc_atomlist.h"
#include "ceammc_datatypes.h"
#include "ceammc_log.h"
#include "ceammc_notify.h"
#include "readerwriterqueue.h"

#include <cstdint>
#include <cstring>
#include <forward_list>
#include <future>
#include <mutex>
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
        OSC_PROTO_UDP = 0,
        OSC_PROTO_TCP = 1,
        OSC_PROTO_UNIX = 2,
        OSC_PROTO_DEFAULT = OSC_PROTO_UDP,
        OSC_PROTO_INVALID = 4,
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
    using OscMethodHash = std::uint32_t;

    class OscRecvMessage {
    public:
        using OscAtomList = boost::container::small_vector<OscMessageAtom, 4>;

    public:
        void add(const OscMessageAtom& a) { atoms_.push_back(a); }
        size_t size() const { return atoms_.size(); }
        const OscAtomList& atoms() const { return atoms_; }
        bool isMidi() const { return atoms_.size() == 1 && atoms_[0].type() == typeid(OscMessageMidi); }
        bool isBlob() const { return atoms_.size() == 1 && atoms_[0].type() == typeid(OscMessageBlob); }
        bool isSpec() const { return atoms_.size() == 1 && atoms_[0].type() == typeid(OscMessageSpec); }
        const OscMessageAtom& operator[](size_t n) const { return atoms_[n]; }
        void setPath(const char* path) { path_ = path; }
        const std::string& path() const { return path_; }

    private:
        std::string path_;
        boost::container::small_vector<OscMessageAtom, 4> atoms_;
    };

    class OscMethodPipe {
        moodycamel::ReaderWriterQueue<OscRecvMessage> pipe_;

    public:
        bool try_enqueue(const OscRecvMessage& m)
        {
            return pipe_.try_enqueue(m);
        }

        bool try_enqueue(OscRecvMessage&& m)
        {
            return pipe_.try_enqueue(std::move(m));
        }

        bool try_dequeue(OscRecvMessage& m)
        {
            return pipe_.try_dequeue(m);
        }
    };

    using OscMethodFn = std::function<bool(const OscRecvMessage&)>;

    class OscAtomVisitor : public boost::static_visitor<> {
        AtomList& r_;

    public:
        OscAtomVisitor(AtomList& res);

        void operator()(float f) const { r_.append(Atom(f)); }
        void operator()(double d) const { r_.append(Atom(d)); }
        void operator()(bool b) const { r_.append(b ? 1 : 0); }
        void operator()(int32_t i) const { r_.append(i); }
        void operator()(int64_t h) const { r_.append(h); }
        void operator()(const std::string& s) const { r_.append(gensym(s.c_str())); }
        void operator()(char c) const;
        void operator()(OscMessageSpec spec);
        void operator()(const OscMessageMidi& midi);
        void operator()(const OscMessageBlob& blob);
    };

    class OscMethodSubscriber {
        SubscriberId id_;
        OscMethodFn fn_;

    public:
        OscMethodSubscriber(SubscriberId id, OscMethodFn fn);

        SubscriberId id() const { return id_; }

#ifdef WITH_LIBLO
        /**
         * notify all method subscribers
         * @note called from worker thread
         */
        void notify(const char* path, const char* types, lo_arg** argv, int argc);
#endif
    };

    class LoOscMessage {
#ifdef WITH_LIBLO
        lo_message message;
#endif

    public:
        LoOscMessage();

        LoOscMessage(const LoOscMessage& m) noexcept;

        ~LoOscMessage() noexcept;

        LoOscMessage& operator=(const LoOscMessage& m) noexcept;

#ifdef WITH_LIBLO
        lo_message get() const noexcept
        {
            return message;
        }
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
        OscServerSubscriberList(SubscriberId id, OscMethodFn fn);

#ifdef WITH_LIBLO
        /**
         * Send to to all subscribers
         * @note called from worker thread
         */
        void notifyAll(const char* path, const char* types, lo_arg** argv, int argc);
#endif

        void subscribe(SubscriberId id, OscMethodFn fn);
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
        void subscribeMethod(const char* path, const char* types, SubscriberId id, OscMethodFn fn);
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
    };

    struct SendOscTask {
        std::string host;
        std::string path;
        LoOscMessage m;
        SubscriberId id;
        uint16_t port;
        OscProto proto;

        void addBool(bool v);
        void addChar(char x);
        void addFloat(float f);
        void addDouble(double x);
        void addSymbol(const char* s);
        void addString(const char* str);
        void addInt32(std::int32_t v);
        void addInt64(std::int64_t v);
        void addNil();
        void addInf();
        void addMidi(std::uint8_t data[4]);
        void addBlob(const std::string& data);

        bool addAtom(const Atom& a, char t, std::string& err);
    };

    class OscSendWorker {
        using Pipe = moodycamel::ReaderWriterQueue<SendOscTask, 64>;
        ThreadPdLogger logger_;

        OscSendWorker();
        ~OscSendWorker();

        Pipe pipe_;
        std::future<bool> future_;
        std::atomic_bool quit_;
        ThreadNotify notify_;

    public:
        static OscSendWorker& instance();
        bool add(const SendOscTask& task);
    };

    bool validOscTypeString(const char* str);
    void dumpVersion();
}
}
#endif // CEAMMC_OSC_H
