#ifndef OSC_PROPERTY_H
#define OSC_PROPERTY_H

#include "ceammc_property.h"
#include "ceammc_osc.h"

namespace ceammc {
namespace net {

    class OscUrlProperty : public AtomProperty {
        t_symbol* host_;
        t_symbol* path_;
        int port_;
        int addr_type_;
        osc::OscProto proto_;

    public:
        OscUrlProperty(const std::string& name, const Atom& def, PropValueAccess ro = PropValueAccess::READWRITE);

        t_symbol* host() const { return host_; }
        osc::OscProto proto() const { return proto_; }
        t_symbol* path() const { return path_; }
        int port() const { return port_; }
        const Atom& url() const { return value(); }

        void registerProps(BaseObject* obj);

        bool isUrlAddr() const;
        bool isProtoPortAddr() const;

    private:
        bool parseUrl(const Atom& url);
    };

}
}

#endif // OSC_PROPERTY_H
