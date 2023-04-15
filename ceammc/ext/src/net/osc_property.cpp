#include "osc_property.h"
#include "ceammc_object.h"
#include "fmt/format.h"
#include "parser_osc.h"

#include <limits>

namespace ceammc {
namespace net {

    using namespace ceammc::osc;

    static t_symbol* oscProtoToSymbol(OscProto proto)
    {
        switch (proto) {
        case OSC_PROTO_UDP:
            return gensym("udp");
        case OSC_PROTO_TCP:
            return gensym("tcp");
        case OSC_PROTO_UNIX:
            return gensym("unix");
        default:
            return gensym("?");
        }
    }

    bool OscUrlProperty::parseUrl(const Atom& url)
    {
        if (url.isSymbol()) {
            auto s = url.asT<t_symbol*>()->s_name;

            auto res = parser::parse_osc_url(s, proto_, host_, port_, path_);

            if (res == parser::OSC_ADDR_INVALID)
                return false;

            addr_type_ = res;

        } else if (url.isInteger()) {
            constexpr int MIN_PORT = 1024;
            constexpr int MAX_PORT = std::numeric_limits<std::int16_t>::max();
            const int port = url.asT<int>();
            if (port <= MIN_PORT || port > MAX_PORT) {
                LIB_ERR << fmt::format("[{}] invalid port value: {}, expected to be in {}..{} range", name()->s_name, port, MIN_PORT, MAX_PORT);
                return false;
            }

            addr_type_ = parser::OSC_ADDR_PROTO_PORT;
            port_ = port;
            proto_ = OSC_PROTO_UDP;
            host_ = &s_;
            setValue(&s_);
        } else {
            LIB_ERR << "OSC url or port number expected";
            return false;
        }

        return true;
    }

    OscUrlProperty::OscUrlProperty(const std::string& name, const Atom& def, PropValueAccess ro)
        : AtomProperty(name, def, ro)
        , host_(&s_)
        , proto_(OSC_PROTO_UDP)
        , path_(&s_)
        , port_(0)
        , addr_type_(parser::OSC_ADDR_URL)
    {
        parseUrl(def);
        setAtomCheckFn([this](const Atom& a) -> bool { return parseUrl(a); });
    }

    void OscUrlProperty::registerProps(BaseObject* obj)
    {
        obj->createCbSymbolProperty("@host", [this]() { return host_; });
        obj->createCbSymbolProperty("@proto", [this]() { return oscProtoToSymbol(proto_); });
        obj->createCbSymbolProperty("@path", [this]() { return path_; });
        obj->createCbIntProperty("@port", [this]() { return port_; });
    }

    bool OscUrlProperty::isUrlAddr() const
    {
        return addr_type_ == parser::OSC_ADDR_URL || addr_type_ == parser::OSC_ADDR_UNIX;
    }

    bool OscUrlProperty::isProtoPortAddr() const
    {
        return addr_type_ == parser::OSC_ADDR_PROTO_PORT;
    }

}
}
