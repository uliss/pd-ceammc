#ifndef CEAMMC_STRING_TYPES_H
#define CEAMMC_STRING_TYPES_H

#include <boost/container/small_vector.hpp>

#include "ceammc_datatypes.h"

namespace ceammc {
namespace string {

    using StaticString = ceammc::BoostStaticString<1000>;
    using SmallString = boost::container::small_vector<char, 31>;
    using MediumString = boost::container::small_vector<char, 255>;

}
}

#endif // CEAMMC_STRING_TYPES_H
