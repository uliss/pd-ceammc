# include "ceammc_editor_object.h"

# include <algorithm>
# include <cstring>

constexpr const char REPLACE_CHAR = '\0';

%%{
    machine editor_unescape_pd;

    esc_tab    = "\\x09"   %{ fpc[-4] = '\t';  memset(fpc-3, REPLACE_CHAR, 3); };
    esc_comma  = "\\x2c"   %{ fpc[-4] = ',';   memset(fpc-3, REPLACE_CHAR, 3); };
    esc_lbr    = "\\x7b"   %{ fpc[-4] = '{';   memset(fpc-3, REPLACE_CHAR, 3); };
    esc_rbr    = "\\x7d"   %{ fpc[-4] = '}';   memset(fpc-3, REPLACE_CHAR, 3); };
    esc_semi   = "\\x3b"   %{ fpc[-4] = ';';   memset(fpc-3, REPLACE_CHAR, 3); };
    esc_slash  = "\\x5c"   %{ fpc[-4] = '\\';  memset(fpc-3, REPLACE_CHAR, 3); };
    esc_dollar = "\\x24"   %{ fpc[-4] = '$';   memset(fpc-3, REPLACE_CHAR, 3); };

    esc_all = esc_tab | esc_comma | esc_lbr | esc_rbr | esc_semi | esc_slash  | esc_dollar;
    other = (any -- esc_all);
    main := (other | esc_all)*;

    write data;
}%%

namespace ceammc {

bool editor_string_unescape_lua(std::string& str)
{
    int cs = 0;
    char* p = const_cast<char*>(str.data());
    auto pe = p + str.length();
    auto eof = pe;

    %% write init;
    %% write exec;

    // algo:
    // replace all unescape gaps with REPLACE_CHAR (we replace 4 characters with 1)
    // remove all REPLACE_CHAR
    // resize string

    auto it = std::remove(str.begin(), str.end(), REPLACE_CHAR);
    str.erase(it, str.end());

    if (cs >= %%{ write first_final; }%%)
        return true;

    return false;
}

bool editor_string_unescape(std::string& str, EditorEscapeMode mode)
{
    switch(mode) {
    case EditorEscapeMode::LUA:
        return editor_string_unescape_lua(str);
    default:
        return false;
    }
}
}
