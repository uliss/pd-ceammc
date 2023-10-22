
#line 1 "parser_osc.rl"
# include "parser_osc.h"
# include "ceammc_string_types.h"


#line 8 "parser_osc.cpp"
static const int parser_osc_start = 1;
static const int parser_osc_first_final = 33;
static const int parser_osc_error = 0;

static const int parser_osc_en_main = 1;


#line 57 "parser_osc.rl"



namespace ceammc {
namespace parser {

using namespace ceammc::osc;

    OscAddress parse_osc_url(const char* str, OscProto& proto, t_symbol*& host, int& port, t_symbol*& unix_path) {
        int cs = 0;
        const char* p = str;
        string::SmallString rl_host;
        string::SmallString rl_path;
        int rl_port = 0;
        OscProto rl_proto = OSC_PROTO_UDP;
        OscAddress rl_addr = OSC_ADDR_INVALID;

        
#line 35 "parser_osc.cpp"
	{
	cs = parser_osc_start;
	}

#line 75 "parser_osc.rl"
        
#line 42 "parser_osc.cpp"
	{
	switch ( cs )
	{
case 1:
	switch( (*p) ) {
		case 111: goto st2;
		case 116: goto st25;
		case 117: goto st30;
	}
	goto st0;
st0:
cs = 0;
	goto _out;
st2:
	p += 1;
case 2:
	if ( (*p) == 115 )
		goto st3;
	goto st0;
st3:
	p += 1;
case 3:
	if ( (*p) == 99 )
		goto st4;
	goto st0;
st4:
	p += 1;
case 4:
	switch( (*p) ) {
		case 46: goto st5;
		case 58: goto st9;
	}
	goto st0;
st5:
	p += 1;
case 5:
	switch( (*p) ) {
		case 116: goto st6;
		case 117: goto st14;
	}
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
	if ( (*p) == 112 )
		goto st8;
	goto st0;
st8:
	p += 1;
case 8:
	if ( (*p) == 58 )
		goto tr12;
	goto st0;
tr12:
#line 39 "parser_osc.rl"
	{ rl_proto = OSC_PROTO_TCP; }
	goto st9;
tr22:
#line 40 "parser_osc.rl"
	{ rl_proto = OSC_PROTO_UDP; }
	goto st9;
st9:
	p += 1;
case 9:
#line 113 "parser_osc.cpp"
	if ( (*p) == 47 )
		goto st10;
	goto st0;
st10:
	p += 1;
case 10:
	if ( (*p) == 47 )
		goto st11;
	goto st0;
tr15:
#line 43 "parser_osc.rl"
	{ rl_host.push_back((*p)); }
	goto st11;
st11:
	p += 1;
case 11:
#line 130 "parser_osc.cpp"
	switch( (*p) ) {
		case 0: goto st0;
		case 58: goto st12;
		case 93: goto st0;
	}
	goto tr15;
st12:
	p += 1;
case 12:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr17;
	goto st0;
tr17:
#line 44 "parser_osc.rl"
	{ (rl_port *= 10) += ((*p) - '0'); }
#line 50 "parser_osc.rl"
	{ rl_addr = OSC_ADDR_URL; }
	goto st13;
st13:
	p += 1;
case 13:
#line 152 "parser_osc.cpp"
	if ( (*p) == 0 )
		goto tr18;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr17;
	goto st0;
tr18:
#line 54 "parser_osc.rl"
	{ {p++; cs = 33; goto _out;} }
	goto st33;
st33:
	p += 1;
case 33:
#line 165 "parser_osc.cpp"
	goto st0;
st14:
	p += 1;
case 14:
	switch( (*p) ) {
		case 100: goto st15;
		case 110: goto st17;
	}
	goto st0;
st15:
	p += 1;
case 15:
	if ( (*p) == 112 )
		goto st16;
	goto st0;
st16:
	p += 1;
case 16:
	if ( (*p) == 58 )
		goto tr22;
	goto st0;
st17:
	p += 1;
case 17:
	if ( (*p) == 105 )
		goto st18;
	goto st0;
st18:
	p += 1;
case 18:
	if ( (*p) == 120 )
		goto st19;
	goto st0;
st19:
	p += 1;
case 19:
	if ( (*p) == 58 )
		goto tr25;
	goto st0;
tr25:
#line 42 "parser_osc.rl"
	{ rl_proto = OSC_PROTO_UNIX; }
	goto st20;
st20:
	p += 1;
case 20:
#line 212 "parser_osc.cpp"
	if ( (*p) == 47 )
		goto st21;
	goto st0;
st21:
	p += 1;
case 21:
	if ( (*p) == 47 )
		goto st22;
	goto st0;
st22:
	p += 1;
case 22:
	switch( (*p) ) {
		case 0: goto st0;
		case 58: goto st24;
	}
	goto tr28;
tr28:
#line 47 "parser_osc.rl"
	{ rl_path.push_back((*p)); }
#line 51 "parser_osc.rl"
	{ rl_addr = OSC_ADDR_UNIX; }
	goto st23;
st23:
	p += 1;
case 23:
#line 239 "parser_osc.cpp"
	if ( (*p) == 0 )
		goto tr18;
	goto tr28;
st24:
	p += 1;
case 24:
	switch( (*p) ) {
		case 0: goto st0;
		case 58: goto st0;
	}
	goto tr28;
st25:
	p += 1;
case 25:
	if ( (*p) == 99 )
		goto st26;
	goto st0;
st26:
	p += 1;
case 26:
	if ( (*p) == 112 )
		goto st27;
	goto st0;
st27:
	p += 1;
case 27:
	if ( (*p) == 58 )
		goto tr32;
	goto st0;
tr32:
#line 39 "parser_osc.rl"
	{ rl_proto = OSC_PROTO_TCP; }
	goto st28;
tr36:
#line 40 "parser_osc.rl"
	{ rl_proto = OSC_PROTO_UDP; }
	goto st28;
st28:
	p += 1;
case 28:
#line 280 "parser_osc.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr33;
	goto st0;
tr33:
#line 44 "parser_osc.rl"
	{ (rl_port *= 10) += ((*p) - '0'); }
#line 52 "parser_osc.rl"
	{ rl_addr = OSC_ADDR_PROTO_PORT; }
	goto st29;
st29:
	p += 1;
case 29:
#line 293 "parser_osc.cpp"
	if ( (*p) == 0 )
		goto tr18;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr33;
	goto st0;
st30:
	p += 1;
case 30:
	if ( (*p) == 100 )
		goto st31;
	goto st0;
st31:
	p += 1;
case 31:
	if ( (*p) == 112 )
		goto st32;
	goto st0;
st32:
	p += 1;
case 32:
	if ( (*p) == 58 )
		goto tr36;
	goto st0;
	}

	_out: {}
	}

#line 76 "parser_osc.rl"

        if (cs >= 33) {
            rl_host.push_back('\0');
            rl_path.push_back('\0');
            proto = rl_proto;
            host = gensym(rl_host.data());
            port = rl_port;
            unix_path = gensym(rl_path.data());
        }

        return rl_addr;
    }
}
}

