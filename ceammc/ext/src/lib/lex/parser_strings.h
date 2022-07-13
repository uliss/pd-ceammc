#ifndef PARSER_STRINGS_H
#define PARSER_STRINGS_H

#include "ceammc_atomlist_view.h"

#include <boost/container/small_vector.hpp>
#include <boost/static_string.hpp>
#include <string>

namespace ceammc {
namespace string {
    using StaticString = boost::static_string<MAXPDSTRING>;
    using SmallString = boost::container::small_vector<char, 31>;
    using MediumString = boost::container::small_vector<char, 255>;

    /**
     * Checks if given string is matched to quoted string end
     * This is usefull if we have list atom Pd atoms and want to do a quick check
     * if they are data string representation
     */
    bool quoted_string_end(const char* str);
    bool quoted_string_end(t_symbol* s);
    bool quoted_string_end(const std::string& str);
    bool quoted_string_end(const Atom& a);

    bool is_quoted_string(const char* str);
    bool is_quoted_string(t_symbol* s);
    bool is_quoted_string(const std::string& str);
    bool is_quoted_string(const AtomListView& lv);

    bool string_need_quotes(const char* str);

    int escape_and_quote(const char* str, std::string& out);
    int escape_and_quote(const char* str, StaticString& out);
    int escape_and_quote(const char* str, SmallString& out);
    int escape_and_quote(const char* str, MediumString& out);
}
}

#endif // PARSER_STRINGS_H
