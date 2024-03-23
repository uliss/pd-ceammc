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
#ifndef NET_ZEROCONF_H
#define NET_ZEROCONF_H

#include <boost/variant.hpp>
#include <functional>
#include <mutex>
#include <unordered_map>

#include "ceammc_pollthread_spsc.h"
#include "pd_rs.h"
using namespace ceammc;

namespace ceammc {
namespace mdns {
    using TxtPropertyEntry = std::pair<std::string, std::string>;
    using TxtPropertyList = std::vector<TxtPropertyEntry>;
    using IpList = std::vector<std::string>;

    struct MdnsServiceInfo {
        std::string service; // <service>.<domain>
        /// See RFC6763 section 7.1 about "Subtypes":
        /// <https://datatracker.ietf.org/doc/html/rfc6763#section-7.1>
        std::string sub_domain; // <subservice>._sub.<service>.<domain>
        std::string fullname; // <instance>.<service>.<domain>
        std::string hostname; // fully qualified name for service host
        IpList addresses;
        TxtPropertyList props;
        std::uint32_t host_ttl { 0 }; // used for SRV and Address records
        std::uint32_t other_ttl { 0 }; // used for PTR and TXT records
        std::uint16_t port { 0 };
        std::uint16_t priority { 0 };
        std::uint16_t weight { 0 };

        MdnsServiceInfo(const ceammc_mdns_service_info& info);
    };

    namespace req {
        struct Subscribe {
            std::string type;
        };
        struct Unsubscribe {
            std::string type;
        };
        struct EnableIface { };
        struct DisableIface { };

        struct RegisterService {
            std::string name; // displayed instance name: My_TEST_SERVER
            std::string service; // service name: _osc._udp
            std::string host; // my_test_server.local.
            TxtPropertyList props;
            std::uint16_t port;
        };
        struct UnregisterService {
            std::string name; // displayed instance name: My_TEST_SERVER
            std::string service; // service name: _osc._udp
        };
    }

    namespace reply {
        struct ServiceAdded {
            std::string type;
            std::string name;
        };
        struct ServiceRemoved {
            std::string type;
            std::string name;
        };
        struct ServiceResolved {
            MdnsServiceInfo info;
        };
    }

    using Request = boost::variant<
        req::Subscribe,
        req::Unsubscribe,
        req::EnableIface,
        req::DisableIface,
        req::RegisterService,
        req::UnregisterService>;

    using Reply = boost::variant<
        reply::ServiceAdded,
        reply::ServiceRemoved,
        reply::ServiceResolved>;

    struct NetZeroconfImpl {
        std::mutex mtx_;
        ceammc_mdns* mdns_ { nullptr };
        std::function<void(const char* msg)> cb_err;
        std::function<void(const char* type, const char* fullname, bool found)> cb_service;
        std::function<void(const MdnsServiceInfo& info)> cb_resolv;

        NetZeroconfImpl();
        ~NetZeroconfImpl();
        void start();
        void stop();
        void subscribe(const std::string& service);
        void unsubscribe(const std::string& service);
        void process(const req::RegisterService& m);
        void process(const req::UnregisterService& m);
        // blocking call
        void process_events();
    };
}
}

using BaseZeroconf = FixedSPSCObject<mdns::Request, mdns::Reply>;

class NetZeroconf : public BaseZeroconf {
    IntProperty* timeout_ { nullptr };
    ListProperty* ifaces_ { nullptr };
    std::unique_ptr<mdns::NetZeroconfImpl> mdns_;

public:
    NetZeroconf(const PdArgs& args);

    void m_subscribe(t_symbol* s, const AtomListView& lv);
    void m_unsubscribe(t_symbol* s, const AtomListView& lv);
    void m_register(t_symbol* s, const AtomListView& lv);
    void m_unregister(t_symbol* s, const AtomListView& lv);

    void processRequest(const mdns::Request& req, ResultCallback fn) final;
    void processResult(const mdns::Reply& r) final;
    void processEvents() final;

private:
    void processReply(const mdns::reply::ServiceAdded& r);
    void processReply(const mdns::reply::ServiceRemoved& r);
    void processReply(const mdns::reply::ServiceResolved& r);

    template <class T>
    bool processReplyT(const mdns::Reply& r)
    {
        if (r.type() == typeid(T)) {
            this->processReply(boost::get<T>(r));
            return true;
        } else
            return false;
    }
};

void setup_net_zeroconf();

#endif // NET_ZEROCONF_H
