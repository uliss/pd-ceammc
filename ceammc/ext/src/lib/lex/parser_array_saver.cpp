
#line 1 "lex/parser_array_saver.rl"
# include "parser_array_saver.h"
# include "parser_numeric.h"
# include "ragel_common.h"
# include "ceammc_convert.h"

# include <algorithm>
# include <cmath>
# include <cstdint>
# include <string>

namespace {

using namespace ceammc::parser;

struct NumCleaner {
    fsm::NumericData& num_;
    ~NumCleaner() { num_ = {}; }
};

template<typename T>
inline T getValue(fsm::NumericData& num, AtomType type)
{
    NumCleaner cleaner { num };
    return (type != TYPE_FLOAT) ? num.getInteger() : num.getFloat();
}

inline size_t clipArraySize(std::int64_t v, size_t arraySize)
{
    return std::min<std::int64_t>(arraySize, std::max<std::int64_t>(0, v));
}

inline std::int64_t getUnitValue(ArraySaverParams& params, fsm::NumericData& num, AtomType type, size_t arraySize)
{
    switch (type) {
    case TYPE_PERCENT:
        return std::round(num.getFloat() * arraySize * 0.01);
    case TYPE_PHASE:
        return std::round(num.getFloat() * arraySize);
    case TYPE_FLOAT:
    case TYPE_INT:
    case TYPE_SAMP:
        return std::round(num.getFloat());
    case TYPE_MSEC:
        return std::round(num.getFloat() * 0.001 * params.in_sr);
    case TYPE_SEC:
        return std::round(num.getFloat() * params.in_sr);
    default:
        return 0;
    }
}

inline size_t getArrayAbsPos(ArraySaverParams& params, std::int64_t pos, std::int64_t arraySize)
{
    return (params.origin == ORIGIN_BEGIN) ? pos : (arraySize + pos);
}

inline size_t getArrayPos(ArraySaverParams& params, fsm::NumericData& num, AtomType type, size_t arraySize)
{
    NumCleaner cleaner { num };

    auto pos = getUnitValue(params, num, type, arraySize);
    return clipArraySize(getArrayAbsPos(params, pos, arraySize), arraySize);
}
}


#line 70 "lex/parser_array_saver.cpp"
static const int array_saver_start = 1;
static const int array_saver_first_final = 109;
static const int array_saver_error = 0;

static const int array_saver_en_main = 1;


#line 135 "lex/parser_array_saver.rl"


namespace ceammc {
namespace parser {

static sound::SoundFileFormat detect_format(const char* str, size_t len);

bool parse_array_saver_params(const char* str, size_t arraySize, ArraySaverParams& params)
{
    auto len = strlen(str);
    int cs = 0;
    const char* p = str;
    const char* pe = p + len;
    const char* eof = p + len;

    params.format = sound::FORMAT_UNKNOWN;
    params.sample_format = sound::SAMPLE_DEFAULT;

    std::string ragel_atom;
    double val = 0;

    DECLARE_RAGEL_COMMON_VARS;
    DECLARE_RAGEL_NUMERIC_VARS;


#line 104 "lex/parser_array_saver.cpp"
	{
	cs = array_saver_start;
	}

#line 109 "lex/parser_array_saver.cpp"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
case 1:
	switch( (*p) ) {
		case 32: goto st0;
		case 64: goto st0;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st0;
	goto tr0;
tr0:
#line 129 "lex/parser_array_saver.rl"
	{ ragel_atom.clear(); }
#line 75 "lex/parser_array_saver.rl"
	{ ragel_atom += (*p); }
	goto st109;
tr137:
#line 75 "lex/parser_array_saver.rl"
	{ ragel_atom += (*p); }
	goto st109;
st109:
	if ( ++p == pe )
		goto _test_eof109;
case 109:
#line 137 "lex/parser_array_saver.cpp"
	if ( (*p) == 32 )
		goto tr138;
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr138;
	goto tr137;
tr138:
#line 129 "lex/parser_array_saver.rl"
	{ params.filename = ragel_atom; }
	goto st2;
tr139:
#line 120 "lex/parser_array_saver.rl"
	{ params.format = sound::FORMAT_AIFF; }
	goto st2;
tr140:
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 71 "lex/parser_array_saver.rl"
	{ ragel_num.vdouble = ragel_num.vint; }
#line 110 "lex/parser_array_saver.rl"
	{ params.begin = getArrayPos(params, ragel_num, ragel_type, arraySize); }
	goto st2;
tr148:
#line 26 "lex/ragel_units.rl"
	{ragel_type = TYPE_PERCENT;}
#line 110 "lex/parser_array_saver.rl"
	{ params.begin = getArrayPos(params, ragel_num, ragel_type, arraySize); }
	goto st2;
tr149:
#line 29 "lex/ragel_units.rl"
	{ragel_type = TYPE_PHASE;}
#line 110 "lex/parser_array_saver.rl"
	{ params.begin = getArrayPos(params, ragel_num, ragel_type, arraySize); }
	goto st2;
tr150:
#line 58 "lex/ragel_numeric.rl"
	{
        const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
        ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
        ragel_type = TYPE_FLOAT;
        ragel_cat  = CAT_NUMBER;
    }
#line 110 "lex/parser_array_saver.rl"
	{ params.begin = getArrayPos(params, ragel_num, ragel_type, arraySize); }
	goto st2;
tr157:
#line 110 "lex/parser_array_saver.rl"
	{ params.begin = getArrayPos(params, ragel_num, ragel_type, arraySize); }
	goto st2;
tr161:
#line 19 "lex/ragel_units.rl"
	{ragel_type = TYPE_MSEC;}
#line 110 "lex/parser_array_saver.rl"
	{ params.begin = getArrayPos(params, ragel_num, ragel_type, arraySize); }
	goto st2;
tr164:
#line 22 "lex/ragel_units.rl"
	{ragel_type = TYPE_SEC;}
#line 110 "lex/parser_array_saver.rl"
	{ params.begin = getArrayPos(params, ragel_num, ragel_type, arraySize); }
	goto st2;
tr167:
#line 21 "lex/ragel_units.rl"
	{ragel_type = TYPE_SAMP;}
#line 110 "lex/parser_array_saver.rl"
	{ params.begin = getArrayPos(params, ragel_num, ragel_type, arraySize); }
	goto st2;
tr169:
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 71 "lex/parser_array_saver.rl"
	{ ragel_num.vdouble = ragel_num.vint; }
#line 111 "lex/parser_array_saver.rl"
	{ params.end = getArrayPos(params, ragel_num, ragel_type, arraySize); }
	goto st2;
tr177:
#line 26 "lex/ragel_units.rl"
	{ragel_type = TYPE_PERCENT;}
#line 111 "lex/parser_array_saver.rl"
	{ params.end = getArrayPos(params, ragel_num, ragel_type, arraySize); }
	goto st2;
tr178:
#line 29 "lex/ragel_units.rl"
	{ragel_type = TYPE_PHASE;}
#line 111 "lex/parser_array_saver.rl"
	{ params.end = getArrayPos(params, ragel_num, ragel_type, arraySize); }
	goto st2;
tr179:
#line 58 "lex/ragel_numeric.rl"
	{
        const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
        ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
        ragel_type = TYPE_FLOAT;
        ragel_cat  = CAT_NUMBER;
    }
#line 111 "lex/parser_array_saver.rl"
	{ params.end = getArrayPos(params, ragel_num, ragel_type, arraySize); }
	goto st2;
tr186:
#line 111 "lex/parser_array_saver.rl"
	{ params.end = getArrayPos(params, ragel_num, ragel_type, arraySize); }
	goto st2;
tr190:
#line 19 "lex/ragel_units.rl"
	{ragel_type = TYPE_MSEC;}
#line 111 "lex/parser_array_saver.rl"
	{ params.end = getArrayPos(params, ragel_num, ragel_type, arraySize); }
	goto st2;
tr193:
#line 22 "lex/ragel_units.rl"
	{ragel_type = TYPE_SEC;}
#line 111 "lex/parser_array_saver.rl"
	{ params.end = getArrayPos(params, ragel_num, ragel_type, arraySize); }
	goto st2;
tr196:
#line 21 "lex/ragel_units.rl"
	{ragel_type = TYPE_SAMP;}
#line 111 "lex/parser_array_saver.rl"
	{ params.end = getArrayPos(params, ragel_num, ragel_type, arraySize); }
	goto st2;
tr198:
#line 123 "lex/parser_array_saver.rl"
	{ params.format = sound::FORMAT_FLAC; }
	goto st2;
tr199:
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 79 "lex/parser_array_saver.rl"
	{ params.gain = getValue<float>(ragel_num, ragel_type); }
	goto st2;
tr203:
#line 58 "lex/ragel_numeric.rl"
	{
        const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
        ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
        ragel_type = TYPE_FLOAT;
        ragel_cat  = CAT_NUMBER;
    }
#line 79 "lex/parser_array_saver.rl"
	{ params.gain = getValue<float>(ragel_num, ragel_type); }
	goto st2;
tr207:
#line 14 "lex/ragel_units.rl"
	{ragel_type = TYPE_DB;}
#line 81 "lex/parser_array_saver.rl"
	{ params.gain = convert::dbfs2amp(params.gain); }
	goto st2;
tr209:
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 113 "lex/parser_array_saver.rl"
	{ params.in_sr  = getValue<float>(ragel_num, ragel_type); }
	goto st2;
tr211:
#line 58 "lex/ragel_numeric.rl"
	{
        const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
        ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
        ragel_type = TYPE_FLOAT;
        ragel_cat  = CAT_NUMBER;
    }
#line 113 "lex/parser_array_saver.rl"
	{ params.in_sr  = getValue<float>(ragel_num, ragel_type); }
	goto st2;
tr212:
#line 117 "lex/parser_array_saver.rl"
	{ params.format = sound::FORMAT_MP3; }
	goto st2;
tr213:
#line 115 "lex/parser_array_saver.rl"
	{ params.normalize = true; }
	goto st2;
tr215:
#line 118 "lex/parser_array_saver.rl"
	{ params.format = sound::FORMAT_OGG; }
	goto st2;
tr216:
#line 119 "lex/parser_array_saver.rl"
	{ params.format = sound::FORMAT_OPUS; }
	goto st2;
tr217:
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 114 "lex/parser_array_saver.rl"
	{ params.out_sr = getValue<float>(ragel_num, ragel_type); }
	goto st2;
tr219:
#line 58 "lex/ragel_numeric.rl"
	{
        const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
        ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
        ragel_type = TYPE_FLOAT;
        ragel_cat  = CAT_NUMBER;
    }
#line 114 "lex/parser_array_saver.rl"
	{ params.out_sr = getValue<float>(ragel_num, ragel_type); }
	goto st2;
tr220:
#line 124 "lex/parser_array_saver.rl"
	{ params.overwrite = true; }
	goto st2;
tr221:
#line 122 "lex/parser_array_saver.rl"
	{ params.format = sound::FORMAT_RAW; }
	goto st2;
tr222:
#line 83 "lex/parser_array_saver.rl"
	{ params.sample_format = sound::SAMPLE_DEFAULT; }
	goto st2;
tr223:
#line 85 "lex/parser_array_saver.rl"
	{ params.sample_format = sound::SAMPLE_PCM_16; }
	goto st2;
tr224:
#line 86 "lex/parser_array_saver.rl"
	{ params.sample_format = sound::SAMPLE_PCM_24; }
	goto st2;
tr225:
#line 87 "lex/parser_array_saver.rl"
	{ params.sample_format = sound::SAMPLE_PCM_32; }
	goto st2;
tr226:
#line 84 "lex/parser_array_saver.rl"
	{ params.sample_format = sound::SAMPLE_PCM_8; }
	goto st2;
tr227:
#line 88 "lex/parser_array_saver.rl"
	{ params.sample_format = sound::SAMPLE_PCM_FLOAT; }
	goto st2;
tr228:
#line 121 "lex/parser_array_saver.rl"
	{ params.format = sound::FORMAT_TEXT; }
	goto st2;
tr229:
#line 116 "lex/parser_array_saver.rl"
	{ params.format = sound::FORMAT_WAV; }
	goto st2;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
#line 398 "lex/parser_array_saver.cpp"
	switch( (*p) ) {
		case 32: goto st2;
		case 64: goto tr3;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st2;
	goto st0;
st0:
cs = 0;
	goto _out;
tr3:
#line 93 "lex/parser_array_saver.rl"
	{ params.origin = ORIGIN_BEGIN; }
#line 94 "lex/parser_array_saver.rl"
	{ params.origin = ORIGIN_BEGIN; }
	goto st3;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
#line 419 "lex/parser_array_saver.cpp"
	switch( (*p) ) {
		case 97: goto st4;
		case 98: goto st7;
		case 101: goto st28;
		case 102: goto st47;
		case 103: goto st50;
		case 105: goto st59;
		case 109: goto st67;
		case 110: goto st69;
		case 111: goto st76;
		case 114: goto st95;
		case 115: goto st97;
		case 116: goto st105;
		case 119: goto st107;
	}
	goto st0;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
	if ( (*p) == 105 )
		goto st5;
	goto st0;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
	if ( (*p) == 102 )
		goto st6;
	goto st0;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
	if ( (*p) == 102 )
		goto st110;
	goto st0;
st110:
	if ( ++p == pe )
		goto _test_eof110;
case 110:
	if ( (*p) == 32 )
		goto tr139;
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr139;
	goto st0;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
	if ( (*p) == 101 )
		goto st8;
	goto st0;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
	if ( (*p) == 103 )
		goto st9;
	goto st0;
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
	if ( (*p) == 105 )
		goto st10;
	goto st0;
st10:
	if ( ++p == pe )
		goto _test_eof10;
case 10:
	if ( (*p) == 110 )
		goto st11;
	goto st0;
st11:
	if ( ++p == pe )
		goto _test_eof11;
case 11:
	if ( (*p) == 32 )
		goto st12;
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st12;
	goto st0;
st12:
	if ( ++p == pe )
		goto _test_eof12;
case 12:
	switch( (*p) ) {
		case 32: goto st12;
		case 36: goto tr25;
		case 43: goto tr26;
		case 45: goto tr26;
		case 48: goto tr27;
	}
	if ( (*p) > 13 ) {
		if ( 49 <= (*p) && (*p) <= 57 )
			goto tr28;
	} else if ( (*p) >= 9 )
		goto st12;
	goto st0;
tr25:
#line 73 "lex/parser_array_saver.rl"
	{ params.origin = ORIGIN_END; }
	goto st13;
st13:
	if ( ++p == pe )
		goto _test_eof13;
case 13:
#line 528 "lex/parser_array_saver.cpp"
	switch( (*p) ) {
		case 43: goto tr26;
		case 45: goto tr26;
		case 48: goto tr27;
	}
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr28;
	goto st0;
tr26:
#line 21 "lex/ragel_numeric.rl"
	{ ragel_num.sign = ((*p)=='-') ? -1 : 1; }
	goto st14;
st14:
	if ( ++p == pe )
		goto _test_eof14;
case 14:
#line 545 "lex/parser_array_saver.cpp"
	if ( (*p) == 48 )
		goto tr27;
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr28;
	goto st0;
tr27:
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
	goto st111;
st111:
	if ( ++p == pe )
		goto _test_eof111;
case 111:
#line 559 "lex/parser_array_saver.cpp"
	switch( (*p) ) {
		case 32: goto tr140;
		case 37: goto tr141;
		case 42: goto tr142;
		case 46: goto tr143;
		case 95: goto tr144;
		case 109: goto tr145;
		case 112: goto tr146;
		case 115: goto tr147;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr140;
	goto st0;
tr141:
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 71 "lex/parser_array_saver.rl"
	{ ragel_num.vdouble = ragel_num.vint; }
	goto st112;
tr151:
#line 58 "lex/ragel_numeric.rl"
	{
        const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
        ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
        ragel_type = TYPE_FLOAT;
        ragel_cat  = CAT_NUMBER;
    }
	goto st112;
st112:
	if ( ++p == pe )
		goto _test_eof112;
case 112:
#line 596 "lex/parser_array_saver.cpp"
	if ( (*p) == 32 )
		goto tr148;
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr148;
	goto st0;
tr142:
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 71 "lex/parser_array_saver.rl"
	{ ragel_num.vdouble = ragel_num.vint; }
	goto st113;
tr152:
#line 58 "lex/ragel_numeric.rl"
	{
        const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
        ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
        ragel_type = TYPE_FLOAT;
        ragel_cat  = CAT_NUMBER;
    }
	goto st113;
st113:
	if ( ++p == pe )
		goto _test_eof113;
case 113:
#line 625 "lex/parser_array_saver.cpp"
	if ( (*p) == 32 )
		goto tr149;
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr149;
	goto st0;
tr143:
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 50 "lex/ragel_numeric.rl"
	{
        ragel_num.ratio.num = 0;
        ragel_num.ratio.den = 1;
    }
	goto st15;
st15:
	if ( ++p == pe )
		goto _test_eof15;
case 15:
#line 648 "lex/parser_array_saver.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr29;
	goto st0;
tr29:
#line 54 "lex/ragel_numeric.rl"
	{
        (ragel_num.ratio.num *= 10) += ((*p) - '0');
        ragel_num.ratio.den *= 10;
    }
	goto st114;
st114:
	if ( ++p == pe )
		goto _test_eof114;
case 114:
#line 663 "lex/parser_array_saver.cpp"
	switch( (*p) ) {
		case 32: goto tr150;
		case 37: goto tr151;
		case 42: goto tr152;
		case 95: goto tr153;
		case 109: goto tr154;
		case 112: goto tr155;
		case 115: goto tr156;
	}
	if ( (*p) > 13 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr29;
	} else if ( (*p) >= 9 )
		goto tr150;
	goto st0;
tr144:
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 71 "lex/parser_array_saver.rl"
	{ ragel_num.vdouble = ragel_num.vint; }
	goto st115;
tr153:
#line 58 "lex/ragel_numeric.rl"
	{
        const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
        ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
        ragel_type = TYPE_FLOAT;
        ragel_cat  = CAT_NUMBER;
    }
	goto st115;
st115:
	if ( ++p == pe )
		goto _test_eof115;
case 115:
#line 702 "lex/parser_array_saver.cpp"
	switch( (*p) ) {
		case 32: goto tr157;
		case 37: goto st112;
		case 42: goto st113;
		case 109: goto st16;
		case 112: goto st18;
		case 115: goto st119;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr157;
	goto st0;
tr145:
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 71 "lex/parser_array_saver.rl"
	{ ragel_num.vdouble = ragel_num.vint; }
	goto st16;
tr154:
#line 58 "lex/ragel_numeric.rl"
	{
        const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
        ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
        ragel_type = TYPE_FLOAT;
        ragel_cat  = CAT_NUMBER;
    }
	goto st16;
st16:
	if ( ++p == pe )
		goto _test_eof16;
case 16:
#line 737 "lex/parser_array_saver.cpp"
	if ( (*p) == 115 )
		goto st116;
	goto st0;
st116:
	if ( ++p == pe )
		goto _test_eof116;
case 116:
	switch( (*p) ) {
		case 32: goto tr161;
		case 101: goto st17;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr161;
	goto st0;
st17:
	if ( ++p == pe )
		goto _test_eof17;
case 17:
	if ( (*p) == 99 )
		goto st117;
	goto st0;
st117:
	if ( ++p == pe )
		goto _test_eof117;
case 117:
	if ( (*p) == 32 )
		goto tr161;
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr161;
	goto st0;
tr146:
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 71 "lex/parser_array_saver.rl"
	{ ragel_num.vdouble = ragel_num.vint; }
	goto st18;
tr155:
#line 58 "lex/ragel_numeric.rl"
	{
        const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
        ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
        ragel_type = TYPE_FLOAT;
        ragel_cat  = CAT_NUMBER;
    }
	goto st18;
st18:
	if ( ++p == pe )
		goto _test_eof18;
case 18:
#line 791 "lex/parser_array_saver.cpp"
	switch( (*p) ) {
		case 101: goto st19;
		case 104: goto st23;
	}
	goto st0;
st19:
	if ( ++p == pe )
		goto _test_eof19;
case 19:
	if ( (*p) == 114 )
		goto st20;
	goto st0;
st20:
	if ( ++p == pe )
		goto _test_eof20;
case 20:
	if ( (*p) == 99 )
		goto st118;
	goto st0;
st118:
	if ( ++p == pe )
		goto _test_eof118;
case 118:
	switch( (*p) ) {
		case 32: goto tr148;
		case 101: goto st21;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr148;
	goto st0;
st21:
	if ( ++p == pe )
		goto _test_eof21;
case 21:
	if ( (*p) == 110 )
		goto st22;
	goto st0;
st22:
	if ( ++p == pe )
		goto _test_eof22;
case 22:
	if ( (*p) == 116 )
		goto st112;
	goto st0;
st23:
	if ( ++p == pe )
		goto _test_eof23;
case 23:
	if ( (*p) == 97 )
		goto st24;
	goto st0;
st24:
	if ( ++p == pe )
		goto _test_eof24;
case 24:
	if ( (*p) == 115 )
		goto st25;
	goto st0;
st25:
	if ( ++p == pe )
		goto _test_eof25;
case 25:
	if ( (*p) == 101 )
		goto st113;
	goto st0;
tr147:
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 71 "lex/parser_array_saver.rl"
	{ ragel_num.vdouble = ragel_num.vint; }
	goto st119;
tr156:
#line 58 "lex/ragel_numeric.rl"
	{
        const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
        ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
        ragel_type = TYPE_FLOAT;
        ragel_cat  = CAT_NUMBER;
    }
	goto st119;
st119:
	if ( ++p == pe )
		goto _test_eof119;
case 119:
#line 880 "lex/parser_array_saver.cpp"
	switch( (*p) ) {
		case 32: goto tr164;
		case 97: goto st120;
		case 101: goto st27;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr164;
	goto st0;
st120:
	if ( ++p == pe )
		goto _test_eof120;
case 120:
	switch( (*p) ) {
		case 32: goto tr167;
		case 109: goto st26;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr167;
	goto st0;
st26:
	if ( ++p == pe )
		goto _test_eof26;
case 26:
	if ( (*p) == 112 )
		goto st121;
	goto st0;
st121:
	if ( ++p == pe )
		goto _test_eof121;
case 121:
	if ( (*p) == 32 )
		goto tr167;
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr167;
	goto st0;
st27:
	if ( ++p == pe )
		goto _test_eof27;
case 27:
	if ( (*p) == 99 )
		goto st122;
	goto st0;
st122:
	if ( ++p == pe )
		goto _test_eof122;
case 122:
	if ( (*p) == 32 )
		goto tr164;
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr164;
	goto st0;
tr28:
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
	goto st123;
st123:
	if ( ++p == pe )
		goto _test_eof123;
case 123:
#line 940 "lex/parser_array_saver.cpp"
	switch( (*p) ) {
		case 32: goto tr140;
		case 37: goto tr141;
		case 42: goto tr142;
		case 46: goto tr143;
		case 95: goto tr144;
		case 109: goto tr145;
		case 112: goto tr146;
		case 115: goto tr147;
	}
	if ( (*p) > 13 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr28;
	} else if ( (*p) >= 9 )
		goto tr140;
	goto st0;
st28:
	if ( ++p == pe )
		goto _test_eof28;
case 28:
	if ( (*p) == 110 )
		goto st29;
	goto st0;
st29:
	if ( ++p == pe )
		goto _test_eof29;
case 29:
	if ( (*p) == 100 )
		goto st30;
	goto st0;
st30:
	if ( ++p == pe )
		goto _test_eof30;
case 30:
	if ( (*p) == 32 )
		goto st31;
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st31;
	goto st0;
st31:
	if ( ++p == pe )
		goto _test_eof31;
case 31:
	switch( (*p) ) {
		case 32: goto st31;
		case 36: goto tr46;
		case 43: goto tr47;
		case 45: goto tr47;
		case 48: goto tr48;
	}
	if ( (*p) > 13 ) {
		if ( 49 <= (*p) && (*p) <= 57 )
			goto tr49;
	} else if ( (*p) >= 9 )
		goto st31;
	goto st0;
tr46:
#line 73 "lex/parser_array_saver.rl"
	{ params.origin = ORIGIN_END; }
	goto st32;
st32:
	if ( ++p == pe )
		goto _test_eof32;
case 32:
#line 1005 "lex/parser_array_saver.cpp"
	switch( (*p) ) {
		case 43: goto tr47;
		case 45: goto tr47;
		case 48: goto tr48;
	}
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr49;
	goto st0;
tr47:
#line 21 "lex/ragel_numeric.rl"
	{ ragel_num.sign = ((*p)=='-') ? -1 : 1; }
	goto st33;
st33:
	if ( ++p == pe )
		goto _test_eof33;
case 33:
#line 1022 "lex/parser_array_saver.cpp"
	if ( (*p) == 48 )
		goto tr48;
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr49;
	goto st0;
tr48:
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
	goto st124;
st124:
	if ( ++p == pe )
		goto _test_eof124;
case 124:
#line 1036 "lex/parser_array_saver.cpp"
	switch( (*p) ) {
		case 32: goto tr169;
		case 37: goto tr170;
		case 42: goto tr171;
		case 46: goto tr172;
		case 95: goto tr173;
		case 109: goto tr174;
		case 112: goto tr175;
		case 115: goto tr176;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr169;
	goto st0;
tr170:
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 71 "lex/parser_array_saver.rl"
	{ ragel_num.vdouble = ragel_num.vint; }
	goto st125;
tr180:
#line 58 "lex/ragel_numeric.rl"
	{
        const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
        ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
        ragel_type = TYPE_FLOAT;
        ragel_cat  = CAT_NUMBER;
    }
	goto st125;
st125:
	if ( ++p == pe )
		goto _test_eof125;
case 125:
#line 1073 "lex/parser_array_saver.cpp"
	if ( (*p) == 32 )
		goto tr177;
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr177;
	goto st0;
tr171:
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 71 "lex/parser_array_saver.rl"
	{ ragel_num.vdouble = ragel_num.vint; }
	goto st126;
tr181:
#line 58 "lex/ragel_numeric.rl"
	{
        const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
        ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
        ragel_type = TYPE_FLOAT;
        ragel_cat  = CAT_NUMBER;
    }
	goto st126;
st126:
	if ( ++p == pe )
		goto _test_eof126;
case 126:
#line 1102 "lex/parser_array_saver.cpp"
	if ( (*p) == 32 )
		goto tr178;
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr178;
	goto st0;
tr172:
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 50 "lex/ragel_numeric.rl"
	{
        ragel_num.ratio.num = 0;
        ragel_num.ratio.den = 1;
    }
	goto st34;
st34:
	if ( ++p == pe )
		goto _test_eof34;
case 34:
#line 1125 "lex/parser_array_saver.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr50;
	goto st0;
tr50:
#line 54 "lex/ragel_numeric.rl"
	{
        (ragel_num.ratio.num *= 10) += ((*p) - '0');
        ragel_num.ratio.den *= 10;
    }
	goto st127;
st127:
	if ( ++p == pe )
		goto _test_eof127;
case 127:
#line 1140 "lex/parser_array_saver.cpp"
	switch( (*p) ) {
		case 32: goto tr179;
		case 37: goto tr180;
		case 42: goto tr181;
		case 95: goto tr182;
		case 109: goto tr183;
		case 112: goto tr184;
		case 115: goto tr185;
	}
	if ( (*p) > 13 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr50;
	} else if ( (*p) >= 9 )
		goto tr179;
	goto st0;
tr173:
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 71 "lex/parser_array_saver.rl"
	{ ragel_num.vdouble = ragel_num.vint; }
	goto st128;
tr182:
#line 58 "lex/ragel_numeric.rl"
	{
        const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
        ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
        ragel_type = TYPE_FLOAT;
        ragel_cat  = CAT_NUMBER;
    }
	goto st128;
st128:
	if ( ++p == pe )
		goto _test_eof128;
case 128:
#line 1179 "lex/parser_array_saver.cpp"
	switch( (*p) ) {
		case 32: goto tr186;
		case 37: goto st125;
		case 42: goto st126;
		case 109: goto st35;
		case 112: goto st37;
		case 115: goto st132;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr186;
	goto st0;
tr174:
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 71 "lex/parser_array_saver.rl"
	{ ragel_num.vdouble = ragel_num.vint; }
	goto st35;
tr183:
#line 58 "lex/ragel_numeric.rl"
	{
        const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
        ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
        ragel_type = TYPE_FLOAT;
        ragel_cat  = CAT_NUMBER;
    }
	goto st35;
st35:
	if ( ++p == pe )
		goto _test_eof35;
case 35:
#line 1214 "lex/parser_array_saver.cpp"
	if ( (*p) == 115 )
		goto st129;
	goto st0;
st129:
	if ( ++p == pe )
		goto _test_eof129;
case 129:
	switch( (*p) ) {
		case 32: goto tr190;
		case 101: goto st36;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr190;
	goto st0;
st36:
	if ( ++p == pe )
		goto _test_eof36;
case 36:
	if ( (*p) == 99 )
		goto st130;
	goto st0;
st130:
	if ( ++p == pe )
		goto _test_eof130;
case 130:
	if ( (*p) == 32 )
		goto tr190;
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr190;
	goto st0;
tr175:
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 71 "lex/parser_array_saver.rl"
	{ ragel_num.vdouble = ragel_num.vint; }
	goto st37;
tr184:
#line 58 "lex/ragel_numeric.rl"
	{
        const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
        ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
        ragel_type = TYPE_FLOAT;
        ragel_cat  = CAT_NUMBER;
    }
	goto st37;
st37:
	if ( ++p == pe )
		goto _test_eof37;
case 37:
#line 1268 "lex/parser_array_saver.cpp"
	switch( (*p) ) {
		case 101: goto st38;
		case 104: goto st42;
	}
	goto st0;
st38:
	if ( ++p == pe )
		goto _test_eof38;
case 38:
	if ( (*p) == 114 )
		goto st39;
	goto st0;
st39:
	if ( ++p == pe )
		goto _test_eof39;
case 39:
	if ( (*p) == 99 )
		goto st131;
	goto st0;
st131:
	if ( ++p == pe )
		goto _test_eof131;
case 131:
	switch( (*p) ) {
		case 32: goto tr177;
		case 101: goto st40;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr177;
	goto st0;
st40:
	if ( ++p == pe )
		goto _test_eof40;
case 40:
	if ( (*p) == 110 )
		goto st41;
	goto st0;
st41:
	if ( ++p == pe )
		goto _test_eof41;
case 41:
	if ( (*p) == 116 )
		goto st125;
	goto st0;
st42:
	if ( ++p == pe )
		goto _test_eof42;
case 42:
	if ( (*p) == 97 )
		goto st43;
	goto st0;
st43:
	if ( ++p == pe )
		goto _test_eof43;
case 43:
	if ( (*p) == 115 )
		goto st44;
	goto st0;
st44:
	if ( ++p == pe )
		goto _test_eof44;
case 44:
	if ( (*p) == 101 )
		goto st126;
	goto st0;
tr176:
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 71 "lex/parser_array_saver.rl"
	{ ragel_num.vdouble = ragel_num.vint; }
	goto st132;
tr185:
#line 58 "lex/ragel_numeric.rl"
	{
        const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
        ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
        ragel_type = TYPE_FLOAT;
        ragel_cat  = CAT_NUMBER;
    }
	goto st132;
st132:
	if ( ++p == pe )
		goto _test_eof132;
case 132:
#line 1357 "lex/parser_array_saver.cpp"
	switch( (*p) ) {
		case 32: goto tr193;
		case 97: goto st133;
		case 101: goto st46;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr193;
	goto st0;
st133:
	if ( ++p == pe )
		goto _test_eof133;
case 133:
	switch( (*p) ) {
		case 32: goto tr196;
		case 109: goto st45;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr196;
	goto st0;
st45:
	if ( ++p == pe )
		goto _test_eof45;
case 45:
	if ( (*p) == 112 )
		goto st134;
	goto st0;
st134:
	if ( ++p == pe )
		goto _test_eof134;
case 134:
	if ( (*p) == 32 )
		goto tr196;
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr196;
	goto st0;
st46:
	if ( ++p == pe )
		goto _test_eof46;
case 46:
	if ( (*p) == 99 )
		goto st135;
	goto st0;
st135:
	if ( ++p == pe )
		goto _test_eof135;
case 135:
	if ( (*p) == 32 )
		goto tr193;
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr193;
	goto st0;
tr49:
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
	goto st136;
st136:
	if ( ++p == pe )
		goto _test_eof136;
case 136:
#line 1417 "lex/parser_array_saver.cpp"
	switch( (*p) ) {
		case 32: goto tr169;
		case 37: goto tr170;
		case 42: goto tr171;
		case 46: goto tr172;
		case 95: goto tr173;
		case 109: goto tr174;
		case 112: goto tr175;
		case 115: goto tr176;
	}
	if ( (*p) > 13 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr49;
	} else if ( (*p) >= 9 )
		goto tr169;
	goto st0;
st47:
	if ( ++p == pe )
		goto _test_eof47;
case 47:
	if ( (*p) == 108 )
		goto st48;
	goto st0;
st48:
	if ( ++p == pe )
		goto _test_eof48;
case 48:
	if ( (*p) == 97 )
		goto st49;
	goto st0;
st49:
	if ( ++p == pe )
		goto _test_eof49;
case 49:
	if ( (*p) == 99 )
		goto st137;
	goto st0;
st137:
	if ( ++p == pe )
		goto _test_eof137;
case 137:
	if ( (*p) == 32 )
		goto tr198;
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr198;
	goto st0;
st50:
	if ( ++p == pe )
		goto _test_eof50;
case 50:
	if ( (*p) == 97 )
		goto st51;
	goto st0;
st51:
	if ( ++p == pe )
		goto _test_eof51;
case 51:
	if ( (*p) == 105 )
		goto st52;
	goto st0;
st52:
	if ( ++p == pe )
		goto _test_eof52;
case 52:
	if ( (*p) == 110 )
		goto st53;
	goto st0;
st53:
	if ( ++p == pe )
		goto _test_eof53;
case 53:
	if ( (*p) == 32 )
		goto st54;
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st54;
	goto st0;
st54:
	if ( ++p == pe )
		goto _test_eof54;
case 54:
	switch( (*p) ) {
		case 32: goto st54;
		case 43: goto tr71;
		case 45: goto tr71;
		case 48: goto tr72;
	}
	if ( (*p) > 13 ) {
		if ( 49 <= (*p) && (*p) <= 57 )
			goto tr73;
	} else if ( (*p) >= 9 )
		goto st54;
	goto st0;
tr71:
#line 21 "lex/ragel_numeric.rl"
	{ ragel_num.sign = ((*p)=='-') ? -1 : 1; }
	goto st55;
st55:
	if ( ++p == pe )
		goto _test_eof55;
case 55:
#line 1518 "lex/parser_array_saver.cpp"
	if ( (*p) == 48 )
		goto tr72;
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr73;
	goto st0;
tr72:
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
	goto st138;
st138:
	if ( ++p == pe )
		goto _test_eof138;
case 138:
#line 1532 "lex/parser_array_saver.cpp"
	switch( (*p) ) {
		case 32: goto tr199;
		case 46: goto tr200;
		case 95: goto tr201;
		case 100: goto tr202;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr199;
	goto st0;
tr200:
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 50 "lex/ragel_numeric.rl"
	{
        ragel_num.ratio.num = 0;
        ragel_num.ratio.den = 1;
    }
	goto st56;
st56:
	if ( ++p == pe )
		goto _test_eof56;
case 56:
#line 1559 "lex/parser_array_saver.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr74;
	goto st0;
tr74:
#line 54 "lex/ragel_numeric.rl"
	{
        (ragel_num.ratio.num *= 10) += ((*p) - '0');
        ragel_num.ratio.den *= 10;
    }
	goto st139;
st139:
	if ( ++p == pe )
		goto _test_eof139;
case 139:
#line 1574 "lex/parser_array_saver.cpp"
	switch( (*p) ) {
		case 32: goto tr203;
		case 95: goto tr204;
		case 100: goto tr205;
	}
	if ( (*p) > 13 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr74;
	} else if ( (*p) >= 9 )
		goto tr203;
	goto st0;
tr201:
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 79 "lex/parser_array_saver.rl"
	{ params.gain = getValue<float>(ragel_num, ragel_type); }
	goto st140;
tr204:
#line 58 "lex/ragel_numeric.rl"
	{
        const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
        ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
        ragel_type = TYPE_FLOAT;
        ragel_cat  = CAT_NUMBER;
    }
#line 79 "lex/parser_array_saver.rl"
	{ params.gain = getValue<float>(ragel_num, ragel_type); }
	goto st140;
st140:
	if ( ++p == pe )
		goto _test_eof140;
case 140:
#line 1611 "lex/parser_array_saver.cpp"
	switch( (*p) ) {
		case 32: goto st2;
		case 100: goto st57;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st2;
	goto st0;
tr202:
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 79 "lex/parser_array_saver.rl"
	{ params.gain = getValue<float>(ragel_num, ragel_type); }
	goto st57;
tr205:
#line 58 "lex/ragel_numeric.rl"
	{
        const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
        ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
        ragel_type = TYPE_FLOAT;
        ragel_cat  = CAT_NUMBER;
    }
#line 79 "lex/parser_array_saver.rl"
	{ params.gain = getValue<float>(ragel_num, ragel_type); }
	goto st57;
st57:
	if ( ++p == pe )
		goto _test_eof57;
case 57:
#line 1644 "lex/parser_array_saver.cpp"
	if ( (*p) == 98 )
		goto st141;
	goto st0;
st141:
	if ( ++p == pe )
		goto _test_eof141;
case 141:
	switch( (*p) ) {
		case 32: goto tr207;
		case 102: goto st58;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr207;
	goto st0;
st58:
	if ( ++p == pe )
		goto _test_eof58;
case 58:
	if ( (*p) == 115 )
		goto st142;
	goto st0;
st142:
	if ( ++p == pe )
		goto _test_eof142;
case 142:
	if ( (*p) == 32 )
		goto tr207;
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr207;
	goto st0;
tr73:
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
	goto st143;
st143:
	if ( ++p == pe )
		goto _test_eof143;
case 143:
#line 1683 "lex/parser_array_saver.cpp"
	switch( (*p) ) {
		case 32: goto tr199;
		case 46: goto tr200;
		case 95: goto tr201;
		case 100: goto tr202;
	}
	if ( (*p) > 13 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr73;
	} else if ( (*p) >= 9 )
		goto tr199;
	goto st0;
st59:
	if ( ++p == pe )
		goto _test_eof59;
case 59:
	if ( (*p) == 110 )
		goto st60;
	goto st0;
st60:
	if ( ++p == pe )
		goto _test_eof60;
case 60:
	if ( (*p) == 95 )
		goto st61;
	goto st0;
st61:
	if ( ++p == pe )
		goto _test_eof61;
case 61:
	if ( (*p) == 115 )
		goto st62;
	goto st0;
st62:
	if ( ++p == pe )
		goto _test_eof62;
case 62:
	if ( (*p) == 114 )
		goto st63;
	goto st0;
st63:
	if ( ++p == pe )
		goto _test_eof63;
case 63:
	if ( (*p) == 32 )
		goto st64;
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st64;
	goto st0;
st64:
	if ( ++p == pe )
		goto _test_eof64;
case 64:
	switch( (*p) ) {
		case 32: goto st64;
		case 43: goto tr82;
		case 45: goto tr82;
		case 48: goto tr83;
	}
	if ( (*p) > 13 ) {
		if ( 49 <= (*p) && (*p) <= 57 )
			goto tr84;
	} else if ( (*p) >= 9 )
		goto st64;
	goto st0;
tr82:
#line 21 "lex/ragel_numeric.rl"
	{ ragel_num.sign = ((*p)=='-') ? -1 : 1; }
	goto st65;
st65:
	if ( ++p == pe )
		goto _test_eof65;
case 65:
#line 1757 "lex/parser_array_saver.cpp"
	if ( (*p) == 48 )
		goto tr83;
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr84;
	goto st0;
tr83:
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
	goto st144;
st144:
	if ( ++p == pe )
		goto _test_eof144;
case 144:
#line 1771 "lex/parser_array_saver.cpp"
	switch( (*p) ) {
		case 32: goto tr209;
		case 46: goto tr210;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr209;
	goto st0;
tr210:
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 50 "lex/ragel_numeric.rl"
	{
        ragel_num.ratio.num = 0;
        ragel_num.ratio.den = 1;
    }
	goto st66;
st66:
	if ( ++p == pe )
		goto _test_eof66;
case 66:
#line 1796 "lex/parser_array_saver.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr85;
	goto st0;
tr85:
#line 54 "lex/ragel_numeric.rl"
	{
        (ragel_num.ratio.num *= 10) += ((*p) - '0');
        ragel_num.ratio.den *= 10;
    }
	goto st145;
st145:
	if ( ++p == pe )
		goto _test_eof145;
case 145:
#line 1811 "lex/parser_array_saver.cpp"
	if ( (*p) == 32 )
		goto tr211;
	if ( (*p) > 13 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr85;
	} else if ( (*p) >= 9 )
		goto tr211;
	goto st0;
tr84:
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
	goto st146;
st146:
	if ( ++p == pe )
		goto _test_eof146;
case 146:
#line 1828 "lex/parser_array_saver.cpp"
	switch( (*p) ) {
		case 32: goto tr209;
		case 46: goto tr210;
	}
	if ( (*p) > 13 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr84;
	} else if ( (*p) >= 9 )
		goto tr209;
	goto st0;
st67:
	if ( ++p == pe )
		goto _test_eof67;
case 67:
	if ( (*p) == 112 )
		goto st68;
	goto st0;
st68:
	if ( ++p == pe )
		goto _test_eof68;
case 68:
	if ( (*p) == 51 )
		goto st147;
	goto st0;
st147:
	if ( ++p == pe )
		goto _test_eof147;
case 147:
	if ( (*p) == 32 )
		goto tr212;
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr212;
	goto st0;
st69:
	if ( ++p == pe )
		goto _test_eof69;
case 69:
	if ( (*p) == 111 )
		goto st70;
	goto st0;
st70:
	if ( ++p == pe )
		goto _test_eof70;
case 70:
	if ( (*p) == 114 )
		goto st71;
	goto st0;
st71:
	if ( ++p == pe )
		goto _test_eof71;
case 71:
	if ( (*p) == 109 )
		goto st148;
	goto st0;
st148:
	if ( ++p == pe )
		goto _test_eof148;
case 148:
	switch( (*p) ) {
		case 32: goto tr213;
		case 97: goto st72;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr213;
	goto st0;
st72:
	if ( ++p == pe )
		goto _test_eof72;
case 72:
	if ( (*p) == 108 )
		goto st73;
	goto st0;
st73:
	if ( ++p == pe )
		goto _test_eof73;
case 73:
	if ( (*p) == 105 )
		goto st74;
	goto st0;
st74:
	if ( ++p == pe )
		goto _test_eof74;
case 74:
	if ( (*p) == 122 )
		goto st75;
	goto st0;
st75:
	if ( ++p == pe )
		goto _test_eof75;
case 75:
	if ( (*p) == 101 )
		goto st149;
	goto st0;
st149:
	if ( ++p == pe )
		goto _test_eof149;
case 149:
	if ( (*p) == 32 )
		goto tr213;
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr213;
	goto st0;
st76:
	if ( ++p == pe )
		goto _test_eof76;
case 76:
	switch( (*p) ) {
		case 103: goto st77;
		case 112: goto st78;
		case 117: goto st80;
		case 118: goto st88;
	}
	goto st0;
st77:
	if ( ++p == pe )
		goto _test_eof77;
case 77:
	if ( (*p) == 103 )
		goto st150;
	goto st0;
st150:
	if ( ++p == pe )
		goto _test_eof150;
case 150:
	if ( (*p) == 32 )
		goto tr215;
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr215;
	goto st0;
st78:
	if ( ++p == pe )
		goto _test_eof78;
case 78:
	if ( (*p) == 117 )
		goto st79;
	goto st0;
st79:
	if ( ++p == pe )
		goto _test_eof79;
case 79:
	if ( (*p) == 115 )
		goto st151;
	goto st0;
st151:
	if ( ++p == pe )
		goto _test_eof151;
case 151:
	if ( (*p) == 32 )
		goto tr216;
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr216;
	goto st0;
st80:
	if ( ++p == pe )
		goto _test_eof80;
case 80:
	if ( (*p) == 116 )
		goto st81;
	goto st0;
st81:
	if ( ++p == pe )
		goto _test_eof81;
case 81:
	if ( (*p) == 95 )
		goto st82;
	goto st0;
st82:
	if ( ++p == pe )
		goto _test_eof82;
case 82:
	if ( (*p) == 115 )
		goto st83;
	goto st0;
st83:
	if ( ++p == pe )
		goto _test_eof83;
case 83:
	if ( (*p) == 114 )
		goto st84;
	goto st0;
st84:
	if ( ++p == pe )
		goto _test_eof84;
case 84:
	if ( (*p) == 32 )
		goto st85;
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st85;
	goto st0;
st85:
	if ( ++p == pe )
		goto _test_eof85;
case 85:
	switch( (*p) ) {
		case 32: goto st85;
		case 43: goto tr107;
		case 45: goto tr107;
		case 48: goto tr108;
	}
	if ( (*p) > 13 ) {
		if ( 49 <= (*p) && (*p) <= 57 )
			goto tr109;
	} else if ( (*p) >= 9 )
		goto st85;
	goto st0;
tr107:
#line 21 "lex/ragel_numeric.rl"
	{ ragel_num.sign = ((*p)=='-') ? -1 : 1; }
	goto st86;
st86:
	if ( ++p == pe )
		goto _test_eof86;
case 86:
#line 2042 "lex/parser_array_saver.cpp"
	if ( (*p) == 48 )
		goto tr108;
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr109;
	goto st0;
tr108:
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
	goto st152;
st152:
	if ( ++p == pe )
		goto _test_eof152;
case 152:
#line 2056 "lex/parser_array_saver.cpp"
	switch( (*p) ) {
		case 32: goto tr217;
		case 46: goto tr218;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr217;
	goto st0;
tr218:
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 50 "lex/ragel_numeric.rl"
	{
        ragel_num.ratio.num = 0;
        ragel_num.ratio.den = 1;
    }
	goto st87;
st87:
	if ( ++p == pe )
		goto _test_eof87;
case 87:
#line 2081 "lex/parser_array_saver.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr110;
	goto st0;
tr110:
#line 54 "lex/ragel_numeric.rl"
	{
        (ragel_num.ratio.num *= 10) += ((*p) - '0');
        ragel_num.ratio.den *= 10;
    }
	goto st153;
st153:
	if ( ++p == pe )
		goto _test_eof153;
case 153:
#line 2096 "lex/parser_array_saver.cpp"
	if ( (*p) == 32 )
		goto tr219;
	if ( (*p) > 13 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr110;
	} else if ( (*p) >= 9 )
		goto tr219;
	goto st0;
tr109:
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
	goto st154;
st154:
	if ( ++p == pe )
		goto _test_eof154;
case 154:
#line 2113 "lex/parser_array_saver.cpp"
	switch( (*p) ) {
		case 32: goto tr217;
		case 46: goto tr218;
	}
	if ( (*p) > 13 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr109;
	} else if ( (*p) >= 9 )
		goto tr217;
	goto st0;
st88:
	if ( ++p == pe )
		goto _test_eof88;
case 88:
	if ( (*p) == 101 )
		goto st89;
	goto st0;
st89:
	if ( ++p == pe )
		goto _test_eof89;
case 89:
	if ( (*p) == 114 )
		goto st90;
	goto st0;
st90:
	if ( ++p == pe )
		goto _test_eof90;
case 90:
	if ( (*p) == 119 )
		goto st91;
	goto st0;
st91:
	if ( ++p == pe )
		goto _test_eof91;
case 91:
	if ( (*p) == 114 )
		goto st92;
	goto st0;
st92:
	if ( ++p == pe )
		goto _test_eof92;
case 92:
	if ( (*p) == 105 )
		goto st93;
	goto st0;
st93:
	if ( ++p == pe )
		goto _test_eof93;
case 93:
	if ( (*p) == 116 )
		goto st94;
	goto st0;
st94:
	if ( ++p == pe )
		goto _test_eof94;
case 94:
	if ( (*p) == 101 )
		goto st155;
	goto st0;
st155:
	if ( ++p == pe )
		goto _test_eof155;
case 155:
	if ( (*p) == 32 )
		goto tr220;
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr220;
	goto st0;
st95:
	if ( ++p == pe )
		goto _test_eof95;
case 95:
	if ( (*p) == 97 )
		goto st96;
	goto st0;
st96:
	if ( ++p == pe )
		goto _test_eof96;
case 96:
	if ( (*p) == 119 )
		goto st156;
	goto st0;
st156:
	if ( ++p == pe )
		goto _test_eof156;
case 156:
	if ( (*p) == 32 )
		goto tr221;
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr221;
	goto st0;
st97:
	if ( ++p == pe )
		goto _test_eof97;
case 97:
	if ( (*p) == 97 )
		goto st98;
	goto st0;
st98:
	if ( ++p == pe )
		goto _test_eof98;
case 98:
	if ( (*p) == 109 )
		goto st99;
	goto st0;
st99:
	if ( ++p == pe )
		goto _test_eof99;
case 99:
	if ( (*p) == 112 )
		goto st100;
	goto st0;
st100:
	if ( ++p == pe )
		goto _test_eof100;
case 100:
	if ( (*p) == 32 )
		goto st101;
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st101;
	goto st0;
st101:
	if ( ++p == pe )
		goto _test_eof101;
case 101:
	switch( (*p) ) {
		case 32: goto st101;
		case 48: goto st157;
		case 49: goto st102;
		case 50: goto st103;
		case 51: goto st104;
		case 56: goto st161;
		case 102: goto st162;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st101;
	goto st0;
st157:
	if ( ++p == pe )
		goto _test_eof157;
case 157:
	if ( (*p) == 32 )
		goto tr222;
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr222;
	goto st0;
st102:
	if ( ++p == pe )
		goto _test_eof102;
case 102:
	if ( (*p) == 54 )
		goto st158;
	goto st0;
st158:
	if ( ++p == pe )
		goto _test_eof158;
case 158:
	if ( (*p) == 32 )
		goto tr223;
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr223;
	goto st0;
st103:
	if ( ++p == pe )
		goto _test_eof103;
case 103:
	if ( (*p) == 52 )
		goto st159;
	goto st0;
st159:
	if ( ++p == pe )
		goto _test_eof159;
case 159:
	if ( (*p) == 32 )
		goto tr224;
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr224;
	goto st0;
st104:
	if ( ++p == pe )
		goto _test_eof104;
case 104:
	if ( (*p) == 50 )
		goto st160;
	goto st0;
st160:
	if ( ++p == pe )
		goto _test_eof160;
case 160:
	if ( (*p) == 32 )
		goto tr225;
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr225;
	goto st0;
st161:
	if ( ++p == pe )
		goto _test_eof161;
case 161:
	if ( (*p) == 32 )
		goto tr226;
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr226;
	goto st0;
st162:
	if ( ++p == pe )
		goto _test_eof162;
case 162:
	if ( (*p) == 32 )
		goto tr227;
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr227;
	goto st0;
st105:
	if ( ++p == pe )
		goto _test_eof105;
case 105:
	if ( (*p) == 120 )
		goto st106;
	goto st0;
st106:
	if ( ++p == pe )
		goto _test_eof106;
case 106:
	if ( (*p) == 116 )
		goto st163;
	goto st0;
st163:
	if ( ++p == pe )
		goto _test_eof163;
case 163:
	if ( (*p) == 32 )
		goto tr228;
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr228;
	goto st0;
st107:
	if ( ++p == pe )
		goto _test_eof107;
case 107:
	if ( (*p) == 97 )
		goto st108;
	goto st0;
st108:
	if ( ++p == pe )
		goto _test_eof108;
case 108:
	if ( (*p) == 118 )
		goto st164;
	goto st0;
st164:
	if ( ++p == pe )
		goto _test_eof164;
case 164:
	if ( (*p) == 32 )
		goto tr229;
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr229;
	goto st0;
	}
	_test_eof109: cs = 109; goto _test_eof; 
	_test_eof2: cs = 2; goto _test_eof; 
	_test_eof3: cs = 3; goto _test_eof; 
	_test_eof4: cs = 4; goto _test_eof; 
	_test_eof5: cs = 5; goto _test_eof; 
	_test_eof6: cs = 6; goto _test_eof; 
	_test_eof110: cs = 110; goto _test_eof; 
	_test_eof7: cs = 7; goto _test_eof; 
	_test_eof8: cs = 8; goto _test_eof; 
	_test_eof9: cs = 9; goto _test_eof; 
	_test_eof10: cs = 10; goto _test_eof; 
	_test_eof11: cs = 11; goto _test_eof; 
	_test_eof12: cs = 12; goto _test_eof; 
	_test_eof13: cs = 13; goto _test_eof; 
	_test_eof14: cs = 14; goto _test_eof; 
	_test_eof111: cs = 111; goto _test_eof; 
	_test_eof112: cs = 112; goto _test_eof; 
	_test_eof113: cs = 113; goto _test_eof; 
	_test_eof15: cs = 15; goto _test_eof; 
	_test_eof114: cs = 114; goto _test_eof; 
	_test_eof115: cs = 115; goto _test_eof; 
	_test_eof16: cs = 16; goto _test_eof; 
	_test_eof116: cs = 116; goto _test_eof; 
	_test_eof17: cs = 17; goto _test_eof; 
	_test_eof117: cs = 117; goto _test_eof; 
	_test_eof18: cs = 18; goto _test_eof; 
	_test_eof19: cs = 19; goto _test_eof; 
	_test_eof20: cs = 20; goto _test_eof; 
	_test_eof118: cs = 118; goto _test_eof; 
	_test_eof21: cs = 21; goto _test_eof; 
	_test_eof22: cs = 22; goto _test_eof; 
	_test_eof23: cs = 23; goto _test_eof; 
	_test_eof24: cs = 24; goto _test_eof; 
	_test_eof25: cs = 25; goto _test_eof; 
	_test_eof119: cs = 119; goto _test_eof; 
	_test_eof120: cs = 120; goto _test_eof; 
	_test_eof26: cs = 26; goto _test_eof; 
	_test_eof121: cs = 121; goto _test_eof; 
	_test_eof27: cs = 27; goto _test_eof; 
	_test_eof122: cs = 122; goto _test_eof; 
	_test_eof123: cs = 123; goto _test_eof; 
	_test_eof28: cs = 28; goto _test_eof; 
	_test_eof29: cs = 29; goto _test_eof; 
	_test_eof30: cs = 30; goto _test_eof; 
	_test_eof31: cs = 31; goto _test_eof; 
	_test_eof32: cs = 32; goto _test_eof; 
	_test_eof33: cs = 33; goto _test_eof; 
	_test_eof124: cs = 124; goto _test_eof; 
	_test_eof125: cs = 125; goto _test_eof; 
	_test_eof126: cs = 126; goto _test_eof; 
	_test_eof34: cs = 34; goto _test_eof; 
	_test_eof127: cs = 127; goto _test_eof; 
	_test_eof128: cs = 128; goto _test_eof; 
	_test_eof35: cs = 35; goto _test_eof; 
	_test_eof129: cs = 129; goto _test_eof; 
	_test_eof36: cs = 36; goto _test_eof; 
	_test_eof130: cs = 130; goto _test_eof; 
	_test_eof37: cs = 37; goto _test_eof; 
	_test_eof38: cs = 38; goto _test_eof; 
	_test_eof39: cs = 39; goto _test_eof; 
	_test_eof131: cs = 131; goto _test_eof; 
	_test_eof40: cs = 40; goto _test_eof; 
	_test_eof41: cs = 41; goto _test_eof; 
	_test_eof42: cs = 42; goto _test_eof; 
	_test_eof43: cs = 43; goto _test_eof; 
	_test_eof44: cs = 44; goto _test_eof; 
	_test_eof132: cs = 132; goto _test_eof; 
	_test_eof133: cs = 133; goto _test_eof; 
	_test_eof45: cs = 45; goto _test_eof; 
	_test_eof134: cs = 134; goto _test_eof; 
	_test_eof46: cs = 46; goto _test_eof; 
	_test_eof135: cs = 135; goto _test_eof; 
	_test_eof136: cs = 136; goto _test_eof; 
	_test_eof47: cs = 47; goto _test_eof; 
	_test_eof48: cs = 48; goto _test_eof; 
	_test_eof49: cs = 49; goto _test_eof; 
	_test_eof137: cs = 137; goto _test_eof; 
	_test_eof50: cs = 50; goto _test_eof; 
	_test_eof51: cs = 51; goto _test_eof; 
	_test_eof52: cs = 52; goto _test_eof; 
	_test_eof53: cs = 53; goto _test_eof; 
	_test_eof54: cs = 54; goto _test_eof; 
	_test_eof55: cs = 55; goto _test_eof; 
	_test_eof138: cs = 138; goto _test_eof; 
	_test_eof56: cs = 56; goto _test_eof; 
	_test_eof139: cs = 139; goto _test_eof; 
	_test_eof140: cs = 140; goto _test_eof; 
	_test_eof57: cs = 57; goto _test_eof; 
	_test_eof141: cs = 141; goto _test_eof; 
	_test_eof58: cs = 58; goto _test_eof; 
	_test_eof142: cs = 142; goto _test_eof; 
	_test_eof143: cs = 143; goto _test_eof; 
	_test_eof59: cs = 59; goto _test_eof; 
	_test_eof60: cs = 60; goto _test_eof; 
	_test_eof61: cs = 61; goto _test_eof; 
	_test_eof62: cs = 62; goto _test_eof; 
	_test_eof63: cs = 63; goto _test_eof; 
	_test_eof64: cs = 64; goto _test_eof; 
	_test_eof65: cs = 65; goto _test_eof; 
	_test_eof144: cs = 144; goto _test_eof; 
	_test_eof66: cs = 66; goto _test_eof; 
	_test_eof145: cs = 145; goto _test_eof; 
	_test_eof146: cs = 146; goto _test_eof; 
	_test_eof67: cs = 67; goto _test_eof; 
	_test_eof68: cs = 68; goto _test_eof; 
	_test_eof147: cs = 147; goto _test_eof; 
	_test_eof69: cs = 69; goto _test_eof; 
	_test_eof70: cs = 70; goto _test_eof; 
	_test_eof71: cs = 71; goto _test_eof; 
	_test_eof148: cs = 148; goto _test_eof; 
	_test_eof72: cs = 72; goto _test_eof; 
	_test_eof73: cs = 73; goto _test_eof; 
	_test_eof74: cs = 74; goto _test_eof; 
	_test_eof75: cs = 75; goto _test_eof; 
	_test_eof149: cs = 149; goto _test_eof; 
	_test_eof76: cs = 76; goto _test_eof; 
	_test_eof77: cs = 77; goto _test_eof; 
	_test_eof150: cs = 150; goto _test_eof; 
	_test_eof78: cs = 78; goto _test_eof; 
	_test_eof79: cs = 79; goto _test_eof; 
	_test_eof151: cs = 151; goto _test_eof; 
	_test_eof80: cs = 80; goto _test_eof; 
	_test_eof81: cs = 81; goto _test_eof; 
	_test_eof82: cs = 82; goto _test_eof; 
	_test_eof83: cs = 83; goto _test_eof; 
	_test_eof84: cs = 84; goto _test_eof; 
	_test_eof85: cs = 85; goto _test_eof; 
	_test_eof86: cs = 86; goto _test_eof; 
	_test_eof152: cs = 152; goto _test_eof; 
	_test_eof87: cs = 87; goto _test_eof; 
	_test_eof153: cs = 153; goto _test_eof; 
	_test_eof154: cs = 154; goto _test_eof; 
	_test_eof88: cs = 88; goto _test_eof; 
	_test_eof89: cs = 89; goto _test_eof; 
	_test_eof90: cs = 90; goto _test_eof; 
	_test_eof91: cs = 91; goto _test_eof; 
	_test_eof92: cs = 92; goto _test_eof; 
	_test_eof93: cs = 93; goto _test_eof; 
	_test_eof94: cs = 94; goto _test_eof; 
	_test_eof155: cs = 155; goto _test_eof; 
	_test_eof95: cs = 95; goto _test_eof; 
	_test_eof96: cs = 96; goto _test_eof; 
	_test_eof156: cs = 156; goto _test_eof; 
	_test_eof97: cs = 97; goto _test_eof; 
	_test_eof98: cs = 98; goto _test_eof; 
	_test_eof99: cs = 99; goto _test_eof; 
	_test_eof100: cs = 100; goto _test_eof; 
	_test_eof101: cs = 101; goto _test_eof; 
	_test_eof157: cs = 157; goto _test_eof; 
	_test_eof102: cs = 102; goto _test_eof; 
	_test_eof158: cs = 158; goto _test_eof; 
	_test_eof103: cs = 103; goto _test_eof; 
	_test_eof159: cs = 159; goto _test_eof; 
	_test_eof104: cs = 104; goto _test_eof; 
	_test_eof160: cs = 160; goto _test_eof; 
	_test_eof161: cs = 161; goto _test_eof; 
	_test_eof162: cs = 162; goto _test_eof; 
	_test_eof105: cs = 105; goto _test_eof; 
	_test_eof106: cs = 106; goto _test_eof; 
	_test_eof163: cs = 163; goto _test_eof; 
	_test_eof107: cs = 107; goto _test_eof; 
	_test_eof108: cs = 108; goto _test_eof; 
	_test_eof164: cs = 164; goto _test_eof; 

	_test_eof: {}
	if ( p == eof )
	{
	switch ( cs ) {
	case 157: 
#line 83 "lex/parser_array_saver.rl"
	{ params.sample_format = sound::SAMPLE_DEFAULT; }
	break;
	case 161: 
#line 84 "lex/parser_array_saver.rl"
	{ params.sample_format = sound::SAMPLE_PCM_8; }
	break;
	case 158: 
#line 85 "lex/parser_array_saver.rl"
	{ params.sample_format = sound::SAMPLE_PCM_16; }
	break;
	case 159: 
#line 86 "lex/parser_array_saver.rl"
	{ params.sample_format = sound::SAMPLE_PCM_24; }
	break;
	case 160: 
#line 87 "lex/parser_array_saver.rl"
	{ params.sample_format = sound::SAMPLE_PCM_32; }
	break;
	case 162: 
#line 88 "lex/parser_array_saver.rl"
	{ params.sample_format = sound::SAMPLE_PCM_FLOAT; }
	break;
	case 115: 
#line 110 "lex/parser_array_saver.rl"
	{ params.begin = getArrayPos(params, ragel_num, ragel_type, arraySize); }
	break;
	case 128: 
#line 111 "lex/parser_array_saver.rl"
	{ params.end = getArrayPos(params, ragel_num, ragel_type, arraySize); }
	break;
	case 148: 
	case 149: 
#line 115 "lex/parser_array_saver.rl"
	{ params.normalize = true; }
	break;
	case 164: 
#line 116 "lex/parser_array_saver.rl"
	{ params.format = sound::FORMAT_WAV; }
	break;
	case 147: 
#line 117 "lex/parser_array_saver.rl"
	{ params.format = sound::FORMAT_MP3; }
	break;
	case 150: 
#line 118 "lex/parser_array_saver.rl"
	{ params.format = sound::FORMAT_OGG; }
	break;
	case 151: 
#line 119 "lex/parser_array_saver.rl"
	{ params.format = sound::FORMAT_OPUS; }
	break;
	case 110: 
#line 120 "lex/parser_array_saver.rl"
	{ params.format = sound::FORMAT_AIFF; }
	break;
	case 163: 
#line 121 "lex/parser_array_saver.rl"
	{ params.format = sound::FORMAT_TEXT; }
	break;
	case 156: 
#line 122 "lex/parser_array_saver.rl"
	{ params.format = sound::FORMAT_RAW; }
	break;
	case 137: 
#line 123 "lex/parser_array_saver.rl"
	{ params.format = sound::FORMAT_FLAC; }
	break;
	case 155: 
#line 124 "lex/parser_array_saver.rl"
	{ params.overwrite = true; }
	break;
	case 109: 
#line 129 "lex/parser_array_saver.rl"
	{ params.filename = ragel_atom; }
	break;
	case 138: 
	case 143: 
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 79 "lex/parser_array_saver.rl"
	{ params.gain = getValue<float>(ragel_num, ragel_type); }
	break;
	case 144: 
	case 146: 
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 113 "lex/parser_array_saver.rl"
	{ params.in_sr  = getValue<float>(ragel_num, ragel_type); }
	break;
	case 152: 
	case 154: 
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 114 "lex/parser_array_saver.rl"
	{ params.out_sr = getValue<float>(ragel_num, ragel_type); }
	break;
	case 139: 
#line 58 "lex/ragel_numeric.rl"
	{
        const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
        ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
        ragel_type = TYPE_FLOAT;
        ragel_cat  = CAT_NUMBER;
    }
#line 79 "lex/parser_array_saver.rl"
	{ params.gain = getValue<float>(ragel_num, ragel_type); }
	break;
	case 114: 
#line 58 "lex/ragel_numeric.rl"
	{
        const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
        ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
        ragel_type = TYPE_FLOAT;
        ragel_cat  = CAT_NUMBER;
    }
#line 110 "lex/parser_array_saver.rl"
	{ params.begin = getArrayPos(params, ragel_num, ragel_type, arraySize); }
	break;
	case 127: 
#line 58 "lex/ragel_numeric.rl"
	{
        const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
        ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
        ragel_type = TYPE_FLOAT;
        ragel_cat  = CAT_NUMBER;
    }
#line 111 "lex/parser_array_saver.rl"
	{ params.end = getArrayPos(params, ragel_num, ragel_type, arraySize); }
	break;
	case 145: 
#line 58 "lex/ragel_numeric.rl"
	{
        const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
        ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
        ragel_type = TYPE_FLOAT;
        ragel_cat  = CAT_NUMBER;
    }
#line 113 "lex/parser_array_saver.rl"
	{ params.in_sr  = getValue<float>(ragel_num, ragel_type); }
	break;
	case 153: 
#line 58 "lex/ragel_numeric.rl"
	{
        const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
        ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
        ragel_type = TYPE_FLOAT;
        ragel_cat  = CAT_NUMBER;
    }
#line 114 "lex/parser_array_saver.rl"
	{ params.out_sr = getValue<float>(ragel_num, ragel_type); }
	break;
	case 141: 
	case 142: 
#line 14 "lex/ragel_units.rl"
	{ragel_type = TYPE_DB;}
#line 81 "lex/parser_array_saver.rl"
	{ params.gain = convert::dbfs2amp(params.gain); }
	break;
	case 116: 
	case 117: 
#line 19 "lex/ragel_units.rl"
	{ragel_type = TYPE_MSEC;}
#line 110 "lex/parser_array_saver.rl"
	{ params.begin = getArrayPos(params, ragel_num, ragel_type, arraySize); }
	break;
	case 129: 
	case 130: 
#line 19 "lex/ragel_units.rl"
	{ragel_type = TYPE_MSEC;}
#line 111 "lex/parser_array_saver.rl"
	{ params.end = getArrayPos(params, ragel_num, ragel_type, arraySize); }
	break;
	case 120: 
	case 121: 
#line 21 "lex/ragel_units.rl"
	{ragel_type = TYPE_SAMP;}
#line 110 "lex/parser_array_saver.rl"
	{ params.begin = getArrayPos(params, ragel_num, ragel_type, arraySize); }
	break;
	case 133: 
	case 134: 
#line 21 "lex/ragel_units.rl"
	{ragel_type = TYPE_SAMP;}
#line 111 "lex/parser_array_saver.rl"
	{ params.end = getArrayPos(params, ragel_num, ragel_type, arraySize); }
	break;
	case 119: 
	case 122: 
#line 22 "lex/ragel_units.rl"
	{ragel_type = TYPE_SEC;}
#line 110 "lex/parser_array_saver.rl"
	{ params.begin = getArrayPos(params, ragel_num, ragel_type, arraySize); }
	break;
	case 132: 
	case 135: 
#line 22 "lex/ragel_units.rl"
	{ragel_type = TYPE_SEC;}
#line 111 "lex/parser_array_saver.rl"
	{ params.end = getArrayPos(params, ragel_num, ragel_type, arraySize); }
	break;
	case 112: 
	case 118: 
#line 26 "lex/ragel_units.rl"
	{ragel_type = TYPE_PERCENT;}
#line 110 "lex/parser_array_saver.rl"
	{ params.begin = getArrayPos(params, ragel_num, ragel_type, arraySize); }
	break;
	case 125: 
	case 131: 
#line 26 "lex/ragel_units.rl"
	{ragel_type = TYPE_PERCENT;}
#line 111 "lex/parser_array_saver.rl"
	{ params.end = getArrayPos(params, ragel_num, ragel_type, arraySize); }
	break;
	case 113: 
#line 29 "lex/ragel_units.rl"
	{ragel_type = TYPE_PHASE;}
#line 110 "lex/parser_array_saver.rl"
	{ params.begin = getArrayPos(params, ragel_num, ragel_type, arraySize); }
	break;
	case 126: 
#line 29 "lex/ragel_units.rl"
	{ragel_type = TYPE_PHASE;}
#line 111 "lex/parser_array_saver.rl"
	{ params.end = getArrayPos(params, ragel_num, ragel_type, arraySize); }
	break;
	case 111: 
	case 123: 
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 71 "lex/parser_array_saver.rl"
	{ ragel_num.vdouble = ragel_num.vint; }
#line 110 "lex/parser_array_saver.rl"
	{ params.begin = getArrayPos(params, ragel_num, ragel_type, arraySize); }
	break;
	case 124: 
	case 136: 
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 71 "lex/parser_array_saver.rl"
	{ ragel_num.vdouble = ragel_num.vint; }
#line 111 "lex/parser_array_saver.rl"
	{ params.end = getArrayPos(params, ragel_num, ragel_type, arraySize); }
	break;
#line 2807 "lex/parser_array_saver.cpp"
	}
	}

	_out: {}
	}

#line 162 "lex/parser_array_saver.rl"


    if (cs >= 109) {
        if (params.format == sound::FORMAT_UNKNOWN)
            params.format = detect_format(params.filename.c_str(), params.filename.size());

        return true;
    } else
        return false;
}


#line 2827 "lex/parser_array_saver.cpp"
static const int auto_format_detector_start = 1;
static const int auto_format_detector_first_final = 30;
static const int auto_format_detector_error = 0;

static const int auto_format_detector_en_main = 1;


#line 186 "lex/parser_array_saver.rl"


sound::SoundFileFormat detect_format(const char* str, size_t len)
{
    int cs = 0;
    const char* p = str;
    const char* pe = p + len;
    const char* eof = p + len;

    sound::SoundFileFormat fmt = sound::FORMAT_UNKNOWN;

    
#line 2848 "lex/parser_array_saver.cpp"
	{
	cs = auto_format_detector_start;
	}

#line 2853 "lex/parser_array_saver.cpp"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
st1:
	if ( ++p == pe )
		goto _test_eof1;
case 1:
	switch( (*p) ) {
		case 0: goto st0;
		case 46: goto st2;
	}
	goto st1;
st0:
cs = 0;
	goto _out;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
	switch( (*p) ) {
		case 0: goto st0;
		case 46: goto st2;
		case 65: goto st3;
		case 70: goto st5;
		case 77: goto st8;
		case 79: goto st10;
		case 82: goto st14;
		case 87: goto st16;
		case 97: goto st3;
		case 102: goto st18;
		case 109: goto st21;
		case 111: goto st22;
		case 114: goto st26;
		case 119: goto st28;
	}
	goto st1;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
	switch( (*p) ) {
		case 0: goto st0;
		case 46: goto st2;
		case 73: goto st4;
		case 105: goto st4;
	}
	goto st1;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
	switch( (*p) ) {
		case 0: goto st0;
		case 46: goto st2;
		case 70: goto st30;
		case 102: goto st30;
	}
	goto st1;
st30:
	if ( ++p == pe )
		goto _test_eof30;
case 30:
	switch( (*p) ) {
		case 70: goto st31;
		case 102: goto st31;
	}
	goto st0;
st31:
	if ( ++p == pe )
		goto _test_eof31;
case 31:
	goto st0;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
	switch( (*p) ) {
		case 0: goto st0;
		case 46: goto st2;
		case 76: goto st6;
	}
	goto st1;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
	switch( (*p) ) {
		case 0: goto st0;
		case 46: goto st2;
		case 65: goto st7;
	}
	goto st1;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
	switch( (*p) ) {
		case 0: goto st0;
		case 46: goto st2;
		case 67: goto st32;
	}
	goto st1;
st32:
	if ( ++p == pe )
		goto _test_eof32;
case 32:
	goto st0;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
	switch( (*p) ) {
		case 0: goto st0;
		case 46: goto st2;
		case 80: goto st9;
	}
	goto st1;
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
	switch( (*p) ) {
		case 0: goto st0;
		case 46: goto st2;
		case 51: goto st33;
	}
	goto st1;
st33:
	if ( ++p == pe )
		goto _test_eof33;
case 33:
	goto st0;
st10:
	if ( ++p == pe )
		goto _test_eof10;
case 10:
	switch( (*p) ) {
		case 0: goto st0;
		case 46: goto st2;
		case 71: goto st11;
		case 80: goto st12;
	}
	goto st1;
st11:
	if ( ++p == pe )
		goto _test_eof11;
case 11:
	switch( (*p) ) {
		case 0: goto st0;
		case 46: goto st2;
		case 71: goto st34;
	}
	goto st1;
st34:
	if ( ++p == pe )
		goto _test_eof34;
case 34:
	goto st0;
st12:
	if ( ++p == pe )
		goto _test_eof12;
case 12:
	switch( (*p) ) {
		case 0: goto st0;
		case 46: goto st2;
		case 85: goto st13;
	}
	goto st1;
st13:
	if ( ++p == pe )
		goto _test_eof13;
case 13:
	switch( (*p) ) {
		case 0: goto st0;
		case 46: goto st2;
		case 83: goto st35;
	}
	goto st1;
st35:
	if ( ++p == pe )
		goto _test_eof35;
case 35:
	goto st0;
st14:
	if ( ++p == pe )
		goto _test_eof14;
case 14:
	switch( (*p) ) {
		case 0: goto st0;
		case 46: goto st2;
		case 65: goto st15;
	}
	goto st1;
st15:
	if ( ++p == pe )
		goto _test_eof15;
case 15:
	switch( (*p) ) {
		case 0: goto st0;
		case 46: goto st2;
		case 87: goto st36;
	}
	goto st1;
st36:
	if ( ++p == pe )
		goto _test_eof36;
case 36:
	goto st0;
st16:
	if ( ++p == pe )
		goto _test_eof16;
case 16:
	switch( (*p) ) {
		case 0: goto st0;
		case 46: goto st2;
		case 65: goto st17;
	}
	goto st1;
st17:
	if ( ++p == pe )
		goto _test_eof17;
case 17:
	switch( (*p) ) {
		case 0: goto st0;
		case 46: goto st2;
		case 86: goto st37;
	}
	goto st1;
st37:
	if ( ++p == pe )
		goto _test_eof37;
case 37:
	goto st0;
st18:
	if ( ++p == pe )
		goto _test_eof18;
case 18:
	switch( (*p) ) {
		case 0: goto st0;
		case 46: goto st2;
		case 108: goto st19;
	}
	goto st1;
st19:
	if ( ++p == pe )
		goto _test_eof19;
case 19:
	switch( (*p) ) {
		case 0: goto st0;
		case 46: goto st2;
		case 97: goto st20;
	}
	goto st1;
st20:
	if ( ++p == pe )
		goto _test_eof20;
case 20:
	switch( (*p) ) {
		case 0: goto st0;
		case 46: goto st2;
		case 99: goto st32;
	}
	goto st1;
st21:
	if ( ++p == pe )
		goto _test_eof21;
case 21:
	switch( (*p) ) {
		case 0: goto st0;
		case 46: goto st2;
		case 112: goto st9;
	}
	goto st1;
st22:
	if ( ++p == pe )
		goto _test_eof22;
case 22:
	switch( (*p) ) {
		case 0: goto st0;
		case 46: goto st2;
		case 103: goto st23;
		case 112: goto st24;
	}
	goto st1;
st23:
	if ( ++p == pe )
		goto _test_eof23;
case 23:
	switch( (*p) ) {
		case 0: goto st0;
		case 46: goto st2;
		case 103: goto st34;
	}
	goto st1;
st24:
	if ( ++p == pe )
		goto _test_eof24;
case 24:
	switch( (*p) ) {
		case 0: goto st0;
		case 46: goto st2;
		case 117: goto st25;
	}
	goto st1;
st25:
	if ( ++p == pe )
		goto _test_eof25;
case 25:
	switch( (*p) ) {
		case 0: goto st0;
		case 46: goto st2;
		case 115: goto st35;
	}
	goto st1;
st26:
	if ( ++p == pe )
		goto _test_eof26;
case 26:
	switch( (*p) ) {
		case 0: goto st0;
		case 46: goto st2;
		case 97: goto st27;
	}
	goto st1;
st27:
	if ( ++p == pe )
		goto _test_eof27;
case 27:
	switch( (*p) ) {
		case 0: goto st0;
		case 46: goto st2;
		case 119: goto st36;
	}
	goto st1;
st28:
	if ( ++p == pe )
		goto _test_eof28;
case 28:
	switch( (*p) ) {
		case 0: goto st0;
		case 46: goto st2;
		case 97: goto st29;
	}
	goto st1;
st29:
	if ( ++p == pe )
		goto _test_eof29;
case 29:
	switch( (*p) ) {
		case 0: goto st0;
		case 46: goto st2;
		case 118: goto st37;
	}
	goto st1;
	}
	_test_eof1: cs = 1; goto _test_eof; 
	_test_eof2: cs = 2; goto _test_eof; 
	_test_eof3: cs = 3; goto _test_eof; 
	_test_eof4: cs = 4; goto _test_eof; 
	_test_eof30: cs = 30; goto _test_eof; 
	_test_eof31: cs = 31; goto _test_eof; 
	_test_eof5: cs = 5; goto _test_eof; 
	_test_eof6: cs = 6; goto _test_eof; 
	_test_eof7: cs = 7; goto _test_eof; 
	_test_eof32: cs = 32; goto _test_eof; 
	_test_eof8: cs = 8; goto _test_eof; 
	_test_eof9: cs = 9; goto _test_eof; 
	_test_eof33: cs = 33; goto _test_eof; 
	_test_eof10: cs = 10; goto _test_eof; 
	_test_eof11: cs = 11; goto _test_eof; 
	_test_eof34: cs = 34; goto _test_eof; 
	_test_eof12: cs = 12; goto _test_eof; 
	_test_eof13: cs = 13; goto _test_eof; 
	_test_eof35: cs = 35; goto _test_eof; 
	_test_eof14: cs = 14; goto _test_eof; 
	_test_eof15: cs = 15; goto _test_eof; 
	_test_eof36: cs = 36; goto _test_eof; 
	_test_eof16: cs = 16; goto _test_eof; 
	_test_eof17: cs = 17; goto _test_eof; 
	_test_eof37: cs = 37; goto _test_eof; 
	_test_eof18: cs = 18; goto _test_eof; 
	_test_eof19: cs = 19; goto _test_eof; 
	_test_eof20: cs = 20; goto _test_eof; 
	_test_eof21: cs = 21; goto _test_eof; 
	_test_eof22: cs = 22; goto _test_eof; 
	_test_eof23: cs = 23; goto _test_eof; 
	_test_eof24: cs = 24; goto _test_eof; 
	_test_eof25: cs = 25; goto _test_eof; 
	_test_eof26: cs = 26; goto _test_eof; 
	_test_eof27: cs = 27; goto _test_eof; 
	_test_eof28: cs = 28; goto _test_eof; 
	_test_eof29: cs = 29; goto _test_eof; 

	_test_eof: {}
	if ( p == eof )
	{
	switch ( cs ) {
	case 33: 
#line 176 "lex/parser_array_saver.rl"
	{ fmt = sound::FORMAT_MP3; }
	break;
	case 37: 
#line 177 "lex/parser_array_saver.rl"
	{ fmt = sound::FORMAT_WAV; }
	break;
	case 30: 
	case 31: 
#line 178 "lex/parser_array_saver.rl"
	{ fmt = sound::FORMAT_AIFF; }
	break;
	case 35: 
#line 179 "lex/parser_array_saver.rl"
	{ fmt = sound::FORMAT_OPUS; }
	break;
	case 34: 
#line 180 "lex/parser_array_saver.rl"
	{ fmt = sound::FORMAT_OGG; }
	break;
	case 32: 
#line 181 "lex/parser_array_saver.rl"
	{ fmt = sound::FORMAT_FLAC; }
	break;
	case 36: 
#line 182 "lex/parser_array_saver.rl"
	{ fmt = sound::FORMAT_RAW; }
	break;
#line 3282 "lex/parser_array_saver.cpp"
	}
	}

	_out: {}
	}

#line 200 "lex/parser_array_saver.rl"


    return fmt;
}

}
}
