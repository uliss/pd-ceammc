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

    using BlobPtr = std::unique_ptr<std::remove_pointer<lo_blob>::type, typeof(&lo_blob_free)>;
    inline BlobPtr make_blob(std::int32_t size, const void* data)
    {
        return BlobPtr(lo_blob_new(size, data), &lo_blob_free);
    }

    using ServerPtr = std::unique_ptr<std::remove_pointer<lo_server>::type, typeof(&lo_server_free)>;
    inline ServerPtr make_server(lo_server srv)
    {
        return ServerPtr(srv, &lo_server_free);
    }
}
}

#endif // NET_OSC_LO_CXX_H
