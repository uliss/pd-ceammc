#ifndef PARSER_STRINGS_H
#define PARSER_STRINGS_H

#include "ceammc_atomlist.h"
#include "ceammc_atomlist_view.h"
#include "ceammc_string_types.h"

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

    /**
     * Escapes special symbol in string and adds outer double quotes
     * @param str - source string (with spaces, commas, semicolons etc.)
     * @param out - string buffer to append data
     * @return number of escaped charactes or -1 on error
     */
    int escape_and_quote(const char* str, StaticString& out);
    int escape_and_quote(const char* str, SmallString& out);
    int escape_and_quote(const char* str, MediumString& out);

    template <typename T>
    bool escape_and_quote(const Atom& a, T& out)
    {
        if (a.isSymbol()) {
            return escape_and_quote(a.asT<t_symbol*>()->s_name, out) > 0 || a.asT<t_symbol*>()->s_name[0] == '\0';
        } else if (a.isSemicolon()) {
            return escape_and_quote(";", out) > 0;
        } else if (a.isComma()) {
            return escape_and_quote(",", out) > 0;
        } else
            return false;
    }

    /**
     * Escape and quote symbol atoms, commas and semicolons
     * @param a - atom to escape
     */
    void escape_and_quote(Atom& a);

    /**
     * Escape and quote symbol atoms, commas and semicolons
     * @param a - atom to escape
     * @return new escaped atom
     */
    Atom escape_and_quote(const Atom& a);

    /**
     * Escape and quote symbol atoms, commas and semicolons in given list
     * @param lst - list to escape
     */
    void escape_and_quote(AtomList& lst);

    /**
     * Escape and quote symbol atoms, commas and semicolons in given list
     * @param lst - list to escape
     * @return new escaped list
     */
    AtomList escape_and_quote(const AtomListView& lv);
}
}

#endif // PARSER_STRINGS_H
