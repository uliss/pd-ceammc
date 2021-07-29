
#line 1 "lex/parser_music.rl"
# include "parser_music.h"
# include "parser_numeric.h"

# include <cstdint>
# include <cstring>

namespace ceammc {
namespace parser {


#line 14 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_music.cpp"
static const int bpm_start = 1;
static const int bpm_first_final = 7;
static const int bpm_error = 0;

static const int bpm_en_main = 1;


#line 16 "lex/parser_music.rl"


std::ostream& operator<<(std::ostream& os, const Duration& dur)
{
    if (dur.repeats > 1)
        os << (int)dur.repeats << '*';

    os << (int)dur.num << '/' << (int)dur.den;
    return os;
}

BpmFullMatch::BpmFullMatch()
{
    reset();
}

void BpmFullMatch::reset()
{
    bpm_ = { 0, 0.25 };
}

bool BpmFullMatch::parse(const Atom& a)
{
    reset();

    if (a.isSymbol())
        return parse(a.asT<t_symbol*>()->s_name);
    else if(a.isFloat()) {
        auto f = a.asT<t_float>();
        if(f < 0)
            return false;

        bpm_.bpm = a.asT<t_float>();
        bpm_.beatlen = 0.25;
        return true;
    } else
        return false;
}

bool BpmFullMatch::parse(const char* str)
{
    const auto len = strlen(str);
    if (len == 0)
        return false;

    const char* p = str;
    const char* pe = p + len;
    const char* eof = pe;
    int cat_ = 0;
    AtomType type_;
    fsm::BpmData bpm;

    reset();

    
#line 78 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_music.cpp"
	{
	cs = bpm_start;
	}

#line 71 "lex/parser_music.rl"
    
#line 85 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_music.cpp"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
case 1:
	if ( (*p) == 48 )
		goto tr0;
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr2;
	goto st0;
st0:
cs = 0;
	goto _out;
tr0:
#line 9 "lex/ragel_music.rl"
	{ bpm.dur_num = 1; bpm.dur_den = 4; }
#line 6 "lex/ragel_music.rl"
	{ (bpm.ival *= 10) += ((*p) - '0'); }
	goto st7;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
#line 110 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_music.cpp"
	switch( (*p) ) {
		case 45: goto st2;
		case 46: goto st6;
		case 95: goto st10;
		case 98: goto st3;
	}
	goto st0;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr3;
	goto st0;
tr3:
#line 11 "lex/ragel_music.rl"
	{ bpm.dur_num = 0; }
#line 12 "lex/ragel_music.rl"
	{ (bpm.dur_num *= 10) += ((*p) - '0'); }
#line 13 "lex/ragel_music.rl"
	{ bpm.dur_den = 0;}
#line 14 "lex/ragel_music.rl"
	{ (bpm.dur_den *= 10) += ((*p) - '0'); }
	goto st8;
tr14:
#line 12 "lex/ragel_music.rl"
	{ (bpm.dur_num *= 10) += ((*p) - '0'); }
#line 14 "lex/ragel_music.rl"
	{ (bpm.dur_den *= 10) += ((*p) - '0'); }
	goto st8;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
#line 145 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_music.cpp"
	switch( (*p) ) {
		case 46: goto tr12;
		case 47: goto st5;
		case 95: goto tr15;
		case 98: goto tr16;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr14;
	goto st0;
tr12:
#line 24 "lex/ragel_music.rl"
	{bpm.dur_num = 1;}
#line 8 "lex/ragel_music.rl"
	{ bpm.dur_num *= 3; bpm.dur_den *= 2; }
	goto st9;
tr17:
#line 8 "lex/ragel_music.rl"
	{ bpm.dur_num *= 3; bpm.dur_den *= 2; }
	goto st9;
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
#line 169 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_music.cpp"
	switch( (*p) ) {
		case 95: goto st10;
		case 98: goto st3;
	}
	goto st0;
tr15:
#line 24 "lex/ragel_music.rl"
	{bpm.dur_num = 1;}
	goto st10;
st10:
	if ( ++p == pe )
		goto _test_eof10;
case 10:
#line 183 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_music.cpp"
	if ( (*p) == 98 )
		goto st3;
	goto st0;
tr16:
#line 24 "lex/ragel_music.rl"
	{bpm.dur_num = 1;}
	goto st3;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
#line 195 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_music.cpp"
	if ( (*p) == 112 )
		goto st4;
	goto st0;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
	if ( (*p) == 109 )
		goto st11;
	goto st0;
st11:
	if ( ++p == pe )
		goto _test_eof11;
case 11:
	goto st0;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr6;
	goto st0;
tr6:
#line 13 "lex/ragel_music.rl"
	{ bpm.dur_den = 0;}
#line 14 "lex/ragel_music.rl"
	{ (bpm.dur_den *= 10) += ((*p) - '0'); }
	goto st12;
tr18:
#line 14 "lex/ragel_music.rl"
	{ (bpm.dur_den *= 10) += ((*p) - '0'); }
	goto st12;
st12:
	if ( ++p == pe )
		goto _test_eof12;
case 12:
#line 232 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_music.cpp"
	switch( (*p) ) {
		case 46: goto tr17;
		case 95: goto st10;
		case 98: goto st3;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr18;
	goto st0;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr7;
	goto st0;
tr7:
#line 7 "lex/ragel_music.rl"
	{ (bpm.fnum *= 10) += ((*p) - '0'); bpm.fden *= 10; }
	goto st13;
st13:
	if ( ++p == pe )
		goto _test_eof13;
case 13:
#line 256 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_music.cpp"
	switch( (*p) ) {
		case 45: goto st2;
		case 95: goto st10;
		case 98: goto st3;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr7;
	goto st0;
tr2:
#line 9 "lex/ragel_music.rl"
	{ bpm.dur_num = 1; bpm.dur_den = 4; }
#line 6 "lex/ragel_music.rl"
	{ (bpm.ival *= 10) += ((*p) - '0'); }
	goto st14;
tr19:
#line 6 "lex/ragel_music.rl"
	{ (bpm.ival *= 10) += ((*p) - '0'); }
	goto st14;
st14:
	if ( ++p == pe )
		goto _test_eof14;
case 14:
#line 279 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_music.cpp"
	switch( (*p) ) {
		case 45: goto st2;
		case 46: goto st6;
		case 95: goto st10;
		case 98: goto st3;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr19;
	goto st0;
	}
	_test_eof7: cs = 7; goto _test_eof; 
	_test_eof2: cs = 2; goto _test_eof; 
	_test_eof8: cs = 8; goto _test_eof; 
	_test_eof9: cs = 9; goto _test_eof; 
	_test_eof10: cs = 10; goto _test_eof; 
	_test_eof3: cs = 3; goto _test_eof; 
	_test_eof4: cs = 4; goto _test_eof; 
	_test_eof11: cs = 11; goto _test_eof; 
	_test_eof5: cs = 5; goto _test_eof; 
	_test_eof12: cs = 12; goto _test_eof; 
	_test_eof6: cs = 6; goto _test_eof; 
	_test_eof13: cs = 13; goto _test_eof; 
	_test_eof14: cs = 14; goto _test_eof; 

	_test_eof: {}
	if ( p == eof )
	{
	switch ( cs ) {
	case 7: 
	case 9: 
	case 10: 
	case 11: 
	case 12: 
	case 13: 
	case 14: 
#line 10 "lex/ragel_music.rl"
	{ cat_ = CAT_UNIT; type_ = TYPE_BPM; }
	break;
	case 8: 
#line 24 "lex/ragel_music.rl"
	{bpm.dur_num = 1;}
#line 10 "lex/ragel_music.rl"
	{ cat_ = CAT_UNIT; type_ = TYPE_BPM; }
	break;
#line 324 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_music.cpp"
	}
	}

	_out: {}
	}

#line 72 "lex/parser_music.rl"

    const bool ok = cs >= 7;

    if (ok)
        bpm_ = bpm;

    return ok;
}

size_t BpmFullMatch::parse(const AtomListView& lv, SmallBpmVec& out)
{
    const size_t N = lv.size();

    for (size_t i = 0; i < N; i++) {
        const auto& a = lv[i];
        if (!parse(a))
            return i;

        out.push_back(bpm_);
    }

    return N;
}


#line 357 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_music.cpp"
static const int spn_start = 1;
static const int spn_first_final = 10;
static const int spn_error = 0;

static const int spn_en_main = 1;


#line 102 "lex/parser_music.rl"


SpnFullMatch::SpnFullMatch()
{
    reset();
}

void SpnFullMatch::reset()
{
    spn_ = { };
}

bool SpnFullMatch::parse(const char* str)
{
    const auto len = strlen(str);
    if (len == 0)
        return false;

    const char* p = str;
    const char* pe = p + len;
    const char* eof = pe;
    int cat_ = 0;
    AtomType type_;
    fsm::SpnData spn;

    reset();

    
#line 394 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_music.cpp"
	{
	cs = spn_start;
	}

#line 130 "lex/parser_music.rl"
    
#line 401 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_music.cpp"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
case 1:
	switch( (*p) ) {
		case 65: goto tr0;
		case 66: goto tr2;
		case 72: goto tr2;
	}
	if ( 67 <= (*p) && (*p) <= 71 )
		goto tr3;
	goto st0;
st0:
cs = 0;
	goto _out;
tr0:
#line 77 "lex/ragel_music.rl"
	{spn.alt = 0; spn.oct = 0; spn.octtype = OCTAVE_REL; spn.dev = 0;}
#line 41 "lex/ragel_music.rl"
	{spn.note = 5;}
	goto st10;
tr2:
#line 77 "lex/ragel_music.rl"
	{spn.alt = 0; spn.oct = 0; spn.octtype = OCTAVE_REL; spn.dev = 0;}
#line 42 "lex/ragel_music.rl"
	{spn.note = 6;}
	goto st10;
tr3:
#line 77 "lex/ragel_music.rl"
	{spn.alt = 0; spn.oct = 0; spn.octtype = OCTAVE_REL; spn.dev = 0;}
#line 43 "lex/ragel_music.rl"
	{spn.note = ((*p) - 'C');}
	goto st10;
st10:
	if ( ++p == pe )
		goto _test_eof10;
case 10:
#line 441 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_music.cpp"
	switch( (*p) ) {
		case 35: goto tr12;
		case 45: goto tr13;
		case 94: goto tr15;
		case 95: goto tr16;
		case 98: goto tr17;
		case 124: goto tr18;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr14;
	goto st0;
tr12:
#line 44 "lex/ragel_music.rl"
	{spn.pitch = (spn.note * 2) - (spn.note > 2);}
#line 37 "lex/ragel_music.rl"
	{spn.alt = 1;}
	goto st11;
st11:
	if ( ++p == pe )
		goto _test_eof11;
case 11:
#line 463 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_music.cpp"
	switch( (*p) ) {
		case 35: goto tr19;
		case 45: goto tr20;
		case 94: goto tr22;
		case 95: goto tr23;
		case 124: goto tr24;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr21;
	goto st0;
tr19:
#line 38 "lex/ragel_music.rl"
	{spn.alt = 2;}
	goto st12;
tr36:
#line 40 "lex/ragel_music.rl"
	{spn.alt = -2;}
	goto st12;
st12:
	if ( ++p == pe )
		goto _test_eof12;
case 12:
#line 486 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_music.cpp"
	switch( (*p) ) {
		case 45: goto tr20;
		case 94: goto tr22;
		case 95: goto tr23;
		case 124: goto tr24;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr21;
	goto st0;
tr13:
#line 44 "lex/ragel_music.rl"
	{spn.pitch = (spn.note * 2) - (spn.note > 2);}
#line 60 "lex/ragel_music.rl"
	{spn.octtype = OCTAVE_REL;}
	goto st2;
tr20:
#line 60 "lex/ragel_music.rl"
	{spn.octtype = OCTAVE_REL;}
	goto st2;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
#line 510 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_music.cpp"
	if ( (*p) == 49 )
		goto tr4;
	goto st0;
tr4:
#line 48 "lex/ragel_music.rl"
	{spn.oct = -1;}
	goto st13;
tr14:
#line 44 "lex/ragel_music.rl"
	{spn.pitch = (spn.note * 2) - (spn.note > 2);}
#line 60 "lex/ragel_music.rl"
	{spn.octtype = OCTAVE_REL;}
#line 49 "lex/ragel_music.rl"
	{spn.oct = ((*p) - '0');}
	goto st13;
tr21:
#line 60 "lex/ragel_music.rl"
	{spn.octtype = OCTAVE_REL;}
#line 49 "lex/ragel_music.rl"
	{spn.oct = ((*p) - '0');}
	goto st13;
st13:
	if ( ++p == pe )
		goto _test_eof13;
case 13:
#line 536 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_music.cpp"
	if ( (*p) == 124 )
		goto tr25;
	goto st0;
tr18:
#line 44 "lex/ragel_music.rl"
	{spn.pitch = (spn.note * 2) - (spn.note > 2);}
#line 60 "lex/ragel_music.rl"
	{spn.octtype = OCTAVE_REL;}
	goto st3;
tr24:
#line 60 "lex/ragel_music.rl"
	{spn.octtype = OCTAVE_REL;}
	goto st3;
tr25:
#line 51 "lex/ragel_music.rl"
	{spn.octtype = OCTAVE_ABS;}
	goto st3;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
#line 558 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_music.cpp"
	switch( (*p) ) {
		case 43: goto tr5;
		case 45: goto tr5;
	}
	goto st0;
tr5:
#line 65 "lex/ragel_music.rl"
	{spn.sign = ((*p)=='-') ? -1 : 1;}
	goto st4;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
#line 572 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_music.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr6;
	goto st0;
tr6:
#line 66 "lex/ragel_music.rl"
	{(spn.dev *= 10) += ((*p) - '0');}
	goto st5;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
#line 584 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_music.cpp"
	switch( (*p) ) {
		case 95: goto st7;
		case 99: goto st14;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr7;
	goto st0;
tr7:
#line 66 "lex/ragel_music.rl"
	{(spn.dev *= 10) += ((*p) - '0');}
	goto st6;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
#line 600 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_music.cpp"
	switch( (*p) ) {
		case 95: goto st7;
		case 99: goto st14;
	}
	goto st0;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
	if ( (*p) == 99 )
		goto st14;
	goto st0;
st14:
	if ( ++p == pe )
		goto _test_eof14;
case 14:
	if ( (*p) == 101 )
		goto st8;
	goto st0;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
	if ( (*p) == 110 )
		goto st9;
	goto st0;
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
	if ( (*p) == 116 )
		goto st15;
	goto st0;
st15:
	if ( ++p == pe )
		goto _test_eof15;
case 15:
	goto st0;
tr15:
#line 44 "lex/ragel_music.rl"
	{spn.pitch = (spn.note * 2) - (spn.note > 2);}
#line 60 "lex/ragel_music.rl"
	{spn.octtype = OCTAVE_REL;}
#line 55 "lex/ragel_music.rl"
	{spn.oct++;}
#line 58 "lex/ragel_music.rl"
	{spn.octtype = OCTAVE_REL;}
	goto st16;
tr22:
#line 60 "lex/ragel_music.rl"
	{spn.octtype = OCTAVE_REL;}
#line 55 "lex/ragel_music.rl"
	{spn.oct++;}
#line 58 "lex/ragel_music.rl"
	{spn.octtype = OCTAVE_REL;}
	goto st16;
st16:
	if ( ++p == pe )
		goto _test_eof16;
case 16:
#line 661 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_music.cpp"
	switch( (*p) ) {
		case 94: goto tr27;
		case 124: goto st3;
	}
	goto st0;
tr27:
#line 55 "lex/ragel_music.rl"
	{spn.oct++;}
#line 58 "lex/ragel_music.rl"
	{spn.octtype = OCTAVE_REL;}
	goto st17;
st17:
	if ( ++p == pe )
		goto _test_eof17;
case 17:
#line 677 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_music.cpp"
	switch( (*p) ) {
		case 94: goto tr29;
		case 124: goto st3;
	}
	goto st0;
tr29:
#line 55 "lex/ragel_music.rl"
	{spn.oct++;}
#line 58 "lex/ragel_music.rl"
	{spn.octtype = OCTAVE_REL;}
	goto st18;
st18:
	if ( ++p == pe )
		goto _test_eof18;
case 18:
#line 693 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_music.cpp"
	switch( (*p) ) {
		case 94: goto tr30;
		case 124: goto st3;
	}
	goto st0;
tr30:
#line 55 "lex/ragel_music.rl"
	{spn.oct++;}
#line 58 "lex/ragel_music.rl"
	{spn.octtype = OCTAVE_REL;}
	goto st19;
st19:
	if ( ++p == pe )
		goto _test_eof19;
case 19:
#line 709 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_music.cpp"
	switch( (*p) ) {
		case 94: goto tr31;
		case 124: goto st3;
	}
	goto st0;
tr31:
#line 55 "lex/ragel_music.rl"
	{spn.oct++;}
#line 58 "lex/ragel_music.rl"
	{spn.octtype = OCTAVE_REL;}
	goto st20;
tr35:
#line 57 "lex/ragel_music.rl"
	{spn.oct--;}
#line 58 "lex/ragel_music.rl"
	{spn.octtype = OCTAVE_REL;}
	goto st20;
st20:
	if ( ++p == pe )
		goto _test_eof20;
case 20:
#line 731 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_music.cpp"
	if ( (*p) == 124 )
		goto st3;
	goto st0;
tr16:
#line 44 "lex/ragel_music.rl"
	{spn.pitch = (spn.note * 2) - (spn.note > 2);}
#line 60 "lex/ragel_music.rl"
	{spn.octtype = OCTAVE_REL;}
#line 57 "lex/ragel_music.rl"
	{spn.oct--;}
#line 58 "lex/ragel_music.rl"
	{spn.octtype = OCTAVE_REL;}
	goto st21;
tr23:
#line 60 "lex/ragel_music.rl"
	{spn.octtype = OCTAVE_REL;}
#line 57 "lex/ragel_music.rl"
	{spn.oct--;}
#line 58 "lex/ragel_music.rl"
	{spn.octtype = OCTAVE_REL;}
	goto st21;
st21:
	if ( ++p == pe )
		goto _test_eof21;
case 21:
#line 757 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_music.cpp"
	switch( (*p) ) {
		case 95: goto tr32;
		case 124: goto st3;
	}
	goto st0;
tr32:
#line 57 "lex/ragel_music.rl"
	{spn.oct--;}
#line 58 "lex/ragel_music.rl"
	{spn.octtype = OCTAVE_REL;}
	goto st22;
st22:
	if ( ++p == pe )
		goto _test_eof22;
case 22:
#line 773 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_music.cpp"
	switch( (*p) ) {
		case 95: goto tr33;
		case 124: goto st3;
	}
	goto st0;
tr33:
#line 57 "lex/ragel_music.rl"
	{spn.oct--;}
#line 58 "lex/ragel_music.rl"
	{spn.octtype = OCTAVE_REL;}
	goto st23;
st23:
	if ( ++p == pe )
		goto _test_eof23;
case 23:
#line 789 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_music.cpp"
	switch( (*p) ) {
		case 95: goto tr34;
		case 124: goto st3;
	}
	goto st0;
tr34:
#line 57 "lex/ragel_music.rl"
	{spn.oct--;}
#line 58 "lex/ragel_music.rl"
	{spn.octtype = OCTAVE_REL;}
	goto st24;
st24:
	if ( ++p == pe )
		goto _test_eof24;
case 24:
#line 805 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_music.cpp"
	switch( (*p) ) {
		case 95: goto tr35;
		case 124: goto st3;
	}
	goto st0;
tr17:
#line 44 "lex/ragel_music.rl"
	{spn.pitch = (spn.note * 2) - (spn.note > 2);}
#line 39 "lex/ragel_music.rl"
	{spn.alt = -1;}
	goto st25;
st25:
	if ( ++p == pe )
		goto _test_eof25;
case 25:
#line 821 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_music.cpp"
	switch( (*p) ) {
		case 45: goto tr20;
		case 94: goto tr22;
		case 95: goto tr23;
		case 98: goto tr36;
		case 124: goto tr24;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr21;
	goto st0;
	}
	_test_eof10: cs = 10; goto _test_eof; 
	_test_eof11: cs = 11; goto _test_eof; 
	_test_eof12: cs = 12; goto _test_eof; 
	_test_eof2: cs = 2; goto _test_eof; 
	_test_eof13: cs = 13; goto _test_eof; 
	_test_eof3: cs = 3; goto _test_eof; 
	_test_eof4: cs = 4; goto _test_eof; 
	_test_eof5: cs = 5; goto _test_eof; 
	_test_eof6: cs = 6; goto _test_eof; 
	_test_eof7: cs = 7; goto _test_eof; 
	_test_eof14: cs = 14; goto _test_eof; 
	_test_eof8: cs = 8; goto _test_eof; 
	_test_eof9: cs = 9; goto _test_eof; 
	_test_eof15: cs = 15; goto _test_eof; 
	_test_eof16: cs = 16; goto _test_eof; 
	_test_eof17: cs = 17; goto _test_eof; 
	_test_eof18: cs = 18; goto _test_eof; 
	_test_eof19: cs = 19; goto _test_eof; 
	_test_eof20: cs = 20; goto _test_eof; 
	_test_eof21: cs = 21; goto _test_eof; 
	_test_eof22: cs = 22; goto _test_eof; 
	_test_eof23: cs = 23; goto _test_eof; 
	_test_eof24: cs = 24; goto _test_eof; 
	_test_eof25: cs = 25; goto _test_eof; 

	_test_eof: {}
	if ( p == eof )
	{
	switch ( cs ) {
	case 16: 
	case 17: 
	case 18: 
	case 19: 
	case 20: 
	case 21: 
	case 22: 
	case 23: 
	case 24: 
#line 78 "lex/ragel_music.rl"
	{cat_ = CAT_UNIT; type_ = TYPE_SPN;}
	break;
	case 13: 
#line 51 "lex/ragel_music.rl"
	{spn.octtype = OCTAVE_ABS;}
#line 78 "lex/ragel_music.rl"
	{cat_ = CAT_UNIT; type_ = TYPE_SPN;}
	break;
	case 11: 
	case 12: 
	case 25: 
#line 60 "lex/ragel_music.rl"
	{spn.octtype = OCTAVE_REL;}
#line 78 "lex/ragel_music.rl"
	{cat_ = CAT_UNIT; type_ = TYPE_SPN;}
	break;
	case 14: 
	case 15: 
#line 68 "lex/ragel_music.rl"
	{spn.dev *= spn.sign;}
#line 78 "lex/ragel_music.rl"
	{cat_ = CAT_UNIT; type_ = TYPE_SPN;}
	break;
	case 10: 
#line 44 "lex/ragel_music.rl"
	{spn.pitch = (spn.note * 2) - (spn.note > 2);}
#line 60 "lex/ragel_music.rl"
	{spn.octtype = OCTAVE_REL;}
#line 78 "lex/ragel_music.rl"
	{cat_ = CAT_UNIT; type_ = TYPE_SPN;}
	break;
#line 903 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_music.cpp"
	}
	}

	_out: {}
	}

#line 131 "lex/parser_music.rl"

    const bool ok = cs >= 10;

    if (ok)
        spn_ = spn;

    return ok;
}

bool SpnFullMatch::parse(const Atom& a)
{
    if (a.isSymbol())
        return parse(a.asT<t_symbol*>()->s_name);
    else
        return false;
}

size_t SpnFullMatch::parse(const AtomListView& lv, SmallSpnVec& out)
{
    const size_t N = lv.size();

    for (size_t i = 0; i < N; i++) {
        const auto& a = lv[i];
        if (!parse(a))
            return i;

        out.push_back(spn_);
    }

    return N;
}


#line 944 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_music.cpp"
static const int notation_start = 1;
static const int notation_first_final = 24;
static const int notation_error = 0;

static const int notation_en_main = 1;


#line 169 "lex/parser_music.rl"


NotationSingle::NotationSingle()
{
    reset();
}

void NotationSingle::reset()
{
    note_.spn = {};
    note_.dur = {};
}

bool NotationSingle::parse(const char* str)
{
    const auto len = strlen(str);
    if (len == 0)
        return false;

    const char* p = str;
    const char* pe = p + len;
    const char* eof = pe;
    int cat_ = 0;
    AtomType type_;
    fsm::NotationData note;
    fsm::SpnData spn;

    reset();

    
#line 983 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_music.cpp"
	{
	cs = notation_start;
	}

#line 199 "lex/parser_music.rl"
    
#line 990 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_music.cpp"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
case 1:
	switch( (*p) ) {
		case 65: goto tr0;
		case 66: goto tr2;
		case 72: goto tr2;
		case 82: goto tr4;
	}
	if ( 67 <= (*p) && (*p) <= 71 )
		goto tr3;
	goto st0;
st0:
cs = 0;
	goto _out;
tr0:
#line 77 "lex/ragel_music.rl"
	{spn.alt = 0; spn.oct = 0; spn.octtype = OCTAVE_REL; spn.dev = 0;}
#line 41 "lex/ragel_music.rl"
	{spn.note = 5;}
	goto st24;
tr2:
#line 77 "lex/ragel_music.rl"
	{spn.alt = 0; spn.oct = 0; spn.octtype = OCTAVE_REL; spn.dev = 0;}
#line 42 "lex/ragel_music.rl"
	{spn.note = 6;}
	goto st24;
tr3:
#line 77 "lex/ragel_music.rl"
	{spn.alt = 0; spn.oct = 0; spn.octtype = OCTAVE_REL; spn.dev = 0;}
#line 43 "lex/ragel_music.rl"
	{spn.note = ((*p) - 'C');}
	goto st24;
st24:
	if ( ++p == pe )
		goto _test_eof24;
case 24:
#line 1031 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_music.cpp"
	switch( (*p) ) {
		case 35: goto tr38;
		case 40: goto tr39;
		case 45: goto tr40;
		case 94: goto tr42;
		case 95: goto tr43;
		case 98: goto tr44;
		case 124: goto tr45;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr41;
	goto st0;
tr38:
#line 44 "lex/ragel_music.rl"
	{spn.pitch = (spn.note * 2) - (spn.note > 2);}
#line 37 "lex/ragel_music.rl"
	{spn.alt = 1;}
	goto st25;
st25:
	if ( ++p == pe )
		goto _test_eof25;
case 25:
#line 1054 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_music.cpp"
	switch( (*p) ) {
		case 35: goto tr46;
		case 40: goto tr47;
		case 45: goto tr48;
		case 94: goto tr50;
		case 95: goto tr51;
		case 124: goto tr52;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr49;
	goto st0;
tr46:
#line 38 "lex/ragel_music.rl"
	{spn.alt = 2;}
	goto st26;
tr67:
#line 40 "lex/ragel_music.rl"
	{spn.alt = -2;}
	goto st26;
st26:
	if ( ++p == pe )
		goto _test_eof26;
case 26:
#line 1078 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_music.cpp"
	switch( (*p) ) {
		case 40: goto tr47;
		case 45: goto tr48;
		case 94: goto tr50;
		case 95: goto tr51;
		case 124: goto tr52;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr49;
	goto st0;
tr39:
#line 44 "lex/ragel_music.rl"
	{spn.pitch = (spn.note * 2) - (spn.note > 2);}
#line 60 "lex/ragel_music.rl"
	{spn.octtype = OCTAVE_REL;}
#line 78 "lex/ragel_music.rl"
	{cat_ = CAT_UNIT; type_ = TYPE_SPN;}
#line 131 "lex/ragel_music.rl"
	{note.num = 1; note.den = 4; note.dots = 0; note.durtype = DURATION_REL;}
	goto st2;
tr47:
#line 60 "lex/ragel_music.rl"
	{spn.octtype = OCTAVE_REL;}
#line 78 "lex/ragel_music.rl"
	{cat_ = CAT_UNIT; type_ = TYPE_SPN;}
#line 131 "lex/ragel_music.rl"
	{note.num = 1; note.den = 4; note.dots = 0; note.durtype = DURATION_REL;}
	goto st2;
tr53:
#line 51 "lex/ragel_music.rl"
	{spn.octtype = OCTAVE_ABS;}
#line 78 "lex/ragel_music.rl"
	{cat_ = CAT_UNIT; type_ = TYPE_SPN;}
#line 131 "lex/ragel_music.rl"
	{note.num = 1; note.den = 4; note.dots = 0; note.durtype = DURATION_REL;}
	goto st2;
tr55:
#line 68 "lex/ragel_music.rl"
	{spn.dev *= spn.sign;}
#line 78 "lex/ragel_music.rl"
	{cat_ = CAT_UNIT; type_ = TYPE_SPN;}
#line 131 "lex/ragel_music.rl"
	{note.num = 1; note.den = 4; note.dots = 0; note.durtype = DURATION_REL;}
	goto st2;
tr57:
#line 78 "lex/ragel_music.rl"
	{cat_ = CAT_UNIT; type_ = TYPE_SPN;}
#line 131 "lex/ragel_music.rl"
	{note.num = 1; note.den = 4; note.dots = 0; note.durtype = DURATION_REL;}
	goto st2;
tr68:
#line 131 "lex/ragel_music.rl"
	{note.num = 1; note.den = 4; note.dots = 0; note.durtype = DURATION_REL;}
	goto st2;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
#line 1137 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_music.cpp"
	switch( (*p) ) {
		case 42: goto st3;
		case 47: goto st7;
	}
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr7;
	goto st0;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr8;
	goto st0;
tr8:
#line 102 "lex/ragel_music.rl"
	{ note.num = 0; }
#line 103 "lex/ragel_music.rl"
	{ (note.num *= 10) += ((*p) - '0'); }
	goto st4;
tr11:
#line 103 "lex/ragel_music.rl"
	{ (note.num *= 10) += ((*p) - '0'); }
	goto st4;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
#line 1166 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_music.cpp"
	switch( (*p) ) {
		case 41: goto tr9;
		case 47: goto st5;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr11;
	goto st0;
tr9:
#line 100 "lex/ragel_music.rl"
	{ note.den = 1; }
#line 98 "lex/ragel_music.rl"
	{ note.durtype = DURATION_REL; }
	goto st27;
tr13:
#line 98 "lex/ragel_music.rl"
	{ note.durtype = DURATION_REL; }
	goto st27;
tr16:
#line 99 "lex/ragel_music.rl"
	{ note.num = 1; }
#line 98 "lex/ragel_music.rl"
	{ note.durtype = DURATION_REL; }
	goto st27;
tr18:
#line 99 "lex/ragel_music.rl"
	{ note.num = 1; }
#line 97 "lex/ragel_music.rl"
	{ note.durtype = DURATION_ABS; }
	goto st27;
tr23:
#line 97 "lex/ragel_music.rl"
	{ note.durtype = DURATION_ABS; }
	goto st27;
st27:
	if ( ++p == pe )
		goto _test_eof27;
case 27:
#line 1204 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_music.cpp"
	goto st0;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr12;
	goto st0;
tr12:
#line 104 "lex/ragel_music.rl"
	{ note.den = 0; }
#line 105 "lex/ragel_music.rl"
	{ (note.den *= 10) += ((*p) - '0'); }
	goto st6;
tr14:
#line 105 "lex/ragel_music.rl"
	{ (note.den *= 10) += ((*p) - '0'); }
	goto st6;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
#line 1227 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_music.cpp"
	if ( (*p) == 41 )
		goto tr13;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr14;
	goto st0;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr15;
	goto st0;
tr15:
#line 104 "lex/ragel_music.rl"
	{ note.den = 0; }
#line 105 "lex/ragel_music.rl"
	{ (note.den *= 10) += ((*p) - '0'); }
	goto st8;
tr17:
#line 105 "lex/ragel_music.rl"
	{ (note.den *= 10) += ((*p) - '0'); }
	goto st8;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
#line 1254 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_music.cpp"
	if ( (*p) == 41 )
		goto tr16;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr17;
	goto st0;
tr7:
#line 102 "lex/ragel_music.rl"
	{ note.num = 0; }
#line 103 "lex/ragel_music.rl"
	{ (note.num *= 10) += ((*p) - '0'); }
#line 104 "lex/ragel_music.rl"
	{ note.den = 0; }
#line 105 "lex/ragel_music.rl"
	{ (note.den *= 10) += ((*p) - '0'); }
	goto st9;
tr21:
#line 103 "lex/ragel_music.rl"
	{ (note.num *= 10) += ((*p) - '0'); }
#line 105 "lex/ragel_music.rl"
	{ (note.den *= 10) += ((*p) - '0'); }
	goto st9;
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
#line 1280 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_music.cpp"
	switch( (*p) ) {
		case 41: goto tr18;
		case 46: goto tr19;
		case 47: goto st13;
		case 95: goto tr22;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr21;
	goto st0;
tr19:
#line 99 "lex/ragel_music.rl"
	{ note.num = 1; }
#line 82 "lex/ragel_music.rl"
	{
    note.dots++;
    if(note.dots == 1) {
        note.num *= 3;
        note.den *= 2;
    } else if(note.dots == 2) {
        note.num = note.num / 3 * 7;
        note.den *= 2;
    } else if(note.dots == 3) {
        note.num = note.num / 7 * 15;
        note.den *= 2;
    }
}
	goto st10;
tr27:
#line 82 "lex/ragel_music.rl"
	{
    note.dots++;
    if(note.dots == 1) {
        note.num *= 3;
        note.den *= 2;
    } else if(note.dots == 2) {
        note.num = note.num / 3 * 7;
        note.den *= 2;
    } else if(note.dots == 3) {
        note.num = note.num / 7 * 15;
        note.den *= 2;
    }
}
	goto st10;
st10:
	if ( ++p == pe )
		goto _test_eof10;
case 10:
#line 1328 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_music.cpp"
	switch( (*p) ) {
		case 41: goto tr23;
		case 46: goto tr24;
	}
	goto st0;
tr24:
#line 82 "lex/ragel_music.rl"
	{
    note.dots++;
    if(note.dots == 1) {
        note.num *= 3;
        note.den *= 2;
    } else if(note.dots == 2) {
        note.num = note.num / 3 * 7;
        note.den *= 2;
    } else if(note.dots == 3) {
        note.num = note.num / 7 * 15;
        note.den *= 2;
    }
}
	goto st11;
st11:
	if ( ++p == pe )
		goto _test_eof11;
case 11:
#line 1354 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_music.cpp"
	switch( (*p) ) {
		case 41: goto tr23;
		case 46: goto tr25;
	}
	goto st0;
tr25:
#line 82 "lex/ragel_music.rl"
	{
    note.dots++;
    if(note.dots == 1) {
        note.num *= 3;
        note.den *= 2;
    } else if(note.dots == 2) {
        note.num = note.num / 3 * 7;
        note.den *= 2;
    } else if(note.dots == 3) {
        note.num = note.num / 7 * 15;
        note.den *= 2;
    }
}
	goto st12;
st12:
	if ( ++p == pe )
		goto _test_eof12;
case 12:
#line 1380 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_music.cpp"
	if ( (*p) == 41 )
		goto tr23;
	goto st0;
st13:
	if ( ++p == pe )
		goto _test_eof13;
case 13:
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr26;
	goto st0;
tr26:
#line 104 "lex/ragel_music.rl"
	{ note.den = 0; }
#line 105 "lex/ragel_music.rl"
	{ (note.den *= 10) += ((*p) - '0'); }
	goto st14;
tr28:
#line 105 "lex/ragel_music.rl"
	{ (note.den *= 10) += ((*p) - '0'); }
	goto st14;
st14:
	if ( ++p == pe )
		goto _test_eof14;
case 14:
#line 1405 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_music.cpp"
	switch( (*p) ) {
		case 41: goto tr23;
		case 46: goto tr27;
		case 95: goto st15;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr28;
	goto st0;
tr22:
#line 99 "lex/ragel_music.rl"
	{ note.num = 1; }
	goto st15;
st15:
	if ( ++p == pe )
		goto _test_eof15;
case 15:
#line 1422 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_music.cpp"
	switch( (*p) ) {
		case 41: goto tr23;
		case 46: goto tr27;
	}
	goto st0;
tr40:
#line 44 "lex/ragel_music.rl"
	{spn.pitch = (spn.note * 2) - (spn.note > 2);}
#line 60 "lex/ragel_music.rl"
	{spn.octtype = OCTAVE_REL;}
	goto st16;
tr48:
#line 60 "lex/ragel_music.rl"
	{spn.octtype = OCTAVE_REL;}
	goto st16;
st16:
	if ( ++p == pe )
		goto _test_eof16;
case 16:
#line 1442 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_music.cpp"
	if ( (*p) == 49 )
		goto tr30;
	goto st0;
tr30:
#line 48 "lex/ragel_music.rl"
	{spn.oct = -1;}
	goto st28;
tr41:
#line 44 "lex/ragel_music.rl"
	{spn.pitch = (spn.note * 2) - (spn.note > 2);}
#line 60 "lex/ragel_music.rl"
	{spn.octtype = OCTAVE_REL;}
#line 49 "lex/ragel_music.rl"
	{spn.oct = ((*p) - '0');}
	goto st28;
tr49:
#line 60 "lex/ragel_music.rl"
	{spn.octtype = OCTAVE_REL;}
#line 49 "lex/ragel_music.rl"
	{spn.oct = ((*p) - '0');}
	goto st28;
st28:
	if ( ++p == pe )
		goto _test_eof28;
case 28:
#line 1468 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_music.cpp"
	switch( (*p) ) {
		case 40: goto tr53;
		case 124: goto tr54;
	}
	goto st0;
tr45:
#line 44 "lex/ragel_music.rl"
	{spn.pitch = (spn.note * 2) - (spn.note > 2);}
#line 60 "lex/ragel_music.rl"
	{spn.octtype = OCTAVE_REL;}
	goto st17;
tr52:
#line 60 "lex/ragel_music.rl"
	{spn.octtype = OCTAVE_REL;}
	goto st17;
tr54:
#line 51 "lex/ragel_music.rl"
	{spn.octtype = OCTAVE_ABS;}
	goto st17;
st17:
	if ( ++p == pe )
		goto _test_eof17;
case 17:
#line 1492 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_music.cpp"
	switch( (*p) ) {
		case 43: goto tr31;
		case 45: goto tr31;
	}
	goto st0;
tr31:
#line 65 "lex/ragel_music.rl"
	{spn.sign = ((*p)=='-') ? -1 : 1;}
	goto st18;
st18:
	if ( ++p == pe )
		goto _test_eof18;
case 18:
#line 1506 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_music.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr32;
	goto st0;
tr32:
#line 66 "lex/ragel_music.rl"
	{(spn.dev *= 10) += ((*p) - '0');}
	goto st19;
st19:
	if ( ++p == pe )
		goto _test_eof19;
case 19:
#line 1518 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_music.cpp"
	switch( (*p) ) {
		case 95: goto st21;
		case 99: goto st29;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr33;
	goto st0;
tr33:
#line 66 "lex/ragel_music.rl"
	{(spn.dev *= 10) += ((*p) - '0');}
	goto st20;
st20:
	if ( ++p == pe )
		goto _test_eof20;
case 20:
#line 1534 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_music.cpp"
	switch( (*p) ) {
		case 95: goto st21;
		case 99: goto st29;
	}
	goto st0;
st21:
	if ( ++p == pe )
		goto _test_eof21;
case 21:
	if ( (*p) == 99 )
		goto st29;
	goto st0;
st29:
	if ( ++p == pe )
		goto _test_eof29;
case 29:
	switch( (*p) ) {
		case 40: goto tr55;
		case 101: goto st22;
	}
	goto st0;
st22:
	if ( ++p == pe )
		goto _test_eof22;
case 22:
	if ( (*p) == 110 )
		goto st23;
	goto st0;
st23:
	if ( ++p == pe )
		goto _test_eof23;
case 23:
	if ( (*p) == 116 )
		goto st30;
	goto st0;
st30:
	if ( ++p == pe )
		goto _test_eof30;
case 30:
	if ( (*p) == 40 )
		goto tr55;
	goto st0;
tr42:
#line 44 "lex/ragel_music.rl"
	{spn.pitch = (spn.note * 2) - (spn.note > 2);}
#line 60 "lex/ragel_music.rl"
	{spn.octtype = OCTAVE_REL;}
#line 55 "lex/ragel_music.rl"
	{spn.oct++;}
#line 58 "lex/ragel_music.rl"
	{spn.octtype = OCTAVE_REL;}
	goto st31;
tr50:
#line 60 "lex/ragel_music.rl"
	{spn.octtype = OCTAVE_REL;}
#line 55 "lex/ragel_music.rl"
	{spn.oct++;}
#line 58 "lex/ragel_music.rl"
	{spn.octtype = OCTAVE_REL;}
	goto st31;
st31:
	if ( ++p == pe )
		goto _test_eof31;
case 31:
#line 1599 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_music.cpp"
	switch( (*p) ) {
		case 40: goto tr57;
		case 94: goto tr58;
		case 124: goto st17;
	}
	goto st0;
tr58:
#line 55 "lex/ragel_music.rl"
	{spn.oct++;}
#line 58 "lex/ragel_music.rl"
	{spn.octtype = OCTAVE_REL;}
	goto st32;
st32:
	if ( ++p == pe )
		goto _test_eof32;
case 32:
#line 1616 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_music.cpp"
	switch( (*p) ) {
		case 40: goto tr57;
		case 94: goto tr60;
		case 124: goto st17;
	}
	goto st0;
tr60:
#line 55 "lex/ragel_music.rl"
	{spn.oct++;}
#line 58 "lex/ragel_music.rl"
	{spn.octtype = OCTAVE_REL;}
	goto st33;
st33:
	if ( ++p == pe )
		goto _test_eof33;
case 33:
#line 1633 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_music.cpp"
	switch( (*p) ) {
		case 40: goto tr57;
		case 94: goto tr61;
		case 124: goto st17;
	}
	goto st0;
tr61:
#line 55 "lex/ragel_music.rl"
	{spn.oct++;}
#line 58 "lex/ragel_music.rl"
	{spn.octtype = OCTAVE_REL;}
	goto st34;
st34:
	if ( ++p == pe )
		goto _test_eof34;
case 34:
#line 1650 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_music.cpp"
	switch( (*p) ) {
		case 40: goto tr57;
		case 94: goto tr62;
		case 124: goto st17;
	}
	goto st0;
tr62:
#line 55 "lex/ragel_music.rl"
	{spn.oct++;}
#line 58 "lex/ragel_music.rl"
	{spn.octtype = OCTAVE_REL;}
	goto st35;
tr66:
#line 57 "lex/ragel_music.rl"
	{spn.oct--;}
#line 58 "lex/ragel_music.rl"
	{spn.octtype = OCTAVE_REL;}
	goto st35;
st35:
	if ( ++p == pe )
		goto _test_eof35;
case 35:
#line 1673 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_music.cpp"
	switch( (*p) ) {
		case 40: goto tr57;
		case 124: goto st17;
	}
	goto st0;
tr43:
#line 44 "lex/ragel_music.rl"
	{spn.pitch = (spn.note * 2) - (spn.note > 2);}
#line 60 "lex/ragel_music.rl"
	{spn.octtype = OCTAVE_REL;}
#line 57 "lex/ragel_music.rl"
	{spn.oct--;}
#line 58 "lex/ragel_music.rl"
	{spn.octtype = OCTAVE_REL;}
	goto st36;
tr51:
#line 60 "lex/ragel_music.rl"
	{spn.octtype = OCTAVE_REL;}
#line 57 "lex/ragel_music.rl"
	{spn.oct--;}
#line 58 "lex/ragel_music.rl"
	{spn.octtype = OCTAVE_REL;}
	goto st36;
st36:
	if ( ++p == pe )
		goto _test_eof36;
case 36:
#line 1701 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_music.cpp"
	switch( (*p) ) {
		case 40: goto tr57;
		case 95: goto tr63;
		case 124: goto st17;
	}
	goto st0;
tr63:
#line 57 "lex/ragel_music.rl"
	{spn.oct--;}
#line 58 "lex/ragel_music.rl"
	{spn.octtype = OCTAVE_REL;}
	goto st37;
st37:
	if ( ++p == pe )
		goto _test_eof37;
case 37:
#line 1718 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_music.cpp"
	switch( (*p) ) {
		case 40: goto tr57;
		case 95: goto tr64;
		case 124: goto st17;
	}
	goto st0;
tr64:
#line 57 "lex/ragel_music.rl"
	{spn.oct--;}
#line 58 "lex/ragel_music.rl"
	{spn.octtype = OCTAVE_REL;}
	goto st38;
st38:
	if ( ++p == pe )
		goto _test_eof38;
case 38:
#line 1735 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_music.cpp"
	switch( (*p) ) {
		case 40: goto tr57;
		case 95: goto tr65;
		case 124: goto st17;
	}
	goto st0;
tr65:
#line 57 "lex/ragel_music.rl"
	{spn.oct--;}
#line 58 "lex/ragel_music.rl"
	{spn.octtype = OCTAVE_REL;}
	goto st39;
st39:
	if ( ++p == pe )
		goto _test_eof39;
case 39:
#line 1752 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_music.cpp"
	switch( (*p) ) {
		case 40: goto tr57;
		case 95: goto tr66;
		case 124: goto st17;
	}
	goto st0;
tr44:
#line 44 "lex/ragel_music.rl"
	{spn.pitch = (spn.note * 2) - (spn.note > 2);}
#line 39 "lex/ragel_music.rl"
	{spn.alt = -1;}
	goto st40;
st40:
	if ( ++p == pe )
		goto _test_eof40;
case 40:
#line 1769 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_music.cpp"
	switch( (*p) ) {
		case 40: goto tr47;
		case 45: goto tr48;
		case 94: goto tr50;
		case 95: goto tr51;
		case 98: goto tr67;
		case 124: goto tr52;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr49;
	goto st0;
tr4:
#line 128 "lex/ragel_music.rl"
	{note.rest = 1;}
	goto st41;
st41:
	if ( ++p == pe )
		goto _test_eof41;
case 41:
#line 1789 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_music.cpp"
	if ( (*p) == 40 )
		goto tr68;
	goto st0;
	}
	_test_eof24: cs = 24; goto _test_eof; 
	_test_eof25: cs = 25; goto _test_eof; 
	_test_eof26: cs = 26; goto _test_eof; 
	_test_eof2: cs = 2; goto _test_eof; 
	_test_eof3: cs = 3; goto _test_eof; 
	_test_eof4: cs = 4; goto _test_eof; 
	_test_eof27: cs = 27; goto _test_eof; 
	_test_eof5: cs = 5; goto _test_eof; 
	_test_eof6: cs = 6; goto _test_eof; 
	_test_eof7: cs = 7; goto _test_eof; 
	_test_eof8: cs = 8; goto _test_eof; 
	_test_eof9: cs = 9; goto _test_eof; 
	_test_eof10: cs = 10; goto _test_eof; 
	_test_eof11: cs = 11; goto _test_eof; 
	_test_eof12: cs = 12; goto _test_eof; 
	_test_eof13: cs = 13; goto _test_eof; 
	_test_eof14: cs = 14; goto _test_eof; 
	_test_eof15: cs = 15; goto _test_eof; 
	_test_eof16: cs = 16; goto _test_eof; 
	_test_eof28: cs = 28; goto _test_eof; 
	_test_eof17: cs = 17; goto _test_eof; 
	_test_eof18: cs = 18; goto _test_eof; 
	_test_eof19: cs = 19; goto _test_eof; 
	_test_eof20: cs = 20; goto _test_eof; 
	_test_eof21: cs = 21; goto _test_eof; 
	_test_eof29: cs = 29; goto _test_eof; 
	_test_eof22: cs = 22; goto _test_eof; 
	_test_eof23: cs = 23; goto _test_eof; 
	_test_eof30: cs = 30; goto _test_eof; 
	_test_eof31: cs = 31; goto _test_eof; 
	_test_eof32: cs = 32; goto _test_eof; 
	_test_eof33: cs = 33; goto _test_eof; 
	_test_eof34: cs = 34; goto _test_eof; 
	_test_eof35: cs = 35; goto _test_eof; 
	_test_eof36: cs = 36; goto _test_eof; 
	_test_eof37: cs = 37; goto _test_eof; 
	_test_eof38: cs = 38; goto _test_eof; 
	_test_eof39: cs = 39; goto _test_eof; 
	_test_eof40: cs = 40; goto _test_eof; 
	_test_eof41: cs = 41; goto _test_eof; 

	_test_eof: {}
	if ( p == eof )
	{
	switch ( cs ) {
	case 27: 
#line 132 "lex/ragel_music.rl"
	{cat_ = CAT_UNIT; type_ = TYPE_SPN;}
	break;
	case 41: 
#line 131 "lex/ragel_music.rl"
	{note.num = 1; note.den = 4; note.dots = 0; note.durtype = DURATION_REL;}
#line 132 "lex/ragel_music.rl"
	{cat_ = CAT_UNIT; type_ = TYPE_SPN;}
	break;
	case 31: 
	case 32: 
	case 33: 
	case 34: 
	case 35: 
	case 36: 
	case 37: 
	case 38: 
	case 39: 
#line 78 "lex/ragel_music.rl"
	{cat_ = CAT_UNIT; type_ = TYPE_SPN;}
#line 131 "lex/ragel_music.rl"
	{note.num = 1; note.den = 4; note.dots = 0; note.durtype = DURATION_REL;}
#line 132 "lex/ragel_music.rl"
	{cat_ = CAT_UNIT; type_ = TYPE_SPN;}
	break;
	case 28: 
#line 51 "lex/ragel_music.rl"
	{spn.octtype = OCTAVE_ABS;}
#line 78 "lex/ragel_music.rl"
	{cat_ = CAT_UNIT; type_ = TYPE_SPN;}
#line 131 "lex/ragel_music.rl"
	{note.num = 1; note.den = 4; note.dots = 0; note.durtype = DURATION_REL;}
#line 132 "lex/ragel_music.rl"
	{cat_ = CAT_UNIT; type_ = TYPE_SPN;}
	break;
	case 25: 
	case 26: 
	case 40: 
#line 60 "lex/ragel_music.rl"
	{spn.octtype = OCTAVE_REL;}
#line 78 "lex/ragel_music.rl"
	{cat_ = CAT_UNIT; type_ = TYPE_SPN;}
#line 131 "lex/ragel_music.rl"
	{note.num = 1; note.den = 4; note.dots = 0; note.durtype = DURATION_REL;}
#line 132 "lex/ragel_music.rl"
	{cat_ = CAT_UNIT; type_ = TYPE_SPN;}
	break;
	case 29: 
	case 30: 
#line 68 "lex/ragel_music.rl"
	{spn.dev *= spn.sign;}
#line 78 "lex/ragel_music.rl"
	{cat_ = CAT_UNIT; type_ = TYPE_SPN;}
#line 131 "lex/ragel_music.rl"
	{note.num = 1; note.den = 4; note.dots = 0; note.durtype = DURATION_REL;}
#line 132 "lex/ragel_music.rl"
	{cat_ = CAT_UNIT; type_ = TYPE_SPN;}
	break;
	case 24: 
#line 44 "lex/ragel_music.rl"
	{spn.pitch = (spn.note * 2) - (spn.note > 2);}
#line 60 "lex/ragel_music.rl"
	{spn.octtype = OCTAVE_REL;}
#line 78 "lex/ragel_music.rl"
	{cat_ = CAT_UNIT; type_ = TYPE_SPN;}
#line 131 "lex/ragel_music.rl"
	{note.num = 1; note.den = 4; note.dots = 0; note.durtype = DURATION_REL;}
#line 132 "lex/ragel_music.rl"
	{cat_ = CAT_UNIT; type_ = TYPE_SPN;}
	break;
#line 1910 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_music.cpp"
	}
	}

	_out: {}
	}

#line 200 "lex/parser_music.rl"

    const bool ok = cs >= 24;

    if (ok)
        note_ = Notation(spn, note);


    return ok;
}

bool NotationSingle::parse(const Atom& a)
{
    if (a.isSymbol())
        return parse(a.asT<t_symbol*>()->s_name);
    else
        return false;
}

size_t NotationSingle::parse(const AtomListView& lv, NoteVec& out)
{
    const size_t N = lv.size();

    for (size_t i = 0; i < N; i++) {
        if (!parse(lv[i]))
            return i;

        out.push_back(note_);
    }

    return N;
}


#line 1951 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_music.cpp"
static const int duration_start = 1;
static const int duration_first_final = 4;
static const int duration_error = 0;

static const int duration_en_main = 1;


#line 238 "lex/parser_music.rl"


DurationFullMatch::DurationFullMatch()
{
    reset();
}

void DurationFullMatch::reset()
{
    dur_ = {};
}

bool DurationFullMatch::parse(const char* str)
{
    const auto len = strlen(str);
    if (len == 0)
        return false;

    const char* p = str;
    const char* pe = p + len;
    const char* eof = pe;
    int cat_ = 0;
    AtomType type_;
    fsm::NotationData note;

    reset();

    
#line 1988 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_music.cpp"
	{
	cs = duration_start;
	}

#line 266 "lex/parser_music.rl"
    
#line 1995 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_music.cpp"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
case 1:
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr0;
	goto st0;
st0:
cs = 0;
	goto _out;
tr0:
#line 102 "lex/ragel_music.rl"
	{ note.num = 0; }
#line 103 "lex/ragel_music.rl"
	{ (note.num *= 10) += ((*p) - '0'); }
#line 104 "lex/ragel_music.rl"
	{ note.den = 0; }
#line 105 "lex/ragel_music.rl"
	{ (note.den *= 10) += ((*p) - '0'); }
	goto st4;
tr7:
#line 103 "lex/ragel_music.rl"
	{ (note.num *= 10) += ((*p) - '0'); }
#line 105 "lex/ragel_music.rl"
	{ (note.den *= 10) += ((*p) - '0'); }
	goto st4;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
#line 2028 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_music.cpp"
	switch( (*p) ) {
		case 42: goto st2;
		case 46: goto tr5;
		case 47: goto st3;
		case 95: goto tr8;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr7;
	goto st0;
st2:
#line 101 "lex/ragel_music.rl"
	{ note.repeats = note.num; }
	if ( ++p == pe )
		goto _test_eof2;
case 2:
#line 2044 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_music.cpp"
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr2;
	goto st0;
tr2:
#line 102 "lex/ragel_music.rl"
	{ note.num = 0; }
#line 103 "lex/ragel_music.rl"
	{ (note.num *= 10) += ((*p) - '0'); }
#line 104 "lex/ragel_music.rl"
	{ note.den = 0; }
#line 105 "lex/ragel_music.rl"
	{ (note.den *= 10) += ((*p) - '0'); }
	goto st5;
tr9:
#line 103 "lex/ragel_music.rl"
	{ (note.num *= 10) += ((*p) - '0'); }
#line 105 "lex/ragel_music.rl"
	{ (note.den *= 10) += ((*p) - '0'); }
	goto st5;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
#line 2068 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_music.cpp"
	switch( (*p) ) {
		case 46: goto tr5;
		case 47: goto st3;
		case 95: goto tr8;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr9;
	goto st0;
tr5:
#line 99 "lex/ragel_music.rl"
	{ note.num = 1; }
#line 82 "lex/ragel_music.rl"
	{
    note.dots++;
    if(note.dots == 1) {
        note.num *= 3;
        note.den *= 2;
    } else if(note.dots == 2) {
        note.num = note.num / 3 * 7;
        note.den *= 2;
    } else if(note.dots == 3) {
        note.num = note.num / 7 * 15;
        note.den *= 2;
    }
}
	goto st6;
tr12:
#line 82 "lex/ragel_music.rl"
	{
    note.dots++;
    if(note.dots == 1) {
        note.num *= 3;
        note.den *= 2;
    } else if(note.dots == 2) {
        note.num = note.num / 3 * 7;
        note.den *= 2;
    } else if(note.dots == 3) {
        note.num = note.num / 7 * 15;
        note.den *= 2;
    }
}
	goto st6;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
#line 2115 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_music.cpp"
	if ( (*p) == 46 )
		goto tr10;
	goto st0;
tr10:
#line 82 "lex/ragel_music.rl"
	{
    note.dots++;
    if(note.dots == 1) {
        note.num *= 3;
        note.den *= 2;
    } else if(note.dots == 2) {
        note.num = note.num / 3 * 7;
        note.den *= 2;
    } else if(note.dots == 3) {
        note.num = note.num / 7 * 15;
        note.den *= 2;
    }
}
	goto st7;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
#line 2139 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_music.cpp"
	if ( (*p) == 46 )
		goto tr11;
	goto st0;
tr11:
#line 82 "lex/ragel_music.rl"
	{
    note.dots++;
    if(note.dots == 1) {
        note.num *= 3;
        note.den *= 2;
    } else if(note.dots == 2) {
        note.num = note.num / 3 * 7;
        note.den *= 2;
    } else if(note.dots == 3) {
        note.num = note.num / 7 * 15;
        note.den *= 2;
    }
}
	goto st8;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
#line 2163 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_music.cpp"
	goto st0;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr3;
	goto st0;
tr3:
#line 104 "lex/ragel_music.rl"
	{ note.den = 0; }
#line 105 "lex/ragel_music.rl"
	{ (note.den *= 10) += ((*p) - '0'); }
	goto st9;
tr13:
#line 105 "lex/ragel_music.rl"
	{ (note.den *= 10) += ((*p) - '0'); }
	goto st9;
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
#line 2186 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_music.cpp"
	switch( (*p) ) {
		case 46: goto tr12;
		case 95: goto st10;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr13;
	goto st0;
tr8:
#line 99 "lex/ragel_music.rl"
	{ note.num = 1; }
	goto st10;
st10:
	if ( ++p == pe )
		goto _test_eof10;
case 10:
#line 2202 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_music.cpp"
	if ( (*p) == 46 )
		goto tr12;
	goto st0;
	}
	_test_eof4: cs = 4; goto _test_eof; 
	_test_eof2: cs = 2; goto _test_eof; 
	_test_eof5: cs = 5; goto _test_eof; 
	_test_eof6: cs = 6; goto _test_eof; 
	_test_eof7: cs = 7; goto _test_eof; 
	_test_eof8: cs = 8; goto _test_eof; 
	_test_eof3: cs = 3; goto _test_eof; 
	_test_eof9: cs = 9; goto _test_eof; 
	_test_eof10: cs = 10; goto _test_eof; 

	_test_eof: {}
	if ( p == eof )
	{
	switch ( cs ) {
	case 6: 
	case 7: 
	case 8: 
	case 9: 
	case 10: 
#line 97 "lex/ragel_music.rl"
	{ note.durtype = DURATION_ABS; }
	break;
	case 4: 
	case 5: 
#line 99 "lex/ragel_music.rl"
	{ note.num = 1; }
#line 97 "lex/ragel_music.rl"
	{ note.durtype = DURATION_ABS; }
	break;
#line 2236 "/Users/serge/work/music/pure-data/ceammc/ext/src/lib/lex/parser_music.cpp"
	}
	}

	_out: {}
	}

#line 267 "lex/parser_music.rl"

    const bool ok = cs >= 4;

    if (ok)
        dur_ = note;

    return ok;
}

bool DurationFullMatch::parse(const Atom& a)
{
    if (a.isSymbol())
        return parse(a.asT<t_symbol*>()->s_name);
    else if(a.isFloat()) {
        const auto i = a.asT<int>();
        if (i >= 0)
            dur_ = Duration(1, i);

        return i >= 0;
    }
    else
        return false;
}

size_t DurationFullMatch::parse(const AtomListView& lv, DurationVec& out)
{
    const size_t N = lv.size();

    for (size_t i = 0; i < N; i++) {
        if (!parse(lv[i]))
            return i;

        out.push_back(dur_);
        if(dur_.repeats > 1) {
            out.insert(out.end(), dur_.repeats - 1, out.back());
            dur_.repeats = 1;
        }
    }

    return N;
}


}
}


