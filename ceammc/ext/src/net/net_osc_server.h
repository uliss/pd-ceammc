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
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include <boost/container/small_vector.hpp>
#include <boost/variant.hpp>
#include <lo/lo.h>

#include "ceammc_poll_dispatcher.h"
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

    using OscMessageAtom = boost::variant<bool, char, int32_t, int64_t, float, double, std::string, OscMessageMidi, OscMessageSpec>;
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

    class OscServer {
        using SubscriberListPtr = std::unique_ptr<OscServerSubscriberList>;
        using MethodSubscriberMap = std::unordered_map<OscMethodHash, SubscriberListPtr>;

    private:
        std::string name_;
        uint32_t name_hash_;
        MethodSubscriberMap subs_;
        lo_server_thread lo_;

    public:
        OscServer(const std::string& name, int port, int proto);
        ~OscServer();

        const std::string& name() const { return name_; }
        uint32_t nameHash() const { return name_hash_; }

        void start();
        void stop();
        bool isValid() const;

        // called from worker thread
        void onMessage(const char* path, const char* types, lo_arg** argv, int argc);

        // called from main thread
        void subscribeMethod(const char* path, const char* types, SubscriberId id, OscMethodPipe* pipe);
        void unsubscribeMethod(const char* path, const char* types, SubscriberId id);
        void unsubscribeAll(SubscriberId id);
    };

    class OscServerList {
        std::forward_list<OscServer> servers_;

        OscServerList()
        {
        }

    public:
        static OscServerList& instance();

        OscServer* findByName(const char* name);

        OscServer* createUdp(const char* name, int port);
    };

}
}

class net_osc_server {
public:
    net_osc_server();
};

#endif // NET_OSC_SERVER_H
