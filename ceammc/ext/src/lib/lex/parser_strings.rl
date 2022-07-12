# include "parser_strings.h"
# include "ceammc_format.h"
# include "fmt/format.h"

%%{
    machine quoted_string_end;
    include string_common "ragel_strings.rl";

    ends_with_double_quote = normal_symbol_nz* '"';

    # zero string version
    main := ends_with_double_quote
            0 @{ fbreak; };
    write data;
}%%

namespace ceammc {
namespace string {

bool quoted_string_end(const Atom& a) {
    if (a.isSymbol())
        return quoted_string_end(a.asT<t_symbol*>());
    else
        return false;
}

bool quoted_string_end(const std::string& str) {
    return quoted_string_end(str.c_str());
}

bool quoted_string_end(t_symbol* sym) {
    if (!sym)
        return false;
    else
        return quoted_string_end(sym->s_name);
}

bool quoted_string_end(const char* str) {
    if (str == nullptr)
        return false;

    int cs = 0;
    const char* p = str;

    %% write init;
    %% write exec noend;

    return (cs >= %%{ write first_final; }%%);
}

%%{
    machine is_quoted_string;
    include string_common "ragel_strings.rl";

    # zero string version
    main := '"' normal_symbol_nz* '"'
            0 @{ fbreak; };
    write data;
}%%

bool is_quoted_string(const AtomListView& lv) {
    auto N = lv.size();
    if (N == 1 && lv[0].isSymbol())
        return is_quoted_string(lv[0].asT<t_symbol*>()->s_name);
    else if(N > 1
            && lv[0].isSymbol()
            && lv[0].asT<t_symbol*>()
            && lv[0].asT<t_symbol*>()->s_name
            && lv[0].asT<t_symbol*>()->s_name[0] == '"'
            && lv[N-1].isSymbol())
        return is_quoted_string(to_string(lv));
    return false;
}

bool is_quoted_string(t_symbol* s) {
    if (!s)
        return false;
    else
        return is_quoted_string(s->s_name);
}

bool is_quoted_string(const std::string& str) {
    return is_quoted_string(str.c_str());
}

bool is_quoted_string(const char* str) {
    if (str == nullptr)
        return false;

    int cs = 0;
    const char* p = str;

    %% write init;
    %% write exec noend;

    return (cs >= %%{ write first_final; }%%);
}

%%{
    machine need_quotes;
    include string_common "ragel_strings.rl";

    # zero string version
    main := (^(all_escapes|0))* 0 @{ fbreak; };
    write data;
}%%

bool string_need_quotes(const char* str) {
    if (str == nullptr || str[0] == '\0')
        return false;

    int cs = 0;
    const char* p = str;

    %% write init;
    %% write exec noend;

    return (cs < %%{ write first_final; }%%);
}

%%{
    machine escape_and_quote;
    include string_common "ragel_strings.rl";

    # zero string version
    esc_all =
        (str_escape      >{ rl_str += '`';   rl_str += '`'; } |
        str_dquote       >{ rl_str += '`';   rl_str += '"'; } |
        str_space        >{ rl_str += '\\';  rl_str += ' '; } |
        str_comma        >{ rl_str += '\\';  rl_str += ','; } |
        str_semicolon    >{ rl_str += '\\';  rl_str += ';'; } |
        str_backslash    >{ rl_str += '`';   rl_str += '/'; })
        % { rl_esc_count++; };

    other = (any - (esc_all|0)) >{ rl_str += fc; };

    main := (esc_all | other)* 0 @{ fbreak; };
    write data;
}%%

bool escape_and_quote(const char* str, std::string& out)
{
    if (str == nullptr || str[0] == '\0') {
        out = "\"\"";
        return true;
    }

    int cs = 0;
    const char* p = str;
    std::string& rl_str = out;
    int rl_esc_count = 0;

    rl_str += '"';

    %% write init;
    %% write exec noend;

    rl_str += '"';
    return rl_esc_count > 0;
}

%%{
    machine escape_and_quote_buf;
    include string_common "ragel_strings.rl";

    # zero string version
    esc_all =
        (str_escape      >{ append_buf('`');   append_buf('`'); } |
        str_dquote       >{ append_buf('`');   append_buf('"'); } |
        str_space        >{ append_buf('\\');  append_buf(' '); } |
        str_comma        >{ append_buf('\\');  append_buf(','); } |
        str_semicolon    >{ append_buf('\\');  append_buf(';'); } |
        str_backslash    >{ append_buf('`');   append_buf('/'); })
        % { rl_esc_n++; };

    other = (any - (esc_all|0)) >{ append_buf(fc); };

    main := (esc_all | other)* 0 @{ fbreak; };
    write data;
}%%

int escape_and_quote(const char* str, char* buf, size_t buf_len)
{
    #define append_buf(c) { if (rl_n-- > 1) *(rl_buf++) = c; else rl_overflow = true; }

    if (!buf || buf_len < 3) {
        std::cerr << fmt::format("invalid buffer {} ({} bytes)\n", (void*)buf, buf_len);
        return -1;
    }

    if (str == nullptr || str[0] == '\0') {
        buf[0] = '"';
        buf[1] = '"';
        buf[2] = 0;
        return 2;
    }

    int cs = 0;
    const char* p = str;
    char* rl_buf = buf;
    size_t rl_n = buf_len;
    size_t rl_esc_n = 0;
    bool rl_overflow = false;

    append_buf('"');

    %% write init;
    %% write exec noend;

    append_buf('"');

    if (rl_overflow) {
        std::cerr << fmt::format("buffer ({} bytes) overflow while quoting string: {}\n", buf_len, str);
        buf[0] = 0;
        return -2;
    } else {
        *rl_buf = 0;
        return buf_len - rl_n;
   }
}

}
}
