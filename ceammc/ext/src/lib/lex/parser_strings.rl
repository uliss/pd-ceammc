# include "parser_strings.h"
# include "ceammc_format.h"

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

}
}
