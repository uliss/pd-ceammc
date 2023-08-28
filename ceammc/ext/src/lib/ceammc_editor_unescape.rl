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

static bool editor_string_unescape_lua(std::string& str)
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

%%{
    machine editor_unescape_data;

    esc_comma    = "\\,"   %{ fpc[-2] = ',';  fpc[-1] = REPLACE_CHAR; };
    esc_lpar     = "`("    %{ fpc[-2] = '{';  fpc[-1] = REPLACE_CHAR; };
    esc_rpar     = "`)"    %{ fpc[-2] = '}';  fpc[-1] = REPLACE_CHAR; };
    esc_semi     = "\;"    %{ fpc[-2] = ';';  fpc[-1] = REPLACE_CHAR; };
    esc_dollar   = "\\$"   %{ fpc[-2] = '$';  fpc[-1] = REPLACE_CHAR; };

    esc_all = esc_comma | esc_lpar | esc_rpar | esc_semi | esc_dollar;
    other = (any -- esc_all);
    main := (other | esc_all)*;

    write data;
}%%

static bool editor_string_unescape_data(std::string& str)
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
    case EditorEscapeMode::DATA:
        return editor_string_unescape_data(str);
    default:
        return false;
    }
}

static bool editor_string_escape_lua(const char* str, AtomList& res)
{
    auto len = strlen(str);
    std::string str_res;
    str_res.reserve(len);

    int c = 0;
    while ((c = *str++)) {
        switch(c) {
        case '\t': str_res += "\\\\x09"; break;
        case ',':  str_res += "\\\\x2c"; break;
        case '{':  str_res += "\\\\x7b"; break;
        case '}':  str_res += "\\\\x7d"; break;
        case ';':  str_res += "\\\\x3b"; break;
        case '\\': str_res += "\\\\x09"; break;
        case '$':  str_res += "\\\\x24";  break;
        default:   str_res += c; break;
        }
    }

    res = AtomList::parseString(str_res.c_str());
    return true;
}

static bool editor_string_escape_data(const char* str, AtomList& res)
{
    return false;
}

bool editor_string_escape(const char* str, AtomList& res, EditorEscapeMode mode)
{
    switch(mode) {
    case EditorEscapeMode::LUA:
        return editor_string_escape_lua(str, res);
    case EditorEscapeMode::DATA:
        return editor_string_escape_data(str, res);
    default:
        return false;
    }
}
}
