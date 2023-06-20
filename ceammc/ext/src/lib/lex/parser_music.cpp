
#line 1 "lex/parser_music.rl"
# include "parser_music.h"
# include "parser_numeric.h"
# include "ceammc_log.h"
# include "ceammc_music_theory_tempo.h"

# include <cstdint>
# include <cstring>

using namespace ceammc;
using namespace ceammc::music;

namespace  {

    struct RagelTempo {
        int ival { 0 };
        int fnum { 0 };
        int fden { 1 };
        int dur_num { 1 };
        int dur_den { 4 };
    };

    struct RagelSpn {
        PitchName note = PitchName::C;
        Alteration alt = Alteration::NATURAL;
        int oct { 0 };
        int dev { 0 };
        int sign { 1 };
        int rest { 0 };
        OctaveType octtype { OCTAVE_ABS };
    };

    struct RagelDuration {
        int16_t num { 0 };
        int16_t den { 1 };
        int8_t dots { 0 };
        int repeats { 1 };
        DurationType durtype { DURATION_ABS };
    };

    struct RagelNotation {
        int dots { 0 };
        int num { 0 };
        int den { 1 };
        int rest { 0 };
        int tnum { 0 };
        int tden { 0 };
        int repeats { 1 };
        DurationType durtype { DURATION_ABS };
    };

    struct RagelTimeSig {
        using Duration = std::pair<std::uint8_t, std::uint8_t>;
        using DurationList = boost::container::small_vector<Duration, 2>;
        DurationList sig;
        std::uint8_t num { 0 }, div { 0 };
    };

    struct RagelChordType {
        std::uint8_t data_[8] = { 0 };

    public:
        RagelChordType() { }
        size_t size() const { return data_[0]; }

        bool assign(std::initializer_list<int> data)
        {
            if (data.size() > 7)
                return false;

            for (int i = 0; i < 7; i++)
                data_[i + 1] = *(data.begin() + i);

            data_[0] = data.size();
            return true;
        }

        std::uint8_t* begin() { return &data_[1]; }
        const std::uint8_t* begin() const { return &data_[1]; }
        std::uint8_t* end() { return &data_[1] + data_[0]; }
        const std::uint8_t* end() const { return &data_[1] + data_[0]; }
    };

    Tempo fromRagel(const RagelTempo& t)
    {
        float bpm = t.ival + t.fnum / float(t.fden);
        Tempo res { bpm, t.dur_den };
        res.setDuration(Duration(t.dur_num, t.dur_den));
        return res;
    }

    Spn fromRagel(const RagelSpn& x)
    {
        if (x.rest)
            return {};

        Spn res(PitchClass(x.note, x.alt), Octave(x.oct, x.octtype));
        res.setDeviation(x.dev);
        return res;
    }

    Duration fromRagel(const RagelDuration& x)
    {
        Duration res(x.num, x.den, x.dots);
        res.setNumRepeats(x.repeats);
        res.setType(x.durtype);
        return res;
    }

    Notation fromRagel(const RagelSpn& spn, const RagelNotation& n)
    {
        auto x_spn = fromRagel(spn);
        auto x_dur = Duration(n.num, n.den, n.dots);
        x_dur.setType(n.durtype);
        Notation res(x_spn, x_dur);
        if (n.repeats > 1)
            res.setNumRepeats(n.repeats);

        return res;
    }

    ChordType fromRagel(const RagelChordType& t)
    {
        ChordType res{{}};
        res.setFrom(t.begin(), t.end());
        return res;
    }
}

namespace ceammc {
namespace parser {


#line 136 "lex/parser_music.cpp"
static const int bpm_start = 1;
static const int bpm_first_final = 7;
static const int bpm_error = 0;

static const int bpm_en_main = 1;


#line 138 "lex/parser_music.rl"


bool parse_tempo(const char* str, music::Tempo& t)
{
    const auto len = strlen(str);
    if (len == 0)
        return false;

    int cs = 0;
    const char* p = str;
    const char* pe = p + len;
    const char* eof = pe;

    DECLARE_RAGEL_COMMON_VARS;
    RagelTempo bpm;

    
#line 162 "lex/parser_music.cpp"
	{
	cs = bpm_start;
	}

#line 155 "lex/parser_music.rl"
    
#line 169 "lex/parser_music.cpp"
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
#line 194 "lex/parser_music.cpp"
	switch( (*p) ) {
		case 46: goto st2;
		case 95: goto st9;
		case 98: goto st3;
		case 124: goto st5;
	}
	goto st0;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr3;
	goto st0;
tr3:
#line 7 "lex/ragel_music.rl"
	{ (bpm.fnum *= 10) += ((*p) - '0'); bpm.fden *= 10; }
	goto st8;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
#line 217 "lex/parser_music.cpp"
	switch( (*p) ) {
		case 95: goto st9;
		case 98: goto st3;
		case 124: goto st5;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr3;
	goto st0;
tr15:
#line 24 "lex/ragel_music.rl"
	{bpm.dur_num = 1;}
	goto st9;
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
#line 234 "lex/parser_music.cpp"
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
#line 246 "lex/parser_music.cpp"
	if ( (*p) == 112 )
		goto st4;
	goto st0;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
	if ( (*p) == 109 )
		goto st10;
	goto st0;
st10:
	if ( ++p == pe )
		goto _test_eof10;
case 10:
	goto st0;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr6;
	goto st0;
tr6:
#line 11 "lex/ragel_music.rl"
	{ bpm.dur_num = 0; }
#line 12 "lex/ragel_music.rl"
	{ (bpm.dur_num *= 10) += ((*p) - '0'); }
#line 13 "lex/ragel_music.rl"
	{ bpm.dur_den = 0;}
#line 14 "lex/ragel_music.rl"
	{ (bpm.dur_den *= 10) += ((*p) - '0'); }
	goto st11;
tr14:
#line 12 "lex/ragel_music.rl"
	{ (bpm.dur_num *= 10) += ((*p) - '0'); }
#line 14 "lex/ragel_music.rl"
	{ (bpm.dur_den *= 10) += ((*p) - '0'); }
	goto st11;
st11:
	if ( ++p == pe )
		goto _test_eof11;
case 11:
#line 289 "lex/parser_music.cpp"
	switch( (*p) ) {
		case 46: goto tr12;
		case 47: goto st6;
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
	goto st12;
tr17:
#line 8 "lex/ragel_music.rl"
	{ bpm.dur_num *= 3; bpm.dur_den *= 2; }
	goto st12;
st12:
	if ( ++p == pe )
		goto _test_eof12;
case 12:
#line 313 "lex/parser_music.cpp"
	switch( (*p) ) {
		case 95: goto st9;
		case 98: goto st3;
	}
	goto st0;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr7;
	goto st0;
tr7:
#line 13 "lex/ragel_music.rl"
	{ bpm.dur_den = 0;}
#line 14 "lex/ragel_music.rl"
	{ (bpm.dur_den *= 10) += ((*p) - '0'); }
	goto st13;
tr18:
#line 14 "lex/ragel_music.rl"
	{ (bpm.dur_den *= 10) += ((*p) - '0'); }
	goto st13;
st13:
	if ( ++p == pe )
		goto _test_eof13;
case 13:
#line 340 "lex/parser_music.cpp"
	switch( (*p) ) {
		case 46: goto tr17;
		case 95: goto st9;
		case 98: goto st3;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr18;
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
#line 363 "lex/parser_music.cpp"
	switch( (*p) ) {
		case 46: goto st2;
		case 95: goto st9;
		case 98: goto st3;
		case 124: goto st5;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr19;
	goto st0;
	}
	_test_eof7: cs = 7; goto _test_eof; 
	_test_eof2: cs = 2; goto _test_eof; 
	_test_eof8: cs = 8; goto _test_eof; 
	_test_eof9: cs = 9; goto _test_eof; 
	_test_eof3: cs = 3; goto _test_eof; 
	_test_eof4: cs = 4; goto _test_eof; 
	_test_eof10: cs = 10; goto _test_eof; 
	_test_eof5: cs = 5; goto _test_eof; 
	_test_eof11: cs = 11; goto _test_eof; 
	_test_eof12: cs = 12; goto _test_eof; 
	_test_eof6: cs = 6; goto _test_eof; 
	_test_eof13: cs = 13; goto _test_eof; 
	_test_eof14: cs = 14; goto _test_eof; 

	_test_eof: {}
	if ( p == eof )
	{
	switch ( cs ) {
	case 7: 
	case 8: 
	case 9: 
	case 10: 
	case 12: 
	case 13: 
	case 14: 
#line 10 "lex/ragel_music.rl"
	{ ragel_cat = CAT_UNIT; ragel_type = TYPE_BPM; }
	break;
	case 11: 
#line 24 "lex/ragel_music.rl"
	{bpm.dur_num = 1;}
#line 10 "lex/ragel_music.rl"
	{ ragel_cat = CAT_UNIT; ragel_type = TYPE_BPM; }
	break;
#line 408 "lex/parser_music.cpp"
	}
	}

	_out: {}
	}

#line 156 "lex/parser_music.rl"

    const bool ok = cs >= 7;

    if (ok)
        t = fromRagel(bpm);

    return ok;
}

bool parse_tempo(const Atom& a, music::Tempo& t)
{
    if (a.isSymbol())
        return parse_tempo(a.asT<t_symbol*>()->s_name, t);
    else if(a.isFloat()) {
        auto f = a.asT<t_float>();
        if (f < 0)
            return false;

        t = Tempo(a.asT<t_float>(), 4);
        return true;
    } else
        return false;
}


#line 441 "lex/parser_music.cpp"
static const int spn_start = 1;
static const int spn_first_final = 8;
static const int spn_error = 0;

static const int spn_en_main = 1;


#line 186 "lex/parser_music.rl"


bool parse_spn(const char* str, music::Spn& res)
{
    const auto len = strlen(str);
    if (len == 0)
        return false;

    int cs = 0;
    const char* p = str;
    const char* pe = p + len;
    const char* eof = pe;

    DECLARE_RAGEL_COMMON_VARS;
    RagelSpn spn;

    
#line 467 "lex/parser_music.cpp"
	{
	cs = spn_start;
	}

#line 203 "lex/parser_music.rl"
    
#line 474 "lex/parser_music.cpp"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
case 1:
	switch( (*p) ) {
		case 65: goto tr0;
		case 67: goto tr3;
		case 68: goto tr4;
		case 69: goto tr5;
		case 70: goto tr6;
		case 71: goto tr7;
	}
	if ( 66 <= (*p) && (*p) <= 72 )
		goto tr2;
	goto st0;
st0:
cs = 0;
	goto _out;
tr0:
#line 81 "lex/ragel_music.rl"
	{spn.alt = Alteration::NATURAL; spn.oct = 0; spn.octtype = OCTAVE_REL; spn.dev = 0;}
#line 41 "lex/ragel_music.rl"
	{spn.note = PitchName::A; }
	goto st8;
tr2:
#line 81 "lex/ragel_music.rl"
	{spn.alt = Alteration::NATURAL; spn.oct = 0; spn.octtype = OCTAVE_REL; spn.dev = 0;}
#line 42 "lex/ragel_music.rl"
	{spn.note = PitchName::B; }
	goto st8;
tr3:
#line 81 "lex/ragel_music.rl"
	{spn.alt = Alteration::NATURAL; spn.oct = 0; spn.octtype = OCTAVE_REL; spn.dev = 0;}
#line 43 "lex/ragel_music.rl"
	{spn.note = PitchName::C; }
	goto st8;
tr4:
#line 81 "lex/ragel_music.rl"
	{spn.alt = Alteration::NATURAL; spn.oct = 0; spn.octtype = OCTAVE_REL; spn.dev = 0;}
#line 44 "lex/ragel_music.rl"
	{spn.note = PitchName::D; }
	goto st8;
tr5:
#line 81 "lex/ragel_music.rl"
	{spn.alt = Alteration::NATURAL; spn.oct = 0; spn.octtype = OCTAVE_REL; spn.dev = 0;}
#line 45 "lex/ragel_music.rl"
	{spn.note = PitchName::E; }
	goto st8;
tr6:
#line 81 "lex/ragel_music.rl"
	{spn.alt = Alteration::NATURAL; spn.oct = 0; spn.octtype = OCTAVE_REL; spn.dev = 0;}
#line 46 "lex/ragel_music.rl"
	{spn.note = PitchName::F; }
	goto st8;
tr7:
#line 81 "lex/ragel_music.rl"
	{spn.alt = Alteration::NATURAL; spn.oct = 0; spn.octtype = OCTAVE_REL; spn.dev = 0;}
#line 47 "lex/ragel_music.rl"
	{spn.note = PitchName::G; }
	goto st8;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
#line 541 "lex/parser_music.cpp"
	switch( (*p) ) {
		case 35: goto tr14;
		case 40: goto tr15;
		case 45: goto tr16;
		case 94: goto tr18;
		case 95: goto tr19;
		case 98: goto tr20;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr17;
	goto st0;
tr14:
#line 37 "lex/ragel_music.rl"
	{spn.alt = Alteration::SHARP; }
	goto st9;
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
#line 561 "lex/parser_music.cpp"
	switch( (*p) ) {
		case 35: goto tr21;
		case 40: goto tr15;
		case 45: goto tr16;
		case 94: goto tr18;
		case 95: goto tr19;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr17;
	goto st0;
tr21:
#line 38 "lex/ragel_music.rl"
	{spn.alt = Alteration::DOUBLE_SHARP; }
	goto st10;
tr32:
#line 40 "lex/ragel_music.rl"
	{spn.alt = Alteration::DOUBLE_FLAT; }
	goto st10;
st10:
	if ( ++p == pe )
		goto _test_eof10;
case 10:
#line 584 "lex/parser_music.cpp"
	switch( (*p) ) {
		case 40: goto tr15;
		case 45: goto tr16;
		case 94: goto tr18;
		case 95: goto tr19;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr17;
	goto st0;
tr15:
#line 64 "lex/ragel_music.rl"
	{spn.octtype = OCTAVE_REL;}
	goto st2;
tr22:
#line 55 "lex/ragel_music.rl"
	{spn.octtype = OCTAVE_ABS;}
	goto st2;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
#line 606 "lex/parser_music.cpp"
	switch( (*p) ) {
		case 43: goto tr8;
		case 45: goto tr8;
	}
	goto st0;
tr8:
#line 69 "lex/ragel_music.rl"
	{spn.sign = ((*p)=='-') ? -1 : 1;}
	goto st3;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
#line 620 "lex/parser_music.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr9;
	goto st0;
tr9:
#line 70 "lex/ragel_music.rl"
	{(spn.dev *= 10) += ((*p) - '0');}
	goto st4;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
#line 632 "lex/parser_music.cpp"
	if ( (*p) == 99 )
		goto st6;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr10;
	goto st0;
tr10:
#line 70 "lex/ragel_music.rl"
	{(spn.dev *= 10) += ((*p) - '0');}
	goto st5;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
#line 646 "lex/parser_music.cpp"
	if ( (*p) == 99 )
		goto st6;
	goto st0;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
	if ( (*p) == 41 )
		goto tr12;
	goto st0;
tr12:
#line 71 "lex/ragel_music.rl"
	{spn.dev *= spn.sign;}
	goto st11;
st11:
	if ( ++p == pe )
		goto _test_eof11;
case 11:
#line 665 "lex/parser_music.cpp"
	goto st0;
tr16:
#line 64 "lex/ragel_music.rl"
	{spn.octtype = OCTAVE_REL;}
	goto st7;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
#line 675 "lex/parser_music.cpp"
	if ( (*p) == 49 )
		goto tr13;
	goto st0;
tr13:
#line 52 "lex/ragel_music.rl"
	{spn.oct = -1;}
	goto st12;
tr17:
#line 64 "lex/ragel_music.rl"
	{spn.octtype = OCTAVE_REL;}
#line 53 "lex/ragel_music.rl"
	{spn.oct = ((*p) - '0');}
	goto st12;
st12:
	if ( ++p == pe )
		goto _test_eof12;
case 12:
#line 693 "lex/parser_music.cpp"
	if ( (*p) == 40 )
		goto tr22;
	goto st0;
tr18:
#line 64 "lex/ragel_music.rl"
	{spn.octtype = OCTAVE_REL;}
#line 59 "lex/ragel_music.rl"
	{spn.oct++;}
#line 62 "lex/ragel_music.rl"
	{spn.octtype = OCTAVE_REL;}
	goto st13;
st13:
	if ( ++p == pe )
		goto _test_eof13;
case 13:
#line 709 "lex/parser_music.cpp"
	switch( (*p) ) {
		case 40: goto st2;
		case 94: goto tr24;
	}
	goto st0;
tr24:
#line 59 "lex/ragel_music.rl"
	{spn.oct++;}
#line 62 "lex/ragel_music.rl"
	{spn.octtype = OCTAVE_REL;}
	goto st14;
st14:
	if ( ++p == pe )
		goto _test_eof14;
case 14:
#line 725 "lex/parser_music.cpp"
	switch( (*p) ) {
		case 40: goto st2;
		case 94: goto tr25;
	}
	goto st0;
tr25:
#line 59 "lex/ragel_music.rl"
	{spn.oct++;}
#line 62 "lex/ragel_music.rl"
	{spn.octtype = OCTAVE_REL;}
	goto st15;
st15:
	if ( ++p == pe )
		goto _test_eof15;
case 15:
#line 741 "lex/parser_music.cpp"
	switch( (*p) ) {
		case 40: goto st2;
		case 94: goto tr26;
	}
	goto st0;
tr26:
#line 59 "lex/ragel_music.rl"
	{spn.oct++;}
#line 62 "lex/ragel_music.rl"
	{spn.octtype = OCTAVE_REL;}
	goto st16;
st16:
	if ( ++p == pe )
		goto _test_eof16;
case 16:
#line 757 "lex/parser_music.cpp"
	switch( (*p) ) {
		case 40: goto st2;
		case 94: goto tr27;
	}
	goto st0;
tr27:
#line 59 "lex/ragel_music.rl"
	{spn.oct++;}
#line 62 "lex/ragel_music.rl"
	{spn.octtype = OCTAVE_REL;}
	goto st17;
tr31:
#line 61 "lex/ragel_music.rl"
	{spn.oct--;}
#line 62 "lex/ragel_music.rl"
	{spn.octtype = OCTAVE_REL;}
	goto st17;
st17:
	if ( ++p == pe )
		goto _test_eof17;
case 17:
#line 779 "lex/parser_music.cpp"
	if ( (*p) == 40 )
		goto st2;
	goto st0;
tr19:
#line 64 "lex/ragel_music.rl"
	{spn.octtype = OCTAVE_REL;}
#line 61 "lex/ragel_music.rl"
	{spn.oct--;}
#line 62 "lex/ragel_music.rl"
	{spn.octtype = OCTAVE_REL;}
	goto st18;
st18:
	if ( ++p == pe )
		goto _test_eof18;
case 18:
#line 795 "lex/parser_music.cpp"
	switch( (*p) ) {
		case 40: goto st2;
		case 95: goto tr28;
	}
	goto st0;
tr28:
#line 61 "lex/ragel_music.rl"
	{spn.oct--;}
#line 62 "lex/ragel_music.rl"
	{spn.octtype = OCTAVE_REL;}
	goto st19;
st19:
	if ( ++p == pe )
		goto _test_eof19;
case 19:
#line 811 "lex/parser_music.cpp"
	switch( (*p) ) {
		case 40: goto st2;
		case 95: goto tr29;
	}
	goto st0;
tr29:
#line 61 "lex/ragel_music.rl"
	{spn.oct--;}
#line 62 "lex/ragel_music.rl"
	{spn.octtype = OCTAVE_REL;}
	goto st20;
st20:
	if ( ++p == pe )
		goto _test_eof20;
case 20:
#line 827 "lex/parser_music.cpp"
	switch( (*p) ) {
		case 40: goto st2;
		case 95: goto tr30;
	}
	goto st0;
tr30:
#line 61 "lex/ragel_music.rl"
	{spn.oct--;}
#line 62 "lex/ragel_music.rl"
	{spn.octtype = OCTAVE_REL;}
	goto st21;
st21:
	if ( ++p == pe )
		goto _test_eof21;
case 21:
#line 843 "lex/parser_music.cpp"
	switch( (*p) ) {
		case 40: goto st2;
		case 95: goto tr31;
	}
	goto st0;
tr20:
#line 39 "lex/ragel_music.rl"
	{spn.alt = Alteration::FLAT; }
	goto st22;
st22:
	if ( ++p == pe )
		goto _test_eof22;
case 22:
#line 857 "lex/parser_music.cpp"
	switch( (*p) ) {
		case 40: goto tr15;
		case 45: goto tr16;
		case 94: goto tr18;
		case 95: goto tr19;
		case 98: goto tr32;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr17;
	goto st0;
	}
	_test_eof8: cs = 8; goto _test_eof; 
	_test_eof9: cs = 9; goto _test_eof; 
	_test_eof10: cs = 10; goto _test_eof; 
	_test_eof2: cs = 2; goto _test_eof; 
	_test_eof3: cs = 3; goto _test_eof; 
	_test_eof4: cs = 4; goto _test_eof; 
	_test_eof5: cs = 5; goto _test_eof; 
	_test_eof6: cs = 6; goto _test_eof; 
	_test_eof11: cs = 11; goto _test_eof; 
	_test_eof7: cs = 7; goto _test_eof; 
	_test_eof12: cs = 12; goto _test_eof; 
	_test_eof13: cs = 13; goto _test_eof; 
	_test_eof14: cs = 14; goto _test_eof; 
	_test_eof15: cs = 15; goto _test_eof; 
	_test_eof16: cs = 16; goto _test_eof; 
	_test_eof17: cs = 17; goto _test_eof; 
	_test_eof18: cs = 18; goto _test_eof; 
	_test_eof19: cs = 19; goto _test_eof; 
	_test_eof20: cs = 20; goto _test_eof; 
	_test_eof21: cs = 21; goto _test_eof; 
	_test_eof22: cs = 22; goto _test_eof; 

	_test_eof: {}
	if ( p == eof )
	{
	switch ( cs ) {
	case 11: 
	case 13: 
	case 14: 
	case 15: 
	case 16: 
	case 17: 
	case 18: 
	case 19: 
	case 20: 
	case 21: 
#line 82 "lex/ragel_music.rl"
	{ragel_cat = CAT_UNIT; ragel_type = TYPE_SPN;}
	break;
	case 12: 
#line 55 "lex/ragel_music.rl"
	{spn.octtype = OCTAVE_ABS;}
#line 82 "lex/ragel_music.rl"
	{ragel_cat = CAT_UNIT; ragel_type = TYPE_SPN;}
	break;
	case 8: 
	case 9: 
	case 10: 
	case 22: 
#line 64 "lex/ragel_music.rl"
	{spn.octtype = OCTAVE_REL;}
#line 82 "lex/ragel_music.rl"
	{ragel_cat = CAT_UNIT; ragel_type = TYPE_SPN;}
	break;
#line 923 "lex/parser_music.cpp"
	}
	}

	_out: {}
	}

#line 204 "lex/parser_music.rl"

    const bool ok = cs >= 8;

    if (ok)
        res = fromRagel(spn);

    return ok;
}

bool parse_spn(const Atom& a, music::Spn& spn)
{
    if (a.isSymbol())
        return parse_spn(a.asT<t_symbol*>()->s_name, spn);
    else
        return false;
}


#line 949 "lex/parser_music.cpp"
static const int pitch_start = 1;
static const int pitch_first_final = 2;
static const int pitch_error = 0;

static const int pitch_en_main = 1;


#line 227 "lex/parser_music.rl"



bool parse_pitch_class(const char* str, music::PitchClass& res)
{
    const auto len = strlen(str);
    if (len == 0)
        return false;

    int cs = 0;
    const char* p = str;
    const char* pe = p + len;
    const char* eof = pe;

    DECLARE_RAGEL_COMMON_VARS;
    RagelSpn spn;

    
#line 976 "lex/parser_music.cpp"
	{
	cs = pitch_start;
	}

#line 245 "lex/parser_music.rl"
    
#line 983 "lex/parser_music.cpp"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
case 1:
	switch( (*p) ) {
		case 65: goto tr0;
		case 67: goto tr3;
		case 68: goto tr4;
		case 69: goto tr5;
		case 70: goto tr6;
		case 71: goto tr7;
	}
	if ( 66 <= (*p) && (*p) <= 72 )
		goto tr2;
	goto st0;
st0:
cs = 0;
	goto _out;
tr0:
#line 85 "lex/ragel_music.rl"
	{spn.alt = Alteration::NATURAL; spn.oct = 0; spn.octtype = OCTAVE_REL; spn.dev = 0;}
#line 41 "lex/ragel_music.rl"
	{spn.note = PitchName::A; }
	goto st2;
tr2:
#line 85 "lex/ragel_music.rl"
	{spn.alt = Alteration::NATURAL; spn.oct = 0; spn.octtype = OCTAVE_REL; spn.dev = 0;}
#line 42 "lex/ragel_music.rl"
	{spn.note = PitchName::B; }
	goto st2;
tr3:
#line 85 "lex/ragel_music.rl"
	{spn.alt = Alteration::NATURAL; spn.oct = 0; spn.octtype = OCTAVE_REL; spn.dev = 0;}
#line 43 "lex/ragel_music.rl"
	{spn.note = PitchName::C; }
	goto st2;
tr4:
#line 85 "lex/ragel_music.rl"
	{spn.alt = Alteration::NATURAL; spn.oct = 0; spn.octtype = OCTAVE_REL; spn.dev = 0;}
#line 44 "lex/ragel_music.rl"
	{spn.note = PitchName::D; }
	goto st2;
tr5:
#line 85 "lex/ragel_music.rl"
	{spn.alt = Alteration::NATURAL; spn.oct = 0; spn.octtype = OCTAVE_REL; spn.dev = 0;}
#line 45 "lex/ragel_music.rl"
	{spn.note = PitchName::E; }
	goto st2;
tr6:
#line 85 "lex/ragel_music.rl"
	{spn.alt = Alteration::NATURAL; spn.oct = 0; spn.octtype = OCTAVE_REL; spn.dev = 0;}
#line 46 "lex/ragel_music.rl"
	{spn.note = PitchName::F; }
	goto st2;
tr7:
#line 85 "lex/ragel_music.rl"
	{spn.alt = Alteration::NATURAL; spn.oct = 0; spn.octtype = OCTAVE_REL; spn.dev = 0;}
#line 47 "lex/ragel_music.rl"
	{spn.note = PitchName::G; }
	goto st2;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
#line 1050 "lex/parser_music.cpp"
	switch( (*p) ) {
		case 35: goto tr8;
		case 98: goto tr9;
	}
	goto st0;
tr8:
#line 37 "lex/ragel_music.rl"
	{spn.alt = Alteration::SHARP; }
	goto st3;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
#line 1064 "lex/parser_music.cpp"
	if ( (*p) == 35 )
		goto tr10;
	goto st0;
tr10:
#line 38 "lex/ragel_music.rl"
	{spn.alt = Alteration::DOUBLE_SHARP; }
	goto st4;
tr11:
#line 40 "lex/ragel_music.rl"
	{spn.alt = Alteration::DOUBLE_FLAT; }
	goto st4;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
#line 1080 "lex/parser_music.cpp"
	goto st0;
tr9:
#line 39 "lex/ragel_music.rl"
	{spn.alt = Alteration::FLAT; }
	goto st5;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
#line 1090 "lex/parser_music.cpp"
	if ( (*p) == 98 )
		goto tr11;
	goto st0;
	}
	_test_eof2: cs = 2; goto _test_eof; 
	_test_eof3: cs = 3; goto _test_eof; 
	_test_eof4: cs = 4; goto _test_eof; 
	_test_eof5: cs = 5; goto _test_eof; 

	_test_eof: {}
	if ( p == eof )
	{
	switch ( cs ) {
	case 2: 
	case 3: 
	case 4: 
	case 5: 
#line 86 "lex/ragel_music.rl"
	{ragel_cat = CAT_UNIT; ragel_type = TYPE_SPN;}
	break;
#line 1111 "lex/parser_music.cpp"
	}
	}

	_out: {}
	}

#line 246 "lex/parser_music.rl"

    const bool ok = cs >= 2;

    if (ok)
        res = PitchClass(static_cast<PitchName>(spn.note), static_cast<Alteration>(spn.alt));

    return ok;
}

bool parse_pitch_class(const Atom& a, music::PitchClass& res)
{
    if (a.isSymbol())
        return parse_pitch_class(a.asT<t_symbol*>()->s_name, res);
    else if(a.isInteger() && a.asT<int>() >= 0 && a.asT<int>() < 12) {
        res = PitchClass(a.asT<int>());
        return true;
    } else
        return false;
}


#line 1140 "lex/parser_music.cpp"
static const int notation_start = 1;
static const int notation_first_final = 15;
static const int notation_error = 0;

static const int notation_en_main = 1;


#line 272 "lex/parser_music.rl"


bool parse_notation(const char* str, Notation& n)
{
    const auto len = strlen(str);
    if (len == 0)
        return false;

    int cs = 0;
    const char* p = str;
    const char* pe = p + len;
    const char* eof = pe;

    DECLARE_RAGEL_COMMON_VARS;
    RagelNotation note;
    RagelSpn spn;

    
#line 1167 "lex/parser_music.cpp"
	{
	cs = notation_start;
	}

#line 290 "lex/parser_music.rl"
    
#line 1174 "lex/parser_music.cpp"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
case 1:
	switch( (*p) ) {
		case 65: goto tr2;
		case 67: goto tr4;
		case 68: goto tr5;
		case 69: goto tr6;
		case 70: goto tr7;
		case 71: goto tr8;
		case 82: goto tr9;
	}
	if ( (*p) > 57 ) {
		if ( 66 <= (*p) && (*p) <= 72 )
			goto tr3;
	} else if ( (*p) >= 49 )
		goto tr0;
	goto st0;
st0:
cs = 0;
	goto _out;
tr0:
#line 130 "lex/ragel_music.rl"
	{ note.repeats = 0; }
#line 131 "lex/ragel_music.rl"
	{ (note.repeats *= 10) += ((*p) - '0'); }
	goto st2;
tr11:
#line 131 "lex/ragel_music.rl"
	{ (note.repeats *= 10) += ((*p) - '0'); }
	goto st2;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
#line 1213 "lex/parser_music.cpp"
	if ( (*p) == 42 )
		goto st3;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr11;
	goto st0;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
	switch( (*p) ) {
		case 65: goto tr2;
		case 67: goto tr4;
		case 68: goto tr5;
		case 69: goto tr6;
		case 70: goto tr7;
		case 71: goto tr8;
		case 82: goto tr9;
	}
	if ( 66 <= (*p) && (*p) <= 72 )
		goto tr3;
	goto st0;
tr2:
#line 81 "lex/ragel_music.rl"
	{spn.alt = Alteration::NATURAL; spn.oct = 0; spn.octtype = OCTAVE_REL; spn.dev = 0;}
#line 41 "lex/ragel_music.rl"
	{spn.note = PitchName::A; }
	goto st15;
tr3:
#line 81 "lex/ragel_music.rl"
	{spn.alt = Alteration::NATURAL; spn.oct = 0; spn.octtype = OCTAVE_REL; spn.dev = 0;}
#line 42 "lex/ragel_music.rl"
	{spn.note = PitchName::B; }
	goto st15;
tr4:
#line 81 "lex/ragel_music.rl"
	{spn.alt = Alteration::NATURAL; spn.oct = 0; spn.octtype = OCTAVE_REL; spn.dev = 0;}
#line 43 "lex/ragel_music.rl"
	{spn.note = PitchName::C; }
	goto st15;
tr5:
#line 81 "lex/ragel_music.rl"
	{spn.alt = Alteration::NATURAL; spn.oct = 0; spn.octtype = OCTAVE_REL; spn.dev = 0;}
#line 44 "lex/ragel_music.rl"
	{spn.note = PitchName::D; }
	goto st15;
tr6:
#line 81 "lex/ragel_music.rl"
	{spn.alt = Alteration::NATURAL; spn.oct = 0; spn.octtype = OCTAVE_REL; spn.dev = 0;}
#line 45 "lex/ragel_music.rl"
	{spn.note = PitchName::E; }
	goto st15;
tr7:
#line 81 "lex/ragel_music.rl"
	{spn.alt = Alteration::NATURAL; spn.oct = 0; spn.octtype = OCTAVE_REL; spn.dev = 0;}
#line 46 "lex/ragel_music.rl"
	{spn.note = PitchName::F; }
	goto st15;
tr8:
#line 81 "lex/ragel_music.rl"
	{spn.alt = Alteration::NATURAL; spn.oct = 0; spn.octtype = OCTAVE_REL; spn.dev = 0;}
#line 47 "lex/ragel_music.rl"
	{spn.note = PitchName::G; }
	goto st15;
st15:
	if ( ++p == pe )
		goto _test_eof15;
case 15:
#line 1281 "lex/parser_music.cpp"
	switch( (*p) ) {
		case 35: goto tr25;
		case 40: goto tr26;
		case 45: goto tr27;
		case 94: goto tr29;
		case 95: goto tr30;
		case 98: goto tr31;
		case 124: goto tr32;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr28;
	goto st0;
tr25:
#line 37 "lex/ragel_music.rl"
	{spn.alt = Alteration::SHARP; }
	goto st16;
st16:
	if ( ++p == pe )
		goto _test_eof16;
case 16:
#line 1302 "lex/parser_music.cpp"
	switch( (*p) ) {
		case 35: goto tr33;
		case 40: goto tr26;
		case 45: goto tr27;
		case 94: goto tr29;
		case 95: goto tr30;
		case 124: goto tr32;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr28;
	goto st0;
tr33:
#line 38 "lex/ragel_music.rl"
	{spn.alt = Alteration::DOUBLE_SHARP; }
	goto st17;
tr59:
#line 40 "lex/ragel_music.rl"
	{spn.alt = Alteration::DOUBLE_FLAT; }
	goto st17;
st17:
	if ( ++p == pe )
		goto _test_eof17;
case 17:
#line 1326 "lex/parser_music.cpp"
	switch( (*p) ) {
		case 40: goto tr26;
		case 45: goto tr27;
		case 94: goto tr29;
		case 95: goto tr30;
		case 124: goto tr32;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr28;
	goto st0;
tr26:
#line 64 "lex/ragel_music.rl"
	{spn.octtype = OCTAVE_REL;}
	goto st4;
tr48:
#line 55 "lex/ragel_music.rl"
	{spn.octtype = OCTAVE_ABS;}
	goto st4;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
#line 1349 "lex/parser_music.cpp"
	switch( (*p) ) {
		case 43: goto tr12;
		case 45: goto tr12;
	}
	goto st0;
tr12:
#line 69 "lex/ragel_music.rl"
	{spn.sign = ((*p)=='-') ? -1 : 1;}
	goto st5;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
#line 1363 "lex/parser_music.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr13;
	goto st0;
tr13:
#line 70 "lex/ragel_music.rl"
	{(spn.dev *= 10) += ((*p) - '0');}
	goto st6;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
#line 1375 "lex/parser_music.cpp"
	if ( (*p) == 99 )
		goto st8;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr14;
	goto st0;
tr14:
#line 70 "lex/ragel_music.rl"
	{(spn.dev *= 10) += ((*p) - '0');}
	goto st7;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
#line 1389 "lex/parser_music.cpp"
	if ( (*p) == 99 )
		goto st8;
	goto st0;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
	if ( (*p) == 41 )
		goto tr16;
	goto st0;
tr16:
#line 71 "lex/ragel_music.rl"
	{spn.dev *= spn.sign;}
	goto st18;
st18:
	if ( ++p == pe )
		goto _test_eof18;
case 18:
#line 1408 "lex/parser_music.cpp"
	if ( (*p) == 124 )
		goto tr34;
	goto st0;
tr32:
#line 64 "lex/ragel_music.rl"
	{spn.octtype = OCTAVE_REL;}
#line 82 "lex/ragel_music.rl"
	{ragel_cat = CAT_UNIT; ragel_type = TYPE_SPN;}
#line 126 "lex/ragel_music.rl"
	{note.num = 1; note.den = 4; note.dots = 0; note.durtype = DURATION_REL;}
	goto st9;
tr34:
#line 82 "lex/ragel_music.rl"
	{ragel_cat = CAT_UNIT; ragel_type = TYPE_SPN;}
#line 126 "lex/ragel_music.rl"
	{note.num = 1; note.den = 4; note.dots = 0; note.durtype = DURATION_REL;}
	goto st9;
tr49:
#line 55 "lex/ragel_music.rl"
	{spn.octtype = OCTAVE_ABS;}
#line 82 "lex/ragel_music.rl"
	{ragel_cat = CAT_UNIT; ragel_type = TYPE_SPN;}
#line 126 "lex/ragel_music.rl"
	{note.num = 1; note.den = 4; note.dots = 0; note.durtype = DURATION_REL;}
	goto st9;
tr60:
#line 126 "lex/ragel_music.rl"
	{note.num = 1; note.den = 4; note.dots = 0; note.durtype = DURATION_REL;}
	goto st9;
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
#line 1442 "lex/parser_music.cpp"
	switch( (*p) ) {
		case 42: goto st10;
		case 47: goto st12;
	}
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr19;
	goto st0;
st10:
	if ( ++p == pe )
		goto _test_eof10;
case 10:
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr20;
	goto st0;
tr20:
#line 97 "lex/ragel_music.rl"
	{ note.num = 0; }
#line 98 "lex/ragel_music.rl"
	{ (note.num *= 10) += ((*p) - '0'); }
	goto st19;
tr36:
#line 98 "lex/ragel_music.rl"
	{ (note.num *= 10) += ((*p) - '0'); }
	goto st19;
st19:
	if ( ++p == pe )
		goto _test_eof19;
case 19:
#line 1471 "lex/parser_music.cpp"
	if ( (*p) == 47 )
		goto st11;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr36;
	goto st0;
st11:
	if ( ++p == pe )
		goto _test_eof11;
case 11:
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr21;
	goto st0;
tr21:
#line 99 "lex/ragel_music.rl"
	{ note.den = 0; }
#line 100 "lex/ragel_music.rl"
	{ (note.den *= 10) += ((*p) - '0'); }
	goto st20;
tr37:
#line 100 "lex/ragel_music.rl"
	{ (note.den *= 10) += ((*p) - '0'); }
	goto st20;
st20:
	if ( ++p == pe )
		goto _test_eof20;
case 20:
#line 1498 "lex/parser_music.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr37;
	goto st0;
st12:
	if ( ++p == pe )
		goto _test_eof12;
case 12:
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr22;
	goto st0;
tr22:
#line 99 "lex/ragel_music.rl"
	{ note.den = 0; }
#line 100 "lex/ragel_music.rl"
	{ (note.den *= 10) += ((*p) - '0'); }
	goto st21;
tr38:
#line 100 "lex/ragel_music.rl"
	{ (note.den *= 10) += ((*p) - '0'); }
	goto st21;
st21:
	if ( ++p == pe )
		goto _test_eof21;
case 21:
#line 1523 "lex/parser_music.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr38;
	goto st0;
tr19:
#line 97 "lex/ragel_music.rl"
	{ note.num = 0; }
#line 98 "lex/ragel_music.rl"
	{ (note.num *= 10) += ((*p) - '0'); }
#line 99 "lex/ragel_music.rl"
	{ note.den = 0; }
#line 100 "lex/ragel_music.rl"
	{ (note.den *= 10) += ((*p) - '0'); }
	goto st22;
tr41:
#line 98 "lex/ragel_music.rl"
	{ (note.num *= 10) += ((*p) - '0'); }
#line 100 "lex/ragel_music.rl"
	{ (note.den *= 10) += ((*p) - '0'); }
	goto st22;
st22:
	if ( ++p == pe )
		goto _test_eof22;
case 22:
#line 1547 "lex/parser_music.cpp"
	switch( (*p) ) {
		case 46: goto tr39;
		case 47: goto st13;
		case 95: goto tr42;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr41;
	goto st0;
tr39:
#line 94 "lex/ragel_music.rl"
	{ note.num = 1; }
#line 89 "lex/ragel_music.rl"
	{ note.dots++; }
	goto st23;
tr45:
#line 89 "lex/ragel_music.rl"
	{ note.dots++; }
	goto st23;
st23:
	if ( ++p == pe )
		goto _test_eof23;
case 23:
#line 1570 "lex/parser_music.cpp"
	if ( (*p) == 46 )
		goto tr43;
	goto st0;
tr43:
#line 89 "lex/ragel_music.rl"
	{ note.dots++; }
	goto st24;
st24:
	if ( ++p == pe )
		goto _test_eof24;
case 24:
#line 1582 "lex/parser_music.cpp"
	if ( (*p) == 46 )
		goto tr44;
	goto st0;
tr44:
#line 89 "lex/ragel_music.rl"
	{ note.dots++; }
	goto st25;
st25:
	if ( ++p == pe )
		goto _test_eof25;
case 25:
#line 1594 "lex/parser_music.cpp"
	goto st0;
st13:
	if ( ++p == pe )
		goto _test_eof13;
case 13:
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr23;
	goto st0;
tr23:
#line 99 "lex/ragel_music.rl"
	{ note.den = 0; }
#line 100 "lex/ragel_music.rl"
	{ (note.den *= 10) += ((*p) - '0'); }
	goto st26;
tr46:
#line 100 "lex/ragel_music.rl"
	{ (note.den *= 10) += ((*p) - '0'); }
	goto st26;
st26:
	if ( ++p == pe )
		goto _test_eof26;
case 26:
#line 1617 "lex/parser_music.cpp"
	switch( (*p) ) {
		case 46: goto tr45;
		case 95: goto st27;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr46;
	goto st0;
tr42:
#line 94 "lex/ragel_music.rl"
	{ note.num = 1; }
	goto st27;
st27:
	if ( ++p == pe )
		goto _test_eof27;
case 27:
#line 1633 "lex/parser_music.cpp"
	if ( (*p) == 46 )
		goto tr45;
	goto st0;
tr27:
#line 64 "lex/ragel_music.rl"
	{spn.octtype = OCTAVE_REL;}
	goto st14;
st14:
	if ( ++p == pe )
		goto _test_eof14;
case 14:
#line 1645 "lex/parser_music.cpp"
	if ( (*p) == 49 )
		goto tr24;
	goto st0;
tr24:
#line 52 "lex/ragel_music.rl"
	{spn.oct = -1;}
	goto st28;
tr28:
#line 64 "lex/ragel_music.rl"
	{spn.octtype = OCTAVE_REL;}
#line 53 "lex/ragel_music.rl"
	{spn.oct = ((*p) - '0');}
	goto st28;
st28:
	if ( ++p == pe )
		goto _test_eof28;
case 28:
#line 1663 "lex/parser_music.cpp"
	switch( (*p) ) {
		case 40: goto tr48;
		case 124: goto tr49;
	}
	goto st0;
tr29:
#line 64 "lex/ragel_music.rl"
	{spn.octtype = OCTAVE_REL;}
#line 59 "lex/ragel_music.rl"
	{spn.oct++;}
#line 62 "lex/ragel_music.rl"
	{spn.octtype = OCTAVE_REL;}
	goto st29;
st29:
	if ( ++p == pe )
		goto _test_eof29;
case 29:
#line 1681 "lex/parser_music.cpp"
	switch( (*p) ) {
		case 40: goto st4;
		case 94: goto tr51;
		case 124: goto tr34;
	}
	goto st0;
tr51:
#line 59 "lex/ragel_music.rl"
	{spn.oct++;}
#line 62 "lex/ragel_music.rl"
	{spn.octtype = OCTAVE_REL;}
	goto st30;
st30:
	if ( ++p == pe )
		goto _test_eof30;
case 30:
#line 1698 "lex/parser_music.cpp"
	switch( (*p) ) {
		case 40: goto st4;
		case 94: goto tr52;
		case 124: goto tr34;
	}
	goto st0;
tr52:
#line 59 "lex/ragel_music.rl"
	{spn.oct++;}
#line 62 "lex/ragel_music.rl"
	{spn.octtype = OCTAVE_REL;}
	goto st31;
st31:
	if ( ++p == pe )
		goto _test_eof31;
case 31:
#line 1715 "lex/parser_music.cpp"
	switch( (*p) ) {
		case 40: goto st4;
		case 94: goto tr53;
		case 124: goto tr34;
	}
	goto st0;
tr53:
#line 59 "lex/ragel_music.rl"
	{spn.oct++;}
#line 62 "lex/ragel_music.rl"
	{spn.octtype = OCTAVE_REL;}
	goto st32;
st32:
	if ( ++p == pe )
		goto _test_eof32;
case 32:
#line 1732 "lex/parser_music.cpp"
	switch( (*p) ) {
		case 40: goto st4;
		case 94: goto tr54;
		case 124: goto tr34;
	}
	goto st0;
tr54:
#line 59 "lex/ragel_music.rl"
	{spn.oct++;}
#line 62 "lex/ragel_music.rl"
	{spn.octtype = OCTAVE_REL;}
	goto st33;
tr58:
#line 61 "lex/ragel_music.rl"
	{spn.oct--;}
#line 62 "lex/ragel_music.rl"
	{spn.octtype = OCTAVE_REL;}
	goto st33;
st33:
	if ( ++p == pe )
		goto _test_eof33;
case 33:
#line 1755 "lex/parser_music.cpp"
	switch( (*p) ) {
		case 40: goto st4;
		case 124: goto tr34;
	}
	goto st0;
tr30:
#line 64 "lex/ragel_music.rl"
	{spn.octtype = OCTAVE_REL;}
#line 61 "lex/ragel_music.rl"
	{spn.oct--;}
#line 62 "lex/ragel_music.rl"
	{spn.octtype = OCTAVE_REL;}
	goto st34;
st34:
	if ( ++p == pe )
		goto _test_eof34;
case 34:
#line 1773 "lex/parser_music.cpp"
	switch( (*p) ) {
		case 40: goto st4;
		case 95: goto tr55;
		case 124: goto tr34;
	}
	goto st0;
tr55:
#line 61 "lex/ragel_music.rl"
	{spn.oct--;}
#line 62 "lex/ragel_music.rl"
	{spn.octtype = OCTAVE_REL;}
	goto st35;
st35:
	if ( ++p == pe )
		goto _test_eof35;
case 35:
#line 1790 "lex/parser_music.cpp"
	switch( (*p) ) {
		case 40: goto st4;
		case 95: goto tr56;
		case 124: goto tr34;
	}
	goto st0;
tr56:
#line 61 "lex/ragel_music.rl"
	{spn.oct--;}
#line 62 "lex/ragel_music.rl"
	{spn.octtype = OCTAVE_REL;}
	goto st36;
st36:
	if ( ++p == pe )
		goto _test_eof36;
case 36:
#line 1807 "lex/parser_music.cpp"
	switch( (*p) ) {
		case 40: goto st4;
		case 95: goto tr57;
		case 124: goto tr34;
	}
	goto st0;
tr57:
#line 61 "lex/ragel_music.rl"
	{spn.oct--;}
#line 62 "lex/ragel_music.rl"
	{spn.octtype = OCTAVE_REL;}
	goto st37;
st37:
	if ( ++p == pe )
		goto _test_eof37;
case 37:
#line 1824 "lex/parser_music.cpp"
	switch( (*p) ) {
		case 40: goto st4;
		case 95: goto tr58;
		case 124: goto tr34;
	}
	goto st0;
tr31:
#line 39 "lex/ragel_music.rl"
	{spn.alt = Alteration::FLAT; }
	goto st38;
st38:
	if ( ++p == pe )
		goto _test_eof38;
case 38:
#line 1839 "lex/parser_music.cpp"
	switch( (*p) ) {
		case 40: goto tr26;
		case 45: goto tr27;
		case 94: goto tr29;
		case 95: goto tr30;
		case 98: goto tr59;
		case 124: goto tr32;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr28;
	goto st0;
tr9:
#line 123 "lex/ragel_music.rl"
	{ spn.rest = 1; }
	goto st39;
st39:
	if ( ++p == pe )
		goto _test_eof39;
case 39:
#line 1859 "lex/parser_music.cpp"
	if ( (*p) == 124 )
		goto tr60;
	goto st0;
	}
	_test_eof2: cs = 2; goto _test_eof; 
	_test_eof3: cs = 3; goto _test_eof; 
	_test_eof15: cs = 15; goto _test_eof; 
	_test_eof16: cs = 16; goto _test_eof; 
	_test_eof17: cs = 17; goto _test_eof; 
	_test_eof4: cs = 4; goto _test_eof; 
	_test_eof5: cs = 5; goto _test_eof; 
	_test_eof6: cs = 6; goto _test_eof; 
	_test_eof7: cs = 7; goto _test_eof; 
	_test_eof8: cs = 8; goto _test_eof; 
	_test_eof18: cs = 18; goto _test_eof; 
	_test_eof9: cs = 9; goto _test_eof; 
	_test_eof10: cs = 10; goto _test_eof; 
	_test_eof19: cs = 19; goto _test_eof; 
	_test_eof11: cs = 11; goto _test_eof; 
	_test_eof20: cs = 20; goto _test_eof; 
	_test_eof12: cs = 12; goto _test_eof; 
	_test_eof21: cs = 21; goto _test_eof; 
	_test_eof22: cs = 22; goto _test_eof; 
	_test_eof23: cs = 23; goto _test_eof; 
	_test_eof24: cs = 24; goto _test_eof; 
	_test_eof25: cs = 25; goto _test_eof; 
	_test_eof13: cs = 13; goto _test_eof; 
	_test_eof26: cs = 26; goto _test_eof; 
	_test_eof27: cs = 27; goto _test_eof; 
	_test_eof14: cs = 14; goto _test_eof; 
	_test_eof28: cs = 28; goto _test_eof; 
	_test_eof29: cs = 29; goto _test_eof; 
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

	_test_eof: {}
	if ( p == eof )
	{
	switch ( cs ) {
	case 23: 
	case 24: 
	case 25: 
	case 26: 
	case 27: 
#line 92 "lex/ragel_music.rl"
	{ note.durtype = DURATION_ABS; }
#line 127 "lex/ragel_music.rl"
	{ragel_cat = CAT_UNIT; ragel_type = TYPE_SPN;}
	break;
	case 20: 
#line 93 "lex/ragel_music.rl"
	{ note.durtype = DURATION_REL; }
#line 127 "lex/ragel_music.rl"
	{ragel_cat = CAT_UNIT; ragel_type = TYPE_SPN;}
	break;
	case 39: 
#line 126 "lex/ragel_music.rl"
	{note.num = 1; note.den = 4; note.dots = 0; note.durtype = DURATION_REL;}
#line 127 "lex/ragel_music.rl"
	{ragel_cat = CAT_UNIT; ragel_type = TYPE_SPN;}
	break;
	case 18: 
	case 29: 
	case 30: 
	case 31: 
	case 32: 
	case 33: 
	case 34: 
	case 35: 
	case 36: 
	case 37: 
#line 82 "lex/ragel_music.rl"
	{ragel_cat = CAT_UNIT; ragel_type = TYPE_SPN;}
#line 126 "lex/ragel_music.rl"
	{note.num = 1; note.den = 4; note.dots = 0; note.durtype = DURATION_REL;}
#line 127 "lex/ragel_music.rl"
	{ragel_cat = CAT_UNIT; ragel_type = TYPE_SPN;}
	break;
	case 22: 
#line 94 "lex/ragel_music.rl"
	{ note.num = 1; }
#line 92 "lex/ragel_music.rl"
	{ note.durtype = DURATION_ABS; }
#line 127 "lex/ragel_music.rl"
	{ragel_cat = CAT_UNIT; ragel_type = TYPE_SPN;}
	break;
	case 21: 
#line 94 "lex/ragel_music.rl"
	{ note.num = 1; }
#line 93 "lex/ragel_music.rl"
	{ note.durtype = DURATION_REL; }
#line 127 "lex/ragel_music.rl"
	{ragel_cat = CAT_UNIT; ragel_type = TYPE_SPN;}
	break;
	case 19: 
#line 95 "lex/ragel_music.rl"
	{ note.den = 1; }
#line 93 "lex/ragel_music.rl"
	{ note.durtype = DURATION_REL; }
#line 127 "lex/ragel_music.rl"
	{ragel_cat = CAT_UNIT; ragel_type = TYPE_SPN;}
	break;
	case 28: 
#line 55 "lex/ragel_music.rl"
	{spn.octtype = OCTAVE_ABS;}
#line 82 "lex/ragel_music.rl"
	{ragel_cat = CAT_UNIT; ragel_type = TYPE_SPN;}
#line 126 "lex/ragel_music.rl"
	{note.num = 1; note.den = 4; note.dots = 0; note.durtype = DURATION_REL;}
#line 127 "lex/ragel_music.rl"
	{ragel_cat = CAT_UNIT; ragel_type = TYPE_SPN;}
	break;
	case 15: 
	case 16: 
	case 17: 
	case 38: 
#line 64 "lex/ragel_music.rl"
	{spn.octtype = OCTAVE_REL;}
#line 82 "lex/ragel_music.rl"
	{ragel_cat = CAT_UNIT; ragel_type = TYPE_SPN;}
#line 126 "lex/ragel_music.rl"
	{note.num = 1; note.den = 4; note.dots = 0; note.durtype = DURATION_REL;}
#line 127 "lex/ragel_music.rl"
	{ragel_cat = CAT_UNIT; ragel_type = TYPE_SPN;}
	break;
#line 1993 "lex/parser_music.cpp"
	}
	}

	_out: {}
	}

#line 291 "lex/parser_music.rl"

    const bool ok = cs >= 15;

    if (ok)
        n = fromRagel(spn, note);

    return ok;
}

bool parse_notation(const Atom& a, Notation& n)
{
    if (a.isSymbol())
        return parse_notation(a.asT<t_symbol*>()->s_name, n);
    else
        return false;
}


#line 2019 "lex/parser_music.cpp"
static const int duration_start = 1;
static const int duration_first_final = 4;
static const int duration_error = 0;

static const int duration_en_main = 1;


#line 314 "lex/parser_music.rl"


bool parse_duration(const char* str, Duration& dur)
{
    const auto len = strlen(str);
    if (len == 0)
        return false;

    int cs = 0;
    const char* p = str;
    const char* pe = p + len;
    const char* eof = pe;
    int cat_ = 0;
    AtomType type_;
    RagelDuration note;

    
#line 2045 "lex/parser_music.cpp"
	{
	cs = duration_start;
	}

#line 331 "lex/parser_music.rl"
    
#line 2052 "lex/parser_music.cpp"
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
#line 97 "lex/ragel_music.rl"
	{ note.num = 0; }
#line 98 "lex/ragel_music.rl"
	{ (note.num *= 10) += ((*p) - '0'); }
#line 99 "lex/ragel_music.rl"
	{ note.den = 0; }
#line 100 "lex/ragel_music.rl"
	{ (note.den *= 10) += ((*p) - '0'); }
	goto st4;
tr7:
#line 98 "lex/ragel_music.rl"
	{ (note.num *= 10) += ((*p) - '0'); }
#line 100 "lex/ragel_music.rl"
	{ (note.den *= 10) += ((*p) - '0'); }
	goto st4;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
#line 2085 "lex/parser_music.cpp"
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
#line 96 "lex/ragel_music.rl"
	{ note.repeats = note.num; }
	if ( ++p == pe )
		goto _test_eof2;
case 2:
#line 2101 "lex/parser_music.cpp"
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr2;
	goto st0;
tr2:
#line 97 "lex/ragel_music.rl"
	{ note.num = 0; }
#line 98 "lex/ragel_music.rl"
	{ (note.num *= 10) += ((*p) - '0'); }
#line 99 "lex/ragel_music.rl"
	{ note.den = 0; }
#line 100 "lex/ragel_music.rl"
	{ (note.den *= 10) += ((*p) - '0'); }
	goto st5;
tr9:
#line 98 "lex/ragel_music.rl"
	{ (note.num *= 10) += ((*p) - '0'); }
#line 100 "lex/ragel_music.rl"
	{ (note.den *= 10) += ((*p) - '0'); }
	goto st5;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
#line 2125 "lex/parser_music.cpp"
	switch( (*p) ) {
		case 46: goto tr5;
		case 47: goto st3;
		case 95: goto tr8;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr9;
	goto st0;
tr5:
#line 94 "lex/ragel_music.rl"
	{ note.num = 1; }
#line 89 "lex/ragel_music.rl"
	{ note.dots++; }
	goto st6;
tr12:
#line 89 "lex/ragel_music.rl"
	{ note.dots++; }
	goto st6;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
#line 2148 "lex/parser_music.cpp"
	if ( (*p) == 46 )
		goto tr10;
	goto st0;
tr10:
#line 89 "lex/ragel_music.rl"
	{ note.dots++; }
	goto st7;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
#line 2160 "lex/parser_music.cpp"
	if ( (*p) == 46 )
		goto tr11;
	goto st0;
tr11:
#line 89 "lex/ragel_music.rl"
	{ note.dots++; }
	goto st8;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
#line 2172 "lex/parser_music.cpp"
	goto st0;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr3;
	goto st0;
tr3:
#line 99 "lex/ragel_music.rl"
	{ note.den = 0; }
#line 100 "lex/ragel_music.rl"
	{ (note.den *= 10) += ((*p) - '0'); }
	goto st9;
tr13:
#line 100 "lex/ragel_music.rl"
	{ (note.den *= 10) += ((*p) - '0'); }
	goto st9;
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
#line 2195 "lex/parser_music.cpp"
	switch( (*p) ) {
		case 46: goto tr12;
		case 95: goto st10;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr13;
	goto st0;
tr8:
#line 94 "lex/ragel_music.rl"
	{ note.num = 1; }
	goto st10;
st10:
	if ( ++p == pe )
		goto _test_eof10;
case 10:
#line 2211 "lex/parser_music.cpp"
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
#line 92 "lex/ragel_music.rl"
	{ note.durtype = DURATION_ABS; }
	break;
	case 4: 
	case 5: 
#line 94 "lex/ragel_music.rl"
	{ note.num = 1; }
#line 92 "lex/ragel_music.rl"
	{ note.durtype = DURATION_ABS; }
	break;
#line 2245 "lex/parser_music.cpp"
	}
	}

	_out: {}
	}

#line 332 "lex/parser_music.rl"

    const bool ok = cs >= 4;

    if (ok)
        dur = fromRagel(note);

    return ok;
}

bool parse_duration(const Atom& a, Duration& dur)
{
    if (a.isSymbol())
        return parse_duration(a.asT<t_symbol*>()->s_name, dur);
    else if(a.isFloat()) {
        const auto i = a.asT<int>();
        if (i >= 0)
            dur = Duration(1, i);

        return i >= 0;
    }
    else
        return false;
}


#line 2278 "lex/parser_music.cpp"
static const int time_signature_start = 1;
static const int time_signature_first_final = 6;
static const int time_signature_error = 0;

static const int time_signature_en_main = 1;


#line 362 "lex/parser_music.rl"


bool parse_time_signature(const char* str, music::TimeSignature& ts)
{
    const auto len = strlen(str);
    if (len == 0)
        return false;

    int cs = 0;
    const char* p = str;
    const char* pe = p + len;
    const char* eof = pe;

    RagelTimeSig ragel_ts;

    
#line 2303 "lex/parser_music.cpp"
	{
	cs = time_signature_start;
	}

#line 378 "lex/parser_music.rl"
    
#line 2310 "lex/parser_music.cpp"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
case 1:
	if ( (*p) == 124 )
		goto st2;
	goto st0;
st0:
cs = 0;
	goto _out;
tr6:
#line 145 "lex/ragel_music.rl"
	{ ragel_ts.sig.push_back({ragel_ts.num, ragel_ts.div}); }
	goto st2;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
#line 2331 "lex/parser_music.cpp"
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr2;
	goto st0;
tr2:
#line 141 "lex/ragel_music.rl"
	{ ragel_ts.num = 0; }
#line 141 "lex/ragel_music.rl"
	{ (ragel_ts.num *= 10) += ((*p) - '0'); }
	goto st3;
tr4:
#line 141 "lex/ragel_music.rl"
	{ (ragel_ts.num *= 10) += ((*p) - '0'); }
	goto st3;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
#line 2349 "lex/parser_music.cpp"
	if ( (*p) == 47 )
		goto st4;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr4;
	goto st0;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr5;
	goto st0;
tr5:
#line 142 "lex/ragel_music.rl"
	{ ragel_ts.div = 0; }
#line 142 "lex/ragel_music.rl"
	{ (ragel_ts.div *= 10) += ((*p) - '0'); }
	goto st5;
tr7:
#line 142 "lex/ragel_music.rl"
	{ (ragel_ts.div *= 10) += ((*p) - '0'); }
	goto st5;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
#line 2376 "lex/parser_music.cpp"
	switch( (*p) ) {
		case 43: goto tr6;
		case 124: goto tr8;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr7;
	goto st0;
tr8:
#line 145 "lex/ragel_music.rl"
	{ ragel_ts.sig.push_back({ragel_ts.num, ragel_ts.div}); }
	goto st6;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
#line 2392 "lex/parser_music.cpp"
	goto st0;
	}
	_test_eof2: cs = 2; goto _test_eof; 
	_test_eof3: cs = 3; goto _test_eof; 
	_test_eof4: cs = 4; goto _test_eof; 
	_test_eof5: cs = 5; goto _test_eof; 
	_test_eof6: cs = 6; goto _test_eof; 

	_test_eof: {}
	_out: {}
	}

#line 379 "lex/parser_music.rl"

    const bool ok = cs >= 6;

    if (ok) {
        auto& data = ragel_ts.sig;
        auto N = data.size();
        if (N == 0)
            return false;

        ts.set(data.front().first, data.front().second);

        for (size_t i = 1; i < N; i++)
            ts.append(data[i].first, data[i].second);
    }

    return ok;
}


#line 2425 "lex/parser_music.cpp"
static const int chord_names_start = 1;
static const int chord_names_first_final = 42;
static const int chord_names_error = 0;

static const int chord_names_en_main = 1;


#line 403 "lex/parser_music.rl"


bool parse_chord_class(const char* str, ChordClass& res)
{
    const auto len = strlen(str);
    if (len == 0)
        return false;

    int cs = 0;
    const char* p = str;
    const char* pe = p + len;
    const char* eof = pe;

    DECLARE_RAGEL_COMMON_VARS;
    RagelSpn spn;
    RagelChordType rg_chord_type;
    const char* name = "";

    
#line 2453 "lex/parser_music.cpp"
	{
	cs = chord_names_start;
	}

#line 422 "lex/parser_music.rl"
    
#line 2460 "lex/parser_music.cpp"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
case 1:
	switch( (*p) ) {
		case 65: goto tr0;
		case 67: goto tr3;
		case 68: goto tr4;
		case 69: goto tr5;
		case 70: goto tr6;
		case 71: goto tr7;
	}
	if ( 66 <= (*p) && (*p) <= 72 )
		goto tr2;
	goto st0;
st0:
cs = 0;
	goto _out;
tr0:
#line 85 "lex/ragel_music.rl"
	{spn.alt = Alteration::NATURAL; spn.oct = 0; spn.octtype = OCTAVE_REL; spn.dev = 0;}
#line 41 "lex/ragel_music.rl"
	{spn.note = PitchName::A; }
	goto st42;
tr2:
#line 85 "lex/ragel_music.rl"
	{spn.alt = Alteration::NATURAL; spn.oct = 0; spn.octtype = OCTAVE_REL; spn.dev = 0;}
#line 42 "lex/ragel_music.rl"
	{spn.note = PitchName::B; }
	goto st42;
tr3:
#line 85 "lex/ragel_music.rl"
	{spn.alt = Alteration::NATURAL; spn.oct = 0; spn.octtype = OCTAVE_REL; spn.dev = 0;}
#line 43 "lex/ragel_music.rl"
	{spn.note = PitchName::C; }
	goto st42;
tr4:
#line 85 "lex/ragel_music.rl"
	{spn.alt = Alteration::NATURAL; spn.oct = 0; spn.octtype = OCTAVE_REL; spn.dev = 0;}
#line 44 "lex/ragel_music.rl"
	{spn.note = PitchName::D; }
	goto st42;
tr5:
#line 85 "lex/ragel_music.rl"
	{spn.alt = Alteration::NATURAL; spn.oct = 0; spn.octtype = OCTAVE_REL; spn.dev = 0;}
#line 45 "lex/ragel_music.rl"
	{spn.note = PitchName::E; }
	goto st42;
tr6:
#line 85 "lex/ragel_music.rl"
	{spn.alt = Alteration::NATURAL; spn.oct = 0; spn.octtype = OCTAVE_REL; spn.dev = 0;}
#line 46 "lex/ragel_music.rl"
	{spn.note = PitchName::F; }
	goto st42;
tr7:
#line 85 "lex/ragel_music.rl"
	{spn.alt = Alteration::NATURAL; spn.oct = 0; spn.octtype = OCTAVE_REL; spn.dev = 0;}
#line 47 "lex/ragel_music.rl"
	{spn.note = PitchName::G; }
	goto st42;
st42:
	if ( ++p == pe )
		goto _test_eof42;
case 42:
#line 2527 "lex/parser_music.cpp"
	switch( (*p) ) {
		case 35: goto tr65;
		case 43: goto tr66;
		case 49: goto tr67;
		case 55: goto tr68;
		case 57: goto tr69;
		case 97: goto tr70;
		case 98: goto tr71;
		case 100: goto tr72;
		case 104: goto tr73;
		case 109: goto tr74;
		case 115: goto tr75;
	}
	goto st0;
tr65:
#line 37 "lex/ragel_music.rl"
	{spn.alt = Alteration::SHARP; }
	goto st43;
st43:
	if ( ++p == pe )
		goto _test_eof43;
case 43:
#line 2550 "lex/parser_music.cpp"
	switch( (*p) ) {
		case 35: goto tr76;
		case 43: goto tr66;
		case 49: goto tr67;
		case 55: goto tr68;
		case 57: goto tr69;
		case 97: goto tr70;
		case 100: goto tr72;
		case 104: goto tr73;
		case 109: goto tr74;
		case 115: goto tr75;
	}
	goto st0;
tr76:
#line 38 "lex/ragel_music.rl"
	{spn.alt = Alteration::DOUBLE_SHARP; }
	goto st44;
tr486:
#line 40 "lex/ragel_music.rl"
	{spn.alt = Alteration::DOUBLE_FLAT; }
	goto st44;
st44:
	if ( ++p == pe )
		goto _test_eof44;
case 44:
#line 2576 "lex/parser_music.cpp"
	switch( (*p) ) {
		case 43: goto tr66;
		case 49: goto tr67;
		case 55: goto tr68;
		case 57: goto tr69;
		case 97: goto tr70;
		case 100: goto tr72;
		case 104: goto tr73;
		case 109: goto tr74;
		case 115: goto tr75;
	}
	goto st0;
tr66:
#line 86 "lex/ragel_music.rl"
	{ragel_cat = CAT_UNIT; ragel_type = TYPE_SPN;}
	goto st2;
tr77:
#line 158 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 10}); }
	goto st2;
tr86:
#line 191 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 7, 10, 14, 17}); }
	goto st2;
tr95:
#line 164 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 7, 10}); }
	goto st2;
tr105:
#line 172 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 6, 10}); }
	goto st2;
tr114:
#line 177 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 7, 10, 14}); }
	goto st2;
tr123:
#line 156 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 8}); name = "Augmented triad";}
	goto st2;
tr132:
#line 196 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 8, 10, 14, 17}); }
	goto st2;
tr141:
#line 157 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 6}); name = "Diminished triad"; }
	goto st2;
tr151:
#line 187 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 6, 9, 13}); }
	goto st2;
tr160:
#line 185 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 6, 10, 13}); }
	goto st2;
tr169:
#line 154 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 7}); name = "Major triad"; }
	goto st2;
tr178:
#line 192 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 7, 11, 14, 17}); }
	goto st2;
tr187:
#line 159 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 5, 7}); }
	goto st2;
tr198:
#line 160 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 2, 7}); }
	goto st2;
tr207:
#line 202 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 7, 11, 14, 17, 21}); }
	goto st2;
tr216:
#line 165 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 7, 11}); }
	goto st2;
tr225:
#line 178 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 7, 11, 14}); }
	goto st2;
tr234:
#line 155 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 7}); name = "Minor triad"; }
	goto st2;
tr243:
#line 193 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 7, 10, 14, 17}); }
	goto st2;
tr252:
#line 204 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 7, 10, 14, 17, 21}); }
	goto st2;
tr261:
#line 166 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 7, 10}); }
	goto st2;
tr270:
#line 179 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 7, 10, 14}); }
	goto st2;
tr279:
#line 194 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 7, 11, 14, 17}); }
	goto st2;
tr288:
#line 203 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 7, 11, 14, 17, 21}); }
	goto st2;
tr297:
#line 167 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 7, 11}); }
	goto st2;
tr306:
#line 181 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 7, 11, 14}); }
	goto st2;
tr315:
#line 197 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 6, 10, 14, 17}); }
	goto st2;
tr324:
#line 207 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 6, 10, 14, 17, 21}); }
	goto st2;
tr333:
#line 171 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 6, 10}); }
	goto st2;
tr342:
#line 184 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 6, 10, 14}); }
	goto st2;
tr351:
#line 198 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 6, 9, 14, 17}); }
	goto st2;
tr360:
#line 170 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 6, 9}); }
	goto st2;
tr369:
#line 186 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 6, 9, 14}); }
	goto st2;
tr378:
#line 206 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 8, 10, 14, 17, 21}); }
	goto st2;
tr387:
#line 168 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 8, 10}); }
	goto st2;
tr396:
#line 183 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 8, 10, 14}); }
	goto st2;
tr405:
#line 195 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 8, 11, 14, 17}); }
	goto st2;
tr414:
#line 205 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 8, 11, 14, 17, 21}); }
	goto st2;
tr423:
#line 169 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 8, 11}); }
	goto st2;
tr432:
#line 182 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 8, 11, 14}); }
	goto st2;
tr441:
#line 188 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 5, 7, 10, 14}); }
	goto st2;
tr450:
#line 180 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 7, 10, 13}); }
	goto st2;
tr459:
#line 173 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 2, 7, 10}); }
	goto st2;
tr468:
#line 174 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 5, 7, 10}); }
	goto st2;
tr477:
#line 201 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 7, 10, 14, 17, 21}); }
	goto st2;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
#line 2777 "lex/parser_music.cpp"
	if ( (*p) == 54 )
		goto st45;
	goto st0;
st45:
	if ( ++p == pe )
		goto _test_eof45;
case 45:
	switch( (*p) ) {
		case 43: goto tr77;
		case 49: goto tr78;
		case 55: goto tr79;
		case 57: goto tr80;
		case 97: goto tr81;
		case 100: goto tr82;
		case 104: goto tr83;
		case 109: goto tr84;
		case 115: goto tr85;
	}
	goto st0;
tr67:
#line 86 "lex/ragel_music.rl"
	{ragel_cat = CAT_UNIT; ragel_type = TYPE_SPN;}
	goto st3;
tr78:
#line 158 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 10}); }
	goto st3;
tr87:
#line 191 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 7, 10, 14, 17}); }
	goto st3;
tr97:
#line 164 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 7, 10}); }
	goto st3;
tr106:
#line 172 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 6, 10}); }
	goto st3;
tr115:
#line 177 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 7, 10, 14}); }
	goto st3;
tr133:
#line 196 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 8, 10, 14, 17}); }
	goto st3;
tr152:
#line 187 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 6, 9, 13}); }
	goto st3;
tr161:
#line 185 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 6, 10, 13}); }
	goto st3;
tr179:
#line 192 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 7, 11, 14, 17}); }
	goto st3;
tr188:
#line 159 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 5, 7}); }
	goto st3;
tr199:
#line 160 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 2, 7}); }
	goto st3;
tr208:
#line 202 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 7, 11, 14, 17, 21}); }
	goto st3;
tr217:
#line 165 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 7, 11}); }
	goto st3;
tr226:
#line 178 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 7, 11, 14}); }
	goto st3;
tr244:
#line 193 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 7, 10, 14, 17}); }
	goto st3;
tr253:
#line 204 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 7, 10, 14, 17, 21}); }
	goto st3;
tr262:
#line 166 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 7, 10}); }
	goto st3;
tr271:
#line 179 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 7, 10, 14}); }
	goto st3;
tr280:
#line 194 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 7, 11, 14, 17}); }
	goto st3;
tr289:
#line 203 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 7, 11, 14, 17, 21}); }
	goto st3;
tr298:
#line 167 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 7, 11}); }
	goto st3;
tr307:
#line 181 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 7, 11, 14}); }
	goto st3;
tr316:
#line 197 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 6, 10, 14, 17}); }
	goto st3;
tr325:
#line 207 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 6, 10, 14, 17, 21}); }
	goto st3;
tr334:
#line 171 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 6, 10}); }
	goto st3;
tr343:
#line 184 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 6, 10, 14}); }
	goto st3;
tr352:
#line 198 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 6, 9, 14, 17}); }
	goto st3;
tr361:
#line 170 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 6, 9}); }
	goto st3;
tr370:
#line 186 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 6, 9, 14}); }
	goto st3;
tr379:
#line 206 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 8, 10, 14, 17, 21}); }
	goto st3;
tr388:
#line 168 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 8, 10}); }
	goto st3;
tr397:
#line 183 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 8, 10, 14}); }
	goto st3;
tr406:
#line 195 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 8, 11, 14, 17}); }
	goto st3;
tr415:
#line 205 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 8, 11, 14, 17, 21}); }
	goto st3;
tr424:
#line 169 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 8, 11}); }
	goto st3;
tr433:
#line 182 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 8, 11, 14}); }
	goto st3;
tr442:
#line 188 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 5, 7, 10, 14}); }
	goto st3;
tr451:
#line 180 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 7, 10, 13}); }
	goto st3;
tr460:
#line 173 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 2, 7, 10}); }
	goto st3;
tr469:
#line 174 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 5, 7, 10}); }
	goto st3;
tr478:
#line 201 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 7, 10, 14, 17, 21}); }
	goto st3;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
#line 2969 "lex/parser_music.cpp"
	switch( (*p) ) {
		case 49: goto st46;
		case 51: goto st90;
	}
	goto st0;
st46:
	if ( ++p == pe )
		goto _test_eof46;
case 46:
	switch( (*p) ) {
		case 43: goto tr86;
		case 49: goto tr87;
		case 55: goto tr88;
		case 57: goto tr89;
		case 97: goto tr90;
		case 100: goto tr91;
		case 104: goto tr92;
		case 109: goto tr93;
		case 115: goto tr94;
	}
	goto st0;
tr68:
#line 86 "lex/ragel_music.rl"
	{ragel_cat = CAT_UNIT; ragel_type = TYPE_SPN;}
	goto st47;
tr79:
#line 158 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 10}); }
	goto st47;
tr88:
#line 191 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 7, 10, 14, 17}); }
	goto st47;
tr98:
#line 164 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 7, 10}); }
	goto st47;
tr107:
#line 172 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 6, 10}); }
	goto st47;
tr116:
#line 177 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 7, 10, 14}); }
	goto st47;
tr134:
#line 196 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 8, 10, 14, 17}); }
	goto st47;
tr153:
#line 187 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 6, 9, 13}); }
	goto st47;
tr162:
#line 185 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 6, 10, 13}); }
	goto st47;
tr180:
#line 192 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 7, 11, 14, 17}); }
	goto st47;
tr191:
#line 159 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 5, 7}); }
	goto st47;
tr200:
#line 160 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 2, 7}); }
	goto st47;
tr209:
#line 202 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 7, 11, 14, 17, 21}); }
	goto st47;
tr218:
#line 165 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 7, 11}); }
	goto st47;
tr227:
#line 178 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 7, 11, 14}); }
	goto st47;
tr245:
#line 193 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 7, 10, 14, 17}); }
	goto st47;
tr254:
#line 204 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 7, 10, 14, 17, 21}); }
	goto st47;
tr263:
#line 166 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 7, 10}); }
	goto st47;
tr272:
#line 179 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 7, 10, 14}); }
	goto st47;
tr281:
#line 194 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 7, 11, 14, 17}); }
	goto st47;
tr290:
#line 203 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 7, 11, 14, 17, 21}); }
	goto st47;
tr299:
#line 167 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 7, 11}); }
	goto st47;
tr308:
#line 181 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 7, 11, 14}); }
	goto st47;
tr317:
#line 197 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 6, 10, 14, 17}); }
	goto st47;
tr326:
#line 207 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 6, 10, 14, 17, 21}); }
	goto st47;
tr335:
#line 171 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 6, 10}); }
	goto st47;
tr344:
#line 184 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 6, 10, 14}); }
	goto st47;
tr353:
#line 198 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 6, 9, 14, 17}); }
	goto st47;
tr362:
#line 170 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 6, 9}); }
	goto st47;
tr371:
#line 186 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 6, 9, 14}); }
	goto st47;
tr380:
#line 206 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 8, 10, 14, 17, 21}); }
	goto st47;
tr389:
#line 168 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 8, 10}); }
	goto st47;
tr398:
#line 183 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 8, 10, 14}); }
	goto st47;
tr407:
#line 195 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 8, 11, 14, 17}); }
	goto st47;
tr416:
#line 205 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 8, 11, 14, 17, 21}); }
	goto st47;
tr425:
#line 169 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 8, 11}); }
	goto st47;
tr434:
#line 182 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 8, 11, 14}); }
	goto st47;
tr443:
#line 188 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 5, 7, 10, 14}); }
	goto st47;
tr452:
#line 180 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 7, 10, 13}); }
	goto st47;
tr461:
#line 173 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 2, 7, 10}); }
	goto st47;
tr470:
#line 174 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 5, 7, 10}); }
	goto st47;
tr479:
#line 201 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 7, 10, 14, 17, 21}); }
	goto st47;
st47:
	if ( ++p == pe )
		goto _test_eof47;
case 47:
#line 3163 "lex/parser_music.cpp"
	switch( (*p) ) {
		case 43: goto tr95;
		case 45: goto st4;
		case 49: goto tr97;
		case 55: goto tr98;
		case 57: goto tr99;
		case 97: goto tr100;
		case 100: goto tr101;
		case 104: goto tr102;
		case 109: goto tr103;
		case 115: goto st39;
	}
	goto st0;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
	switch( (*p) ) {
		case 53: goto st48;
		case 57: goto st87;
	}
	goto st0;
st48:
	if ( ++p == pe )
		goto _test_eof48;
case 48:
	switch( (*p) ) {
		case 43: goto tr105;
		case 49: goto tr106;
		case 55: goto tr107;
		case 57: goto tr108;
		case 97: goto tr109;
		case 100: goto tr110;
		case 104: goto tr111;
		case 109: goto tr112;
		case 115: goto tr113;
	}
	goto st0;
tr69:
#line 86 "lex/ragel_music.rl"
	{ragel_cat = CAT_UNIT; ragel_type = TYPE_SPN;}
	goto st49;
tr80:
#line 158 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 10}); }
	goto st49;
tr89:
#line 191 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 7, 10, 14, 17}); }
	goto st49;
tr99:
#line 164 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 7, 10}); }
	goto st49;
tr108:
#line 172 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 6, 10}); }
	goto st49;
tr117:
#line 177 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 7, 10, 14}); }
	goto st49;
tr135:
#line 196 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 8, 10, 14, 17}); }
	goto st49;
tr154:
#line 187 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 6, 9, 13}); }
	goto st49;
tr163:
#line 185 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 6, 10, 13}); }
	goto st49;
tr181:
#line 192 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 7, 11, 14, 17}); }
	goto st49;
tr192:
#line 159 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 5, 7}); }
	goto st49;
tr201:
#line 160 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 2, 7}); }
	goto st49;
tr210:
#line 202 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 7, 11, 14, 17, 21}); }
	goto st49;
tr219:
#line 165 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 7, 11}); }
	goto st49;
tr228:
#line 178 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 7, 11, 14}); }
	goto st49;
tr246:
#line 193 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 7, 10, 14, 17}); }
	goto st49;
tr255:
#line 204 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 7, 10, 14, 17, 21}); }
	goto st49;
tr264:
#line 166 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 7, 10}); }
	goto st49;
tr273:
#line 179 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 7, 10, 14}); }
	goto st49;
tr282:
#line 194 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 7, 11, 14, 17}); }
	goto st49;
tr291:
#line 203 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 7, 11, 14, 17, 21}); }
	goto st49;
tr300:
#line 167 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 7, 11}); }
	goto st49;
tr309:
#line 181 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 7, 11, 14}); }
	goto st49;
tr318:
#line 197 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 6, 10, 14, 17}); }
	goto st49;
tr327:
#line 207 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 6, 10, 14, 17, 21}); }
	goto st49;
tr336:
#line 171 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 6, 10}); }
	goto st49;
tr345:
#line 184 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 6, 10, 14}); }
	goto st49;
tr354:
#line 198 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 6, 9, 14, 17}); }
	goto st49;
tr363:
#line 170 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 6, 9}); }
	goto st49;
tr372:
#line 186 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 6, 9, 14}); }
	goto st49;
tr381:
#line 206 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 8, 10, 14, 17, 21}); }
	goto st49;
tr390:
#line 168 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 8, 10}); }
	goto st49;
tr399:
#line 183 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 8, 10, 14}); }
	goto st49;
tr408:
#line 195 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 8, 11, 14, 17}); }
	goto st49;
tr417:
#line 205 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 8, 11, 14, 17, 21}); }
	goto st49;
tr426:
#line 169 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 8, 11}); }
	goto st49;
tr435:
#line 182 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 8, 11, 14}); }
	goto st49;
tr444:
#line 188 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 5, 7, 10, 14}); }
	goto st49;
tr453:
#line 180 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 7, 10, 13}); }
	goto st49;
tr462:
#line 173 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 2, 7, 10}); }
	goto st49;
tr471:
#line 174 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 5, 7, 10}); }
	goto st49;
tr480:
#line 201 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 7, 10, 14, 17, 21}); }
	goto st49;
st49:
	if ( ++p == pe )
		goto _test_eof49;
case 49:
#line 3374 "lex/parser_music.cpp"
	switch( (*p) ) {
		case 43: goto tr114;
		case 49: goto tr115;
		case 55: goto tr116;
		case 57: goto tr117;
		case 97: goto tr118;
		case 100: goto tr119;
		case 104: goto tr120;
		case 109: goto tr121;
		case 115: goto st36;
	}
	goto st0;
tr70:
#line 86 "lex/ragel_music.rl"
	{ragel_cat = CAT_UNIT; ragel_type = TYPE_SPN;}
	goto st5;
tr81:
#line 158 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 10}); }
	goto st5;
tr90:
#line 191 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 7, 10, 14, 17}); }
	goto st5;
tr100:
#line 164 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 7, 10}); }
	goto st5;
tr109:
#line 172 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 6, 10}); }
	goto st5;
tr118:
#line 177 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 7, 10, 14}); }
	goto st5;
tr127:
#line 156 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 8}); name = "Augmented triad";}
	goto st5;
tr136:
#line 196 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 8, 10, 14, 17}); }
	goto st5;
tr146:
#line 157 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 6}); name = "Diminished triad"; }
	goto st5;
tr155:
#line 187 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 6, 9, 13}); }
	goto st5;
tr164:
#line 185 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 6, 10, 13}); }
	goto st5;
tr173:
#line 154 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 7}); name = "Major triad"; }
	goto st5;
tr182:
#line 192 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 7, 11, 14, 17}); }
	goto st5;
tr193:
#line 159 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 5, 7}); }
	goto st5;
tr202:
#line 160 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 2, 7}); }
	goto st5;
tr211:
#line 202 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 7, 11, 14, 17, 21}); }
	goto st5;
tr220:
#line 165 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 7, 11}); }
	goto st5;
tr229:
#line 178 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 7, 11, 14}); }
	goto st5;
tr238:
#line 155 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 7}); name = "Minor triad"; }
	goto st5;
tr247:
#line 193 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 7, 10, 14, 17}); }
	goto st5;
tr256:
#line 204 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 7, 10, 14, 17, 21}); }
	goto st5;
tr265:
#line 166 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 7, 10}); }
	goto st5;
tr274:
#line 179 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 7, 10, 14}); }
	goto st5;
tr283:
#line 194 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 7, 11, 14, 17}); }
	goto st5;
tr292:
#line 203 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 7, 11, 14, 17, 21}); }
	goto st5;
tr301:
#line 167 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 7, 11}); }
	goto st5;
tr310:
#line 181 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 7, 11, 14}); }
	goto st5;
tr319:
#line 197 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 6, 10, 14, 17}); }
	goto st5;
tr328:
#line 207 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 6, 10, 14, 17, 21}); }
	goto st5;
tr337:
#line 171 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 6, 10}); }
	goto st5;
tr346:
#line 184 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 6, 10, 14}); }
	goto st5;
tr355:
#line 198 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 6, 9, 14, 17}); }
	goto st5;
tr364:
#line 170 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 6, 9}); }
	goto st5;
tr373:
#line 186 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 6, 9, 14}); }
	goto st5;
tr382:
#line 206 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 8, 10, 14, 17, 21}); }
	goto st5;
tr391:
#line 168 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 8, 10}); }
	goto st5;
tr400:
#line 183 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 8, 10, 14}); }
	goto st5;
tr409:
#line 195 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 8, 11, 14, 17}); }
	goto st5;
tr418:
#line 205 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 8, 11, 14, 17, 21}); }
	goto st5;
tr427:
#line 169 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 8, 11}); }
	goto st5;
tr436:
#line 182 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 8, 11, 14}); }
	goto st5;
tr445:
#line 188 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 5, 7, 10, 14}); }
	goto st5;
tr454:
#line 180 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 7, 10, 13}); }
	goto st5;
tr463:
#line 173 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 2, 7, 10}); }
	goto st5;
tr472:
#line 174 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 5, 7, 10}); }
	goto st5;
tr481:
#line 201 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 7, 10, 14, 17, 21}); }
	goto st5;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
#line 3575 "lex/parser_music.cpp"
	if ( (*p) == 117 )
		goto st6;
	goto st0;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
	if ( (*p) == 103 )
		goto st50;
	goto st0;
st50:
	if ( ++p == pe )
		goto _test_eof50;
case 50:
	switch( (*p) ) {
		case 43: goto tr123;
		case 49: goto st7;
		case 55: goto st80;
		case 57: goto st81;
		case 97: goto tr127;
		case 100: goto tr128;
		case 104: goto tr129;
		case 109: goto st32;
		case 115: goto tr131;
	}
	goto st0;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
	switch( (*p) ) {
		case 49: goto st51;
		case 51: goto st79;
	}
	goto st0;
st51:
	if ( ++p == pe )
		goto _test_eof51;
case 51:
	switch( (*p) ) {
		case 43: goto tr132;
		case 49: goto tr133;
		case 55: goto tr134;
		case 57: goto tr135;
		case 97: goto tr136;
		case 100: goto tr137;
		case 104: goto tr138;
		case 109: goto tr139;
		case 115: goto tr140;
	}
	goto st0;
tr72:
#line 86 "lex/ragel_music.rl"
	{ragel_cat = CAT_UNIT; ragel_type = TYPE_SPN;}
	goto st8;
tr82:
#line 158 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 10}); }
	goto st8;
tr91:
#line 191 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 7, 10, 14, 17}); }
	goto st8;
tr101:
#line 164 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 7, 10}); }
	goto st8;
tr110:
#line 172 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 6, 10}); }
	goto st8;
tr119:
#line 177 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 7, 10, 14}); }
	goto st8;
tr128:
#line 156 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 8}); name = "Augmented triad";}
	goto st8;
tr137:
#line 196 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 8, 10, 14, 17}); }
	goto st8;
tr147:
#line 157 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 6}); name = "Diminished triad"; }
	goto st8;
tr156:
#line 187 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 6, 9, 13}); }
	goto st8;
tr165:
#line 185 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 6, 10, 13}); }
	goto st8;
tr174:
#line 154 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 7}); name = "Major triad"; }
	goto st8;
tr183:
#line 192 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 7, 11, 14, 17}); }
	goto st8;
tr194:
#line 159 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 5, 7}); }
	goto st8;
tr203:
#line 160 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 2, 7}); }
	goto st8;
tr212:
#line 202 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 7, 11, 14, 17, 21}); }
	goto st8;
tr221:
#line 165 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 7, 11}); }
	goto st8;
tr230:
#line 178 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 7, 11, 14}); }
	goto st8;
tr239:
#line 155 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 7}); name = "Minor triad"; }
	goto st8;
tr248:
#line 193 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 7, 10, 14, 17}); }
	goto st8;
tr257:
#line 204 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 7, 10, 14, 17, 21}); }
	goto st8;
tr266:
#line 166 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 7, 10}); }
	goto st8;
tr275:
#line 179 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 7, 10, 14}); }
	goto st8;
tr284:
#line 194 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 7, 11, 14, 17}); }
	goto st8;
tr293:
#line 203 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 7, 11, 14, 17, 21}); }
	goto st8;
tr302:
#line 167 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 7, 11}); }
	goto st8;
tr311:
#line 181 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 7, 11, 14}); }
	goto st8;
tr320:
#line 197 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 6, 10, 14, 17}); }
	goto st8;
tr329:
#line 207 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 6, 10, 14, 17, 21}); }
	goto st8;
tr338:
#line 171 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 6, 10}); }
	goto st8;
tr347:
#line 184 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 6, 10, 14}); }
	goto st8;
tr356:
#line 198 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 6, 9, 14, 17}); }
	goto st8;
tr365:
#line 170 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 6, 9}); }
	goto st8;
tr374:
#line 186 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 6, 9, 14}); }
	goto st8;
tr383:
#line 206 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 8, 10, 14, 17, 21}); }
	goto st8;
tr392:
#line 168 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 8, 10}); }
	goto st8;
tr401:
#line 183 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 8, 10, 14}); }
	goto st8;
tr410:
#line 195 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 8, 11, 14, 17}); }
	goto st8;
tr419:
#line 205 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 8, 11, 14, 17, 21}); }
	goto st8;
tr428:
#line 169 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 8, 11}); }
	goto st8;
tr437:
#line 182 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 8, 11, 14}); }
	goto st8;
tr446:
#line 188 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 5, 7, 10, 14}); }
	goto st8;
tr455:
#line 180 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 7, 10, 13}); }
	goto st8;
tr464:
#line 173 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 2, 7, 10}); }
	goto st8;
tr473:
#line 174 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 5, 7, 10}); }
	goto st8;
tr482:
#line 201 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 7, 10, 14, 17, 21}); }
	goto st8;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
#line 3815 "lex/parser_music.cpp"
	if ( (*p) == 105 )
		goto st9;
	goto st0;
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
	if ( (*p) == 109 )
		goto st52;
	goto st0;
st52:
	if ( ++p == pe )
		goto _test_eof52;
case 52:
	switch( (*p) ) {
		case 43: goto tr141;
		case 45: goto st10;
		case 49: goto st31;
		case 55: goto st77;
		case 57: goto st78;
		case 97: goto tr146;
		case 100: goto tr147;
		case 104: goto tr148;
		case 109: goto tr149;
		case 115: goto tr150;
	}
	goto st0;
st10:
	if ( ++p == pe )
		goto _test_eof10;
case 10:
	if ( (*p) == 57 )
		goto st53;
	goto st0;
st53:
	if ( ++p == pe )
		goto _test_eof53;
case 53:
	switch( (*p) ) {
		case 43: goto tr151;
		case 49: goto tr152;
		case 55: goto tr153;
		case 57: goto tr154;
		case 97: goto tr155;
		case 100: goto tr156;
		case 104: goto tr157;
		case 109: goto tr158;
		case 115: goto tr159;
	}
	goto st0;
tr73:
#line 86 "lex/ragel_music.rl"
	{ragel_cat = CAT_UNIT; ragel_type = TYPE_SPN;}
	goto st11;
tr83:
#line 158 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 10}); }
	goto st11;
tr92:
#line 191 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 7, 10, 14, 17}); }
	goto st11;
tr102:
#line 164 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 7, 10}); }
	goto st11;
tr111:
#line 172 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 6, 10}); }
	goto st11;
tr120:
#line 177 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 7, 10, 14}); }
	goto st11;
tr129:
#line 156 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 8}); name = "Augmented triad";}
	goto st11;
tr138:
#line 196 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 8, 10, 14, 17}); }
	goto st11;
tr148:
#line 157 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 6}); name = "Diminished triad"; }
	goto st11;
tr157:
#line 187 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 6, 9, 13}); }
	goto st11;
tr166:
#line 185 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 6, 10, 13}); }
	goto st11;
tr175:
#line 154 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 7}); name = "Major triad"; }
	goto st11;
tr184:
#line 192 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 7, 11, 14, 17}); }
	goto st11;
tr195:
#line 159 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 5, 7}); }
	goto st11;
tr204:
#line 160 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 2, 7}); }
	goto st11;
tr213:
#line 202 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 7, 11, 14, 17, 21}); }
	goto st11;
tr222:
#line 165 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 7, 11}); }
	goto st11;
tr231:
#line 178 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 7, 11, 14}); }
	goto st11;
tr240:
#line 155 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 7}); name = "Minor triad"; }
	goto st11;
tr249:
#line 193 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 7, 10, 14, 17}); }
	goto st11;
tr258:
#line 204 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 7, 10, 14, 17, 21}); }
	goto st11;
tr267:
#line 166 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 7, 10}); }
	goto st11;
tr276:
#line 179 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 7, 10, 14}); }
	goto st11;
tr285:
#line 194 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 7, 11, 14, 17}); }
	goto st11;
tr294:
#line 203 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 7, 11, 14, 17, 21}); }
	goto st11;
tr303:
#line 167 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 7, 11}); }
	goto st11;
tr312:
#line 181 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 7, 11, 14}); }
	goto st11;
tr321:
#line 197 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 6, 10, 14, 17}); }
	goto st11;
tr330:
#line 207 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 6, 10, 14, 17, 21}); }
	goto st11;
tr339:
#line 171 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 6, 10}); }
	goto st11;
tr348:
#line 184 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 6, 10, 14}); }
	goto st11;
tr357:
#line 198 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 6, 9, 14, 17}); }
	goto st11;
tr366:
#line 170 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 6, 9}); }
	goto st11;
tr375:
#line 186 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 6, 9, 14}); }
	goto st11;
tr384:
#line 206 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 8, 10, 14, 17, 21}); }
	goto st11;
tr393:
#line 168 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 8, 10}); }
	goto st11;
tr402:
#line 183 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 8, 10, 14}); }
	goto st11;
tr411:
#line 195 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 8, 11, 14, 17}); }
	goto st11;
tr420:
#line 205 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 8, 11, 14, 17, 21}); }
	goto st11;
tr429:
#line 169 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 8, 11}); }
	goto st11;
tr438:
#line 182 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 8, 11, 14}); }
	goto st11;
tr447:
#line 188 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 5, 7, 10, 14}); }
	goto st11;
tr456:
#line 180 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 7, 10, 13}); }
	goto st11;
tr465:
#line 173 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 2, 7, 10}); }
	goto st11;
tr474:
#line 174 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 5, 7, 10}); }
	goto st11;
tr483:
#line 201 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 7, 10, 14, 17, 21}); }
	goto st11;
st11:
	if ( ++p == pe )
		goto _test_eof11;
case 11:
#line 4054 "lex/parser_music.cpp"
	if ( (*p) == 97 )
		goto st12;
	goto st0;
st12:
	if ( ++p == pe )
		goto _test_eof12;
case 12:
	if ( (*p) == 108 )
		goto st13;
	goto st0;
st13:
	if ( ++p == pe )
		goto _test_eof13;
case 13:
	if ( (*p) == 102 )
		goto st14;
	goto st0;
st14:
	if ( ++p == pe )
		goto _test_eof14;
case 14:
	if ( (*p) == 100 )
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
	if ( (*p) == 109 )
		goto st17;
	goto st0;
st17:
	if ( ++p == pe )
		goto _test_eof17;
case 17:
	switch( (*p) ) {
		case 45: goto st18;
		case 49: goto st30;
		case 55: goto st74;
		case 57: goto st75;
	}
	goto st0;
st18:
	if ( ++p == pe )
		goto _test_eof18;
case 18:
	if ( (*p) == 57 )
		goto st54;
	goto st0;
st54:
	if ( ++p == pe )
		goto _test_eof54;
case 54:
	switch( (*p) ) {
		case 43: goto tr160;
		case 49: goto tr161;
		case 55: goto tr162;
		case 57: goto tr163;
		case 97: goto tr164;
		case 100: goto tr165;
		case 104: goto tr166;
		case 109: goto tr167;
		case 115: goto tr168;
	}
	goto st0;
tr74:
#line 86 "lex/ragel_music.rl"
	{ragel_cat = CAT_UNIT; ragel_type = TYPE_SPN;}
	goto st19;
tr84:
#line 158 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 10}); }
	goto st19;
tr93:
#line 191 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 7, 10, 14, 17}); }
	goto st19;
tr103:
#line 164 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 7, 10}); }
	goto st19;
tr112:
#line 172 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 6, 10}); }
	goto st19;
tr121:
#line 177 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 7, 10, 14}); }
	goto st19;
tr139:
#line 196 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 8, 10, 14, 17}); }
	goto st19;
tr149:
#line 157 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 6}); name = "Diminished triad"; }
	goto st19;
tr158:
#line 187 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 6, 9, 13}); }
	goto st19;
tr167:
#line 185 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 6, 10, 13}); }
	goto st19;
tr176:
#line 154 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 7}); name = "Major triad"; }
	goto st19;
tr185:
#line 192 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 7, 11, 14, 17}); }
	goto st19;
tr196:
#line 159 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 5, 7}); }
	goto st19;
tr205:
#line 160 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 2, 7}); }
	goto st19;
tr214:
#line 202 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 7, 11, 14, 17, 21}); }
	goto st19;
tr223:
#line 165 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 7, 11}); }
	goto st19;
tr232:
#line 178 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 7, 11, 14}); }
	goto st19;
tr250:
#line 193 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 7, 10, 14, 17}); }
	goto st19;
tr259:
#line 204 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 7, 10, 14, 17, 21}); }
	goto st19;
tr268:
#line 166 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 7, 10}); }
	goto st19;
tr277:
#line 179 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 7, 10, 14}); }
	goto st19;
tr286:
#line 194 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 7, 11, 14, 17}); }
	goto st19;
tr295:
#line 203 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 7, 11, 14, 17, 21}); }
	goto st19;
tr304:
#line 167 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 7, 11}); }
	goto st19;
tr313:
#line 181 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 7, 11, 14}); }
	goto st19;
tr322:
#line 197 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 6, 10, 14, 17}); }
	goto st19;
tr331:
#line 207 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 6, 10, 14, 17, 21}); }
	goto st19;
tr340:
#line 171 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 6, 10}); }
	goto st19;
tr349:
#line 184 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 6, 10, 14}); }
	goto st19;
tr358:
#line 198 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 6, 9, 14, 17}); }
	goto st19;
tr367:
#line 170 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 6, 9}); }
	goto st19;
tr376:
#line 186 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 6, 9, 14}); }
	goto st19;
tr385:
#line 206 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 8, 10, 14, 17, 21}); }
	goto st19;
tr394:
#line 168 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 8, 10}); }
	goto st19;
tr403:
#line 183 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 8, 10, 14}); }
	goto st19;
tr412:
#line 195 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 8, 11, 14, 17}); }
	goto st19;
tr421:
#line 205 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 8, 11, 14, 17, 21}); }
	goto st19;
tr430:
#line 169 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 8, 11}); }
	goto st19;
tr439:
#line 182 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 8, 11, 14}); }
	goto st19;
tr448:
#line 188 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 5, 7, 10, 14}); }
	goto st19;
tr457:
#line 180 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 7, 10, 13}); }
	goto st19;
tr466:
#line 173 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 2, 7, 10}); }
	goto st19;
tr475:
#line 174 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 5, 7, 10}); }
	goto st19;
tr484:
#line 201 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 7, 10, 14, 17, 21}); }
	goto st19;
st19:
	if ( ++p == pe )
		goto _test_eof19;
case 19:
#line 4307 "lex/parser_music.cpp"
	switch( (*p) ) {
		case 97: goto st20;
		case 105: goto st24;
	}
	goto st0;
st20:
	if ( ++p == pe )
		goto _test_eof20;
case 20:
	if ( (*p) == 106 )
		goto st55;
	goto st0;
st55:
	if ( ++p == pe )
		goto _test_eof55;
case 55:
	switch( (*p) ) {
		case 43: goto tr169;
		case 49: goto st21;
		case 55: goto st61;
		case 57: goto st62;
		case 97: goto tr173;
		case 100: goto tr174;
		case 104: goto tr175;
		case 109: goto tr176;
		case 115: goto tr177;
	}
	goto st0;
st21:
	if ( ++p == pe )
		goto _test_eof21;
case 21:
	switch( (*p) ) {
		case 49: goto st56;
		case 51: goto st60;
	}
	goto st0;
st56:
	if ( ++p == pe )
		goto _test_eof56;
case 56:
	switch( (*p) ) {
		case 43: goto tr178;
		case 49: goto tr179;
		case 55: goto tr180;
		case 57: goto tr181;
		case 97: goto tr182;
		case 100: goto tr183;
		case 104: goto tr184;
		case 109: goto tr185;
		case 115: goto tr186;
	}
	goto st0;
tr75:
#line 86 "lex/ragel_music.rl"
	{ragel_cat = CAT_UNIT; ragel_type = TYPE_SPN;}
	goto st22;
tr85:
#line 158 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 10}); }
	goto st22;
tr94:
#line 191 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 7, 10, 14, 17}); }
	goto st22;
tr113:
#line 172 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 6, 10}); }
	goto st22;
tr131:
#line 156 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 8}); name = "Augmented triad";}
	goto st22;
tr140:
#line 196 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 8, 10, 14, 17}); }
	goto st22;
tr150:
#line 157 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 6}); name = "Diminished triad"; }
	goto st22;
tr159:
#line 187 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 6, 9, 13}); }
	goto st22;
tr168:
#line 185 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 6, 10, 13}); }
	goto st22;
tr177:
#line 154 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 7}); name = "Major triad"; }
	goto st22;
tr186:
#line 192 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 7, 11, 14, 17}); }
	goto st22;
tr197:
#line 159 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 5, 7}); }
	goto st22;
tr206:
#line 160 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 2, 7}); }
	goto st22;
tr215:
#line 202 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 7, 11, 14, 17, 21}); }
	goto st22;
tr224:
#line 165 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 7, 11}); }
	goto st22;
tr233:
#line 178 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 7, 11, 14}); }
	goto st22;
tr242:
#line 155 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 7}); name = "Minor triad"; }
	goto st22;
tr251:
#line 193 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 7, 10, 14, 17}); }
	goto st22;
tr260:
#line 204 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 7, 10, 14, 17, 21}); }
	goto st22;
tr269:
#line 166 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 7, 10}); }
	goto st22;
tr278:
#line 179 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 7, 10, 14}); }
	goto st22;
tr287:
#line 194 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 7, 11, 14, 17}); }
	goto st22;
tr296:
#line 203 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 7, 11, 14, 17, 21}); }
	goto st22;
tr305:
#line 167 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 7, 11}); }
	goto st22;
tr314:
#line 181 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 7, 11, 14}); }
	goto st22;
tr323:
#line 197 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 6, 10, 14, 17}); }
	goto st22;
tr332:
#line 207 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 6, 10, 14, 17, 21}); }
	goto st22;
tr341:
#line 171 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 6, 10}); }
	goto st22;
tr350:
#line 184 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 6, 10, 14}); }
	goto st22;
tr359:
#line 198 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 6, 9, 14, 17}); }
	goto st22;
tr368:
#line 170 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 6, 9}); }
	goto st22;
tr377:
#line 186 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 6, 9, 14}); }
	goto st22;
tr386:
#line 206 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 8, 10, 14, 17, 21}); }
	goto st22;
tr395:
#line 168 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 8, 10}); }
	goto st22;
tr404:
#line 183 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 8, 10, 14}); }
	goto st22;
tr413:
#line 195 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 8, 11, 14, 17}); }
	goto st22;
tr422:
#line 205 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 8, 11, 14, 17, 21}); }
	goto st22;
tr431:
#line 169 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 8, 11}); }
	goto st22;
tr440:
#line 182 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 8, 11, 14}); }
	goto st22;
tr449:
#line 188 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 5, 7, 10, 14}); }
	goto st22;
tr458:
#line 180 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 7, 10, 13}); }
	goto st22;
tr467:
#line 173 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 2, 7, 10}); }
	goto st22;
tr476:
#line 174 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 5, 7, 10}); }
	goto st22;
tr485:
#line 201 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 7, 10, 14, 17, 21}); }
	goto st22;
st22:
	if ( ++p == pe )
		goto _test_eof22;
case 22:
#line 4541 "lex/parser_music.cpp"
	if ( (*p) == 117 )
		goto st23;
	goto st0;
st23:
	if ( ++p == pe )
		goto _test_eof23;
case 23:
	if ( (*p) == 115 )
		goto st57;
	goto st0;
st57:
	if ( ++p == pe )
		goto _test_eof57;
case 57:
	switch( (*p) ) {
		case 43: goto tr187;
		case 49: goto tr188;
		case 50: goto st58;
		case 52: goto st59;
		case 55: goto tr191;
		case 57: goto tr192;
		case 97: goto tr193;
		case 100: goto tr194;
		case 104: goto tr195;
		case 109: goto tr196;
		case 115: goto tr197;
	}
	goto st0;
st58:
	if ( ++p == pe )
		goto _test_eof58;
case 58:
	switch( (*p) ) {
		case 43: goto tr198;
		case 49: goto tr199;
		case 55: goto tr200;
		case 57: goto tr201;
		case 97: goto tr202;
		case 100: goto tr203;
		case 104: goto tr204;
		case 109: goto tr205;
		case 115: goto tr206;
	}
	goto st0;
st59:
	if ( ++p == pe )
		goto _test_eof59;
case 59:
	switch( (*p) ) {
		case 43: goto tr187;
		case 49: goto tr188;
		case 55: goto tr191;
		case 57: goto tr192;
		case 97: goto tr193;
		case 100: goto tr194;
		case 104: goto tr195;
		case 109: goto tr196;
		case 115: goto tr197;
	}
	goto st0;
st60:
	if ( ++p == pe )
		goto _test_eof60;
case 60:
	switch( (*p) ) {
		case 43: goto tr207;
		case 49: goto tr208;
		case 55: goto tr209;
		case 57: goto tr210;
		case 97: goto tr211;
		case 100: goto tr212;
		case 104: goto tr213;
		case 109: goto tr214;
		case 115: goto tr215;
	}
	goto st0;
st61:
	if ( ++p == pe )
		goto _test_eof61;
case 61:
	switch( (*p) ) {
		case 43: goto tr216;
		case 49: goto tr217;
		case 55: goto tr218;
		case 57: goto tr219;
		case 97: goto tr220;
		case 100: goto tr221;
		case 104: goto tr222;
		case 109: goto tr223;
		case 115: goto tr224;
	}
	goto st0;
st62:
	if ( ++p == pe )
		goto _test_eof62;
case 62:
	switch( (*p) ) {
		case 43: goto tr225;
		case 49: goto tr226;
		case 55: goto tr227;
		case 57: goto tr228;
		case 97: goto tr229;
		case 100: goto tr230;
		case 104: goto tr231;
		case 109: goto tr232;
		case 115: goto tr233;
	}
	goto st0;
st24:
	if ( ++p == pe )
		goto _test_eof24;
case 24:
	if ( (*p) == 110 )
		goto st63;
	goto st0;
st63:
	if ( ++p == pe )
		goto _test_eof63;
case 63:
	switch( (*p) ) {
		case 43: goto tr234;
		case 49: goto st25;
		case 55: goto st66;
		case 57: goto st67;
		case 97: goto tr238;
		case 100: goto tr239;
		case 104: goto tr240;
		case 109: goto st26;
		case 115: goto tr242;
	}
	goto st0;
st25:
	if ( ++p == pe )
		goto _test_eof25;
case 25:
	switch( (*p) ) {
		case 49: goto st64;
		case 51: goto st65;
	}
	goto st0;
st64:
	if ( ++p == pe )
		goto _test_eof64;
case 64:
	switch( (*p) ) {
		case 43: goto tr243;
		case 49: goto tr244;
		case 55: goto tr245;
		case 57: goto tr246;
		case 97: goto tr247;
		case 100: goto tr248;
		case 104: goto tr249;
		case 109: goto tr250;
		case 115: goto tr251;
	}
	goto st0;
st65:
	if ( ++p == pe )
		goto _test_eof65;
case 65:
	switch( (*p) ) {
		case 43: goto tr252;
		case 49: goto tr253;
		case 55: goto tr254;
		case 57: goto tr255;
		case 97: goto tr256;
		case 100: goto tr257;
		case 104: goto tr258;
		case 109: goto tr259;
		case 115: goto tr260;
	}
	goto st0;
st66:
	if ( ++p == pe )
		goto _test_eof66;
case 66:
	switch( (*p) ) {
		case 43: goto tr261;
		case 49: goto tr262;
		case 55: goto tr263;
		case 57: goto tr264;
		case 97: goto tr265;
		case 100: goto tr266;
		case 104: goto tr267;
		case 109: goto tr268;
		case 115: goto tr269;
	}
	goto st0;
st67:
	if ( ++p == pe )
		goto _test_eof67;
case 67:
	switch( (*p) ) {
		case 43: goto tr270;
		case 49: goto tr271;
		case 55: goto tr272;
		case 57: goto tr273;
		case 97: goto tr274;
		case 100: goto tr275;
		case 104: goto tr276;
		case 109: goto tr277;
		case 115: goto tr278;
	}
	goto st0;
st26:
	if ( ++p == pe )
		goto _test_eof26;
case 26:
	if ( (*p) == 97 )
		goto st27;
	goto st0;
st27:
	if ( ++p == pe )
		goto _test_eof27;
case 27:
	if ( (*p) == 106 )
		goto st28;
	goto st0;
st28:
	if ( ++p == pe )
		goto _test_eof28;
case 28:
	switch( (*p) ) {
		case 49: goto st29;
		case 55: goto st70;
		case 57: goto st71;
	}
	goto st0;
st29:
	if ( ++p == pe )
		goto _test_eof29;
case 29:
	switch( (*p) ) {
		case 49: goto st68;
		case 51: goto st69;
	}
	goto st0;
st68:
	if ( ++p == pe )
		goto _test_eof68;
case 68:
	switch( (*p) ) {
		case 43: goto tr279;
		case 49: goto tr280;
		case 55: goto tr281;
		case 57: goto tr282;
		case 97: goto tr283;
		case 100: goto tr284;
		case 104: goto tr285;
		case 109: goto tr286;
		case 115: goto tr287;
	}
	goto st0;
st69:
	if ( ++p == pe )
		goto _test_eof69;
case 69:
	switch( (*p) ) {
		case 43: goto tr288;
		case 49: goto tr289;
		case 55: goto tr290;
		case 57: goto tr291;
		case 97: goto tr292;
		case 100: goto tr293;
		case 104: goto tr294;
		case 109: goto tr295;
		case 115: goto tr296;
	}
	goto st0;
st70:
	if ( ++p == pe )
		goto _test_eof70;
case 70:
	switch( (*p) ) {
		case 43: goto tr297;
		case 49: goto tr298;
		case 55: goto tr299;
		case 57: goto tr300;
		case 97: goto tr301;
		case 100: goto tr302;
		case 104: goto tr303;
		case 109: goto tr304;
		case 115: goto tr305;
	}
	goto st0;
st71:
	if ( ++p == pe )
		goto _test_eof71;
case 71:
	switch( (*p) ) {
		case 43: goto tr306;
		case 49: goto tr307;
		case 55: goto tr308;
		case 57: goto tr309;
		case 97: goto tr310;
		case 100: goto tr311;
		case 104: goto tr312;
		case 109: goto tr313;
		case 115: goto tr314;
	}
	goto st0;
st30:
	if ( ++p == pe )
		goto _test_eof30;
case 30:
	switch( (*p) ) {
		case 49: goto st72;
		case 51: goto st73;
	}
	goto st0;
st72:
	if ( ++p == pe )
		goto _test_eof72;
case 72:
	switch( (*p) ) {
		case 43: goto tr315;
		case 49: goto tr316;
		case 55: goto tr317;
		case 57: goto tr318;
		case 97: goto tr319;
		case 100: goto tr320;
		case 104: goto tr321;
		case 109: goto tr322;
		case 115: goto tr323;
	}
	goto st0;
st73:
	if ( ++p == pe )
		goto _test_eof73;
case 73:
	switch( (*p) ) {
		case 43: goto tr324;
		case 49: goto tr325;
		case 55: goto tr326;
		case 57: goto tr327;
		case 97: goto tr328;
		case 100: goto tr329;
		case 104: goto tr330;
		case 109: goto tr331;
		case 115: goto tr332;
	}
	goto st0;
st74:
	if ( ++p == pe )
		goto _test_eof74;
case 74:
	switch( (*p) ) {
		case 43: goto tr333;
		case 49: goto tr334;
		case 55: goto tr335;
		case 57: goto tr336;
		case 97: goto tr337;
		case 100: goto tr338;
		case 104: goto tr339;
		case 109: goto tr340;
		case 115: goto tr341;
	}
	goto st0;
st75:
	if ( ++p == pe )
		goto _test_eof75;
case 75:
	switch( (*p) ) {
		case 43: goto tr342;
		case 49: goto tr343;
		case 55: goto tr344;
		case 57: goto tr345;
		case 97: goto tr346;
		case 100: goto tr347;
		case 104: goto tr348;
		case 109: goto tr349;
		case 115: goto tr350;
	}
	goto st0;
st31:
	if ( ++p == pe )
		goto _test_eof31;
case 31:
	if ( (*p) == 49 )
		goto st76;
	goto st0;
st76:
	if ( ++p == pe )
		goto _test_eof76;
case 76:
	switch( (*p) ) {
		case 43: goto tr351;
		case 49: goto tr352;
		case 55: goto tr353;
		case 57: goto tr354;
		case 97: goto tr355;
		case 100: goto tr356;
		case 104: goto tr357;
		case 109: goto tr358;
		case 115: goto tr359;
	}
	goto st0;
st77:
	if ( ++p == pe )
		goto _test_eof77;
case 77:
	switch( (*p) ) {
		case 43: goto tr360;
		case 49: goto tr361;
		case 55: goto tr362;
		case 57: goto tr363;
		case 97: goto tr364;
		case 100: goto tr365;
		case 104: goto tr366;
		case 109: goto tr367;
		case 115: goto tr368;
	}
	goto st0;
st78:
	if ( ++p == pe )
		goto _test_eof78;
case 78:
	switch( (*p) ) {
		case 43: goto tr369;
		case 49: goto tr370;
		case 55: goto tr371;
		case 57: goto tr372;
		case 97: goto tr373;
		case 100: goto tr374;
		case 104: goto tr375;
		case 109: goto tr376;
		case 115: goto tr377;
	}
	goto st0;
st79:
	if ( ++p == pe )
		goto _test_eof79;
case 79:
	switch( (*p) ) {
		case 43: goto tr378;
		case 49: goto tr379;
		case 55: goto tr380;
		case 57: goto tr381;
		case 97: goto tr382;
		case 100: goto tr383;
		case 104: goto tr384;
		case 109: goto tr385;
		case 115: goto tr386;
	}
	goto st0;
st80:
	if ( ++p == pe )
		goto _test_eof80;
case 80:
	switch( (*p) ) {
		case 43: goto tr387;
		case 49: goto tr388;
		case 55: goto tr389;
		case 57: goto tr390;
		case 97: goto tr391;
		case 100: goto tr392;
		case 104: goto tr393;
		case 109: goto tr394;
		case 115: goto tr395;
	}
	goto st0;
st81:
	if ( ++p == pe )
		goto _test_eof81;
case 81:
	switch( (*p) ) {
		case 43: goto tr396;
		case 49: goto tr397;
		case 55: goto tr398;
		case 57: goto tr399;
		case 97: goto tr400;
		case 100: goto tr401;
		case 104: goto tr402;
		case 109: goto tr403;
		case 115: goto tr404;
	}
	goto st0;
st32:
	if ( ++p == pe )
		goto _test_eof32;
case 32:
	if ( (*p) == 97 )
		goto st33;
	goto st0;
st33:
	if ( ++p == pe )
		goto _test_eof33;
case 33:
	if ( (*p) == 106 )
		goto st34;
	goto st0;
st34:
	if ( ++p == pe )
		goto _test_eof34;
case 34:
	switch( (*p) ) {
		case 49: goto st35;
		case 55: goto st84;
		case 57: goto st85;
	}
	goto st0;
st35:
	if ( ++p == pe )
		goto _test_eof35;
case 35:
	switch( (*p) ) {
		case 49: goto st82;
		case 51: goto st83;
	}
	goto st0;
st82:
	if ( ++p == pe )
		goto _test_eof82;
case 82:
	switch( (*p) ) {
		case 43: goto tr405;
		case 49: goto tr406;
		case 55: goto tr407;
		case 57: goto tr408;
		case 97: goto tr409;
		case 100: goto tr410;
		case 104: goto tr411;
		case 109: goto tr412;
		case 115: goto tr413;
	}
	goto st0;
st83:
	if ( ++p == pe )
		goto _test_eof83;
case 83:
	switch( (*p) ) {
		case 43: goto tr414;
		case 49: goto tr415;
		case 55: goto tr416;
		case 57: goto tr417;
		case 97: goto tr418;
		case 100: goto tr419;
		case 104: goto tr420;
		case 109: goto tr421;
		case 115: goto tr422;
	}
	goto st0;
st84:
	if ( ++p == pe )
		goto _test_eof84;
case 84:
	switch( (*p) ) {
		case 43: goto tr423;
		case 49: goto tr424;
		case 55: goto tr425;
		case 57: goto tr426;
		case 97: goto tr427;
		case 100: goto tr428;
		case 104: goto tr429;
		case 109: goto tr430;
		case 115: goto tr431;
	}
	goto st0;
st85:
	if ( ++p == pe )
		goto _test_eof85;
case 85:
	switch( (*p) ) {
		case 43: goto tr432;
		case 49: goto tr433;
		case 55: goto tr434;
		case 57: goto tr435;
		case 97: goto tr436;
		case 100: goto tr437;
		case 104: goto tr438;
		case 109: goto tr439;
		case 115: goto tr440;
	}
	goto st0;
st36:
	if ( ++p == pe )
		goto _test_eof36;
case 36:
	if ( (*p) == 117 )
		goto st37;
	goto st0;
st37:
	if ( ++p == pe )
		goto _test_eof37;
case 37:
	if ( (*p) == 115 )
		goto st38;
	goto st0;
st38:
	if ( ++p == pe )
		goto _test_eof38;
case 38:
	if ( (*p) == 52 )
		goto st86;
	goto st0;
st86:
	if ( ++p == pe )
		goto _test_eof86;
case 86:
	switch( (*p) ) {
		case 43: goto tr441;
		case 49: goto tr442;
		case 55: goto tr443;
		case 57: goto tr444;
		case 97: goto tr445;
		case 100: goto tr446;
		case 104: goto tr447;
		case 109: goto tr448;
		case 115: goto tr449;
	}
	goto st0;
st87:
	if ( ++p == pe )
		goto _test_eof87;
case 87:
	switch( (*p) ) {
		case 43: goto tr450;
		case 49: goto tr451;
		case 55: goto tr452;
		case 57: goto tr453;
		case 97: goto tr454;
		case 100: goto tr455;
		case 104: goto tr456;
		case 109: goto tr457;
		case 115: goto tr458;
	}
	goto st0;
st39:
	if ( ++p == pe )
		goto _test_eof39;
case 39:
	if ( (*p) == 117 )
		goto st40;
	goto st0;
st40:
	if ( ++p == pe )
		goto _test_eof40;
case 40:
	if ( (*p) == 115 )
		goto st41;
	goto st0;
st41:
	if ( ++p == pe )
		goto _test_eof41;
case 41:
	switch( (*p) ) {
		case 50: goto st88;
		case 52: goto st89;
	}
	goto st0;
st88:
	if ( ++p == pe )
		goto _test_eof88;
case 88:
	switch( (*p) ) {
		case 43: goto tr459;
		case 49: goto tr460;
		case 55: goto tr461;
		case 57: goto tr462;
		case 97: goto tr463;
		case 100: goto tr464;
		case 104: goto tr465;
		case 109: goto tr466;
		case 115: goto tr467;
	}
	goto st0;
st89:
	if ( ++p == pe )
		goto _test_eof89;
case 89:
	switch( (*p) ) {
		case 43: goto tr468;
		case 49: goto tr469;
		case 55: goto tr470;
		case 57: goto tr471;
		case 97: goto tr472;
		case 100: goto tr473;
		case 104: goto tr474;
		case 109: goto tr475;
		case 115: goto tr476;
	}
	goto st0;
st90:
	if ( ++p == pe )
		goto _test_eof90;
case 90:
	switch( (*p) ) {
		case 43: goto tr477;
		case 49: goto tr478;
		case 55: goto tr479;
		case 57: goto tr480;
		case 97: goto tr481;
		case 100: goto tr482;
		case 104: goto tr483;
		case 109: goto tr484;
		case 115: goto tr485;
	}
	goto st0;
tr71:
#line 39 "lex/ragel_music.rl"
	{spn.alt = Alteration::FLAT; }
	goto st91;
st91:
	if ( ++p == pe )
		goto _test_eof91;
case 91:
#line 5248 "lex/parser_music.cpp"
	switch( (*p) ) {
		case 43: goto tr66;
		case 49: goto tr67;
		case 55: goto tr68;
		case 57: goto tr69;
		case 97: goto tr70;
		case 98: goto tr486;
		case 100: goto tr72;
		case 104: goto tr73;
		case 109: goto tr74;
		case 115: goto tr75;
	}
	goto st0;
	}
	_test_eof42: cs = 42; goto _test_eof; 
	_test_eof43: cs = 43; goto _test_eof; 
	_test_eof44: cs = 44; goto _test_eof; 
	_test_eof2: cs = 2; goto _test_eof; 
	_test_eof45: cs = 45; goto _test_eof; 
	_test_eof3: cs = 3; goto _test_eof; 
	_test_eof46: cs = 46; goto _test_eof; 
	_test_eof47: cs = 47; goto _test_eof; 
	_test_eof4: cs = 4; goto _test_eof; 
	_test_eof48: cs = 48; goto _test_eof; 
	_test_eof49: cs = 49; goto _test_eof; 
	_test_eof5: cs = 5; goto _test_eof; 
	_test_eof6: cs = 6; goto _test_eof; 
	_test_eof50: cs = 50; goto _test_eof; 
	_test_eof7: cs = 7; goto _test_eof; 
	_test_eof51: cs = 51; goto _test_eof; 
	_test_eof8: cs = 8; goto _test_eof; 
	_test_eof9: cs = 9; goto _test_eof; 
	_test_eof52: cs = 52; goto _test_eof; 
	_test_eof10: cs = 10; goto _test_eof; 
	_test_eof53: cs = 53; goto _test_eof; 
	_test_eof11: cs = 11; goto _test_eof; 
	_test_eof12: cs = 12; goto _test_eof; 
	_test_eof13: cs = 13; goto _test_eof; 
	_test_eof14: cs = 14; goto _test_eof; 
	_test_eof15: cs = 15; goto _test_eof; 
	_test_eof16: cs = 16; goto _test_eof; 
	_test_eof17: cs = 17; goto _test_eof; 
	_test_eof18: cs = 18; goto _test_eof; 
	_test_eof54: cs = 54; goto _test_eof; 
	_test_eof19: cs = 19; goto _test_eof; 
	_test_eof20: cs = 20; goto _test_eof; 
	_test_eof55: cs = 55; goto _test_eof; 
	_test_eof21: cs = 21; goto _test_eof; 
	_test_eof56: cs = 56; goto _test_eof; 
	_test_eof22: cs = 22; goto _test_eof; 
	_test_eof23: cs = 23; goto _test_eof; 
	_test_eof57: cs = 57; goto _test_eof; 
	_test_eof58: cs = 58; goto _test_eof; 
	_test_eof59: cs = 59; goto _test_eof; 
	_test_eof60: cs = 60; goto _test_eof; 
	_test_eof61: cs = 61; goto _test_eof; 
	_test_eof62: cs = 62; goto _test_eof; 
	_test_eof24: cs = 24; goto _test_eof; 
	_test_eof63: cs = 63; goto _test_eof; 
	_test_eof25: cs = 25; goto _test_eof; 
	_test_eof64: cs = 64; goto _test_eof; 
	_test_eof65: cs = 65; goto _test_eof; 
	_test_eof66: cs = 66; goto _test_eof; 
	_test_eof67: cs = 67; goto _test_eof; 
	_test_eof26: cs = 26; goto _test_eof; 
	_test_eof27: cs = 27; goto _test_eof; 
	_test_eof28: cs = 28; goto _test_eof; 
	_test_eof29: cs = 29; goto _test_eof; 
	_test_eof68: cs = 68; goto _test_eof; 
	_test_eof69: cs = 69; goto _test_eof; 
	_test_eof70: cs = 70; goto _test_eof; 
	_test_eof71: cs = 71; goto _test_eof; 
	_test_eof30: cs = 30; goto _test_eof; 
	_test_eof72: cs = 72; goto _test_eof; 
	_test_eof73: cs = 73; goto _test_eof; 
	_test_eof74: cs = 74; goto _test_eof; 
	_test_eof75: cs = 75; goto _test_eof; 
	_test_eof31: cs = 31; goto _test_eof; 
	_test_eof76: cs = 76; goto _test_eof; 
	_test_eof77: cs = 77; goto _test_eof; 
	_test_eof78: cs = 78; goto _test_eof; 
	_test_eof79: cs = 79; goto _test_eof; 
	_test_eof80: cs = 80; goto _test_eof; 
	_test_eof81: cs = 81; goto _test_eof; 
	_test_eof32: cs = 32; goto _test_eof; 
	_test_eof33: cs = 33; goto _test_eof; 
	_test_eof34: cs = 34; goto _test_eof; 
	_test_eof35: cs = 35; goto _test_eof; 
	_test_eof82: cs = 82; goto _test_eof; 
	_test_eof83: cs = 83; goto _test_eof; 
	_test_eof84: cs = 84; goto _test_eof; 
	_test_eof85: cs = 85; goto _test_eof; 
	_test_eof36: cs = 36; goto _test_eof; 
	_test_eof37: cs = 37; goto _test_eof; 
	_test_eof38: cs = 38; goto _test_eof; 
	_test_eof86: cs = 86; goto _test_eof; 
	_test_eof87: cs = 87; goto _test_eof; 
	_test_eof39: cs = 39; goto _test_eof; 
	_test_eof40: cs = 40; goto _test_eof; 
	_test_eof41: cs = 41; goto _test_eof; 
	_test_eof88: cs = 88; goto _test_eof; 
	_test_eof89: cs = 89; goto _test_eof; 
	_test_eof90: cs = 90; goto _test_eof; 
	_test_eof91: cs = 91; goto _test_eof; 

	_test_eof: {}
	if ( p == eof )
	{
	switch ( cs ) {
	case 42: 
	case 43: 
	case 44: 
	case 91: 
#line 86 "lex/ragel_music.rl"
	{ragel_cat = CAT_UNIT; ragel_type = TYPE_SPN;}
	break;
	case 55: 
#line 154 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 7}); name = "Major triad"; }
	break;
	case 63: 
#line 155 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 7}); name = "Minor triad"; }
	break;
	case 50: 
#line 156 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 8}); name = "Augmented triad";}
	break;
	case 52: 
#line 157 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 6}); name = "Diminished triad"; }
	break;
	case 45: 
#line 158 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 10}); }
	break;
	case 57: 
	case 59: 
#line 159 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 5, 7}); }
	break;
	case 58: 
#line 160 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 2, 7}); }
	break;
	case 47: 
#line 164 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 7, 10}); }
	break;
	case 61: 
#line 165 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 7, 11}); }
	break;
	case 66: 
#line 166 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 7, 10}); }
	break;
	case 70: 
#line 167 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 7, 11}); }
	break;
	case 80: 
#line 168 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 8, 10}); }
	break;
	case 84: 
#line 169 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 8, 11}); }
	break;
	case 77: 
#line 170 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 6, 9}); }
	break;
	case 74: 
#line 171 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 6, 10}); }
	break;
	case 48: 
#line 172 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 6, 10}); }
	break;
	case 88: 
#line 173 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 2, 7, 10}); }
	break;
	case 89: 
#line 174 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 5, 7, 10}); }
	break;
	case 49: 
#line 177 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 7, 10, 14}); }
	break;
	case 62: 
#line 178 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 7, 11, 14}); }
	break;
	case 67: 
#line 179 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 7, 10, 14}); }
	break;
	case 87: 
#line 180 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 7, 10, 13}); }
	break;
	case 71: 
#line 181 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 7, 11, 14}); }
	break;
	case 85: 
#line 182 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 8, 11, 14}); }
	break;
	case 81: 
#line 183 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 8, 10, 14}); }
	break;
	case 75: 
#line 184 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 6, 10, 14}); }
	break;
	case 54: 
#line 185 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 6, 10, 13}); }
	break;
	case 78: 
#line 186 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 6, 9, 14}); }
	break;
	case 53: 
#line 187 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 6, 9, 13}); }
	break;
	case 86: 
#line 188 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 5, 7, 10, 14}); }
	break;
	case 46: 
#line 191 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 7, 10, 14, 17}); }
	break;
	case 56: 
#line 192 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 7, 11, 14, 17}); }
	break;
	case 64: 
#line 193 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 7, 10, 14, 17}); }
	break;
	case 68: 
#line 194 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 7, 11, 14, 17}); }
	break;
	case 82: 
#line 195 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 8, 11, 14, 17}); }
	break;
	case 51: 
#line 196 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 8, 10, 14, 17}); }
	break;
	case 72: 
#line 197 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 6, 10, 14, 17}); }
	break;
	case 76: 
#line 198 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 6, 9, 14, 17}); }
	break;
	case 90: 
#line 201 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 7, 10, 14, 17, 21}); }
	break;
	case 60: 
#line 202 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 7, 11, 14, 17, 21}); }
	break;
	case 69: 
#line 203 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 7, 11, 14, 17, 21}); }
	break;
	case 65: 
#line 204 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 7, 10, 14, 17, 21}); }
	break;
	case 83: 
#line 205 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 8, 11, 14, 17, 21}); }
	break;
	case 79: 
#line 206 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 4, 8, 10, 14, 17, 21}); }
	break;
	case 73: 
#line 207 "lex/ragel_music.rl"
	{ rg_chord_type.assign({0, 3, 6, 10, 14, 17, 21}); }
	break;
#line 5546 "lex/parser_music.cpp"
	}
	}

	_out: {}
	}

#line 423 "lex/parser_music.rl"

    const bool ok = cs >= 42;

    if (ok) {
        auto x_spn = fromRagel(spn);
        auto x_type = fromRagel(rg_chord_type);
        res = ChordClass(x_spn, x_type);
        return true;
    }

    return ok;
}


}
}


