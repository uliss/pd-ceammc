#ifndef PARSER_STRINGS_H
#define PARSER_STRINGS_H

#include "ceammc_atomlist_view.h"

#include <string>

namespace ceammc {
namespace string {

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

    int escape_and_quote(const char* str, char* buf, size_t buf_len);
    void escape_and_quote(const char* str, std::string& out);
}
}

#endif // PARSER_STRINGS_H
