
#line 1 "lex/parser_array_saver.rl"
# include "parser_array_saver.h"
# include "parser_numeric.h"
# include "ragel_common.h"

# include <algorithm>
# include <cmath>
# include <cstdint>
# include <string>

namespace {
struct NumCleaner {
    ceammc::parser::fsm::NumericData& num_;
    ~NumCleaner() { num_ = {}; }
};

template<typename T>
inline T getValue(ceammc::parser::fsm::NumericData& num, ceammc::parser::AtomType type)
{
    NumCleaner cleaner { num };
    return (type != ceammc::parser::TYPE_FLOAT) ? num.getInteger() : num.getFloat();
}

inline size_t clipArraySize(std::int64_t v, size_t arraySize)
{
    return std::min<std::int64_t>(arraySize - 1, std::max<std::int64_t>(0, v));
}

inline std::int64_t getUnitValue(
    ceammc::parser::ArraySaverParams& params,
    ceammc::parser::fsm::NumericData& num,
    ceammc::parser::AtomType type,
    size_t arraySize)
{
    switch (type) {
    case ceammc::parser::TYPE_PERCENT:
        return std::round(num.getFloat() * arraySize * 0.01);
    case ceammc::parser::TYPE_PHASE:
        return std::round(num.getFloat() * arraySize);
    case ceammc::parser::TYPE_FLOAT:
    case ceammc::parser::TYPE_INT:
    case ceammc::parser::TYPE_SAMP:
        return std::round(num.getFloat());
    case ceammc::parser::TYPE_MSEC:
        return std::round(num.getFloat() * 0.001 * params.in_sr);
    case ceammc::parser::TYPE_SEC:
        return std::round(num.getFloat() * params.in_sr);
    default:
        return 0;
    }
}

inline size_t getArrayAbsPos(ceammc::parser::ArraySaverParams& params, std::int64_t pos, std::int64_t arraySize)
{
    return (params.origin == ceammc::parser::ORIGIN_BEGIN) ? pos : (arraySize + pos);
}

inline size_t getArrayPos(ceammc::parser::ArraySaverParams& params,
    ceammc::parser::fsm::NumericData& num,
    ceammc::parser::AtomType type,
    size_t arraySize)
{
    NumCleaner cleaner { num };

    auto pos = getUnitValue(params, num, type, arraySize);
    return clipArraySize(getArrayAbsPos(params, pos, arraySize), arraySize);
}
}


#line 73 "lex/parser_array_saver.cpp"
static const int array_saver_start = 1;
static const int array_saver_first_final = 98;
static const int array_saver_error = 0;

static const int array_saver_en_main = 1;


#line 137 "lex/parser_array_saver.rl"


namespace ceammc {
namespace parser {

bool parse_array_saver_params(const char* str, size_t arraySize, ArraySaverParams& params)
{
    auto len = strlen(str);
    int cs = 0;
    const char* p = str;
    const char* pe = p + len;
    const char* eof = p + len;

    std::string ragel_atom;
    double val = 0;

    DECLARE_RAGEL_COMMON_VARS;
    DECLARE_RAGEL_NUMERIC_VARS;


#line 102 "lex/parser_array_saver.cpp"
	{
	cs = array_saver_start;
	}

#line 107 "lex/parser_array_saver.cpp"
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
#line 124 "lex/parser_array_saver.rl"
	{ ragel_atom.clear(); }
#line 78 "lex/parser_array_saver.rl"
	{ ragel_atom += (*p); }
	goto st2;
tr2:
#line 78 "lex/parser_array_saver.rl"
	{ ragel_atom += (*p); }
	goto st2;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
#line 135 "lex/parser_array_saver.cpp"
	if ( (*p) == 32 )
		goto tr3;
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr3;
	goto tr2;
tr3:
#line 124 "lex/parser_array_saver.rl"
	{ params.arrays.push_back(ragel_atom); }
	goto st3;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
#line 149 "lex/parser_array_saver.cpp"
	switch( (*p) ) {
		case 32: goto st3;
		case 64: goto st4;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st3;
	goto tr0;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
	if ( (*p) == 116 )
		goto st5;
	goto st0;
st0:
cs = 0;
	goto _out;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
	if ( (*p) == 111 )
		goto st6;
	goto st0;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
	if ( (*p) == 32 )
		goto st7;
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st7;
	goto st0;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
	switch( (*p) ) {
		case 32: goto st7;
		case 64: goto st0;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st7;
	goto tr9;
tr125:
#line 78 "lex/parser_array_saver.rl"
	{ ragel_atom += (*p); }
	goto st98;
tr9:
#line 123 "lex/parser_array_saver.rl"
	{ ragel_atom.clear(); }
#line 78 "lex/parser_array_saver.rl"
	{ ragel_atom += (*p); }
	goto st98;
st98:
	if ( ++p == pe )
		goto _test_eof98;
case 98:
#line 208 "lex/parser_array_saver.cpp"
	if ( (*p) == 32 )
		goto tr126;
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr126;
	goto tr125;
tr126:
#line 123 "lex/parser_array_saver.rl"
	{ params.filename = ragel_atom; }
	goto st8;
tr127:
#line 116 "lex/parser_array_saver.rl"
	{ params.format = FORMAT_AIFF; }
	goto st8;
tr128:
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 74 "lex/parser_array_saver.rl"
	{ ragel_num.vdouble = ragel_num.vint; }
#line 106 "lex/parser_array_saver.rl"
	{ params.begin = getArrayPos(params, ragel_num, ragel_type, arraySize); }
	goto st8;
tr136:
#line 25 "lex/ragel_units.rl"
	{ragel_type = TYPE_PERCENT;}
#line 106 "lex/parser_array_saver.rl"
	{ params.begin = getArrayPos(params, ragel_num, ragel_type, arraySize); }
	goto st8;
tr137:
#line 28 "lex/ragel_units.rl"
	{ragel_type = TYPE_PHASE;}
#line 106 "lex/parser_array_saver.rl"
	{ params.begin = getArrayPos(params, ragel_num, ragel_type, arraySize); }
	goto st8;
tr138:
#line 58 "lex/ragel_numeric.rl"
	{
        const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
        ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
        ragel_type = TYPE_FLOAT;
        ragel_cat  = CAT_NUMBER;
    }
#line 106 "lex/parser_array_saver.rl"
	{ params.begin = getArrayPos(params, ragel_num, ragel_type, arraySize); }
	goto st8;
tr145:
#line 106 "lex/parser_array_saver.rl"
	{ params.begin = getArrayPos(params, ragel_num, ragel_type, arraySize); }
	goto st8;
tr149:
#line 20 "lex/ragel_units.rl"
	{ragel_type = TYPE_MSEC;}
#line 106 "lex/parser_array_saver.rl"
	{ params.begin = getArrayPos(params, ragel_num, ragel_type, arraySize); }
	goto st8;
tr152:
#line 19 "lex/ragel_units.rl"
	{ragel_type = TYPE_SEC;}
#line 106 "lex/parser_array_saver.rl"
	{ params.begin = getArrayPos(params, ragel_num, ragel_type, arraySize); }
	goto st8;
tr155:
#line 12 "lex/ragel_units.rl"
	{ragel_type = TYPE_SAMP;}
#line 106 "lex/parser_array_saver.rl"
	{ params.begin = getArrayPos(params, ragel_num, ragel_type, arraySize); }
	goto st8;
tr157:
#line 83 "lex/parser_array_saver.rl"
	{ params.bits = 0; }
	goto st8;
tr158:
#line 85 "lex/parser_array_saver.rl"
	{ params.bits = 16; }
	goto st8;
tr159:
#line 86 "lex/parser_array_saver.rl"
	{ params.bits = 24; }
	goto st8;
tr160:
#line 87 "lex/parser_array_saver.rl"
	{ params.bits = 32; }
	goto st8;
tr161:
#line 84 "lex/parser_array_saver.rl"
	{ params.bits = 8; }
	goto st8;
tr162:
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 74 "lex/parser_array_saver.rl"
	{ ragel_num.vdouble = ragel_num.vint; }
#line 107 "lex/parser_array_saver.rl"
	{ params.end = getArrayPos(params, ragel_num, ragel_type, arraySize); }
	goto st8;
tr170:
#line 25 "lex/ragel_units.rl"
	{ragel_type = TYPE_PERCENT;}
#line 107 "lex/parser_array_saver.rl"
	{ params.end = getArrayPos(params, ragel_num, ragel_type, arraySize); }
	goto st8;
tr171:
#line 28 "lex/ragel_units.rl"
	{ragel_type = TYPE_PHASE;}
#line 107 "lex/parser_array_saver.rl"
	{ params.end = getArrayPos(params, ragel_num, ragel_type, arraySize); }
	goto st8;
tr172:
#line 58 "lex/ragel_numeric.rl"
	{
        const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
        ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
        ragel_type = TYPE_FLOAT;
        ragel_cat  = CAT_NUMBER;
    }
#line 107 "lex/parser_array_saver.rl"
	{ params.end = getArrayPos(params, ragel_num, ragel_type, arraySize); }
	goto st8;
tr179:
#line 107 "lex/parser_array_saver.rl"
	{ params.end = getArrayPos(params, ragel_num, ragel_type, arraySize); }
	goto st8;
tr183:
#line 20 "lex/ragel_units.rl"
	{ragel_type = TYPE_MSEC;}
#line 107 "lex/parser_array_saver.rl"
	{ params.end = getArrayPos(params, ragel_num, ragel_type, arraySize); }
	goto st8;
tr186:
#line 19 "lex/ragel_units.rl"
	{ragel_type = TYPE_SEC;}
#line 107 "lex/parser_array_saver.rl"
	{ params.end = getArrayPos(params, ragel_num, ragel_type, arraySize); }
	goto st8;
tr189:
#line 12 "lex/ragel_units.rl"
	{ragel_type = TYPE_SAMP;}
#line 107 "lex/parser_array_saver.rl"
	{ params.end = getArrayPos(params, ragel_num, ragel_type, arraySize); }
	goto st8;
tr191:
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 105 "lex/parser_array_saver.rl"
	{ params.gain = getValue<float>(ragel_num, ragel_type); }
	goto st8;
tr193:
#line 58 "lex/ragel_numeric.rl"
	{
        const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
        ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
        ragel_type = TYPE_FLOAT;
        ragel_cat  = CAT_NUMBER;
    }
#line 105 "lex/parser_array_saver.rl"
	{ params.gain = getValue<float>(ragel_num, ragel_type); }
	goto st8;
tr194:
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 109 "lex/parser_array_saver.rl"
	{ params.in_sr  = getValue<float>(ragel_num, ragel_type); }
	goto st8;
tr196:
#line 58 "lex/ragel_numeric.rl"
	{
        const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
        ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
        ragel_type = TYPE_FLOAT;
        ragel_cat  = CAT_NUMBER;
    }
#line 109 "lex/parser_array_saver.rl"
	{ params.in_sr  = getValue<float>(ragel_num, ragel_type); }
	goto st8;
tr197:
#line 113 "lex/parser_array_saver.rl"
	{ params.format = FORMAT_MP3; }
	goto st8;
tr198:
#line 111 "lex/parser_array_saver.rl"
	{ params.normalize = true; }
	goto st8;
tr199:
#line 114 "lex/parser_array_saver.rl"
	{ params.format = FORMAT_VORBIS; }
	goto st8;
tr200:
#line 115 "lex/parser_array_saver.rl"
	{ params.format = FORMAT_OPUS; }
	goto st8;
tr201:
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 110 "lex/parser_array_saver.rl"
	{ params.out_sr = getValue<float>(ragel_num, ragel_type); }
	goto st8;
tr203:
#line 58 "lex/ragel_numeric.rl"
	{
        const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
        ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
        ragel_type = TYPE_FLOAT;
        ragel_cat  = CAT_NUMBER;
    }
#line 110 "lex/parser_array_saver.rl"
	{ params.out_sr = getValue<float>(ragel_num, ragel_type); }
	goto st8;
tr204:
#line 118 "lex/parser_array_saver.rl"
	{ params.format = FORMAT_RAW; }
	goto st8;
tr205:
#line 117 "lex/parser_array_saver.rl"
	{ params.format = FORMAT_TEXT; }
	goto st8;
tr206:
#line 112 "lex/parser_array_saver.rl"
	{ params.format = FORMAT_WAV; }
	goto st8;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
#line 451 "lex/parser_array_saver.cpp"
	switch( (*p) ) {
		case 32: goto st8;
		case 64: goto tr11;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st8;
	goto st0;
tr11:
#line 91 "lex/parser_array_saver.rl"
	{ params.origin = ORIGIN_BEGIN; }
#line 92 "lex/parser_array_saver.rl"
	{ params.origin = ORIGIN_BEGIN; }
	goto st9;
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
#line 469 "lex/parser_array_saver.cpp"
	switch( (*p) ) {
		case 97: goto st10;
		case 98: goto st13;
		case 101: goto st41;
		case 103: goto st60;
		case 105: goto st67;
		case 109: goto st75;
		case 110: goto st77;
		case 111: goto st80;
		case 114: goto st92;
		case 116: goto st94;
		case 119: goto st96;
	}
	goto st0;
st10:
	if ( ++p == pe )
		goto _test_eof10;
case 10:
	if ( (*p) == 105 )
		goto st11;
	goto st0;
st11:
	if ( ++p == pe )
		goto _test_eof11;
case 11:
	if ( (*p) == 102 )
		goto st12;
	goto st0;
st12:
	if ( ++p == pe )
		goto _test_eof12;
case 12:
	if ( (*p) == 102 )
		goto st99;
	goto st0;
st99:
	if ( ++p == pe )
		goto _test_eof99;
case 99:
	if ( (*p) == 32 )
		goto tr127;
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr127;
	goto st0;
st13:
	if ( ++p == pe )
		goto _test_eof13;
case 13:
	switch( (*p) ) {
		case 101: goto st14;
		case 105: goto st34;
	}
	goto st0;
st14:
	if ( ++p == pe )
		goto _test_eof14;
case 14:
	if ( (*p) == 103 )
		goto st15;
	goto st0;
st15:
	if ( ++p == pe )
		goto _test_eof15;
case 15:
	if ( (*p) == 105 )
		goto st16;
	goto st0;
st16:
	if ( ++p == pe )
		goto _test_eof16;
case 16:
	if ( (*p) == 110 )
		goto st17;
	goto st0;
st17:
	if ( ++p == pe )
		goto _test_eof17;
case 17:
	if ( (*p) == 32 )
		goto st18;
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st18;
	goto st0;
st18:
	if ( ++p == pe )
		goto _test_eof18;
case 18:
	switch( (*p) ) {
		case 32: goto st18;
		case 36: goto tr32;
		case 43: goto tr33;
		case 45: goto tr33;
		case 48: goto tr34;
	}
	if ( (*p) > 13 ) {
		if ( 49 <= (*p) && (*p) <= 57 )
			goto tr35;
	} else if ( (*p) >= 9 )
		goto st18;
	goto st0;
tr32:
#line 76 "lex/parser_array_saver.rl"
	{ params.origin = ORIGIN_END; }
	goto st19;
st19:
	if ( ++p == pe )
		goto _test_eof19;
case 19:
#line 578 "lex/parser_array_saver.cpp"
	switch( (*p) ) {
		case 43: goto tr33;
		case 45: goto tr33;
		case 48: goto tr34;
	}
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr35;
	goto st0;
tr33:
#line 21 "lex/ragel_numeric.rl"
	{ ragel_num.sign = ((*p)=='-') ? -1 : 1; }
	goto st20;
st20:
	if ( ++p == pe )
		goto _test_eof20;
case 20:
#line 595 "lex/parser_array_saver.cpp"
	if ( (*p) == 48 )
		goto tr34;
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr35;
	goto st0;
tr34:
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
	goto st100;
st100:
	if ( ++p == pe )
		goto _test_eof100;
case 100:
#line 609 "lex/parser_array_saver.cpp"
	switch( (*p) ) {
		case 32: goto tr128;
		case 37: goto tr129;
		case 42: goto tr130;
		case 46: goto tr131;
		case 95: goto tr132;
		case 109: goto tr133;
		case 112: goto tr134;
		case 115: goto tr135;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr128;
	goto st0;
tr129:
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 74 "lex/parser_array_saver.rl"
	{ ragel_num.vdouble = ragel_num.vint; }
	goto st101;
tr139:
#line 58 "lex/ragel_numeric.rl"
	{
        const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
        ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
        ragel_type = TYPE_FLOAT;
        ragel_cat  = CAT_NUMBER;
    }
	goto st101;
st101:
	if ( ++p == pe )
		goto _test_eof101;
case 101:
#line 646 "lex/parser_array_saver.cpp"
	if ( (*p) == 32 )
		goto tr136;
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr136;
	goto st0;
tr130:
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 74 "lex/parser_array_saver.rl"
	{ ragel_num.vdouble = ragel_num.vint; }
	goto st102;
tr140:
#line 58 "lex/ragel_numeric.rl"
	{
        const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
        ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
        ragel_type = TYPE_FLOAT;
        ragel_cat  = CAT_NUMBER;
    }
	goto st102;
st102:
	if ( ++p == pe )
		goto _test_eof102;
case 102:
#line 675 "lex/parser_array_saver.cpp"
	if ( (*p) == 32 )
		goto tr137;
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr137;
	goto st0;
tr131:
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
	goto st21;
st21:
	if ( ++p == pe )
		goto _test_eof21;
case 21:
#line 698 "lex/parser_array_saver.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr36;
	goto st0;
tr36:
#line 54 "lex/ragel_numeric.rl"
	{
        (ragel_num.ratio.num *= 10) += ((*p) - '0');
        ragel_num.ratio.den *= 10;
    }
	goto st103;
st103:
	if ( ++p == pe )
		goto _test_eof103;
case 103:
#line 713 "lex/parser_array_saver.cpp"
	switch( (*p) ) {
		case 32: goto tr138;
		case 37: goto tr139;
		case 42: goto tr140;
		case 95: goto tr141;
		case 109: goto tr142;
		case 112: goto tr143;
		case 115: goto tr144;
	}
	if ( (*p) > 13 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr36;
	} else if ( (*p) >= 9 )
		goto tr138;
	goto st0;
tr132:
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 74 "lex/parser_array_saver.rl"
	{ ragel_num.vdouble = ragel_num.vint; }
	goto st104;
tr141:
#line 58 "lex/ragel_numeric.rl"
	{
        const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
        ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
        ragel_type = TYPE_FLOAT;
        ragel_cat  = CAT_NUMBER;
    }
	goto st104;
st104:
	if ( ++p == pe )
		goto _test_eof104;
case 104:
#line 752 "lex/parser_array_saver.cpp"
	switch( (*p) ) {
		case 32: goto tr145;
		case 37: goto st101;
		case 42: goto st102;
		case 109: goto st22;
		case 112: goto st24;
		case 115: goto st108;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr145;
	goto st0;
tr133:
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 74 "lex/parser_array_saver.rl"
	{ ragel_num.vdouble = ragel_num.vint; }
	goto st22;
tr142:
#line 58 "lex/ragel_numeric.rl"
	{
        const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
        ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
        ragel_type = TYPE_FLOAT;
        ragel_cat  = CAT_NUMBER;
    }
	goto st22;
st22:
	if ( ++p == pe )
		goto _test_eof22;
case 22:
#line 787 "lex/parser_array_saver.cpp"
	if ( (*p) == 115 )
		goto st105;
	goto st0;
st105:
	if ( ++p == pe )
		goto _test_eof105;
case 105:
	switch( (*p) ) {
		case 32: goto tr149;
		case 101: goto st23;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr149;
	goto st0;
st23:
	if ( ++p == pe )
		goto _test_eof23;
case 23:
	if ( (*p) == 99 )
		goto st106;
	goto st0;
st106:
	if ( ++p == pe )
		goto _test_eof106;
case 106:
	if ( (*p) == 32 )
		goto tr149;
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr149;
	goto st0;
tr134:
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 74 "lex/parser_array_saver.rl"
	{ ragel_num.vdouble = ragel_num.vint; }
	goto st24;
tr143:
#line 58 "lex/ragel_numeric.rl"
	{
        const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
        ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
        ragel_type = TYPE_FLOAT;
        ragel_cat  = CAT_NUMBER;
    }
	goto st24;
st24:
	if ( ++p == pe )
		goto _test_eof24;
case 24:
#line 841 "lex/parser_array_saver.cpp"
	switch( (*p) ) {
		case 101: goto st25;
		case 104: goto st29;
	}
	goto st0;
st25:
	if ( ++p == pe )
		goto _test_eof25;
case 25:
	if ( (*p) == 114 )
		goto st26;
	goto st0;
st26:
	if ( ++p == pe )
		goto _test_eof26;
case 26:
	if ( (*p) == 99 )
		goto st107;
	goto st0;
st107:
	if ( ++p == pe )
		goto _test_eof107;
case 107:
	switch( (*p) ) {
		case 32: goto tr136;
		case 101: goto st27;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr136;
	goto st0;
st27:
	if ( ++p == pe )
		goto _test_eof27;
case 27:
	if ( (*p) == 110 )
		goto st28;
	goto st0;
st28:
	if ( ++p == pe )
		goto _test_eof28;
case 28:
	if ( (*p) == 116 )
		goto st101;
	goto st0;
st29:
	if ( ++p == pe )
		goto _test_eof29;
case 29:
	if ( (*p) == 97 )
		goto st30;
	goto st0;
st30:
	if ( ++p == pe )
		goto _test_eof30;
case 30:
	if ( (*p) == 115 )
		goto st31;
	goto st0;
st31:
	if ( ++p == pe )
		goto _test_eof31;
case 31:
	if ( (*p) == 101 )
		goto st102;
	goto st0;
tr135:
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 74 "lex/parser_array_saver.rl"
	{ ragel_num.vdouble = ragel_num.vint; }
	goto st108;
tr144:
#line 58 "lex/ragel_numeric.rl"
	{
        const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
        ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
        ragel_type = TYPE_FLOAT;
        ragel_cat  = CAT_NUMBER;
    }
	goto st108;
st108:
	if ( ++p == pe )
		goto _test_eof108;
case 108:
#line 930 "lex/parser_array_saver.cpp"
	switch( (*p) ) {
		case 32: goto tr152;
		case 97: goto st109;
		case 101: goto st33;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr152;
	goto st0;
st109:
	if ( ++p == pe )
		goto _test_eof109;
case 109:
	switch( (*p) ) {
		case 32: goto tr155;
		case 109: goto st32;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr155;
	goto st0;
st32:
	if ( ++p == pe )
		goto _test_eof32;
case 32:
	if ( (*p) == 112 )
		goto st110;
	goto st0;
st110:
	if ( ++p == pe )
		goto _test_eof110;
case 110:
	if ( (*p) == 32 )
		goto tr155;
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr155;
	goto st0;
st33:
	if ( ++p == pe )
		goto _test_eof33;
case 33:
	if ( (*p) == 99 )
		goto st111;
	goto st0;
st111:
	if ( ++p == pe )
		goto _test_eof111;
case 111:
	if ( (*p) == 32 )
		goto tr152;
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr152;
	goto st0;
tr35:
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
	goto st112;
st112:
	if ( ++p == pe )
		goto _test_eof112;
case 112:
#line 990 "lex/parser_array_saver.cpp"
	switch( (*p) ) {
		case 32: goto tr128;
		case 37: goto tr129;
		case 42: goto tr130;
		case 46: goto tr131;
		case 95: goto tr132;
		case 109: goto tr133;
		case 112: goto tr134;
		case 115: goto tr135;
	}
	if ( (*p) > 13 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr35;
	} else if ( (*p) >= 9 )
		goto tr128;
	goto st0;
st34:
	if ( ++p == pe )
		goto _test_eof34;
case 34:
	if ( (*p) == 116 )
		goto st35;
	goto st0;
st35:
	if ( ++p == pe )
		goto _test_eof35;
case 35:
	if ( (*p) == 115 )
		goto st36;
	goto st0;
st36:
	if ( ++p == pe )
		goto _test_eof36;
case 36:
	if ( (*p) == 32 )
		goto st37;
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st37;
	goto st0;
st37:
	if ( ++p == pe )
		goto _test_eof37;
case 37:
	switch( (*p) ) {
		case 32: goto st37;
		case 48: goto st113;
		case 49: goto st38;
		case 50: goto st39;
		case 51: goto st40;
		case 56: goto st117;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st37;
	goto st0;
st113:
	if ( ++p == pe )
		goto _test_eof113;
case 113:
	if ( (*p) == 32 )
		goto tr157;
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr157;
	goto st0;
st38:
	if ( ++p == pe )
		goto _test_eof38;
case 38:
	if ( (*p) == 54 )
		goto st114;
	goto st0;
st114:
	if ( ++p == pe )
		goto _test_eof114;
case 114:
	if ( (*p) == 32 )
		goto tr158;
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr158;
	goto st0;
st39:
	if ( ++p == pe )
		goto _test_eof39;
case 39:
	if ( (*p) == 52 )
		goto st115;
	goto st0;
st115:
	if ( ++p == pe )
		goto _test_eof115;
case 115:
	if ( (*p) == 32 )
		goto tr159;
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr159;
	goto st0;
st40:
	if ( ++p == pe )
		goto _test_eof40;
case 40:
	if ( (*p) == 50 )
		goto st116;
	goto st0;
st116:
	if ( ++p == pe )
		goto _test_eof116;
case 116:
	if ( (*p) == 32 )
		goto tr160;
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr160;
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
st41:
	if ( ++p == pe )
		goto _test_eof41;
case 41:
	if ( (*p) == 110 )
		goto st42;
	goto st0;
st42:
	if ( ++p == pe )
		goto _test_eof42;
case 42:
	if ( (*p) == 100 )
		goto st43;
	goto st0;
st43:
	if ( ++p == pe )
		goto _test_eof43;
case 43:
	if ( (*p) == 32 )
		goto st44;
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st44;
	goto st0;
st44:
	if ( ++p == pe )
		goto _test_eof44;
case 44:
	switch( (*p) ) {
		case 32: goto st44;
		case 36: goto tr64;
		case 43: goto tr65;
		case 45: goto tr65;
		case 48: goto tr66;
	}
	if ( (*p) > 13 ) {
		if ( 49 <= (*p) && (*p) <= 57 )
			goto tr67;
	} else if ( (*p) >= 9 )
		goto st44;
	goto st0;
tr64:
#line 76 "lex/parser_array_saver.rl"
	{ params.origin = ORIGIN_END; }
	goto st45;
st45:
	if ( ++p == pe )
		goto _test_eof45;
case 45:
#line 1159 "lex/parser_array_saver.cpp"
	switch( (*p) ) {
		case 43: goto tr65;
		case 45: goto tr65;
		case 48: goto tr66;
	}
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr67;
	goto st0;
tr65:
#line 21 "lex/ragel_numeric.rl"
	{ ragel_num.sign = ((*p)=='-') ? -1 : 1; }
	goto st46;
st46:
	if ( ++p == pe )
		goto _test_eof46;
case 46:
#line 1176 "lex/parser_array_saver.cpp"
	if ( (*p) == 48 )
		goto tr66;
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr67;
	goto st0;
tr66:
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
	goto st118;
st118:
	if ( ++p == pe )
		goto _test_eof118;
case 118:
#line 1190 "lex/parser_array_saver.cpp"
	switch( (*p) ) {
		case 32: goto tr162;
		case 37: goto tr163;
		case 42: goto tr164;
		case 46: goto tr165;
		case 95: goto tr166;
		case 109: goto tr167;
		case 112: goto tr168;
		case 115: goto tr169;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr162;
	goto st0;
tr163:
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 74 "lex/parser_array_saver.rl"
	{ ragel_num.vdouble = ragel_num.vint; }
	goto st119;
tr173:
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
#line 1227 "lex/parser_array_saver.cpp"
	if ( (*p) == 32 )
		goto tr170;
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr170;
	goto st0;
tr164:
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 74 "lex/parser_array_saver.rl"
	{ ragel_num.vdouble = ragel_num.vint; }
	goto st120;
tr174:
#line 58 "lex/ragel_numeric.rl"
	{
        const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
        ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
        ragel_type = TYPE_FLOAT;
        ragel_cat  = CAT_NUMBER;
    }
	goto st120;
st120:
	if ( ++p == pe )
		goto _test_eof120;
case 120:
#line 1256 "lex/parser_array_saver.cpp"
	if ( (*p) == 32 )
		goto tr171;
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr171;
	goto st0;
tr165:
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
	goto st47;
st47:
	if ( ++p == pe )
		goto _test_eof47;
case 47:
#line 1279 "lex/parser_array_saver.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr68;
	goto st0;
tr68:
#line 54 "lex/ragel_numeric.rl"
	{
        (ragel_num.ratio.num *= 10) += ((*p) - '0');
        ragel_num.ratio.den *= 10;
    }
	goto st121;
st121:
	if ( ++p == pe )
		goto _test_eof121;
case 121:
#line 1294 "lex/parser_array_saver.cpp"
	switch( (*p) ) {
		case 32: goto tr172;
		case 37: goto tr173;
		case 42: goto tr174;
		case 95: goto tr175;
		case 109: goto tr176;
		case 112: goto tr177;
		case 115: goto tr178;
	}
	if ( (*p) > 13 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr68;
	} else if ( (*p) >= 9 )
		goto tr172;
	goto st0;
tr166:
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 74 "lex/parser_array_saver.rl"
	{ ragel_num.vdouble = ragel_num.vint; }
	goto st122;
tr175:
#line 58 "lex/ragel_numeric.rl"
	{
        const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
        ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
        ragel_type = TYPE_FLOAT;
        ragel_cat  = CAT_NUMBER;
    }
	goto st122;
st122:
	if ( ++p == pe )
		goto _test_eof122;
case 122:
#line 1333 "lex/parser_array_saver.cpp"
	switch( (*p) ) {
		case 32: goto tr179;
		case 37: goto st119;
		case 42: goto st120;
		case 109: goto st48;
		case 112: goto st50;
		case 115: goto st126;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr179;
	goto st0;
tr167:
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 74 "lex/parser_array_saver.rl"
	{ ragel_num.vdouble = ragel_num.vint; }
	goto st48;
tr176:
#line 58 "lex/ragel_numeric.rl"
	{
        const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
        ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
        ragel_type = TYPE_FLOAT;
        ragel_cat  = CAT_NUMBER;
    }
	goto st48;
st48:
	if ( ++p == pe )
		goto _test_eof48;
case 48:
#line 1368 "lex/parser_array_saver.cpp"
	if ( (*p) == 115 )
		goto st123;
	goto st0;
st123:
	if ( ++p == pe )
		goto _test_eof123;
case 123:
	switch( (*p) ) {
		case 32: goto tr183;
		case 101: goto st49;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr183;
	goto st0;
st49:
	if ( ++p == pe )
		goto _test_eof49;
case 49:
	if ( (*p) == 99 )
		goto st124;
	goto st0;
st124:
	if ( ++p == pe )
		goto _test_eof124;
case 124:
	if ( (*p) == 32 )
		goto tr183;
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr183;
	goto st0;
tr168:
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 74 "lex/parser_array_saver.rl"
	{ ragel_num.vdouble = ragel_num.vint; }
	goto st50;
tr177:
#line 58 "lex/ragel_numeric.rl"
	{
        const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
        ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
        ragel_type = TYPE_FLOAT;
        ragel_cat  = CAT_NUMBER;
    }
	goto st50;
st50:
	if ( ++p == pe )
		goto _test_eof50;
case 50:
#line 1422 "lex/parser_array_saver.cpp"
	switch( (*p) ) {
		case 101: goto st51;
		case 104: goto st55;
	}
	goto st0;
st51:
	if ( ++p == pe )
		goto _test_eof51;
case 51:
	if ( (*p) == 114 )
		goto st52;
	goto st0;
st52:
	if ( ++p == pe )
		goto _test_eof52;
case 52:
	if ( (*p) == 99 )
		goto st125;
	goto st0;
st125:
	if ( ++p == pe )
		goto _test_eof125;
case 125:
	switch( (*p) ) {
		case 32: goto tr170;
		case 101: goto st53;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr170;
	goto st0;
st53:
	if ( ++p == pe )
		goto _test_eof53;
case 53:
	if ( (*p) == 110 )
		goto st54;
	goto st0;
st54:
	if ( ++p == pe )
		goto _test_eof54;
case 54:
	if ( (*p) == 116 )
		goto st119;
	goto st0;
st55:
	if ( ++p == pe )
		goto _test_eof55;
case 55:
	if ( (*p) == 97 )
		goto st56;
	goto st0;
st56:
	if ( ++p == pe )
		goto _test_eof56;
case 56:
	if ( (*p) == 115 )
		goto st57;
	goto st0;
st57:
	if ( ++p == pe )
		goto _test_eof57;
case 57:
	if ( (*p) == 101 )
		goto st120;
	goto st0;
tr169:
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 74 "lex/parser_array_saver.rl"
	{ ragel_num.vdouble = ragel_num.vint; }
	goto st126;
tr178:
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
#line 1511 "lex/parser_array_saver.cpp"
	switch( (*p) ) {
		case 32: goto tr186;
		case 97: goto st127;
		case 101: goto st59;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr186;
	goto st0;
st127:
	if ( ++p == pe )
		goto _test_eof127;
case 127:
	switch( (*p) ) {
		case 32: goto tr189;
		case 109: goto st58;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr189;
	goto st0;
st58:
	if ( ++p == pe )
		goto _test_eof58;
case 58:
	if ( (*p) == 112 )
		goto st128;
	goto st0;
st128:
	if ( ++p == pe )
		goto _test_eof128;
case 128:
	if ( (*p) == 32 )
		goto tr189;
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr189;
	goto st0;
st59:
	if ( ++p == pe )
		goto _test_eof59;
case 59:
	if ( (*p) == 99 )
		goto st129;
	goto st0;
st129:
	if ( ++p == pe )
		goto _test_eof129;
case 129:
	if ( (*p) == 32 )
		goto tr186;
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr186;
	goto st0;
tr67:
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
	goto st130;
st130:
	if ( ++p == pe )
		goto _test_eof130;
case 130:
#line 1571 "lex/parser_array_saver.cpp"
	switch( (*p) ) {
		case 32: goto tr162;
		case 37: goto tr163;
		case 42: goto tr164;
		case 46: goto tr165;
		case 95: goto tr166;
		case 109: goto tr167;
		case 112: goto tr168;
		case 115: goto tr169;
	}
	if ( (*p) > 13 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr67;
	} else if ( (*p) >= 9 )
		goto tr162;
	goto st0;
st60:
	if ( ++p == pe )
		goto _test_eof60;
case 60:
	if ( (*p) == 97 )
		goto st61;
	goto st0;
st61:
	if ( ++p == pe )
		goto _test_eof61;
case 61:
	if ( (*p) == 105 )
		goto st62;
	goto st0;
st62:
	if ( ++p == pe )
		goto _test_eof62;
case 62:
	if ( (*p) == 110 )
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
		case 43: goto tr86;
		case 45: goto tr86;
		case 48: goto tr87;
	}
	if ( (*p) > 13 ) {
		if ( 49 <= (*p) && (*p) <= 57 )
			goto tr88;
	} else if ( (*p) >= 9 )
		goto st64;
	goto st0;
tr86:
#line 21 "lex/ragel_numeric.rl"
	{ ragel_num.sign = ((*p)=='-') ? -1 : 1; }
	goto st65;
st65:
	if ( ++p == pe )
		goto _test_eof65;
case 65:
#line 1642 "lex/parser_array_saver.cpp"
	if ( (*p) == 48 )
		goto tr87;
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr88;
	goto st0;
tr87:
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
	goto st131;
st131:
	if ( ++p == pe )
		goto _test_eof131;
case 131:
#line 1656 "lex/parser_array_saver.cpp"
	switch( (*p) ) {
		case 32: goto tr191;
		case 46: goto tr192;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr191;
	goto st0;
tr192:
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
#line 1681 "lex/parser_array_saver.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr89;
	goto st0;
tr89:
#line 54 "lex/ragel_numeric.rl"
	{
        (ragel_num.ratio.num *= 10) += ((*p) - '0');
        ragel_num.ratio.den *= 10;
    }
	goto st132;
st132:
	if ( ++p == pe )
		goto _test_eof132;
case 132:
#line 1696 "lex/parser_array_saver.cpp"
	if ( (*p) == 32 )
		goto tr193;
	if ( (*p) > 13 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr89;
	} else if ( (*p) >= 9 )
		goto tr193;
	goto st0;
tr88:
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
	goto st133;
st133:
	if ( ++p == pe )
		goto _test_eof133;
case 133:
#line 1713 "lex/parser_array_saver.cpp"
	switch( (*p) ) {
		case 32: goto tr191;
		case 46: goto tr192;
	}
	if ( (*p) > 13 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr88;
	} else if ( (*p) >= 9 )
		goto tr191;
	goto st0;
st67:
	if ( ++p == pe )
		goto _test_eof67;
case 67:
	if ( (*p) == 110 )
		goto st68;
	goto st0;
st68:
	if ( ++p == pe )
		goto _test_eof68;
case 68:
	if ( (*p) == 95 )
		goto st69;
	goto st0;
st69:
	if ( ++p == pe )
		goto _test_eof69;
case 69:
	if ( (*p) == 115 )
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
	if ( (*p) == 32 )
		goto st72;
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st72;
	goto st0;
st72:
	if ( ++p == pe )
		goto _test_eof72;
case 72:
	switch( (*p) ) {
		case 32: goto st72;
		case 43: goto tr95;
		case 45: goto tr95;
		case 48: goto tr96;
	}
	if ( (*p) > 13 ) {
		if ( 49 <= (*p) && (*p) <= 57 )
			goto tr97;
	} else if ( (*p) >= 9 )
		goto st72;
	goto st0;
tr95:
#line 21 "lex/ragel_numeric.rl"
	{ ragel_num.sign = ((*p)=='-') ? -1 : 1; }
	goto st73;
st73:
	if ( ++p == pe )
		goto _test_eof73;
case 73:
#line 1785 "lex/parser_array_saver.cpp"
	if ( (*p) == 48 )
		goto tr96;
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr97;
	goto st0;
tr96:
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
	goto st134;
st134:
	if ( ++p == pe )
		goto _test_eof134;
case 134:
#line 1799 "lex/parser_array_saver.cpp"
	switch( (*p) ) {
		case 32: goto tr194;
		case 46: goto tr195;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr194;
	goto st0;
tr195:
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
	goto st74;
st74:
	if ( ++p == pe )
		goto _test_eof74;
case 74:
#line 1824 "lex/parser_array_saver.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr98;
	goto st0;
tr98:
#line 54 "lex/ragel_numeric.rl"
	{
        (ragel_num.ratio.num *= 10) += ((*p) - '0');
        ragel_num.ratio.den *= 10;
    }
	goto st135;
st135:
	if ( ++p == pe )
		goto _test_eof135;
case 135:
#line 1839 "lex/parser_array_saver.cpp"
	if ( (*p) == 32 )
		goto tr196;
	if ( (*p) > 13 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr98;
	} else if ( (*p) >= 9 )
		goto tr196;
	goto st0;
tr97:
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
	goto st136;
st136:
	if ( ++p == pe )
		goto _test_eof136;
case 136:
#line 1856 "lex/parser_array_saver.cpp"
	switch( (*p) ) {
		case 32: goto tr194;
		case 46: goto tr195;
	}
	if ( (*p) > 13 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr97;
	} else if ( (*p) >= 9 )
		goto tr194;
	goto st0;
st75:
	if ( ++p == pe )
		goto _test_eof75;
case 75:
	if ( (*p) == 112 )
		goto st76;
	goto st0;
st76:
	if ( ++p == pe )
		goto _test_eof76;
case 76:
	if ( (*p) == 51 )
		goto st137;
	goto st0;
st137:
	if ( ++p == pe )
		goto _test_eof137;
case 137:
	if ( (*p) == 32 )
		goto tr197;
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr197;
	goto st0;
st77:
	if ( ++p == pe )
		goto _test_eof77;
case 77:
	if ( (*p) == 111 )
		goto st78;
	goto st0;
st78:
	if ( ++p == pe )
		goto _test_eof78;
case 78:
	if ( (*p) == 114 )
		goto st79;
	goto st0;
st79:
	if ( ++p == pe )
		goto _test_eof79;
case 79:
	if ( (*p) == 109 )
		goto st138;
	goto st0;
st138:
	if ( ++p == pe )
		goto _test_eof138;
case 138:
	if ( (*p) == 32 )
		goto tr198;
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr198;
	goto st0;
st80:
	if ( ++p == pe )
		goto _test_eof80;
case 80:
	switch( (*p) ) {
		case 103: goto st81;
		case 112: goto st82;
		case 117: goto st84;
	}
	goto st0;
st81:
	if ( ++p == pe )
		goto _test_eof81;
case 81:
	if ( (*p) == 103 )
		goto st139;
	goto st0;
st139:
	if ( ++p == pe )
		goto _test_eof139;
case 139:
	if ( (*p) == 32 )
		goto tr199;
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr199;
	goto st0;
st82:
	if ( ++p == pe )
		goto _test_eof82;
case 82:
	if ( (*p) == 117 )
		goto st83;
	goto st0;
st83:
	if ( ++p == pe )
		goto _test_eof83;
case 83:
	if ( (*p) == 115 )
		goto st140;
	goto st0;
st140:
	if ( ++p == pe )
		goto _test_eof140;
case 140:
	if ( (*p) == 32 )
		goto tr200;
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr200;
	goto st0;
st84:
	if ( ++p == pe )
		goto _test_eof84;
case 84:
	if ( (*p) == 116 )
		goto st85;
	goto st0;
st85:
	if ( ++p == pe )
		goto _test_eof85;
case 85:
	if ( (*p) == 95 )
		goto st86;
	goto st0;
st86:
	if ( ++p == pe )
		goto _test_eof86;
case 86:
	if ( (*p) == 115 )
		goto st87;
	goto st0;
st87:
	if ( ++p == pe )
		goto _test_eof87;
case 87:
	if ( (*p) == 114 )
		goto st88;
	goto st0;
st88:
	if ( ++p == pe )
		goto _test_eof88;
case 88:
	if ( (*p) == 32 )
		goto st89;
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st89;
	goto st0;
st89:
	if ( ++p == pe )
		goto _test_eof89;
case 89:
	switch( (*p) ) {
		case 32: goto st89;
		case 43: goto tr115;
		case 45: goto tr115;
		case 48: goto tr116;
	}
	if ( (*p) > 13 ) {
		if ( 49 <= (*p) && (*p) <= 57 )
			goto tr117;
	} else if ( (*p) >= 9 )
		goto st89;
	goto st0;
tr115:
#line 21 "lex/ragel_numeric.rl"
	{ ragel_num.sign = ((*p)=='-') ? -1 : 1; }
	goto st90;
st90:
	if ( ++p == pe )
		goto _test_eof90;
case 90:
#line 2030 "lex/parser_array_saver.cpp"
	if ( (*p) == 48 )
		goto tr116;
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr117;
	goto st0;
tr116:
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
	goto st141;
st141:
	if ( ++p == pe )
		goto _test_eof141;
case 141:
#line 2044 "lex/parser_array_saver.cpp"
	switch( (*p) ) {
		case 32: goto tr201;
		case 46: goto tr202;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr201;
	goto st0;
tr202:
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
	goto st91;
st91:
	if ( ++p == pe )
		goto _test_eof91;
case 91:
#line 2069 "lex/parser_array_saver.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr118;
	goto st0;
tr118:
#line 54 "lex/ragel_numeric.rl"
	{
        (ragel_num.ratio.num *= 10) += ((*p) - '0');
        ragel_num.ratio.den *= 10;
    }
	goto st142;
st142:
	if ( ++p == pe )
		goto _test_eof142;
case 142:
#line 2084 "lex/parser_array_saver.cpp"
	if ( (*p) == 32 )
		goto tr203;
	if ( (*p) > 13 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr118;
	} else if ( (*p) >= 9 )
		goto tr203;
	goto st0;
tr117:
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
	goto st143;
st143:
	if ( ++p == pe )
		goto _test_eof143;
case 143:
#line 2101 "lex/parser_array_saver.cpp"
	switch( (*p) ) {
		case 32: goto tr201;
		case 46: goto tr202;
	}
	if ( (*p) > 13 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr117;
	} else if ( (*p) >= 9 )
		goto tr201;
	goto st0;
st92:
	if ( ++p == pe )
		goto _test_eof92;
case 92:
	if ( (*p) == 97 )
		goto st93;
	goto st0;
st93:
	if ( ++p == pe )
		goto _test_eof93;
case 93:
	if ( (*p) == 119 )
		goto st144;
	goto st0;
st144:
	if ( ++p == pe )
		goto _test_eof144;
case 144:
	if ( (*p) == 32 )
		goto tr204;
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr204;
	goto st0;
st94:
	if ( ++p == pe )
		goto _test_eof94;
case 94:
	if ( (*p) == 120 )
		goto st95;
	goto st0;
st95:
	if ( ++p == pe )
		goto _test_eof95;
case 95:
	if ( (*p) == 116 )
		goto st145;
	goto st0;
st145:
	if ( ++p == pe )
		goto _test_eof145;
case 145:
	if ( (*p) == 32 )
		goto tr205;
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr205;
	goto st0;
st96:
	if ( ++p == pe )
		goto _test_eof96;
case 96:
	if ( (*p) == 97 )
		goto st97;
	goto st0;
st97:
	if ( ++p == pe )
		goto _test_eof97;
case 97:
	if ( (*p) == 118 )
		goto st146;
	goto st0;
st146:
	if ( ++p == pe )
		goto _test_eof146;
case 146:
	if ( (*p) == 32 )
		goto tr206;
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr206;
	goto st0;
	}
	_test_eof2: cs = 2; goto _test_eof; 
	_test_eof3: cs = 3; goto _test_eof; 
	_test_eof4: cs = 4; goto _test_eof; 
	_test_eof5: cs = 5; goto _test_eof; 
	_test_eof6: cs = 6; goto _test_eof; 
	_test_eof7: cs = 7; goto _test_eof; 
	_test_eof98: cs = 98; goto _test_eof; 
	_test_eof8: cs = 8; goto _test_eof; 
	_test_eof9: cs = 9; goto _test_eof; 
	_test_eof10: cs = 10; goto _test_eof; 
	_test_eof11: cs = 11; goto _test_eof; 
	_test_eof12: cs = 12; goto _test_eof; 
	_test_eof99: cs = 99; goto _test_eof; 
	_test_eof13: cs = 13; goto _test_eof; 
	_test_eof14: cs = 14; goto _test_eof; 
	_test_eof15: cs = 15; goto _test_eof; 
	_test_eof16: cs = 16; goto _test_eof; 
	_test_eof17: cs = 17; goto _test_eof; 
	_test_eof18: cs = 18; goto _test_eof; 
	_test_eof19: cs = 19; goto _test_eof; 
	_test_eof20: cs = 20; goto _test_eof; 
	_test_eof100: cs = 100; goto _test_eof; 
	_test_eof101: cs = 101; goto _test_eof; 
	_test_eof102: cs = 102; goto _test_eof; 
	_test_eof21: cs = 21; goto _test_eof; 
	_test_eof103: cs = 103; goto _test_eof; 
	_test_eof104: cs = 104; goto _test_eof; 
	_test_eof22: cs = 22; goto _test_eof; 
	_test_eof105: cs = 105; goto _test_eof; 
	_test_eof23: cs = 23; goto _test_eof; 
	_test_eof106: cs = 106; goto _test_eof; 
	_test_eof24: cs = 24; goto _test_eof; 
	_test_eof25: cs = 25; goto _test_eof; 
	_test_eof26: cs = 26; goto _test_eof; 
	_test_eof107: cs = 107; goto _test_eof; 
	_test_eof27: cs = 27; goto _test_eof; 
	_test_eof28: cs = 28; goto _test_eof; 
	_test_eof29: cs = 29; goto _test_eof; 
	_test_eof30: cs = 30; goto _test_eof; 
	_test_eof31: cs = 31; goto _test_eof; 
	_test_eof108: cs = 108; goto _test_eof; 
	_test_eof109: cs = 109; goto _test_eof; 
	_test_eof32: cs = 32; goto _test_eof; 
	_test_eof110: cs = 110; goto _test_eof; 
	_test_eof33: cs = 33; goto _test_eof; 
	_test_eof111: cs = 111; goto _test_eof; 
	_test_eof112: cs = 112; goto _test_eof; 
	_test_eof34: cs = 34; goto _test_eof; 
	_test_eof35: cs = 35; goto _test_eof; 
	_test_eof36: cs = 36; goto _test_eof; 
	_test_eof37: cs = 37; goto _test_eof; 
	_test_eof113: cs = 113; goto _test_eof; 
	_test_eof38: cs = 38; goto _test_eof; 
	_test_eof114: cs = 114; goto _test_eof; 
	_test_eof39: cs = 39; goto _test_eof; 
	_test_eof115: cs = 115; goto _test_eof; 
	_test_eof40: cs = 40; goto _test_eof; 
	_test_eof116: cs = 116; goto _test_eof; 
	_test_eof117: cs = 117; goto _test_eof; 
	_test_eof41: cs = 41; goto _test_eof; 
	_test_eof42: cs = 42; goto _test_eof; 
	_test_eof43: cs = 43; goto _test_eof; 
	_test_eof44: cs = 44; goto _test_eof; 
	_test_eof45: cs = 45; goto _test_eof; 
	_test_eof46: cs = 46; goto _test_eof; 
	_test_eof118: cs = 118; goto _test_eof; 
	_test_eof119: cs = 119; goto _test_eof; 
	_test_eof120: cs = 120; goto _test_eof; 
	_test_eof47: cs = 47; goto _test_eof; 
	_test_eof121: cs = 121; goto _test_eof; 
	_test_eof122: cs = 122; goto _test_eof; 
	_test_eof48: cs = 48; goto _test_eof; 
	_test_eof123: cs = 123; goto _test_eof; 
	_test_eof49: cs = 49; goto _test_eof; 
	_test_eof124: cs = 124; goto _test_eof; 
	_test_eof50: cs = 50; goto _test_eof; 
	_test_eof51: cs = 51; goto _test_eof; 
	_test_eof52: cs = 52; goto _test_eof; 
	_test_eof125: cs = 125; goto _test_eof; 
	_test_eof53: cs = 53; goto _test_eof; 
	_test_eof54: cs = 54; goto _test_eof; 
	_test_eof55: cs = 55; goto _test_eof; 
	_test_eof56: cs = 56; goto _test_eof; 
	_test_eof57: cs = 57; goto _test_eof; 
	_test_eof126: cs = 126; goto _test_eof; 
	_test_eof127: cs = 127; goto _test_eof; 
	_test_eof58: cs = 58; goto _test_eof; 
	_test_eof128: cs = 128; goto _test_eof; 
	_test_eof59: cs = 59; goto _test_eof; 
	_test_eof129: cs = 129; goto _test_eof; 
	_test_eof130: cs = 130; goto _test_eof; 
	_test_eof60: cs = 60; goto _test_eof; 
	_test_eof61: cs = 61; goto _test_eof; 
	_test_eof62: cs = 62; goto _test_eof; 
	_test_eof63: cs = 63; goto _test_eof; 
	_test_eof64: cs = 64; goto _test_eof; 
	_test_eof65: cs = 65; goto _test_eof; 
	_test_eof131: cs = 131; goto _test_eof; 
	_test_eof66: cs = 66; goto _test_eof; 
	_test_eof132: cs = 132; goto _test_eof; 
	_test_eof133: cs = 133; goto _test_eof; 
	_test_eof67: cs = 67; goto _test_eof; 
	_test_eof68: cs = 68; goto _test_eof; 
	_test_eof69: cs = 69; goto _test_eof; 
	_test_eof70: cs = 70; goto _test_eof; 
	_test_eof71: cs = 71; goto _test_eof; 
	_test_eof72: cs = 72; goto _test_eof; 
	_test_eof73: cs = 73; goto _test_eof; 
	_test_eof134: cs = 134; goto _test_eof; 
	_test_eof74: cs = 74; goto _test_eof; 
	_test_eof135: cs = 135; goto _test_eof; 
	_test_eof136: cs = 136; goto _test_eof; 
	_test_eof75: cs = 75; goto _test_eof; 
	_test_eof76: cs = 76; goto _test_eof; 
	_test_eof137: cs = 137; goto _test_eof; 
	_test_eof77: cs = 77; goto _test_eof; 
	_test_eof78: cs = 78; goto _test_eof; 
	_test_eof79: cs = 79; goto _test_eof; 
	_test_eof138: cs = 138; goto _test_eof; 
	_test_eof80: cs = 80; goto _test_eof; 
	_test_eof81: cs = 81; goto _test_eof; 
	_test_eof139: cs = 139; goto _test_eof; 
	_test_eof82: cs = 82; goto _test_eof; 
	_test_eof83: cs = 83; goto _test_eof; 
	_test_eof140: cs = 140; goto _test_eof; 
	_test_eof84: cs = 84; goto _test_eof; 
	_test_eof85: cs = 85; goto _test_eof; 
	_test_eof86: cs = 86; goto _test_eof; 
	_test_eof87: cs = 87; goto _test_eof; 
	_test_eof88: cs = 88; goto _test_eof; 
	_test_eof89: cs = 89; goto _test_eof; 
	_test_eof90: cs = 90; goto _test_eof; 
	_test_eof141: cs = 141; goto _test_eof; 
	_test_eof91: cs = 91; goto _test_eof; 
	_test_eof142: cs = 142; goto _test_eof; 
	_test_eof143: cs = 143; goto _test_eof; 
	_test_eof92: cs = 92; goto _test_eof; 
	_test_eof93: cs = 93; goto _test_eof; 
	_test_eof144: cs = 144; goto _test_eof; 
	_test_eof94: cs = 94; goto _test_eof; 
	_test_eof95: cs = 95; goto _test_eof; 
	_test_eof145: cs = 145; goto _test_eof; 
	_test_eof96: cs = 96; goto _test_eof; 
	_test_eof97: cs = 97; goto _test_eof; 
	_test_eof146: cs = 146; goto _test_eof; 

	_test_eof: {}
	if ( p == eof )
	{
	switch ( cs ) {
	case 113: 
#line 83 "lex/parser_array_saver.rl"
	{ params.bits = 0; }
	break;
	case 117: 
#line 84 "lex/parser_array_saver.rl"
	{ params.bits = 8; }
	break;
	case 114: 
#line 85 "lex/parser_array_saver.rl"
	{ params.bits = 16; }
	break;
	case 115: 
#line 86 "lex/parser_array_saver.rl"
	{ params.bits = 24; }
	break;
	case 116: 
#line 87 "lex/parser_array_saver.rl"
	{ params.bits = 32; }
	break;
	case 104: 
#line 106 "lex/parser_array_saver.rl"
	{ params.begin = getArrayPos(params, ragel_num, ragel_type, arraySize); }
	break;
	case 122: 
#line 107 "lex/parser_array_saver.rl"
	{ params.end = getArrayPos(params, ragel_num, ragel_type, arraySize); }
	break;
	case 138: 
#line 111 "lex/parser_array_saver.rl"
	{ params.normalize = true; }
	break;
	case 146: 
#line 112 "lex/parser_array_saver.rl"
	{ params.format = FORMAT_WAV; }
	break;
	case 137: 
#line 113 "lex/parser_array_saver.rl"
	{ params.format = FORMAT_MP3; }
	break;
	case 139: 
#line 114 "lex/parser_array_saver.rl"
	{ params.format = FORMAT_VORBIS; }
	break;
	case 140: 
#line 115 "lex/parser_array_saver.rl"
	{ params.format = FORMAT_OPUS; }
	break;
	case 99: 
#line 116 "lex/parser_array_saver.rl"
	{ params.format = FORMAT_AIFF; }
	break;
	case 145: 
#line 117 "lex/parser_array_saver.rl"
	{ params.format = FORMAT_TEXT; }
	break;
	case 144: 
#line 118 "lex/parser_array_saver.rl"
	{ params.format = FORMAT_RAW; }
	break;
	case 98: 
#line 123 "lex/parser_array_saver.rl"
	{ params.filename = ragel_atom; }
	break;
	case 131: 
	case 133: 
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 105 "lex/parser_array_saver.rl"
	{ params.gain = getValue<float>(ragel_num, ragel_type); }
	break;
	case 134: 
	case 136: 
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 109 "lex/parser_array_saver.rl"
	{ params.in_sr  = getValue<float>(ragel_num, ragel_type); }
	break;
	case 141: 
	case 143: 
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 110 "lex/parser_array_saver.rl"
	{ params.out_sr = getValue<float>(ragel_num, ragel_type); }
	break;
	case 132: 
#line 58 "lex/ragel_numeric.rl"
	{
        const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
        ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
        ragel_type = TYPE_FLOAT;
        ragel_cat  = CAT_NUMBER;
    }
#line 105 "lex/parser_array_saver.rl"
	{ params.gain = getValue<float>(ragel_num, ragel_type); }
	break;
	case 103: 
#line 58 "lex/ragel_numeric.rl"
	{
        const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
        ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
        ragel_type = TYPE_FLOAT;
        ragel_cat  = CAT_NUMBER;
    }
#line 106 "lex/parser_array_saver.rl"
	{ params.begin = getArrayPos(params, ragel_num, ragel_type, arraySize); }
	break;
	case 121: 
#line 58 "lex/ragel_numeric.rl"
	{
        const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
        ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
        ragel_type = TYPE_FLOAT;
        ragel_cat  = CAT_NUMBER;
    }
#line 107 "lex/parser_array_saver.rl"
	{ params.end = getArrayPos(params, ragel_num, ragel_type, arraySize); }
	break;
	case 135: 
#line 58 "lex/ragel_numeric.rl"
	{
        const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
        ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
        ragel_type = TYPE_FLOAT;
        ragel_cat  = CAT_NUMBER;
    }
#line 109 "lex/parser_array_saver.rl"
	{ params.in_sr  = getValue<float>(ragel_num, ragel_type); }
	break;
	case 142: 
#line 58 "lex/ragel_numeric.rl"
	{
        const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
        ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
        ragel_type = TYPE_FLOAT;
        ragel_cat  = CAT_NUMBER;
    }
#line 110 "lex/parser_array_saver.rl"
	{ params.out_sr = getValue<float>(ragel_num, ragel_type); }
	break;
	case 109: 
	case 110: 
#line 12 "lex/ragel_units.rl"
	{ragel_type = TYPE_SAMP;}
#line 106 "lex/parser_array_saver.rl"
	{ params.begin = getArrayPos(params, ragel_num, ragel_type, arraySize); }
	break;
	case 127: 
	case 128: 
#line 12 "lex/ragel_units.rl"
	{ragel_type = TYPE_SAMP;}
#line 107 "lex/parser_array_saver.rl"
	{ params.end = getArrayPos(params, ragel_num, ragel_type, arraySize); }
	break;
	case 108: 
	case 111: 
#line 19 "lex/ragel_units.rl"
	{ragel_type = TYPE_SEC;}
#line 106 "lex/parser_array_saver.rl"
	{ params.begin = getArrayPos(params, ragel_num, ragel_type, arraySize); }
	break;
	case 126: 
	case 129: 
#line 19 "lex/ragel_units.rl"
	{ragel_type = TYPE_SEC;}
#line 107 "lex/parser_array_saver.rl"
	{ params.end = getArrayPos(params, ragel_num, ragel_type, arraySize); }
	break;
	case 105: 
	case 106: 
#line 20 "lex/ragel_units.rl"
	{ragel_type = TYPE_MSEC;}
#line 106 "lex/parser_array_saver.rl"
	{ params.begin = getArrayPos(params, ragel_num, ragel_type, arraySize); }
	break;
	case 123: 
	case 124: 
#line 20 "lex/ragel_units.rl"
	{ragel_type = TYPE_MSEC;}
#line 107 "lex/parser_array_saver.rl"
	{ params.end = getArrayPos(params, ragel_num, ragel_type, arraySize); }
	break;
	case 101: 
	case 107: 
#line 25 "lex/ragel_units.rl"
	{ragel_type = TYPE_PERCENT;}
#line 106 "lex/parser_array_saver.rl"
	{ params.begin = getArrayPos(params, ragel_num, ragel_type, arraySize); }
	break;
	case 119: 
	case 125: 
#line 25 "lex/ragel_units.rl"
	{ragel_type = TYPE_PERCENT;}
#line 107 "lex/parser_array_saver.rl"
	{ params.end = getArrayPos(params, ragel_num, ragel_type, arraySize); }
	break;
	case 102: 
#line 28 "lex/ragel_units.rl"
	{ragel_type = TYPE_PHASE;}
#line 106 "lex/parser_array_saver.rl"
	{ params.begin = getArrayPos(params, ragel_num, ragel_type, arraySize); }
	break;
	case 120: 
#line 28 "lex/ragel_units.rl"
	{ragel_type = TYPE_PHASE;}
#line 107 "lex/parser_array_saver.rl"
	{ params.end = getArrayPos(params, ragel_num, ragel_type, arraySize); }
	break;
	case 100: 
	case 112: 
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 74 "lex/parser_array_saver.rl"
	{ ragel_num.vdouble = ragel_num.vint; }
#line 106 "lex/parser_array_saver.rl"
	{ params.begin = getArrayPos(params, ragel_num, ragel_type, arraySize); }
	break;
	case 118: 
	case 130: 
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 74 "lex/parser_array_saver.rl"
	{ ragel_num.vdouble = ragel_num.vint; }
#line 107 "lex/parser_array_saver.rl"
	{ params.end = getArrayPos(params, ragel_num, ragel_type, arraySize); }
	break;
#line 2578 "lex/parser_array_saver.cpp"
	}
	}

	_out: {}
	}

#line 159 "lex/parser_array_saver.rl"


    return cs >= 98;
}


}
}
