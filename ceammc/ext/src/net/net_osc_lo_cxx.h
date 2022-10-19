#ifndef NET_OSC_LO_CXX_H
#define NET_OSC_LO_CXX_H

#include "lo/lo.h"

#include <memory>
#include <type_traits>

namespace ceammc {
namespace lo {

    using AddressPtr = std::unique_ptr<std::remove_pointer<lo_address>::type, typeof(&lo_address_free)>;
    inline AddressPtr make_address(lo_address addr)
    {
        return AddressPtr(addr, &lo_address_free);
    }

}
}

#endif // NET_OSC_LO_CXX_H
