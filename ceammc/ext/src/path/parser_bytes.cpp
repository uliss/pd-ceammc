
#line 1 "parser_bytes.rl"
# include "parser_bytes.h"
# include "fmt/core.h"
# include "ceammc_format.h"


#line 9 "parser_bytes.cpp"
static const int bytes_start = 1;
static const int bytes_first_final = 6;
static const int bytes_error = 0;

static const int bytes_en_main = 1;


#line 18 "parser_bytes.rl"


namespace ceammc {

int parse_byte_string(const char* str)
{
    int cs = 0;
    int rl_val = 0;

    const char* p = str;

    
#line 30 "parser_bytes.cpp"
	{
	cs = bytes_start;
	}

#line 30 "parser_bytes.rl"
    
#line 37 "parser_bytes.cpp"
	{
	switch ( cs )
	{
case 1:
	if ( (*p) == 48 )
		goto tr0;
	goto st0;
st0:
cs = 0;
	goto _out;
tr0:
#line 8 "parser_bytes.rl"
	{ rl_val = 0; }
	goto st2;
st2:
	p += 1;
case 2:
#line 55 "parser_bytes.cpp"
	if ( (*p) == 120 )
		goto st3;
	goto st0;
st3:
	p += 1;
case 3:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr3;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr5;
	} else
		goto tr4;
	goto st0;
tr3:
#line 9 "parser_bytes.rl"
	{ (rl_val <<= 4) |= ((*p) - '0'); }
	goto st4;
tr4:
#line 11 "parser_bytes.rl"
	{ (rl_val <<= 4) |= ((*p) - 'A' + 10); }
	goto st4;
tr5:
#line 10 "parser_bytes.rl"
	{ (rl_val <<= 4) |= ((*p) - 'a' + 10); }
	goto st4;
st4:
	p += 1;
case 4:
#line 86 "parser_bytes.cpp"
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr6;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr8;
	} else
		goto tr7;
	goto st0;
tr6:
#line 9 "parser_bytes.rl"
	{ (rl_val <<= 4) |= ((*p) - '0'); }
	goto st5;
tr7:
#line 11 "parser_bytes.rl"
	{ (rl_val <<= 4) |= ((*p) - 'A' + 10); }
	goto st5;
tr8:
#line 10 "parser_bytes.rl"
	{ (rl_val <<= 4) |= ((*p) - 'a' + 10); }
	goto st5;
st5:
	p += 1;
case 5:
#line 111 "parser_bytes.cpp"
	if ( (*p) == 0 )
		goto tr9;
	goto st0;
tr9:
#line 15 "parser_bytes.rl"
	{ {p++; cs = 6; goto _out;} }
	goto st6;
st6:
	p += 1;
case 6:
#line 122 "parser_bytes.cpp"
	goto st0;
	}

	_out: {}
	}

#line 31 "parser_bytes.rl"

    if (cs >= 6)
        return rl_val;

    return BYTE_PARSE_RESULT_INVALID_STRING;
}

std::pair<int, size_t> parse_bytes(const AtomListView& lv, std::vector<char>& out)
{
    out.clear();
    out.reserve(lv.size());

    for (size_t i = 0; i < lv.size(); i++) {
        auto& a = lv[i];
        if (a.isInteger()) {
            auto val = a.asT<t_int>();
            if (val >= 0 && val < 255) {
                out.push_back(static_cast<char>(val));
            } else {
                return { BYTE_PARSE_RESULT_INVALID_RANGE, i };
            }
        } else if (a.isSymbol()) {
            auto val = parse_byte_string(a.asT<t_symbol*>()->s_name);
            if (val < 0)
                return { val, i };

            out.push_back(static_cast<char>(val));
        } else {
            return { BYTE_PARSE_RESULT_INVALID_ATOM, i };
        }
    }

    return { 0, 0 };
}

std::string parser_byte_error(const AtomListView& lv, const std::pair<int, size_t>& res)
{
    switch(res.first) {
    case BYTE_PARSE_RESULT_INVALID_RANGE:
        return fmt::format("byte value out of range at [{}]: {}", res.second, to_string(lv.at(res.second)));
    case BYTE_PARSE_RESULT_INVALID_STRING:
        return fmt::format("invalid string at [{}]: {}", res.second, to_string(lv.at(res.second)));
    case BYTE_PARSE_RESULT_INVALID_ATOM:
        return fmt::format("invalid atom at [{}]: {}", res.second, to_string(lv.at(res.second)));
    default:
        return {};
    }
}


#line 180 "parser_bytes.cpp"
static const int open_mode_start = 1;
static const int open_mode_first_final = 4;
static const int open_mode_error = 0;

static const int open_mode_en_main = 1;


#line 91 "parser_bytes.rl"


std::ios::openmode parse_mode(const char* str)
{
    int cs = 0;
    std::ios::openmode rl_mode = static_cast<std::ios::openmode>(0);
    const char* p = str;

    if (!p || p[0] == '\0')
        return std::ios::in | std::ios::out;

    
#line 201 "parser_bytes.cpp"
	{
	cs = open_mode_start;
	}

#line 103 "parser_bytes.rl"
    
#line 208 "parser_bytes.cpp"
	{
	switch ( cs )
	{
case 1:
	switch( (*p) ) {
		case 97: goto tr0;
		case 114: goto tr2;
		case 119: goto tr3;
	}
	goto st0;
st0:
cs = 0;
	goto _out;
tr0:
#line 85 "parser_bytes.rl"
	{ rl_mode = std::ios::out | std::ios::app; }
	goto st2;
tr2:
#line 83 "parser_bytes.rl"
	{ rl_mode = std::ios::in; }
	goto st2;
tr3:
#line 84 "parser_bytes.rl"
	{ rl_mode = std::ios::out | std::ios::trunc; }
	goto st2;
st2:
	p += 1;
case 2:
#line 237 "parser_bytes.cpp"
	switch( (*p) ) {
		case 0: goto tr4;
		case 43: goto tr5;
	}
	goto st0;
tr4:
#line 88 "parser_bytes.rl"
	{ {p++; cs = 4; goto _out;} }
	goto st4;
st4:
	p += 1;
case 4:
#line 250 "parser_bytes.cpp"
	goto st0;
tr5:
#line 88 "parser_bytes.rl"
	{ rl_mode |= std::ios::in; }
	goto st3;
st3:
	p += 1;
case 3:
#line 259 "parser_bytes.cpp"
	if ( (*p) == 0 )
		goto tr4;
	goto st0;
	}

	_out: {}
	}

#line 104 "parser_bytes.rl"

    if (cs >= 4)
        return rl_mode;

    return static_cast<std::ios::openmode>(0);;
}

}
