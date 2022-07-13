#ifndef CEAMMC_STRING_TYPES_H
#define CEAMMC_STRING_TYPES_H

#include <boost/container/small_vector.hpp>
#include <boost/static_string.hpp>

namespace ceammc {
namespace string {

    using StaticString = boost::static_string<1000>;
    using SmallString = boost::container::small_vector<char, 31>;
    using MediumString = boost::container::small_vector<char, 255>;

}
}

#endif // CEAMMC_STRING_TYPES_H
