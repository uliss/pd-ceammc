
#line 1 "lex/parser_strings.rl"
# include "parser_strings.h"
# include "ceammc_containers.h"
# include "ceammc_format.h"
# include "ceammc_log.h"
# include "ceammc_string.h"
# include "fmt/format.h"


#line 12 "lex/parser_strings.cpp"
static const int quoted_string_end_start = 1;
static const int quoted_string_end_first_final = 4;
static const int quoted_string_end_error = 0;

static const int quoted_string_end_en_main = 1;


#line 18 "lex/parser_strings.rl"


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

    
#line 52 "lex/parser_strings.cpp"
	{
	cs = quoted_string_end_start;
	}

#line 49 "lex/parser_strings.rl"
    
#line 59 "lex/parser_strings.cpp"
	{
	switch ( cs )
	{
st1:
	p += 1;
case 1:
	switch( (*p) ) {
		case 0: goto st0;
		case 34: goto st2;
		case 96: goto st3;
	}
	goto st1;
st0:
cs = 0;
	goto _out;
st2:
	p += 1;
case 2:
	if ( (*p) == 0 )
		goto tr4;
	goto st0;
tr4:
#line 16 "lex/parser_strings.rl"
	{ {p++; cs = 4; goto _out;} }
	goto st4;
st4:
	p += 1;
case 4:
#line 88 "lex/parser_strings.cpp"
	goto st0;
st3:
	p += 1;
case 3:
	switch( (*p) ) {
		case 34: goto st1;
		case 96: goto st1;
	}
	if ( 40 <= (*p) && (*p) <= 41 )
		goto st1;
	goto st0;
	}

	_out: {}
	}

#line 50 "lex/parser_strings.rl"

    return (cs >= 4);
}


#line 111 "lex/parser_strings.cpp"
static const int is_quoted_string_start = 1;
static const int is_quoted_string_first_final = 5;
static const int is_quoted_string_error = 0;

static const int is_quoted_string_en_main = 1;


#line 62 "lex/parser_strings.rl"


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

    
#line 155 "lex/parser_strings.cpp"
	{
	cs = is_quoted_string_start;
	}

#line 97 "lex/parser_strings.rl"
    
#line 162 "lex/parser_strings.cpp"
	{
	switch ( cs )
	{
case 1:
	if ( (*p) == 34 )
		goto st2;
	goto st0;
st0:
cs = 0;
	goto _out;
st2:
	p += 1;
case 2:
	switch( (*p) ) {
		case 0: goto st0;
		case 34: goto st3;
		case 96: goto st4;
	}
	goto st2;
st3:
	p += 1;
case 3:
	if ( (*p) == 0 )
		goto tr4;
	goto st0;
tr4:
#line 60 "lex/parser_strings.rl"
	{ {p++; cs = 5; goto _out;} }
	goto st5;
st5:
	p += 1;
case 5:
#line 195 "lex/parser_strings.cpp"
	goto st0;
st4:
	p += 1;
case 4:
	switch( (*p) ) {
		case 34: goto st2;
		case 96: goto st2;
	}
	if ( 40 <= (*p) && (*p) <= 41 )
		goto st2;
	goto st0;
	}

	_out: {}
	}

#line 98 "lex/parser_strings.rl"

    return (cs >= 5);
}


#line 218 "lex/parser_strings.cpp"
static const int need_quotes_start = 1;
static const int need_quotes_first_final = 2;
static const int need_quotes_error = 0;

static const int need_quotes_en_main = 1;


#line 109 "lex/parser_strings.rl"


bool string_need_quotes(const char* str) {
    if (str == nullptr || str[0] == '\0')
        return false;

    int cs = 0;
    const char* p = str;

    
#line 237 "lex/parser_strings.cpp"
	{
	cs = need_quotes_start;
	}

#line 119 "lex/parser_strings.rl"
    
#line 244 "lex/parser_strings.cpp"
	{
	switch ( cs )
	{
st1:
	p += 1;
case 1:
	switch( (*p) ) {
		case 0: goto tr1;
		case 32: goto st0;
		case 37: goto st0;
		case 44: goto st0;
		case 64: goto st0;
		case 96: goto st0;
	}
	if ( (*p) < 40 ) {
		if ( 34 <= (*p) && (*p) <= 35 )
			goto st0;
	} else if ( (*p) > 41 ) {
		if ( (*p) > 59 ) {
			if ( 91 <= (*p) && (*p) <= 93 )
				goto st0;
		} else if ( (*p) >= 58 )
			goto st0;
	} else
		goto st0;
	goto st1;
tr1:
#line 107 "lex/parser_strings.rl"
	{ {p++; cs = 2; goto _out;} }
	goto st2;
st2:
	p += 1;
case 2:
#line 278 "lex/parser_strings.cpp"
	goto st0;
st0:
cs = 0;
	goto _out;
	}

	_out: {}
	}

#line 120 "lex/parser_strings.rl"

    return (cs < 2);
}


#line 294 "lex/parser_strings.cpp"
static const int escape_and_quote_start = 1;
static const int escape_and_quote_first_final = 3;
static const int escape_and_quote_error = 0;

static const int escape_and_quote_en_main = 1;


#line 152 "lex/parser_strings.rl"


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

        
#line 322 "lex/parser_strings.cpp"
	{
	cs = escape_and_quote_start;
	}

#line 171 "lex/parser_strings.rl"
        
#line 329 "lex/parser_strings.cpp"
	{
	switch ( cs )
	{
tr0:
#line 148 "lex/parser_strings.rl"
	{ out.push_back((*p)); }
	goto st1;
tr18:
#line 146 "lex/parser_strings.rl"
	{ rl_esc_count++; }
#line 148 "lex/parser_strings.rl"
	{ out.push_back((*p)); }
	goto st1;
st1:
	p += 1;
case 1:
#line 346 "lex/parser_strings.cpp"
	switch( (*p) ) {
		case 0: goto tr1;
		case 32: goto tr2;
		case 34: goto tr3;
		case 35: goto tr4;
		case 37: goto tr5;
		case 40: goto tr6;
		case 41: goto tr7;
		case 44: goto tr8;
		case 58: goto tr9;
		case 59: goto tr10;
		case 64: goto tr11;
		case 91: goto tr12;
		case 92: goto tr13;
		case 93: goto tr14;
		case 96: goto tr15;
		case 123: goto tr16;
		case 125: goto tr17;
	}
	goto tr0;
tr1:
#line 150 "lex/parser_strings.rl"
	{ {p++; cs = 3; goto _out;} }
	goto st3;
tr19:
#line 146 "lex/parser_strings.rl"
	{ rl_esc_count++; }
#line 150 "lex/parser_strings.rl"
	{ {p++; cs = 3; goto _out;} }
	goto st3;
st3:
	p += 1;
case 3:
#line 380 "lex/parser_strings.cpp"
	goto st0;
st0:
cs = 0;
	goto _out;
tr2:
#line 135 "lex/parser_strings.rl"
	{ out.push_back(' '); }
	goto st2;
tr3:
#line 131 "lex/parser_strings.rl"
	{ out.push_back('`');   out.push_back('"'); }
	goto st2;
tr4:
#line 145 "lex/parser_strings.rl"
	{ out.push_back('#'); }
	goto st2;
tr5:
#line 144 "lex/parser_strings.rl"
	{ out.push_back('%'); }
	goto st2;
tr6:
#line 138 "lex/parser_strings.rl"
	{ out.push_back('('); }
	goto st2;
tr7:
#line 139 "lex/parser_strings.rl"
	{ out.push_back(')'); }
	goto st2;
tr8:
#line 136 "lex/parser_strings.rl"
	{ out.push_back(','); }
	goto st2;
tr9:
#line 142 "lex/parser_strings.rl"
	{ out.push_back(':'); }
	goto st2;
tr10:
#line 137 "lex/parser_strings.rl"
	{ out.push_back(';'); }
	goto st2;
tr11:
#line 143 "lex/parser_strings.rl"
	{ out.push_back('@'); }
	goto st2;
tr12:
#line 140 "lex/parser_strings.rl"
	{ out.push_back('['); }
	goto st2;
tr13:
#line 134 "lex/parser_strings.rl"
	{ out.push_back('\\'); }
	goto st2;
tr14:
#line 141 "lex/parser_strings.rl"
	{ out.push_back(']'); }
	goto st2;
tr15:
#line 130 "lex/parser_strings.rl"
	{ out.push_back('`');   out.push_back('`'); }
	goto st2;
tr16:
#line 132 "lex/parser_strings.rl"
	{ out.push_back('`');   out.push_back('('); }
	goto st2;
tr17:
#line 133 "lex/parser_strings.rl"
	{ out.push_back('`');   out.push_back(')'); }
	goto st2;
tr20:
#line 146 "lex/parser_strings.rl"
	{ rl_esc_count++; }
#line 135 "lex/parser_strings.rl"
	{ out.push_back(' '); }
	goto st2;
tr21:
#line 146 "lex/parser_strings.rl"
	{ rl_esc_count++; }
#line 131 "lex/parser_strings.rl"
	{ out.push_back('`');   out.push_back('"'); }
	goto st2;
tr22:
#line 146 "lex/parser_strings.rl"
	{ rl_esc_count++; }
#line 145 "lex/parser_strings.rl"
	{ out.push_back('#'); }
	goto st2;
tr23:
#line 146 "lex/parser_strings.rl"
	{ rl_esc_count++; }
#line 144 "lex/parser_strings.rl"
	{ out.push_back('%'); }
	goto st2;
tr24:
#line 146 "lex/parser_strings.rl"
	{ rl_esc_count++; }
#line 138 "lex/parser_strings.rl"
	{ out.push_back('('); }
	goto st2;
tr25:
#line 146 "lex/parser_strings.rl"
	{ rl_esc_count++; }
#line 139 "lex/parser_strings.rl"
	{ out.push_back(')'); }
	goto st2;
tr26:
#line 146 "lex/parser_strings.rl"
	{ rl_esc_count++; }
#line 136 "lex/parser_strings.rl"
	{ out.push_back(','); }
	goto st2;
tr27:
#line 146 "lex/parser_strings.rl"
	{ rl_esc_count++; }
#line 142 "lex/parser_strings.rl"
	{ out.push_back(':'); }
	goto st2;
tr28:
#line 146 "lex/parser_strings.rl"
	{ rl_esc_count++; }
#line 137 "lex/parser_strings.rl"
	{ out.push_back(';'); }
	goto st2;
tr29:
#line 146 "lex/parser_strings.rl"
	{ rl_esc_count++; }
#line 143 "lex/parser_strings.rl"
	{ out.push_back('@'); }
	goto st2;
tr30:
#line 146 "lex/parser_strings.rl"
	{ rl_esc_count++; }
#line 140 "lex/parser_strings.rl"
	{ out.push_back('['); }
	goto st2;
tr31:
#line 146 "lex/parser_strings.rl"
	{ rl_esc_count++; }
#line 134 "lex/parser_strings.rl"
	{ out.push_back('\\'); }
	goto st2;
tr32:
#line 146 "lex/parser_strings.rl"
	{ rl_esc_count++; }
#line 141 "lex/parser_strings.rl"
	{ out.push_back(']'); }
	goto st2;
tr33:
#line 146 "lex/parser_strings.rl"
	{ rl_esc_count++; }
#line 130 "lex/parser_strings.rl"
	{ out.push_back('`');   out.push_back('`'); }
	goto st2;
tr34:
#line 146 "lex/parser_strings.rl"
	{ rl_esc_count++; }
#line 132 "lex/parser_strings.rl"
	{ out.push_back('`');   out.push_back('('); }
	goto st2;
tr35:
#line 146 "lex/parser_strings.rl"
	{ rl_esc_count++; }
#line 133 "lex/parser_strings.rl"
	{ out.push_back('`');   out.push_back(')'); }
	goto st2;
st2:
	p += 1;
case 2:
#line 548 "lex/parser_strings.cpp"
	switch( (*p) ) {
		case 0: goto tr19;
		case 32: goto tr20;
		case 34: goto tr21;
		case 35: goto tr22;
		case 37: goto tr23;
		case 40: goto tr24;
		case 41: goto tr25;
		case 44: goto tr26;
		case 58: goto tr27;
		case 59: goto tr28;
		case 64: goto tr29;
		case 91: goto tr30;
		case 92: goto tr31;
		case 93: goto tr32;
		case 96: goto tr33;
		case 123: goto tr34;
		case 125: goto tr35;
	}
	goto tr18;
	}

	_out: {}
	}

#line 172 "lex/parser_strings.rl"

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


#line 641 "lex/parser_strings.cpp"
static const int unquote_and_unescape_start = 1;
static const int unquote_and_unescape_first_final = 8;
static const int unquote_and_unescape_error = 0;

static const int unquote_and_unescape_en_envvar = 6;
static const int unquote_and_unescape_en_main = 1;


#line 275 "lex/parser_strings.rl"


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

        
#line 671 "lex/parser_strings.cpp"
	{
	cs = unquote_and_unescape_start;
	top = 0;
	}

#line 295 "lex/parser_strings.rl"
        
#line 679 "lex/parser_strings.cpp"
	{
	goto _resume;

_again:
	switch ( cs ) {
		case 1: goto st1;
		case 0: goto st0;
		case 2: goto st2;
		case 3: goto st3;
		case 8: goto st8;
		case 4: goto st4;
		case 5: goto st5;
		case 6: goto st6;
		case 7: goto st7;
		case 9: goto st9;
	default: break;
	}

	p += 1;
_resume:
	switch ( cs )
	{
st1:
	p += 1;
case 1:
	switch( (*p) ) {
		case 34: goto st2;
		case 83: goto st5;
	}
	goto st0;
tr13:
#line 251 "lex/parser_strings.rl"
	{
        rl_string.push_back('%');
        rl_string.insert(rl_string.end(), rl_envvar.begin(), rl_envvar.end());
        p--;
        {cs = stack[--top];goto _again;}
    }
	goto st0;
#line 719 "lex/parser_strings.cpp"
st0:
cs = 0;
	goto _out;
tr3:
#line 268 "lex/parser_strings.rl"
	{ rl_string.push_back((*p)); }
	goto st2;
tr5:
#line 265 "lex/parser_strings.rl"
	{ {stack[top++] = 2;goto st6;} }
	goto st2;
tr8:
#line 263 "lex/parser_strings.rl"
	{ rl_string.push_back('"'); }
	goto st2;
tr9:
#line 264 "lex/parser_strings.rl"
	{ rl_string.push_back('%'); }
	goto st2;
tr10:
#line 266 "lex/parser_strings.rl"
	{ rl_string.push_back('{'); }
	goto st2;
tr11:
#line 267 "lex/parser_strings.rl"
	{ rl_string.push_back('}'); }
	goto st2;
tr12:
#line 262 "lex/parser_strings.rl"
	{ rl_string.push_back('`'); }
	goto st2;
st2:
	p += 1;
case 2:
#line 754 "lex/parser_strings.cpp"
	switch( (*p) ) {
		case 0: goto st0;
		case 34: goto st3;
		case 37: goto tr5;
		case 96: goto st4;
	}
	goto tr3;
st3:
	p += 1;
case 3:
	if ( (*p) == 0 )
		goto tr7;
	goto st0;
tr7:
#line 273 "lex/parser_strings.rl"
	{ {p++; cs = 8; goto _out;} }
	goto st8;
st8:
	p += 1;
case 8:
#line 775 "lex/parser_strings.cpp"
	goto st0;
st4:
	p += 1;
case 4:
	switch( (*p) ) {
		case 34: goto tr8;
		case 37: goto tr9;
		case 40: goto tr10;
		case 41: goto tr11;
		case 96: goto tr12;
	}
	goto st0;
st5:
	p += 1;
case 5:
	if ( (*p) == 34 )
		goto st2;
	goto st0;
st6:
	p += 1;
case 6:
	if ( (*p) == 95 )
		goto tr14;
	if ( (*p) > 57 ) {
		if ( 65 <= (*p) && (*p) <= 90 )
			goto tr14;
	} else if ( (*p) >= 48 )
		goto tr14;
	goto tr13;
tr14:
#line 258 "lex/parser_strings.rl"
	{ rl_envvar += (*p); }
	goto st7;
st7:
	p += 1;
case 7:
#line 812 "lex/parser_strings.cpp"
	switch( (*p) ) {
		case 37: goto tr15;
		case 95: goto tr14;
	}
	if ( (*p) > 57 ) {
		if ( 65 <= (*p) && (*p) <= 90 )
			goto tr14;
	} else if ( (*p) >= 48 )
		goto tr14;
	goto tr13;
tr15:
#line 241 "lex/parser_strings.rl"
	{
        auto env = std::getenv(rl_envvar.c_str());
        if (env)
            rl_string.insert(rl_string.end(), env, env + strlen(env));
        else
            rl_string.insert(rl_string.end(), rl_envvar.begin(), rl_envvar.end());

        {cs = stack[--top];goto _again;}
    }
	goto st9;
st9:
	p += 1;
case 9:
#line 838 "lex/parser_strings.cpp"
	goto st0;
	}

	if ( p == eof )
	{
	switch ( cs ) {
	case 6: 
	case 7: 
#line 251 "lex/parser_strings.rl"
	{
        rl_string.push_back('%');
        rl_string.insert(rl_string.end(), rl_envvar.begin(), rl_envvar.end());
        p--;
        {cs = stack[--top];goto _again;}
    }
	break;
#line 855 "lex/parser_strings.cpp"
	}
	}

	_out: {}
	}

#line 296 "lex/parser_strings.rl"

        if (cs < 8)
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


#line 980 "lex/parser_strings.cpp"
static const int mdns_hostname_start = 1;
static const int mdns_hostname_first_final = 14;
static const int mdns_hostname_error = 0;

static const int mdns_hostname_en_main = 1;


#line 423 "lex/parser_strings.rl"


std::string mdns_hostname_from_service(const char* str) noexcept
{
    if (str == nullptr)
        return {};
    else if(str[0] == '\0')
        return {};

    int cs = 0;
    const char* p = str;
    std::string host;

    
#line 1003 "lex/parser_strings.cpp"
	{
	cs = mdns_hostname_start;
	}

#line 437 "lex/parser_strings.rl"
    
#line 1010 "lex/parser_strings.cpp"
	{
	switch ( cs )
	{
case 1:
	if ( (*p) == 45 )
		goto tr0;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr0;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr0;
	} else
		goto tr0;
	goto st0;
st0:
cs = 0;
	goto _out;
tr0:
#line 417 "lex/parser_strings.rl"
	{ host += (*p); }
	goto st2;
st2:
	p += 1;
case 2:
#line 1036 "lex/parser_strings.cpp"
	switch( (*p) ) {
		case 45: goto tr0;
		case 46: goto st3;
		case 95: goto tr0;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr0;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr0;
	} else
		goto tr0;
	goto st0;
st3:
	p += 1;
case 3:
	switch( (*p) ) {
		case 0: goto tr3;
		case 46: goto st4;
		case 95: goto st11;
	}
	goto st0;
tr3:
#line 421 "lex/parser_strings.rl"
	{ {p++; cs = 14; goto _out;} }
	goto st14;
st14:
	p += 1;
case 14:
#line 1067 "lex/parser_strings.cpp"
	goto st0;
st4:
	p += 1;
case 4:
	if ( (*p) == 108 )
		goto st5;
	goto st0;
st5:
	p += 1;
case 5:
	if ( (*p) == 111 )
		goto st6;
	goto st0;
st6:
	p += 1;
case 6:
	if ( (*p) == 99 )
		goto st7;
	goto st0;
st7:
	p += 1;
case 7:
	if ( (*p) == 97 )
		goto st8;
	goto st0;
st8:
	p += 1;
case 8:
	if ( (*p) == 108 )
		goto st9;
	goto st0;
st9:
	p += 1;
case 9:
	switch( (*p) ) {
		case 0: goto tr3;
		case 46: goto st10;
	}
	goto st0;
st10:
	p += 1;
case 10:
	if ( (*p) == 0 )
		goto tr3;
	goto st0;
st11:
	p += 1;
case 11:
	switch( (*p) ) {
		case 45: goto st12;
		case 95: goto st12;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st12;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st12;
	} else
		goto st12;
	goto st0;
st12:
	p += 1;
case 12:
	switch( (*p) ) {
		case 0: goto tr3;
		case 45: goto st12;
		case 46: goto st13;
		case 95: goto st12;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st12;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st12;
	} else
		goto st12;
	goto st0;
st13:
	p += 1;
case 13:
	switch( (*p) ) {
		case 95: goto st11;
		case 108: goto st5;
	}
	goto st0;
	}

	_out: {}
	}

#line 438 "lex/parser_strings.rl"

    if (cs >= 14)
        return host;
    else
        return {};
}

}
}
