
#line 1 "new_file_pattern.rl"
# include "new_file_pattern.h"
# include "ceammc_log.h"

# include <cmath>
# include <cstring>
# include <ctime>
# include <limits>

# include "fmt/core.h"


#line 15 "new_file_pattern.cpp"
static const int expand_filename_pattern_start = 63;
static const int expand_filename_pattern_first_final = 63;
static const int expand_filename_pattern_error = 0;

static const int expand_filename_pattern_en_main = 63;


#line 113 "new_file_pattern.rl"


namespace ceammc {
namespace path {

PatternStatus expand_filename_pattern(const std::string& pattern, std::string& res, uint32_t idx)
{
    int cs = 0;
    auto* p = pattern.data();
    auto* pe = p + pattern.size();
    auto* eof = pe;

    auto tm = std::time(nullptr);
    auto rl_now = *std::localtime(&tm);
    bool rl_pattern_found = false;
    bool rl_date_found = false;
    const char* rl_err = 0;
    int rl_pat_cnum = 0;
    std::string rl_fname;
    rl_fname.reserve(pattern.size());

    
#line 46 "new_file_pattern.cpp"
	{
	cs = expand_filename_pattern_start;
	}

#line 135 "new_file_pattern.rl"
    
#line 53 "new_file_pattern.cpp"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
tr81:
#line 104 "new_file_pattern.rl"
	{ rl_fname += (*p); }
	goto st63;
tr109:
#line 98 "new_file_pattern.rl"
	{ rl_fname += '%'; }
#line 100 "new_file_pattern.rl"
	{ rl_date_found = true; }
#line 104 "new_file_pattern.rl"
	{ rl_fname += (*p); }
	goto st63;
tr112:
#line 71 "new_file_pattern.rl"
	{
    char buf[32];
    if (std::strftime(buf, sizeof(buf), "%Y-%m-%d", &rl_now))
        rl_fname.append(buf);
}
#line 100 "new_file_pattern.rl"
	{ rl_date_found = true; }
#line 104 "new_file_pattern.rl"
	{ rl_fname += (*p); }
	goto st63;
tr115:
#line 47 "new_file_pattern.rl"
	{
    char buf[8];
    if (std::strftime(buf, sizeof(buf), "%d", &rl_now))
        rl_fname.append(buf);
}
#line 100 "new_file_pattern.rl"
	{ rl_date_found = true; }
#line 104 "new_file_pattern.rl"
	{ rl_fname += (*p); }
	goto st63;
tr118:
#line 53 "new_file_pattern.rl"
	{
    char buf[8];
    if (std::strftime(buf, sizeof(buf), "%H", &rl_now))
        rl_fname.append(buf);
}
#line 100 "new_file_pattern.rl"
	{ rl_date_found = true; }
#line 104 "new_file_pattern.rl"
	{ rl_fname += (*p); }
	goto st63;
tr121:
#line 59 "new_file_pattern.rl"
	{
    char buf[8];
    if (std::strftime(buf, sizeof(buf), "%M", &rl_now))
        rl_fname.append(buf);
}
#line 100 "new_file_pattern.rl"
	{ rl_date_found = true; }
#line 104 "new_file_pattern.rl"
	{ rl_fname += (*p); }
	goto st63;
tr124:
#line 40 "new_file_pattern.rl"
	{
    std::time_t t = std::time(nullptr);
    char buf[8];
    if (std::strftime(buf, sizeof(buf), "%m", &rl_now))
        rl_fname.append(buf);
}
#line 100 "new_file_pattern.rl"
	{ rl_date_found = true; }
#line 104 "new_file_pattern.rl"
	{ rl_fname += (*p); }
	goto st63;
tr127:
#line 83 "new_file_pattern.rl"
	{
    char buf[64];
    if (std::strftime(buf, sizeof(buf), "%Y-%m-%d_%H-%M-%S", &rl_now))
        rl_fname.append(buf);
}
#line 100 "new_file_pattern.rl"
	{ rl_date_found = true; }
#line 104 "new_file_pattern.rl"
	{ rl_fname += (*p); }
	goto st63;
tr130:
#line 65 "new_file_pattern.rl"
	{
    char buf[8];
    if (std::strftime(buf, sizeof(buf), "%S", &rl_now))
        rl_fname.append(buf);
}
#line 100 "new_file_pattern.rl"
	{ rl_date_found = true; }
#line 104 "new_file_pattern.rl"
	{ rl_fname += (*p); }
	goto st63;
tr133:
#line 77 "new_file_pattern.rl"
	{
    char buf[32];
    if (std::strftime(buf, sizeof(buf), "%H-%M-%S", &rl_now))
        rl_fname.append(buf);
}
#line 100 "new_file_pattern.rl"
	{ rl_date_found = true; }
#line 104 "new_file_pattern.rl"
	{ rl_fname += (*p); }
	goto st63;
tr136:
#line 33 "new_file_pattern.rl"
	{
    std::time_t t = std::time(nullptr);
    char buf[8];
    if (std::strftime(buf, sizeof(buf), "%Y", &rl_now))
        rl_fname.append(buf);
}
#line 100 "new_file_pattern.rl"
	{ rl_date_found = true; }
#line 104 "new_file_pattern.rl"
	{ rl_fname += (*p); }
	goto st63;
st63:
	if ( ++p == pe )
		goto _test_eof63;
case 63:
#line 185 "new_file_pattern.cpp"
	switch( (*p) ) {
		case 35: goto tr82;
		case 37: goto tr83;
	}
	goto tr81;
tr82:
#line 107 "new_file_pattern.rl"
	{ rl_pat_cnum++; }
	goto st64;
tr110:
#line 98 "new_file_pattern.rl"
	{ rl_fname += '%'; }
#line 100 "new_file_pattern.rl"
	{ rl_date_found = true; }
#line 107 "new_file_pattern.rl"
	{ rl_pat_cnum++; }
	goto st64;
tr113:
#line 71 "new_file_pattern.rl"
	{
    char buf[32];
    if (std::strftime(buf, sizeof(buf), "%Y-%m-%d", &rl_now))
        rl_fname.append(buf);
}
#line 100 "new_file_pattern.rl"
	{ rl_date_found = true; }
#line 107 "new_file_pattern.rl"
	{ rl_pat_cnum++; }
	goto st64;
tr116:
#line 47 "new_file_pattern.rl"
	{
    char buf[8];
    if (std::strftime(buf, sizeof(buf), "%d", &rl_now))
        rl_fname.append(buf);
}
#line 100 "new_file_pattern.rl"
	{ rl_date_found = true; }
#line 107 "new_file_pattern.rl"
	{ rl_pat_cnum++; }
	goto st64;
tr119:
#line 53 "new_file_pattern.rl"
	{
    char buf[8];
    if (std::strftime(buf, sizeof(buf), "%H", &rl_now))
        rl_fname.append(buf);
}
#line 100 "new_file_pattern.rl"
	{ rl_date_found = true; }
#line 107 "new_file_pattern.rl"
	{ rl_pat_cnum++; }
	goto st64;
tr122:
#line 59 "new_file_pattern.rl"
	{
    char buf[8];
    if (std::strftime(buf, sizeof(buf), "%M", &rl_now))
        rl_fname.append(buf);
}
#line 100 "new_file_pattern.rl"
	{ rl_date_found = true; }
#line 107 "new_file_pattern.rl"
	{ rl_pat_cnum++; }
	goto st64;
tr125:
#line 40 "new_file_pattern.rl"
	{
    std::time_t t = std::time(nullptr);
    char buf[8];
    if (std::strftime(buf, sizeof(buf), "%m", &rl_now))
        rl_fname.append(buf);
}
#line 100 "new_file_pattern.rl"
	{ rl_date_found = true; }
#line 107 "new_file_pattern.rl"
	{ rl_pat_cnum++; }
	goto st64;
tr128:
#line 83 "new_file_pattern.rl"
	{
    char buf[64];
    if (std::strftime(buf, sizeof(buf), "%Y-%m-%d_%H-%M-%S", &rl_now))
        rl_fname.append(buf);
}
#line 100 "new_file_pattern.rl"
	{ rl_date_found = true; }
#line 107 "new_file_pattern.rl"
	{ rl_pat_cnum++; }
	goto st64;
tr131:
#line 65 "new_file_pattern.rl"
	{
    char buf[8];
    if (std::strftime(buf, sizeof(buf), "%S", &rl_now))
        rl_fname.append(buf);
}
#line 100 "new_file_pattern.rl"
	{ rl_date_found = true; }
#line 107 "new_file_pattern.rl"
	{ rl_pat_cnum++; }
	goto st64;
tr134:
#line 77 "new_file_pattern.rl"
	{
    char buf[32];
    if (std::strftime(buf, sizeof(buf), "%H-%M-%S", &rl_now))
        rl_fname.append(buf);
}
#line 100 "new_file_pattern.rl"
	{ rl_date_found = true; }
#line 107 "new_file_pattern.rl"
	{ rl_pat_cnum++; }
	goto st64;
tr137:
#line 33 "new_file_pattern.rl"
	{
    std::time_t t = std::time(nullptr);
    char buf[8];
    if (std::strftime(buf, sizeof(buf), "%Y", &rl_now))
        rl_fname.append(buf);
}
#line 100 "new_file_pattern.rl"
	{ rl_date_found = true; }
#line 107 "new_file_pattern.rl"
	{ rl_pat_cnum++; }
	goto st64;
st64:
	if ( ++p == pe )
		goto _test_eof64;
case 64:
#line 317 "new_file_pattern.cpp"
	switch( (*p) ) {
		case 35: goto tr82;
		case 37: goto tr85;
	}
	goto tr84;
tr86:
#line 104 "new_file_pattern.rl"
	{ rl_fname += (*p); }
	goto st65;
tr84:
#line 15 "new_file_pattern.rl"
	{
    using IndexT = decltype(idx);
    constexpr auto NDIGITS10 = std::numeric_limits<IndexT>::digits10;
    if (rl_pat_cnum > NDIGITS10) {
        LIB_ERR << fmt::format("[{}] '#'-pattern is too long: {}, max length is {}", __FUNCTION__, rl_pat_cnum, NDIGITS10);
        return PATTERN_LENGTH_OVERFLOW;
    }

    char buf[NDIGITS10+1];
    buf[rl_pat_cnum] = '\0';
    for (IndexT i = 0, dig = 1; i < rl_pat_cnum; i++, dig *= 10)
        buf[rl_pat_cnum - (i+1)] = ('0' + (idx / dig) % 10);


    rl_fname.append(buf);
    rl_pattern_found = true;
}
#line 104 "new_file_pattern.rl"
	{ rl_fname += (*p); }
	goto st65;
tr89:
#line 98 "new_file_pattern.rl"
	{ rl_fname += '%'; }
#line 100 "new_file_pattern.rl"
	{ rl_date_found = true; }
#line 104 "new_file_pattern.rl"
	{ rl_fname += (*p); }
	goto st65;
tr91:
#line 71 "new_file_pattern.rl"
	{
    char buf[32];
    if (std::strftime(buf, sizeof(buf), "%Y-%m-%d", &rl_now))
        rl_fname.append(buf);
}
#line 100 "new_file_pattern.rl"
	{ rl_date_found = true; }
#line 104 "new_file_pattern.rl"
	{ rl_fname += (*p); }
	goto st65;
tr93:
#line 47 "new_file_pattern.rl"
	{
    char buf[8];
    if (std::strftime(buf, sizeof(buf), "%d", &rl_now))
        rl_fname.append(buf);
}
#line 100 "new_file_pattern.rl"
	{ rl_date_found = true; }
#line 104 "new_file_pattern.rl"
	{ rl_fname += (*p); }
	goto st65;
tr95:
#line 53 "new_file_pattern.rl"
	{
    char buf[8];
    if (std::strftime(buf, sizeof(buf), "%H", &rl_now))
        rl_fname.append(buf);
}
#line 100 "new_file_pattern.rl"
	{ rl_date_found = true; }
#line 104 "new_file_pattern.rl"
	{ rl_fname += (*p); }
	goto st65;
tr97:
#line 59 "new_file_pattern.rl"
	{
    char buf[8];
    if (std::strftime(buf, sizeof(buf), "%M", &rl_now))
        rl_fname.append(buf);
}
#line 100 "new_file_pattern.rl"
	{ rl_date_found = true; }
#line 104 "new_file_pattern.rl"
	{ rl_fname += (*p); }
	goto st65;
tr99:
#line 40 "new_file_pattern.rl"
	{
    std::time_t t = std::time(nullptr);
    char buf[8];
    if (std::strftime(buf, sizeof(buf), "%m", &rl_now))
        rl_fname.append(buf);
}
#line 100 "new_file_pattern.rl"
	{ rl_date_found = true; }
#line 104 "new_file_pattern.rl"
	{ rl_fname += (*p); }
	goto st65;
tr101:
#line 83 "new_file_pattern.rl"
	{
    char buf[64];
    if (std::strftime(buf, sizeof(buf), "%Y-%m-%d_%H-%M-%S", &rl_now))
        rl_fname.append(buf);
}
#line 100 "new_file_pattern.rl"
	{ rl_date_found = true; }
#line 104 "new_file_pattern.rl"
	{ rl_fname += (*p); }
	goto st65;
tr103:
#line 65 "new_file_pattern.rl"
	{
    char buf[8];
    if (std::strftime(buf, sizeof(buf), "%S", &rl_now))
        rl_fname.append(buf);
}
#line 100 "new_file_pattern.rl"
	{ rl_date_found = true; }
#line 104 "new_file_pattern.rl"
	{ rl_fname += (*p); }
	goto st65;
tr105:
#line 77 "new_file_pattern.rl"
	{
    char buf[32];
    if (std::strftime(buf, sizeof(buf), "%H-%M-%S", &rl_now))
        rl_fname.append(buf);
}
#line 100 "new_file_pattern.rl"
	{ rl_date_found = true; }
#line 104 "new_file_pattern.rl"
	{ rl_fname += (*p); }
	goto st65;
tr107:
#line 33 "new_file_pattern.rl"
	{
    std::time_t t = std::time(nullptr);
    char buf[8];
    if (std::strftime(buf, sizeof(buf), "%Y", &rl_now))
        rl_fname.append(buf);
}
#line 100 "new_file_pattern.rl"
	{ rl_date_found = true; }
#line 104 "new_file_pattern.rl"
	{ rl_fname += (*p); }
	goto st65;
st65:
	if ( ++p == pe )
		goto _test_eof65;
case 65:
#line 471 "new_file_pattern.cpp"
	switch( (*p) ) {
		case 35: goto st0;
		case 37: goto tr88;
	}
	goto tr86;
tr0:
#line 101 "new_file_pattern.rl"
	{ rl_fname += '%'; p = rl_err; {goto st63;} }
	goto st0;
#line 481 "new_file_pattern.cpp"
st0:
cs = 0;
	goto _out;
tr88:
#line 99 "new_file_pattern.rl"
	{ rl_err = p; }
	goto st1;
tr85:
#line 15 "new_file_pattern.rl"
	{
    using IndexT = decltype(idx);
    constexpr auto NDIGITS10 = std::numeric_limits<IndexT>::digits10;
    if (rl_pat_cnum > NDIGITS10) {
        LIB_ERR << fmt::format("[{}] '#'-pattern is too long: {}, max length is {}", __FUNCTION__, rl_pat_cnum, NDIGITS10);
        return PATTERN_LENGTH_OVERFLOW;
    }

    char buf[NDIGITS10+1];
    buf[rl_pat_cnum] = '\0';
    for (IndexT i = 0, dig = 1; i < rl_pat_cnum; i++, dig *= 10)
        buf[rl_pat_cnum - (i+1)] = ('0' + (idx / dig) % 10);


    rl_fname.append(buf);
    rl_pattern_found = true;
}
#line 99 "new_file_pattern.rl"
	{ rl_err = p; }
	goto st1;
tr90:
#line 98 "new_file_pattern.rl"
	{ rl_fname += '%'; }
#line 100 "new_file_pattern.rl"
	{ rl_date_found = true; }
#line 99 "new_file_pattern.rl"
	{ rl_err = p; }
	goto st1;
tr92:
#line 71 "new_file_pattern.rl"
	{
    char buf[32];
    if (std::strftime(buf, sizeof(buf), "%Y-%m-%d", &rl_now))
        rl_fname.append(buf);
}
#line 100 "new_file_pattern.rl"
	{ rl_date_found = true; }
#line 99 "new_file_pattern.rl"
	{ rl_err = p; }
	goto st1;
tr94:
#line 47 "new_file_pattern.rl"
	{
    char buf[8];
    if (std::strftime(buf, sizeof(buf), "%d", &rl_now))
        rl_fname.append(buf);
}
#line 100 "new_file_pattern.rl"
	{ rl_date_found = true; }
#line 99 "new_file_pattern.rl"
	{ rl_err = p; }
	goto st1;
tr96:
#line 53 "new_file_pattern.rl"
	{
    char buf[8];
    if (std::strftime(buf, sizeof(buf), "%H", &rl_now))
        rl_fname.append(buf);
}
#line 100 "new_file_pattern.rl"
	{ rl_date_found = true; }
#line 99 "new_file_pattern.rl"
	{ rl_err = p; }
	goto st1;
tr98:
#line 59 "new_file_pattern.rl"
	{
    char buf[8];
    if (std::strftime(buf, sizeof(buf), "%M", &rl_now))
        rl_fname.append(buf);
}
#line 100 "new_file_pattern.rl"
	{ rl_date_found = true; }
#line 99 "new_file_pattern.rl"
	{ rl_err = p; }
	goto st1;
tr100:
#line 40 "new_file_pattern.rl"
	{
    std::time_t t = std::time(nullptr);
    char buf[8];
    if (std::strftime(buf, sizeof(buf), "%m", &rl_now))
        rl_fname.append(buf);
}
#line 100 "new_file_pattern.rl"
	{ rl_date_found = true; }
#line 99 "new_file_pattern.rl"
	{ rl_err = p; }
	goto st1;
tr102:
#line 83 "new_file_pattern.rl"
	{
    char buf[64];
    if (std::strftime(buf, sizeof(buf), "%Y-%m-%d_%H-%M-%S", &rl_now))
        rl_fname.append(buf);
}
#line 100 "new_file_pattern.rl"
	{ rl_date_found = true; }
#line 99 "new_file_pattern.rl"
	{ rl_err = p; }
	goto st1;
tr104:
#line 65 "new_file_pattern.rl"
	{
    char buf[8];
    if (std::strftime(buf, sizeof(buf), "%S", &rl_now))
        rl_fname.append(buf);
}
#line 100 "new_file_pattern.rl"
	{ rl_date_found = true; }
#line 99 "new_file_pattern.rl"
	{ rl_err = p; }
	goto st1;
tr106:
#line 77 "new_file_pattern.rl"
	{
    char buf[32];
    if (std::strftime(buf, sizeof(buf), "%H-%M-%S", &rl_now))
        rl_fname.append(buf);
}
#line 100 "new_file_pattern.rl"
	{ rl_date_found = true; }
#line 99 "new_file_pattern.rl"
	{ rl_err = p; }
	goto st1;
tr108:
#line 33 "new_file_pattern.rl"
	{
    std::time_t t = std::time(nullptr);
    char buf[8];
    if (std::strftime(buf, sizeof(buf), "%Y", &rl_now))
        rl_fname.append(buf);
}
#line 100 "new_file_pattern.rl"
	{ rl_date_found = true; }
#line 99 "new_file_pattern.rl"
	{ rl_err = p; }
	goto st1;
st1:
	if ( ++p == pe )
		goto _test_eof1;
case 1:
#line 633 "new_file_pattern.cpp"
	switch( (*p) ) {
		case 37: goto st66;
		case 68: goto st2;
		case 72: goto st7;
		case 77: goto st11;
		case 78: goto st18;
		case 83: goto st21;
		case 84: goto st24;
		case 89: goto st28;
	}
	goto tr0;
st66:
	if ( ++p == pe )
		goto _test_eof66;
case 66:
	switch( (*p) ) {
		case 35: goto tr0;
		case 37: goto tr90;
	}
	goto tr89;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
	if ( (*p) == 65 )
		goto st3;
	goto tr0;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
	switch( (*p) ) {
		case 84: goto st4;
		case 89: goto st6;
	}
	goto tr0;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
	if ( (*p) == 69 )
		goto st5;
	goto tr0;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
	if ( (*p) == 37 )
		goto st67;
	goto tr0;
st67:
	if ( ++p == pe )
		goto _test_eof67;
case 67:
	switch( (*p) ) {
		case 35: goto tr0;
		case 37: goto tr92;
	}
	goto tr91;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
	if ( (*p) == 37 )
		goto st68;
	goto tr0;
st68:
	if ( ++p == pe )
		goto _test_eof68;
case 68:
	switch( (*p) ) {
		case 35: goto tr0;
		case 37: goto tr94;
	}
	goto tr93;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
	if ( (*p) == 79 )
		goto st8;
	goto tr0;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
	if ( (*p) == 85 )
		goto st9;
	goto tr0;
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
	if ( (*p) == 82 )
		goto st10;
	goto tr0;
st10:
	if ( ++p == pe )
		goto _test_eof10;
case 10:
	if ( (*p) == 37 )
		goto st69;
	goto tr0;
st69:
	if ( ++p == pe )
		goto _test_eof69;
case 69:
	switch( (*p) ) {
		case 35: goto tr0;
		case 37: goto tr96;
	}
	goto tr95;
st11:
	if ( ++p == pe )
		goto _test_eof11;
case 11:
	switch( (*p) ) {
		case 73: goto st12;
		case 79: goto st14;
	}
	goto tr0;
st12:
	if ( ++p == pe )
		goto _test_eof12;
case 12:
	if ( (*p) == 78 )
		goto st13;
	goto tr0;
st13:
	if ( ++p == pe )
		goto _test_eof13;
case 13:
	if ( (*p) == 37 )
		goto st70;
	goto tr0;
st70:
	if ( ++p == pe )
		goto _test_eof70;
case 70:
	switch( (*p) ) {
		case 35: goto tr0;
		case 37: goto tr98;
	}
	goto tr97;
st14:
	if ( ++p == pe )
		goto _test_eof14;
case 14:
	if ( (*p) == 78 )
		goto st15;
	goto tr0;
st15:
	if ( ++p == pe )
		goto _test_eof15;
case 15:
	if ( (*p) == 84 )
		goto st16;
	goto tr0;
st16:
	if ( ++p == pe )
		goto _test_eof16;
case 16:
	if ( (*p) == 72 )
		goto st17;
	goto tr0;
st17:
	if ( ++p == pe )
		goto _test_eof17;
case 17:
	if ( (*p) == 37 )
		goto st71;
	goto tr0;
st71:
	if ( ++p == pe )
		goto _test_eof71;
case 71:
	switch( (*p) ) {
		case 35: goto tr0;
		case 37: goto tr100;
	}
	goto tr99;
st18:
	if ( ++p == pe )
		goto _test_eof18;
case 18:
	if ( (*p) == 79 )
		goto st19;
	goto tr0;
st19:
	if ( ++p == pe )
		goto _test_eof19;
case 19:
	if ( (*p) == 87 )
		goto st20;
	goto tr0;
st20:
	if ( ++p == pe )
		goto _test_eof20;
case 20:
	if ( (*p) == 37 )
		goto st72;
	goto tr0;
st72:
	if ( ++p == pe )
		goto _test_eof72;
case 72:
	switch( (*p) ) {
		case 35: goto tr0;
		case 37: goto tr102;
	}
	goto tr101;
st21:
	if ( ++p == pe )
		goto _test_eof21;
case 21:
	if ( (*p) == 69 )
		goto st22;
	goto tr0;
st22:
	if ( ++p == pe )
		goto _test_eof22;
case 22:
	if ( (*p) == 67 )
		goto st23;
	goto tr0;
st23:
	if ( ++p == pe )
		goto _test_eof23;
case 23:
	if ( (*p) == 37 )
		goto st73;
	goto tr0;
st73:
	if ( ++p == pe )
		goto _test_eof73;
case 73:
	switch( (*p) ) {
		case 35: goto tr0;
		case 37: goto tr104;
	}
	goto tr103;
st24:
	if ( ++p == pe )
		goto _test_eof24;
case 24:
	if ( (*p) == 73 )
		goto st25;
	goto tr0;
st25:
	if ( ++p == pe )
		goto _test_eof25;
case 25:
	if ( (*p) == 77 )
		goto st26;
	goto tr0;
st26:
	if ( ++p == pe )
		goto _test_eof26;
case 26:
	if ( (*p) == 69 )
		goto st27;
	goto tr0;
st27:
	if ( ++p == pe )
		goto _test_eof27;
case 27:
	if ( (*p) == 37 )
		goto st74;
	goto tr0;
st74:
	if ( ++p == pe )
		goto _test_eof74;
case 74:
	switch( (*p) ) {
		case 35: goto tr0;
		case 37: goto tr106;
	}
	goto tr105;
st28:
	if ( ++p == pe )
		goto _test_eof28;
case 28:
	if ( (*p) == 69 )
		goto st29;
	goto tr0;
st29:
	if ( ++p == pe )
		goto _test_eof29;
case 29:
	if ( (*p) == 65 )
		goto st30;
	goto tr0;
st30:
	if ( ++p == pe )
		goto _test_eof30;
case 30:
	if ( (*p) == 82 )
		goto st31;
	goto tr0;
st31:
	if ( ++p == pe )
		goto _test_eof31;
case 31:
	if ( (*p) == 37 )
		goto st75;
	goto tr0;
st75:
	if ( ++p == pe )
		goto _test_eof75;
case 75:
	switch( (*p) ) {
		case 35: goto tr0;
		case 37: goto tr108;
	}
	goto tr107;
tr83:
#line 99 "new_file_pattern.rl"
	{ rl_err = p; }
	goto st32;
tr111:
#line 98 "new_file_pattern.rl"
	{ rl_fname += '%'; }
#line 100 "new_file_pattern.rl"
	{ rl_date_found = true; }
#line 99 "new_file_pattern.rl"
	{ rl_err = p; }
	goto st32;
tr114:
#line 71 "new_file_pattern.rl"
	{
    char buf[32];
    if (std::strftime(buf, sizeof(buf), "%Y-%m-%d", &rl_now))
        rl_fname.append(buf);
}
#line 100 "new_file_pattern.rl"
	{ rl_date_found = true; }
#line 99 "new_file_pattern.rl"
	{ rl_err = p; }
	goto st32;
tr117:
#line 47 "new_file_pattern.rl"
	{
    char buf[8];
    if (std::strftime(buf, sizeof(buf), "%d", &rl_now))
        rl_fname.append(buf);
}
#line 100 "new_file_pattern.rl"
	{ rl_date_found = true; }
#line 99 "new_file_pattern.rl"
	{ rl_err = p; }
	goto st32;
tr120:
#line 53 "new_file_pattern.rl"
	{
    char buf[8];
    if (std::strftime(buf, sizeof(buf), "%H", &rl_now))
        rl_fname.append(buf);
}
#line 100 "new_file_pattern.rl"
	{ rl_date_found = true; }
#line 99 "new_file_pattern.rl"
	{ rl_err = p; }
	goto st32;
tr123:
#line 59 "new_file_pattern.rl"
	{
    char buf[8];
    if (std::strftime(buf, sizeof(buf), "%M", &rl_now))
        rl_fname.append(buf);
}
#line 100 "new_file_pattern.rl"
	{ rl_date_found = true; }
#line 99 "new_file_pattern.rl"
	{ rl_err = p; }
	goto st32;
tr126:
#line 40 "new_file_pattern.rl"
	{
    std::time_t t = std::time(nullptr);
    char buf[8];
    if (std::strftime(buf, sizeof(buf), "%m", &rl_now))
        rl_fname.append(buf);
}
#line 100 "new_file_pattern.rl"
	{ rl_date_found = true; }
#line 99 "new_file_pattern.rl"
	{ rl_err = p; }
	goto st32;
tr129:
#line 83 "new_file_pattern.rl"
	{
    char buf[64];
    if (std::strftime(buf, sizeof(buf), "%Y-%m-%d_%H-%M-%S", &rl_now))
        rl_fname.append(buf);
}
#line 100 "new_file_pattern.rl"
	{ rl_date_found = true; }
#line 99 "new_file_pattern.rl"
	{ rl_err = p; }
	goto st32;
tr132:
#line 65 "new_file_pattern.rl"
	{
    char buf[8];
    if (std::strftime(buf, sizeof(buf), "%S", &rl_now))
        rl_fname.append(buf);
}
#line 100 "new_file_pattern.rl"
	{ rl_date_found = true; }
#line 99 "new_file_pattern.rl"
	{ rl_err = p; }
	goto st32;
tr135:
#line 77 "new_file_pattern.rl"
	{
    char buf[32];
    if (std::strftime(buf, sizeof(buf), "%H-%M-%S", &rl_now))
        rl_fname.append(buf);
}
#line 100 "new_file_pattern.rl"
	{ rl_date_found = true; }
#line 99 "new_file_pattern.rl"
	{ rl_err = p; }
	goto st32;
tr138:
#line 33 "new_file_pattern.rl"
	{
    std::time_t t = std::time(nullptr);
    char buf[8];
    if (std::strftime(buf, sizeof(buf), "%Y", &rl_now))
        rl_fname.append(buf);
}
#line 100 "new_file_pattern.rl"
	{ rl_date_found = true; }
#line 99 "new_file_pattern.rl"
	{ rl_err = p; }
	goto st32;
st32:
	if ( ++p == pe )
		goto _test_eof32;
case 32:
#line 1075 "new_file_pattern.cpp"
	switch( (*p) ) {
		case 37: goto st76;
		case 68: goto st33;
		case 72: goto st38;
		case 77: goto st42;
		case 78: goto st49;
		case 83: goto st52;
		case 84: goto st55;
		case 89: goto st59;
	}
	goto tr0;
st76:
	if ( ++p == pe )
		goto _test_eof76;
case 76:
	switch( (*p) ) {
		case 35: goto tr110;
		case 37: goto tr111;
	}
	goto tr109;
st33:
	if ( ++p == pe )
		goto _test_eof33;
case 33:
	if ( (*p) == 65 )
		goto st34;
	goto tr0;
st34:
	if ( ++p == pe )
		goto _test_eof34;
case 34:
	switch( (*p) ) {
		case 84: goto st35;
		case 89: goto st37;
	}
	goto tr0;
st35:
	if ( ++p == pe )
		goto _test_eof35;
case 35:
	if ( (*p) == 69 )
		goto st36;
	goto tr0;
st36:
	if ( ++p == pe )
		goto _test_eof36;
case 36:
	if ( (*p) == 37 )
		goto st77;
	goto tr0;
st77:
	if ( ++p == pe )
		goto _test_eof77;
case 77:
	switch( (*p) ) {
		case 35: goto tr113;
		case 37: goto tr114;
	}
	goto tr112;
st37:
	if ( ++p == pe )
		goto _test_eof37;
case 37:
	if ( (*p) == 37 )
		goto st78;
	goto tr0;
st78:
	if ( ++p == pe )
		goto _test_eof78;
case 78:
	switch( (*p) ) {
		case 35: goto tr116;
		case 37: goto tr117;
	}
	goto tr115;
st38:
	if ( ++p == pe )
		goto _test_eof38;
case 38:
	if ( (*p) == 79 )
		goto st39;
	goto tr0;
st39:
	if ( ++p == pe )
		goto _test_eof39;
case 39:
	if ( (*p) == 85 )
		goto st40;
	goto tr0;
st40:
	if ( ++p == pe )
		goto _test_eof40;
case 40:
	if ( (*p) == 82 )
		goto st41;
	goto tr0;
st41:
	if ( ++p == pe )
		goto _test_eof41;
case 41:
	if ( (*p) == 37 )
		goto st79;
	goto tr0;
st79:
	if ( ++p == pe )
		goto _test_eof79;
case 79:
	switch( (*p) ) {
		case 35: goto tr119;
		case 37: goto tr120;
	}
	goto tr118;
st42:
	if ( ++p == pe )
		goto _test_eof42;
case 42:
	switch( (*p) ) {
		case 73: goto st43;
		case 79: goto st45;
	}
	goto tr0;
st43:
	if ( ++p == pe )
		goto _test_eof43;
case 43:
	if ( (*p) == 78 )
		goto st44;
	goto tr0;
st44:
	if ( ++p == pe )
		goto _test_eof44;
case 44:
	if ( (*p) == 37 )
		goto st80;
	goto tr0;
st80:
	if ( ++p == pe )
		goto _test_eof80;
case 80:
	switch( (*p) ) {
		case 35: goto tr122;
		case 37: goto tr123;
	}
	goto tr121;
st45:
	if ( ++p == pe )
		goto _test_eof45;
case 45:
	if ( (*p) == 78 )
		goto st46;
	goto tr0;
st46:
	if ( ++p == pe )
		goto _test_eof46;
case 46:
	if ( (*p) == 84 )
		goto st47;
	goto tr0;
st47:
	if ( ++p == pe )
		goto _test_eof47;
case 47:
	if ( (*p) == 72 )
		goto st48;
	goto tr0;
st48:
	if ( ++p == pe )
		goto _test_eof48;
case 48:
	if ( (*p) == 37 )
		goto st81;
	goto tr0;
st81:
	if ( ++p == pe )
		goto _test_eof81;
case 81:
	switch( (*p) ) {
		case 35: goto tr125;
		case 37: goto tr126;
	}
	goto tr124;
st49:
	if ( ++p == pe )
		goto _test_eof49;
case 49:
	if ( (*p) == 79 )
		goto st50;
	goto tr0;
st50:
	if ( ++p == pe )
		goto _test_eof50;
case 50:
	if ( (*p) == 87 )
		goto st51;
	goto tr0;
st51:
	if ( ++p == pe )
		goto _test_eof51;
case 51:
	if ( (*p) == 37 )
		goto st82;
	goto tr0;
st82:
	if ( ++p == pe )
		goto _test_eof82;
case 82:
	switch( (*p) ) {
		case 35: goto tr128;
		case 37: goto tr129;
	}
	goto tr127;
st52:
	if ( ++p == pe )
		goto _test_eof52;
case 52:
	if ( (*p) == 69 )
		goto st53;
	goto tr0;
st53:
	if ( ++p == pe )
		goto _test_eof53;
case 53:
	if ( (*p) == 67 )
		goto st54;
	goto tr0;
st54:
	if ( ++p == pe )
		goto _test_eof54;
case 54:
	if ( (*p) == 37 )
		goto st83;
	goto tr0;
st83:
	if ( ++p == pe )
		goto _test_eof83;
case 83:
	switch( (*p) ) {
		case 35: goto tr131;
		case 37: goto tr132;
	}
	goto tr130;
st55:
	if ( ++p == pe )
		goto _test_eof55;
case 55:
	if ( (*p) == 73 )
		goto st56;
	goto tr0;
st56:
	if ( ++p == pe )
		goto _test_eof56;
case 56:
	if ( (*p) == 77 )
		goto st57;
	goto tr0;
st57:
	if ( ++p == pe )
		goto _test_eof57;
case 57:
	if ( (*p) == 69 )
		goto st58;
	goto tr0;
st58:
	if ( ++p == pe )
		goto _test_eof58;
case 58:
	if ( (*p) == 37 )
		goto st84;
	goto tr0;
st84:
	if ( ++p == pe )
		goto _test_eof84;
case 84:
	switch( (*p) ) {
		case 35: goto tr134;
		case 37: goto tr135;
	}
	goto tr133;
st59:
	if ( ++p == pe )
		goto _test_eof59;
case 59:
	if ( (*p) == 69 )
		goto st60;
	goto tr0;
st60:
	if ( ++p == pe )
		goto _test_eof60;
case 60:
	if ( (*p) == 65 )
		goto st61;
	goto tr0;
st61:
	if ( ++p == pe )
		goto _test_eof61;
case 61:
	if ( (*p) == 82 )
		goto st62;
	goto tr0;
st62:
	if ( ++p == pe )
		goto _test_eof62;
case 62:
	if ( (*p) == 37 )
		goto st85;
	goto tr0;
st85:
	if ( ++p == pe )
		goto _test_eof85;
case 85:
	switch( (*p) ) {
		case 35: goto tr137;
		case 37: goto tr138;
	}
	goto tr136;
	}
	_test_eof63: cs = 63; goto _test_eof; 
	_test_eof64: cs = 64; goto _test_eof; 
	_test_eof65: cs = 65; goto _test_eof; 
	_test_eof1: cs = 1; goto _test_eof; 
	_test_eof66: cs = 66; goto _test_eof; 
	_test_eof2: cs = 2; goto _test_eof; 
	_test_eof3: cs = 3; goto _test_eof; 
	_test_eof4: cs = 4; goto _test_eof; 
	_test_eof5: cs = 5; goto _test_eof; 
	_test_eof67: cs = 67; goto _test_eof; 
	_test_eof6: cs = 6; goto _test_eof; 
	_test_eof68: cs = 68; goto _test_eof; 
	_test_eof7: cs = 7; goto _test_eof; 
	_test_eof8: cs = 8; goto _test_eof; 
	_test_eof9: cs = 9; goto _test_eof; 
	_test_eof10: cs = 10; goto _test_eof; 
	_test_eof69: cs = 69; goto _test_eof; 
	_test_eof11: cs = 11; goto _test_eof; 
	_test_eof12: cs = 12; goto _test_eof; 
	_test_eof13: cs = 13; goto _test_eof; 
	_test_eof70: cs = 70; goto _test_eof; 
	_test_eof14: cs = 14; goto _test_eof; 
	_test_eof15: cs = 15; goto _test_eof; 
	_test_eof16: cs = 16; goto _test_eof; 
	_test_eof17: cs = 17; goto _test_eof; 
	_test_eof71: cs = 71; goto _test_eof; 
	_test_eof18: cs = 18; goto _test_eof; 
	_test_eof19: cs = 19; goto _test_eof; 
	_test_eof20: cs = 20; goto _test_eof; 
	_test_eof72: cs = 72; goto _test_eof; 
	_test_eof21: cs = 21; goto _test_eof; 
	_test_eof22: cs = 22; goto _test_eof; 
	_test_eof23: cs = 23; goto _test_eof; 
	_test_eof73: cs = 73; goto _test_eof; 
	_test_eof24: cs = 24; goto _test_eof; 
	_test_eof25: cs = 25; goto _test_eof; 
	_test_eof26: cs = 26; goto _test_eof; 
	_test_eof27: cs = 27; goto _test_eof; 
	_test_eof74: cs = 74; goto _test_eof; 
	_test_eof28: cs = 28; goto _test_eof; 
	_test_eof29: cs = 29; goto _test_eof; 
	_test_eof30: cs = 30; goto _test_eof; 
	_test_eof31: cs = 31; goto _test_eof; 
	_test_eof75: cs = 75; goto _test_eof; 
	_test_eof32: cs = 32; goto _test_eof; 
	_test_eof76: cs = 76; goto _test_eof; 
	_test_eof33: cs = 33; goto _test_eof; 
	_test_eof34: cs = 34; goto _test_eof; 
	_test_eof35: cs = 35; goto _test_eof; 
	_test_eof36: cs = 36; goto _test_eof; 
	_test_eof77: cs = 77; goto _test_eof; 
	_test_eof37: cs = 37; goto _test_eof; 
	_test_eof78: cs = 78; goto _test_eof; 
	_test_eof38: cs = 38; goto _test_eof; 
	_test_eof39: cs = 39; goto _test_eof; 
	_test_eof40: cs = 40; goto _test_eof; 
	_test_eof41: cs = 41; goto _test_eof; 
	_test_eof79: cs = 79; goto _test_eof; 
	_test_eof42: cs = 42; goto _test_eof; 
	_test_eof43: cs = 43; goto _test_eof; 
	_test_eof44: cs = 44; goto _test_eof; 
	_test_eof80: cs = 80; goto _test_eof; 
	_test_eof45: cs = 45; goto _test_eof; 
	_test_eof46: cs = 46; goto _test_eof; 
	_test_eof47: cs = 47; goto _test_eof; 
	_test_eof48: cs = 48; goto _test_eof; 
	_test_eof81: cs = 81; goto _test_eof; 
	_test_eof49: cs = 49; goto _test_eof; 
	_test_eof50: cs = 50; goto _test_eof; 
	_test_eof51: cs = 51; goto _test_eof; 
	_test_eof82: cs = 82; goto _test_eof; 
	_test_eof52: cs = 52; goto _test_eof; 
	_test_eof53: cs = 53; goto _test_eof; 
	_test_eof54: cs = 54; goto _test_eof; 
	_test_eof83: cs = 83; goto _test_eof; 
	_test_eof55: cs = 55; goto _test_eof; 
	_test_eof56: cs = 56; goto _test_eof; 
	_test_eof57: cs = 57; goto _test_eof; 
	_test_eof58: cs = 58; goto _test_eof; 
	_test_eof84: cs = 84; goto _test_eof; 
	_test_eof59: cs = 59; goto _test_eof; 
	_test_eof60: cs = 60; goto _test_eof; 
	_test_eof61: cs = 61; goto _test_eof; 
	_test_eof62: cs = 62; goto _test_eof; 
	_test_eof85: cs = 85; goto _test_eof; 

	_test_eof: {}
	if ( p == eof )
	{
	switch ( cs ) {
	case 64: 
#line 15 "new_file_pattern.rl"
	{
    using IndexT = decltype(idx);
    constexpr auto NDIGITS10 = std::numeric_limits<IndexT>::digits10;
    if (rl_pat_cnum > NDIGITS10) {
        LIB_ERR << fmt::format("[{}] '#'-pattern is too long: {}, max length is {}", __FUNCTION__, rl_pat_cnum, NDIGITS10);
        return PATTERN_LENGTH_OVERFLOW;
    }

    char buf[NDIGITS10+1];
    buf[rl_pat_cnum] = '\0';
    for (IndexT i = 0, dig = 1; i < rl_pat_cnum; i++, dig *= 10)
        buf[rl_pat_cnum - (i+1)] = ('0' + (idx / dig) % 10);


    rl_fname.append(buf);
    rl_pattern_found = true;
}
	break;
	case 1: 
	case 2: 
	case 3: 
	case 4: 
	case 5: 
	case 6: 
	case 7: 
	case 8: 
	case 9: 
	case 10: 
	case 11: 
	case 12: 
	case 13: 
	case 14: 
	case 15: 
	case 16: 
	case 17: 
	case 18: 
	case 19: 
	case 20: 
	case 21: 
	case 22: 
	case 23: 
	case 24: 
	case 25: 
	case 26: 
	case 27: 
	case 28: 
	case 29: 
	case 30: 
	case 31: 
	case 32: 
	case 33: 
	case 34: 
	case 35: 
	case 36: 
	case 37: 
	case 38: 
	case 39: 
	case 40: 
	case 41: 
	case 42: 
	case 43: 
	case 44: 
	case 45: 
	case 46: 
	case 47: 
	case 48: 
	case 49: 
	case 50: 
	case 51: 
	case 52: 
	case 53: 
	case 54: 
	case 55: 
	case 56: 
	case 57: 
	case 58: 
	case 59: 
	case 60: 
	case 61: 
	case 62: 
#line 101 "new_file_pattern.rl"
	{ rl_fname += '%'; p = rl_err; {       if ( p == pe )
               goto _test_eof63;
goto st63;} }
	break;
	case 75: 
	case 85: 
#line 33 "new_file_pattern.rl"
	{
    std::time_t t = std::time(nullptr);
    char buf[8];
    if (std::strftime(buf, sizeof(buf), "%Y", &rl_now))
        rl_fname.append(buf);
}
#line 100 "new_file_pattern.rl"
	{ rl_date_found = true; }
	break;
	case 71: 
	case 81: 
#line 40 "new_file_pattern.rl"
	{
    std::time_t t = std::time(nullptr);
    char buf[8];
    if (std::strftime(buf, sizeof(buf), "%m", &rl_now))
        rl_fname.append(buf);
}
#line 100 "new_file_pattern.rl"
	{ rl_date_found = true; }
	break;
	case 68: 
	case 78: 
#line 47 "new_file_pattern.rl"
	{
    char buf[8];
    if (std::strftime(buf, sizeof(buf), "%d", &rl_now))
        rl_fname.append(buf);
}
#line 100 "new_file_pattern.rl"
	{ rl_date_found = true; }
	break;
	case 69: 
	case 79: 
#line 53 "new_file_pattern.rl"
	{
    char buf[8];
    if (std::strftime(buf, sizeof(buf), "%H", &rl_now))
        rl_fname.append(buf);
}
#line 100 "new_file_pattern.rl"
	{ rl_date_found = true; }
	break;
	case 70: 
	case 80: 
#line 59 "new_file_pattern.rl"
	{
    char buf[8];
    if (std::strftime(buf, sizeof(buf), "%M", &rl_now))
        rl_fname.append(buf);
}
#line 100 "new_file_pattern.rl"
	{ rl_date_found = true; }
	break;
	case 73: 
	case 83: 
#line 65 "new_file_pattern.rl"
	{
    char buf[8];
    if (std::strftime(buf, sizeof(buf), "%S", &rl_now))
        rl_fname.append(buf);
}
#line 100 "new_file_pattern.rl"
	{ rl_date_found = true; }
	break;
	case 67: 
	case 77: 
#line 71 "new_file_pattern.rl"
	{
    char buf[32];
    if (std::strftime(buf, sizeof(buf), "%Y-%m-%d", &rl_now))
        rl_fname.append(buf);
}
#line 100 "new_file_pattern.rl"
	{ rl_date_found = true; }
	break;
	case 74: 
	case 84: 
#line 77 "new_file_pattern.rl"
	{
    char buf[32];
    if (std::strftime(buf, sizeof(buf), "%H-%M-%S", &rl_now))
        rl_fname.append(buf);
}
#line 100 "new_file_pattern.rl"
	{ rl_date_found = true; }
	break;
	case 72: 
	case 82: 
#line 83 "new_file_pattern.rl"
	{
    char buf[64];
    if (std::strftime(buf, sizeof(buf), "%Y-%m-%d_%H-%M-%S", &rl_now))
        rl_fname.append(buf);
}
#line 100 "new_file_pattern.rl"
	{ rl_date_found = true; }
	break;
	case 66: 
	case 76: 
#line 98 "new_file_pattern.rl"
	{ rl_fname += '%'; }
#line 100 "new_file_pattern.rl"
	{ rl_date_found = true; }
	break;
#line 1677 "new_file_pattern.cpp"
	}
	}

	_out: {}
	}

#line 136 "new_file_pattern.rl"

    if ( cs >= 63) {
        if (rl_pattern_found || rl_date_found) {
            res = rl_fname;
            return PATTERN_DONE;
        } else {
            char suffix[8];
            snprintf(suffix, sizeof(suffix), "-%03d", idx % 1000);
            auto extpos = rl_fname.find_last_of(".");
            if (extpos == std::string::npos) {
                rl_fname += suffix;
            } else {
                rl_fname.insert(extpos, suffix);
            }
            res = rl_fname;
            return PATTERN_NOMATCH;
        }
    } else
        return PATTERN_ERROR;
}

}
}


