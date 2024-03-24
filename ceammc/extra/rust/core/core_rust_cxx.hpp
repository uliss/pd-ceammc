#ifndef ceammc_rust_core_fwd_h
#define ceammc_rust_core_fwd_h

#include "core_rust.hpp"

#include <boost/variant.hpp>
#include <cstring>

namespace ceammc {
namespace net {

    enum IpAddrType {
        IP_V4 = 4,
        IP_V6 = 16, // do not change values!
    };

    template <IpAddrType V>
    struct IpAddrT {
        std::string str;
        std::uint8_t octets[V];

        template <size_t>
        struct BitFields {
        };
        template <>
        struct BitFields<IP_V4> {
            bool is_broadcast : 1;
            bool is_docum : 1;
            bool is_link_local : 1;
            bool is_loopback : 1;
            bool is_multicast : 1;
            bool is_private : 1;
            bool is_unspec : 1;
            BitFields(const ceammc_net_addr4* addr)
            {
                is_broadcast = ceammc_net_addr4_is_bcast(addr);
                is_docum = ceammc_net_addr4_is_doc(addr);
                is_link_local = ceammc_net_addr4_is_link(addr);
                is_loopback = ceammc_net_addr4_is_loop(addr);
                is_multicast = ceammc_net_addr4_is_mcast(addr);
                is_private = ceammc_net_addr4_is_priv(addr);
                is_unspec = ceammc_net_addr4_is_unspec(addr);
            }
        };
        template <>
        struct BitFields<IP_V6> {
            bool is_multicast : 1;
            bool is_loopback : 1;
            bool is_unspec : 1;
            BitFields(const ceammc_net_addr6* addr)
            {
                is_multicast = ceammc_net_addr6_is_mcast(addr);
                is_loopback = ceammc_net_addr6_is_loop(addr);
                is_unspec = ceammc_net_addr6_is_unspec(addr);
            }
        };

        BitFields<V> bits;

    public:
        template <class T>
        IpAddrT(const T* addr);
    };

    using IpAddrV4 = IpAddrT<IP_V4>;
    using IpAddrV6 = IpAddrT<IP_V6>;

    using IpAddr = boost::variant<IpAddrV4, IpAddrV6>;
    using IpAddrVisitor4Fn = std::function<void(const IpAddrV4&)>;
    using IpAddrVisitor6Fn = std::function<void(const IpAddrV6&)>;

    class IpAddrVisitor
        : public boost::static_visitor<> {
        IpAddrVisitor4Fn on_v4;
        IpAddrVisitor6Fn on_v6;

    public:
        IpAddrVisitor(IpAddrVisitor4Fn f4, IpAddrVisitor6Fn f6)
            : on_v4(f4)
            , on_v6(f6)
        {
        }

        void operator()(const IpAddrV4& v4) const
        {
            if (on_v4)
                on_v4(v4);
        }

        void operator()(const IpAddrV6& v6) const
        {
            if (on_v6)
                on_v6(v6);
        }
    };

    template <class Addr>
    struct IfaceAddrT {
        Addr ip;
        std::shared_ptr<Addr> bcast, mask;

    public:
        template <class T>
        IfaceAddrT(const T* addr);
    };

    using IfaceAddrV4 = IfaceAddrT<IpAddrV4>;
    using IfaceAddrV6 = IfaceAddrT<IpAddrV6>;

    using IfaceAddr = boost::variant<IfaceAddrV4, IfaceAddrV6>;
    using IfaceAddr4VisitorFn = std::function<void(const IfaceAddrV4&)>;
    using IfaceAddr6VisitorFn = std::function<void(const IfaceAddrV6&)>;

    class IfaceAddrVisitor
        : public boost::static_visitor<> {
        IfaceAddr4VisitorFn on_v4;
        IfaceAddr6VisitorFn on_v6;

    public:
        IfaceAddrVisitor(IfaceAddr4VisitorFn f4, IfaceAddr6VisitorFn f6)
            : on_v4(f4)
            , on_v6(f6)
        {
        }

        void operator()(const IfaceAddrV4& v4) const
        {
            if (on_v4)
                on_v4(v4);
        }

        void operator()(const IfaceAddrV6& v6) const
        {
            if (on_v6)
                on_v6(v6);
        }
    };

    template <>
    template <>
    IpAddrV4::IpAddrT(const ceammc_net_addr4* addr)
        : str(ceammc_net_addr4_str(addr))
        , bits(addr)
    {
        memcpy(octets, ceammc_net_addr4_octets(addr), IP_V4);
    }

    template <>
    template <>
    IpAddrV6::IpAddrT(const ceammc_net_addr6* addr)
        : str(ceammc_net_addr6_str(addr))
        , bits(addr)
    {
        memcpy(octets, ceammc_net_addr6_octets(addr), IP_V6);
    }

    template <>
    template <>
    IfaceAddrT<IpAddrV4>::IfaceAddrT(const ceammc_net_iface_addr4* ifa)
        : ip(ceammc_net_ifa_ip4(ifa))
    {
        auto bc = ceammc_net_ifa_bcast4(ifa);
        if (bc)
            this->bcast.reset(new IpAddrV4(bc));

        auto msk = ceammc_net_ifa_mask4(ifa);
        if (msk)
            this->mask.reset(new IpAddrV4(msk));
    }

    template <>
    template <>
    IfaceAddrT<IpAddrV6>::IfaceAddrT(const ceammc_net_iface_addr6* ifa)
        : ip(ceammc_net_ifa_ip6(ifa))
    {
        auto bc = ceammc_net_ifa_bcast6(ifa);
        if (bc)
            this->bcast.reset(new IpAddrV6(bc));

        auto msk = ceammc_net_ifa_mask6(ifa);
        if (msk)
            this->mask.reset(new IpAddrV6(msk));
    }

    struct Iface {
        std::string name, mac;
        std::vector<IfaceAddr> addrs;
        int index;

        Iface(const ceammc_net_iface& iface)
            : name(ceammc_net_iface_name(&iface))
            , mac(ceammc_net_iface_mac(&iface))
            , index(ceammc_net_iface_index(&iface))
        {
            auto ifn = ceammc_net_iface_num_addr(&iface);
            for (size_t i = 0; i < ifn; i++) {
                auto ifa = ceammc_net_iface_get_addr(&iface, i);

                if (ceammc_net_is_ifa_v4(ifa)) {
                    auto v4 = net::IfaceAddrV4(ceammc_net_as_ifa_v4(ifa));
                    addrs.push_back(v4);
                } else {
                    auto v6 = net::IfaceAddrV6(ceammc_net_as_ifa_v6(ifa));
                    addrs.push_back(v6);
                }
            }
        }
    };
}
}

#endif
