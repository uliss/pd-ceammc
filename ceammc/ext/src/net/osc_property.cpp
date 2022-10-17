#include "osc_property.h"
#include "ceammc_object.h"
#include "fmt/format.h"
#include "parser_osc.h"

#include <limits>

namespace ceammc {
namespace net {

    bool OscUrlProperty::parseUrl(const Atom& url)
    {
        if (url.isSymbol()) {
            auto s = url.asT<t_symbol*>()->s_name;

            if (!parser::parse_osc_url(s, proto_, host_, port_, path_))
                return false;

        } else if (url.isInteger()) {
            constexpr int MIN_PORT = 1024;
            constexpr int MAX_PORT = std::numeric_limits<std::int16_t>::max();
            const int port = url.asT<int>();
            if (port <= MIN_PORT || port > MAX_PORT) {
                LIB_ERR << fmt::format("[{}] invalid port value: {}, expected to be in {}..{} range", name()->s_name, port, MIN_PORT, MAX_PORT);
                return false;
            }

            port_ = port;
            proto_ = gensym("udp");
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
        , proto_(&s_)
        , path_(&s_)
        , port_(0)
    {
        parseUrl(def);
        setAtomCheckFn([this](const Atom& a) -> bool { return parseUrl(a); });
    }

    void OscUrlProperty::registerProps(BaseObject* obj)
    {
        obj->createCbSymbolProperty("@host", [this]() { return host_; });
        obj->createCbSymbolProperty("@proto", [this]() { return proto_; });
        obj->createCbSymbolProperty("@path", [this]() { return path_; });
        obj->createCbIntProperty("@port", [this]() { return port_; });
    }

}
}
