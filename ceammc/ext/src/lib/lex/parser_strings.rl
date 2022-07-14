# include "parser_strings.h"
# include "ceammc_format.h"
# include "ceammc_log.h"
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
    esc_all = (
        str_escape       >{ out.push_back('`');   out.push_back('`'); } |
        str_dquote       >{ out.push_back('`');   out.push_back('"'); } |
        str_backslash    >{ out.push_back('\\'); } |
        str_space        >{ out.push_back(' '); } |
        str_comma        >{ out.push_back(','); } |
        str_semicolon    >{ out.push_back(';'); } |
        str_lpar         >{ out.push_back('('); } |
        str_rpar         >{ out.push_back(')'); } |
        str_lbrac        >{ out.push_back('['); } |
        str_rbrac        >{ out.push_back(']'); } |
        ':'              >{ out.push_back(':'); } |
        '@'              >{ out.push_back('@'); } |
        '%'              >{ out.push_back('%'); } |
        '#'              >{ out.push_back('#'); }
    ) % { rl_esc_count++; };

    other = (any - (esc_all|0)) >{ out.push_back(fc); };

    main := (esc_all | other)* 0 @{ fbreak; };
    write data;
}%%

template <typename T>
static int escape_and_quote_t(const char* str, T& out) noexcept
{
    try {
        if (str == nullptr || str[0] == '\0') {
            out.push_back('"');
            out.push_back('"');
            return 0;
        }

        int cs = 0;
        const char* p = str;
        int rl_esc_count = 0;

        out.push_back('"');

        %% write init;
        %% write exec noend;

        out.push_back('"');

        return rl_esc_count;
    } catch(std::exception& e) {
        LIB_ERR << fmt::format("[{}] error '{}'", __FUNCTION__, e.what());
        return -1;
    }
}

int escape_and_quote(const char* str, SmallString& out)
{
    return escape_and_quote_t(str, out);
}

int  escape_and_quote(const char* str, MediumString& out)
{
    return escape_and_quote_t(str, out);
}

int escape_and_quote(const char* str, StaticString& out)
{
    return escape_and_quote_t(str, out);
}

}
}
