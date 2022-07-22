# include "parser_strings.h"
# include "ceammc_containers.h"
# include "ceammc_format.h"
# include "ceammc_log.h"
# include "ceammc_string.h"
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
        str_lcurly       >{ out.push_back('`');   out.push_back('('); } |
        str_rcurly       >{ out.push_back('`');   out.push_back(')'); } |
        str_backslash    >{ out.push_back('\\'); } |
        str_space        >{ out.push_back(' '); } |
        str_comma        >{ out.push_back(','); } |
        str_semicolon    >{ out.push_back(';'); } |
        str_lpar         >{ out.push_back('('); } |
        str_rpar         >{ out.push_back(')'); } |
        str_lbrac        >{ out.push_back('['); } |
        str_rbrac        >{ out.push_back(']'); } |
        str_colon        >{ out.push_back(':'); } |
        str_at           >{ out.push_back('@'); } |
        str_perc         >{ out.push_back('%'); } |
        str_hash         >{ out.push_back('#'); }
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

void escape_and_quote(Atom& a)
{
    if (a.isSymbol() || a.isComma() || a.isSemicolon()) {
        SmallString out;
        if (escape_and_quote(a, out)) {
            out.push_back(0);
            a = gensym(out.data());
        }
    }
}

Atom escape_and_quote(const Atom& a)
{
    if (a.isSymbol() || a.isComma() || a.isSemicolon()) {
        SmallString out;
        if (escape_and_quote(a, out)) {
            out.push_back(0);
            return gensym(out.data());
        }
    }

    return a;
}

void escape_and_quote(AtomList& lst)
{
    for (auto& a: lst)
        escape_and_quote(a);
}

AtomList escape_and_quote(const AtomListView& lv)
{
    AtomList res;
    res.reserve(lv.size());
    for (auto& a: lv)
        res.append(escape_and_quote(a));

    return res;
}

%%{
    machine unquote_and_unescape;
    include string_common "ragel_strings.rl";

    action on_envvar_done {
        auto env = std::getenv(rl_envvar.c_str());
        if (env)
            rl_string.insert(rl_string.end(), env, env + strlen(env));
        else
            rl_string.insert(rl_string.end(), rl_envvar.begin(), rl_envvar.end());

        fret;
    }

    action on_envvar_err {
        rl_string.push_back('%');
        rl_string.insert(rl_string.end(), rl_envvar.begin(), rl_envvar.end());
        fhold;
        fret;
    }

    envvar       := ([A-Z_0-9]+ ${ rl_envvar += fc; }
                    '%' @on_envvar_done)
                    @lerr(on_envvar_err);

    esc_tick     = '``'            @{ rl_string.push_back('`'); };
    esc_dblq     = '`"'            @{ rl_string.push_back('"'); };
    esc_perc     = '`%'            @{ rl_string.push_back('%'); };
    perc         = '%'             @{ fcall envvar; };
    lcurly       = esc_lcurly      @{ rl_string.push_back('{'); };
    rcurly       = esc_rcurly      @{ rl_string.push_back('}'); };
    char         = [^`"%]          @{ rl_string.push_back(fc); };
    esc_char     = lcurly | rcurly | perc | char | esc_tick | esc_dblq | esc_perc;
    string       = 'S'? '"' (esc_char-0)* '"';

    # zero string version
    main := string 0 @{ fbreak; };
    write data;
}%%

template <typename T>
static int unquote_and_unescape_t(const char* str, T& out) noexcept
{
    try {
        if (str == nullptr)
            return -1;
        else if(str[0] == '\0')
            return 0;

        int cs = 0;
        int top = 0;
        int stack[2];
        const char* p = str;
        const char* eof = p + strlen(p);
        T& rl_string = out;
        std::string rl_envvar;

        %% write init;
        %% write exec noend;

        if (cs < %%{ write first_final; }%%)
            return 0;
        else
            return 1;

    } catch(std::exception& e) {
        LIB_ERR << fmt::format("[{}] error '{}'", __FUNCTION__, e.what());
        return -1;
    }
}

int unquote_and_unescape(const char* str, StaticString& out)
{
    return unquote_and_unescape_t(str, out);
}

int unquote_and_unescape(const char* str, SmallString& out)
{
    return unquote_and_unescape_t(str, out);
}

int unquote_and_unescape(const char* str, MediumString& out)
{
    return unquote_and_unescape_t(str, out);
}

t_symbol* parse_ceammc_quoted_string(t_symbol* s)
{
    if (s == nullptr) return &s_;
    SmallString str;
    if (unquote_and_unescape(s->s_name, str) > 0) {
        str.push_back(0);
        return gensym(str.data());
    } else
        return s;
}

Atom parse_ceammc_quoted_string(const Atom& a)
{
    switch(a.atom().a_type) {
    case A_SYMBOL:
    case A_DOLLSYM:
        return parse_ceammc_quoted_string(a.asT<t_symbol*>());
    default:
        return a;
    }
}

AtomList parse_ceammc_quoted_string(const AtomListView& lv)
{
    AtomList res;
    res.reserve(lv.size());

    bool in_string = false;

    SmallAtomListN<8> str_atoms;

    for (auto& a: lv) {
        if (a.isQuoted() && !in_string)
            res.append(parse_ceammc_quoted_string(a));
        else if (a.beginQuote() && !in_string) {
            in_string = true;
            str_atoms.push_back(a);
        } else if (a.endQuote()) {
            str_atoms.push_back(a);
            MediumString buf;
            for (auto& a: str_atoms) {
                atom_to_string(a, buf);
                buf.push_back(' ');
            }

            // remove trailing space
            if (buf.size() > 0 && buf.back() == ' ')
                buf.pop_back();

            // make zero-terminated string
            buf.push_back('\0');

            // unquoted and unescape
            MediumString str;
            if (unquote_and_unescape(buf.data(), str)) {
                str.push_back('\0');
                auto sym = gensym(str.data());
                res.append(sym);
            }

            str_atoms.clear();
            in_string = false;
        } else if (in_string) {
            str_atoms.push_back(a);
        } else {
            res.append(a);
        }
    }

    // non-balanced quotes
    if (str_atoms.size() > 0)  {
        LIB_LOG << "non-balanced quotes: " << lv;
        for (auto& a: str_atoms)
            res.append(a);
    }

    return res;
}

bool maybe_ceammc_quoted_string(const AtomListView& lv)
{
    for (auto& a: lv) {
        if (a.beginQuote() || a.endQuote() || a.isQuoted())
            return true;
    }

    return false;
}

}
}
