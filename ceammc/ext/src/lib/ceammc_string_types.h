#ifndef CEAMMC_STRING_TYPES_H
#define CEAMMC_STRING_TYPES_H

#include <boost/version.hpp>
#if BOOST_VERSION < 107500
#define USE_BOOST_BEAST
#include <boost/beast/core/static_string.hpp>
#else
#include <boost/static_string.hpp>
#endif

#include <boost/container/small_vector.hpp>

namespace ceammc {

#if BOOST_VERSION < 107500
#define STATIC_STRING_SIZE(type) type::max_size_n
template <size_t N>
using BoostStaticString = boost::beast::static_string<N>;
#else
#define STATIC_STRING_SIZE(type) type::static_capacity
template <size_t N>
using BoostStaticString = boost::static_string<N>;
#endif

namespace string {

    using StaticString = ceammc::BoostStaticString<1000>;
    using SmallString = boost::container::small_vector<char, 31>;
    using MediumString = boost::container::small_vector<char, 255>;

}
}

#endif // CEAMMC_STRING_TYPES_H
