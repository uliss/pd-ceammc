#ifndef OSC_PROPERTY_H
#define OSC_PROPERTY_H

#include "ceammc_property.h"

namespace ceammc {
namespace net {

class OscUrlProperty : public AtomProperty {
    t_symbol* host_;
    t_symbol* port_;
    t_symbol* proto_;
    int iport_;

public:
    OscUrlProperty(const std::string& name, const Atom& def, PropValueAccess ro = PropValueAccess::READWRITE);

    t_symbol* host() const { return host_; }
    t_symbol* port() const { return port_; }
    int iport() const { return iport_; }
    t_symbol* proto() const { return proto_; }
    const Atom& url() const { return value(); }

private:
    bool parseUrl(const Atom& url);
};

}
}

#endif // OSC_PROPERTY_H
