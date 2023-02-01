#ifndef CEAMMC_STRING_SPLIT_H
#define CEAMMC_STRING_SPLIT_H

#include <boost/container/small_vector.hpp>
// we are using C++11, so
#include <boost/utility/string_view.hpp>

namespace ceammc {
namespace string {

    using StringView = boost::string_view;
    using StringSplitResult = boost::container::small_vector<StringView, 8>;

    void split_by_sep(StringSplitResult& vec, const StringView& sv, const char* sep);
    void split_every_char(StringSplitResult& vec, const char* str);
}
}

#endif // CEAMMC_STRING_SPLIT_H
