
#line 1 "lex/parser_timeline.rl"
# include "ceammc_music_theory_tempo.h"
# include "fmt/core.h"
# include "parser_numeric.h"
# include "parser_timeline.h"
# include "parser_units.h"
# include "ragel_common.h"

# include <cstring>
# include <limits>
# include <string>

using namespace ceammc::music;
using namespace ceammc::parser;
using namespace ceammc;

namespace {

struct RagelTempo {
    int ival { 0 };
    int fnum { 0 };
    int fden { 1 };
    int dur_num { 1 };
    int dur_den { 4 };
};

Tempo fromRagel(const RagelTempo& t)
{
    float bpm = t.ival + t.fnum / float(t.fden);
    Tempo res { bpm, t.dur_den };
    res.setDuration(Duration(t.dur_num, t.dur_den));
    return res;
}

struct RagelEventTime {
    double time;
    std::vector<double> times;
    int sign {0};
    t_symbol* rel_event {&s_};
    int bar  {-1};
    int beat {0};
    bool relative {false};

    void reset() { *this = RagelEventTime{}; }
};

struct RagelBar {
    int count = 0;
    int num = 0;
    int div = 0;
    bool inf = false;
};

struct FSM {
    double time_unit {0};
    std::string symbol;
    t_symbol* event_id {&s_};
    TimeLineEventSend act_send;
    TimeLineEventOutput act_out;
    TimeLineEventPreset act_preset;
    double event_time {0};
    ceammc::AtomList args;
    int bpm_bar {0};

    void onTimeSmpteDone(const ceammc::parser::fsm::SmpteData& smpte) {
        time_unit = smpte.sec * 1000 + smpte.min * 60000 + smpte.hour * 3600000;
    }

    void onTimeInfDone() {
        time_unit = std::numeric_limits<t_float>::max();
    }

    void onEventIdDone() {
        event_id = ::gensym(symbol.c_str());
        symbol.clear();
    }

    void onSendTarget() {
        act_send.target = ::gensym(symbol.c_str());
        symbol.clear();
    }

    void onArgsDone() {
        args = ceammc::AtomList::parseString(symbol.c_str());
    }

    t_symbol* gensym() { return ::gensym(symbol.c_str()); }

    t_symbol* genAnonEventName() {
        static int id = 0;
        char buf[64];
        sprintf(buf, ".event%d", id++);
        return ::gensym(buf);
    }
};
}


#line 101 "lex/parser_timeline.cpp"
static const int timeline_parser_start = 1;
static const int timeline_parser_first_final = 268;
static const int timeline_parser_error = 0;

static const int timeline_parser_en_main = 1;


#line 352 "lex/parser_timeline.rl"


namespace ceammc {
namespace parser {

bool parse_timelime(const char* str, TimeLine& tl)
{
    const auto len = strlen(str);
    if (len == 0)
        return false;

    int cs = 0;
    const char* p = str;
    const char* pe = p + len;
    const char* eof = pe;

    DECLARE_RAGEL_COMMON_VARS;
    DECLARE_RAGEL_NUMERIC_VARS;
    FSM fsm;
    fsm::SmpteData smpte;
    RagelEventTime event_time;
    RagelBar bar;
    RagelTempo bpm;

    
#line 135 "lex/parser_timeline.cpp"
	{
	cs = timeline_parser_start;
	}

#line 377 "lex/parser_timeline.rl"
    
#line 142 "lex/parser_timeline.cpp"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
case 1:
	switch( (*p) ) {
		case 32: goto st2;
		case 35: goto st4;
		case 43: goto tr3;
		case 45: goto tr4;
		case 47: goto st3;
		case 48: goto tr6;
		case 98: goto st146;
		case 101: goto st159;
		case 116: goto st208;
		case 118: goto st243;
	}
	if ( (*p) > 53 ) {
		if ( 54 <= (*p) && (*p) <= 57 )
			goto tr8;
	} else if ( (*p) >= 49 )
		goto tr7;
	goto st0;
st0:
cs = 0;
	goto _out;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
	switch( (*p) ) {
		case 32: goto st2;
		case 47: goto st3;
	}
	goto st0;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
	if ( (*p) == 47 )
		goto st268;
	goto st0;
st268:
	if ( ++p == pe )
		goto _test_eof268;
case 268:
	if ( (*p) == 10 )
		goto st0;
	goto st268;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
	switch( (*p) ) {
		case 48: goto tr14;
		case 95: goto tr16;
	}
	if ( (*p) < 65 ) {
		if ( 49 <= (*p) && (*p) <= 57 )
			goto tr15;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr16;
	} else
		goto tr16;
	goto st0;
tr14:
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
	goto st5;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
#line 218 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr17;
		case 43: goto tr18;
		case 45: goto tr18;
		case 46: goto tr19;
	}
	goto st0;
tr17:
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 177 "lex/parser_timeline.rl"
	{
        event_time.bar = ragel_num.vint;
        ragel_num.vint = 0;
    }
#line 203 "lex/parser_timeline.rl"
	{
        fmt::println("#{}.{}{}{}", event_time.bar, event_time.beat, event_time.sign > 0 ? '+' : '-', event_time.time);

        auto t = tl.findBarTime(event_time.bar, event_time.beat);
        event_time.times.clear();

        if (t >= 0) {
            event_time.times.push_back(t + (event_time.time * event_time.sign));
        } else {
            fmt::println("can't find bar #{}.{}", event_time.bar, event_time.beat);
            event_time.time = -1;
        }

        fsm.time_unit = 0;
    }
	goto st6;
tr116:
#line 17 "lex/ragel_units.rl"
	{ragel_type = TYPE_HOUR;}
#line 103 "lex/parser_timeline.rl"
	{
        switch(ragel_type) {
        case TYPE_MSEC:  fsm.time_unit = ragel_num.getFloat();   break;
        case TYPE_SEC:   fsm.time_unit = ragel_num.getFloat() * 1000; break;
        case TYPE_MIN:   fsm.time_unit = ragel_num.getFloat() * 60000; break;
        case TYPE_HOUR:  fsm.time_unit = ragel_num.getFloat() * 3600000; break;
        default:
        break;
        }
    }
#line 172 "lex/parser_timeline.rl"
	{
        event_time.time = fsm.time_unit;
        fsm.time_unit = 0;
    }
#line 203 "lex/parser_timeline.rl"
	{
        fmt::println("#{}.{}{}{}", event_time.bar, event_time.beat, event_time.sign > 0 ? '+' : '-', event_time.time);

        auto t = tl.findBarTime(event_time.bar, event_time.beat);
        event_time.times.clear();

        if (t >= 0) {
            event_time.times.push_back(t + (event_time.time * event_time.sign));
        } else {
            fmt::println("can't find bar #{}.{}", event_time.bar, event_time.beat);
            event_time.time = -1;
        }

        fsm.time_unit = 0;
    }
	goto st6;
tr120:
#line 18 "lex/ragel_units.rl"
	{ragel_type = TYPE_MIN;}
#line 103 "lex/parser_timeline.rl"
	{
        switch(ragel_type) {
        case TYPE_MSEC:  fsm.time_unit = ragel_num.getFloat();   break;
        case TYPE_SEC:   fsm.time_unit = ragel_num.getFloat() * 1000; break;
        case TYPE_MIN:   fsm.time_unit = ragel_num.getFloat() * 60000; break;
        case TYPE_HOUR:  fsm.time_unit = ragel_num.getFloat() * 3600000; break;
        default:
        break;
        }
    }
#line 172 "lex/parser_timeline.rl"
	{
        event_time.time = fsm.time_unit;
        fsm.time_unit = 0;
    }
#line 203 "lex/parser_timeline.rl"
	{
        fmt::println("#{}.{}{}{}", event_time.bar, event_time.beat, event_time.sign > 0 ? '+' : '-', event_time.time);

        auto t = tl.findBarTime(event_time.bar, event_time.beat);
        event_time.times.clear();

        if (t >= 0) {
            event_time.times.push_back(t + (event_time.time * event_time.sign));
        } else {
            fmt::println("can't find bar #{}.{}", event_time.bar, event_time.beat);
            event_time.time = -1;
        }

        fsm.time_unit = 0;
    }
	goto st6;
tr124:
#line 19 "lex/ragel_units.rl"
	{ragel_type = TYPE_MSEC;}
#line 103 "lex/parser_timeline.rl"
	{
        switch(ragel_type) {
        case TYPE_MSEC:  fsm.time_unit = ragel_num.getFloat();   break;
        case TYPE_SEC:   fsm.time_unit = ragel_num.getFloat() * 1000; break;
        case TYPE_MIN:   fsm.time_unit = ragel_num.getFloat() * 60000; break;
        case TYPE_HOUR:  fsm.time_unit = ragel_num.getFloat() * 3600000; break;
        default:
        break;
        }
    }
#line 172 "lex/parser_timeline.rl"
	{
        event_time.time = fsm.time_unit;
        fsm.time_unit = 0;
    }
#line 203 "lex/parser_timeline.rl"
	{
        fmt::println("#{}.{}{}{}", event_time.bar, event_time.beat, event_time.sign > 0 ? '+' : '-', event_time.time);

        auto t = tl.findBarTime(event_time.bar, event_time.beat);
        event_time.times.clear();

        if (t >= 0) {
            event_time.times.push_back(t + (event_time.time * event_time.sign));
        } else {
            fmt::println("can't find bar #{}.{}", event_time.bar, event_time.beat);
            event_time.time = -1;
        }

        fsm.time_unit = 0;
    }
	goto st6;
tr127:
#line 22 "lex/ragel_units.rl"
	{ragel_type = TYPE_SEC;}
#line 103 "lex/parser_timeline.rl"
	{
        switch(ragel_type) {
        case TYPE_MSEC:  fsm.time_unit = ragel_num.getFloat();   break;
        case TYPE_SEC:   fsm.time_unit = ragel_num.getFloat() * 1000; break;
        case TYPE_MIN:   fsm.time_unit = ragel_num.getFloat() * 60000; break;
        case TYPE_HOUR:  fsm.time_unit = ragel_num.getFloat() * 3600000; break;
        default:
        break;
        }
    }
#line 172 "lex/parser_timeline.rl"
	{
        event_time.time = fsm.time_unit;
        fsm.time_unit = 0;
    }
#line 203 "lex/parser_timeline.rl"
	{
        fmt::println("#{}.{}{}{}", event_time.bar, event_time.beat, event_time.sign > 0 ? '+' : '-', event_time.time);

        auto t = tl.findBarTime(event_time.bar, event_time.beat);
        event_time.times.clear();

        if (t >= 0) {
            event_time.times.push_back(t + (event_time.time * event_time.sign));
        } else {
            fmt::println("can't find bar #{}.{}", event_time.bar, event_time.beat);
            event_time.time = -1;
        }

        fsm.time_unit = 0;
    }
	goto st6;
tr132:
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 182 "lex/parser_timeline.rl"
	{
        event_time.beat = ragel_num.vint;
        ragel_num.vint = 0;
    }
#line 203 "lex/parser_timeline.rl"
	{
        fmt::println("#{}.{}{}{}", event_time.bar, event_time.beat, event_time.sign > 0 ? '+' : '-', event_time.time);

        auto t = tl.findBarTime(event_time.bar, event_time.beat);
        event_time.times.clear();

        if (t >= 0) {
            event_time.times.push_back(t + (event_time.time * event_time.sign));
        } else {
            fmt::println("can't find bar #{}.{}", event_time.bar, event_time.beat);
            event_time.time = -1;
        }

        fsm.time_unit = 0;
    }
	goto st6;
tr134:
#line 161 "lex/parser_timeline.rl"
	{
       event_time.rel_event = fsm.gensym();
       fsm.symbol.clear();
    }
#line 187 "lex/parser_timeline.rl"
	{
        //fmt::println("#{}{}{}", event_time.rel_event->s_name, event_time.sign > 0 ? '+' : '-', event_time.time);

        bool found = false;
        for (auto t: tl.findEventTime(event_time.rel_event)) {
            found = true;
            event_time.times.push_back(t + event_time.time * event_time.sign);
        }

        if (!found)
            fmt::println("can't find event: '{}'", event_time.rel_event->s_name);

        fsm.time_unit = 0;
        fsm.symbol.clear();
    }
	goto st6;
tr146:
#line 17 "lex/ragel_units.rl"
	{ragel_type = TYPE_HOUR;}
#line 103 "lex/parser_timeline.rl"
	{
        switch(ragel_type) {
        case TYPE_MSEC:  fsm.time_unit = ragel_num.getFloat();   break;
        case TYPE_SEC:   fsm.time_unit = ragel_num.getFloat() * 1000; break;
        case TYPE_MIN:   fsm.time_unit = ragel_num.getFloat() * 60000; break;
        case TYPE_HOUR:  fsm.time_unit = ragel_num.getFloat() * 3600000; break;
        default:
        break;
        }
    }
#line 172 "lex/parser_timeline.rl"
	{
        event_time.time = fsm.time_unit;
        fsm.time_unit = 0;
    }
#line 187 "lex/parser_timeline.rl"
	{
        //fmt::println("#{}{}{}", event_time.rel_event->s_name, event_time.sign > 0 ? '+' : '-', event_time.time);

        bool found = false;
        for (auto t: tl.findEventTime(event_time.rel_event)) {
            found = true;
            event_time.times.push_back(t + event_time.time * event_time.sign);
        }

        if (!found)
            fmt::println("can't find event: '{}'", event_time.rel_event->s_name);

        fsm.time_unit = 0;
        fsm.symbol.clear();
    }
	goto st6;
tr150:
#line 18 "lex/ragel_units.rl"
	{ragel_type = TYPE_MIN;}
#line 103 "lex/parser_timeline.rl"
	{
        switch(ragel_type) {
        case TYPE_MSEC:  fsm.time_unit = ragel_num.getFloat();   break;
        case TYPE_SEC:   fsm.time_unit = ragel_num.getFloat() * 1000; break;
        case TYPE_MIN:   fsm.time_unit = ragel_num.getFloat() * 60000; break;
        case TYPE_HOUR:  fsm.time_unit = ragel_num.getFloat() * 3600000; break;
        default:
        break;
        }
    }
#line 172 "lex/parser_timeline.rl"
	{
        event_time.time = fsm.time_unit;
        fsm.time_unit = 0;
    }
#line 187 "lex/parser_timeline.rl"
	{
        //fmt::println("#{}{}{}", event_time.rel_event->s_name, event_time.sign > 0 ? '+' : '-', event_time.time);

        bool found = false;
        for (auto t: tl.findEventTime(event_time.rel_event)) {
            found = true;
            event_time.times.push_back(t + event_time.time * event_time.sign);
        }

        if (!found)
            fmt::println("can't find event: '{}'", event_time.rel_event->s_name);

        fsm.time_unit = 0;
        fsm.symbol.clear();
    }
	goto st6;
tr154:
#line 19 "lex/ragel_units.rl"
	{ragel_type = TYPE_MSEC;}
#line 103 "lex/parser_timeline.rl"
	{
        switch(ragel_type) {
        case TYPE_MSEC:  fsm.time_unit = ragel_num.getFloat();   break;
        case TYPE_SEC:   fsm.time_unit = ragel_num.getFloat() * 1000; break;
        case TYPE_MIN:   fsm.time_unit = ragel_num.getFloat() * 60000; break;
        case TYPE_HOUR:  fsm.time_unit = ragel_num.getFloat() * 3600000; break;
        default:
        break;
        }
    }
#line 172 "lex/parser_timeline.rl"
	{
        event_time.time = fsm.time_unit;
        fsm.time_unit = 0;
    }
#line 187 "lex/parser_timeline.rl"
	{
        //fmt::println("#{}{}{}", event_time.rel_event->s_name, event_time.sign > 0 ? '+' : '-', event_time.time);

        bool found = false;
        for (auto t: tl.findEventTime(event_time.rel_event)) {
            found = true;
            event_time.times.push_back(t + event_time.time * event_time.sign);
        }

        if (!found)
            fmt::println("can't find event: '{}'", event_time.rel_event->s_name);

        fsm.time_unit = 0;
        fsm.symbol.clear();
    }
	goto st6;
tr157:
#line 22 "lex/ragel_units.rl"
	{ragel_type = TYPE_SEC;}
#line 103 "lex/parser_timeline.rl"
	{
        switch(ragel_type) {
        case TYPE_MSEC:  fsm.time_unit = ragel_num.getFloat();   break;
        case TYPE_SEC:   fsm.time_unit = ragel_num.getFloat() * 1000; break;
        case TYPE_MIN:   fsm.time_unit = ragel_num.getFloat() * 60000; break;
        case TYPE_HOUR:  fsm.time_unit = ragel_num.getFloat() * 3600000; break;
        default:
        break;
        }
    }
#line 172 "lex/parser_timeline.rl"
	{
        event_time.time = fsm.time_unit;
        fsm.time_unit = 0;
    }
#line 187 "lex/parser_timeline.rl"
	{
        //fmt::println("#{}{}{}", event_time.rel_event->s_name, event_time.sign > 0 ? '+' : '-', event_time.time);

        bool found = false;
        for (auto t: tl.findEventTime(event_time.rel_event)) {
            found = true;
            event_time.times.push_back(t + event_time.time * event_time.sign);
        }

        if (!found)
            fmt::println("can't find event: '{}'", event_time.rel_event->s_name);

        fsm.time_unit = 0;
        fsm.symbol.clear();
    }
	goto st6;
tr165:
#line 53 "lex/ragel_units.rl"
	{
    ragel_type = TYPE_SMPTE;
    smpte.hour *= smpte.sign;
    smpte.min *= smpte.sign;
    smpte.sec *= smpte.sign;
    smpte.frame *= smpte.sign;

    if (smpte.np == 2) {
        smpte.sec = smpte.min;
        smpte.min = smpte.hour;
        smpte.hour = 0;
    }
}
#line 114 "lex/parser_timeline.rl"
	{ fsm.onTimeSmpteDone(smpte); }
#line 166 "lex/parser_timeline.rl"
	{
        event_time.time = fsm.time_unit;
        event_time.times.assign(1, fsm.time_unit);
        fsm.time_unit = 0;
    }
	goto st6;
tr180:
#line 17 "lex/ragel_units.rl"
	{ragel_type = TYPE_HOUR;}
#line 103 "lex/parser_timeline.rl"
	{
        switch(ragel_type) {
        case TYPE_MSEC:  fsm.time_unit = ragel_num.getFloat();   break;
        case TYPE_SEC:   fsm.time_unit = ragel_num.getFloat() * 1000; break;
        case TYPE_MIN:   fsm.time_unit = ragel_num.getFloat() * 60000; break;
        case TYPE_HOUR:  fsm.time_unit = ragel_num.getFloat() * 3600000; break;
        default:
        break;
        }
    }
#line 166 "lex/parser_timeline.rl"
	{
        event_time.time = fsm.time_unit;
        event_time.times.assign(1, fsm.time_unit);
        fsm.time_unit = 0;
    }
	goto st6;
tr184:
#line 18 "lex/ragel_units.rl"
	{ragel_type = TYPE_MIN;}
#line 103 "lex/parser_timeline.rl"
	{
        switch(ragel_type) {
        case TYPE_MSEC:  fsm.time_unit = ragel_num.getFloat();   break;
        case TYPE_SEC:   fsm.time_unit = ragel_num.getFloat() * 1000; break;
        case TYPE_MIN:   fsm.time_unit = ragel_num.getFloat() * 60000; break;
        case TYPE_HOUR:  fsm.time_unit = ragel_num.getFloat() * 3600000; break;
        default:
        break;
        }
    }
#line 166 "lex/parser_timeline.rl"
	{
        event_time.time = fsm.time_unit;
        event_time.times.assign(1, fsm.time_unit);
        fsm.time_unit = 0;
    }
	goto st6;
tr188:
#line 19 "lex/ragel_units.rl"
	{ragel_type = TYPE_MSEC;}
#line 103 "lex/parser_timeline.rl"
	{
        switch(ragel_type) {
        case TYPE_MSEC:  fsm.time_unit = ragel_num.getFloat();   break;
        case TYPE_SEC:   fsm.time_unit = ragel_num.getFloat() * 1000; break;
        case TYPE_MIN:   fsm.time_unit = ragel_num.getFloat() * 60000; break;
        case TYPE_HOUR:  fsm.time_unit = ragel_num.getFloat() * 3600000; break;
        default:
        break;
        }
    }
#line 166 "lex/parser_timeline.rl"
	{
        event_time.time = fsm.time_unit;
        event_time.times.assign(1, fsm.time_unit);
        fsm.time_unit = 0;
    }
	goto st6;
tr191:
#line 22 "lex/ragel_units.rl"
	{ragel_type = TYPE_SEC;}
#line 103 "lex/parser_timeline.rl"
	{
        switch(ragel_type) {
        case TYPE_MSEC:  fsm.time_unit = ragel_num.getFloat();   break;
        case TYPE_SEC:   fsm.time_unit = ragel_num.getFloat() * 1000; break;
        case TYPE_MIN:   fsm.time_unit = ragel_num.getFloat() * 60000; break;
        case TYPE_HOUR:  fsm.time_unit = ragel_num.getFloat() * 3600000; break;
        default:
        break;
        }
    }
#line 166 "lex/parser_timeline.rl"
	{
        event_time.time = fsm.time_unit;
        event_time.times.assign(1, fsm.time_unit);
        fsm.time_unit = 0;
    }
	goto st6;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
#line 707 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto st6;
		case 101: goto st7;
		case 118: goto st31;
	}
	goto st0;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
	if ( (*p) == 118 )
		goto st8;
	goto st0;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
	if ( (*p) == 101 )
		goto st9;
	goto st0;
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
	if ( (*p) == 110 )
		goto st10;
	goto st0;
st10:
	if ( ++p == pe )
		goto _test_eof10;
case 10:
	if ( (*p) == 116 )
		goto st11;
	goto st0;
st11:
	if ( ++p == pe )
		goto _test_eof11;
case 11:
	if ( (*p) == 32 )
		goto st12;
	goto st0;
st12:
	if ( ++p == pe )
		goto _test_eof12;
case 12:
	switch( (*p) ) {
		case 32: goto st12;
		case 33: goto st13;
		case 95: goto tr29;
	}
	if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr29;
	} else if ( (*p) >= 65 )
		goto tr29;
	goto st0;
st13:
	if ( ++p == pe )
		goto _test_eof13;
case 13:
	switch( (*p) ) {
		case 111: goto st14;
		case 112: goto st16;
		case 115: goto st23;
	}
	goto st0;
st14:
	if ( ++p == pe )
		goto _test_eof14;
case 14:
	if ( (*p) == 117 )
		goto st15;
	goto st0;
st15:
	if ( ++p == pe )
		goto _test_eof15;
case 15:
	if ( (*p) == 116 )
		goto st269;
	goto st0;
st269:
	if ( ++p == pe )
		goto _test_eof269;
case 269:
	if ( (*p) == 32 )
		goto st270;
	goto st0;
tr354:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st270;
st270:
	if ( ++p == pe )
		goto _test_eof270;
case 270:
#line 803 "lex/parser_timeline.cpp"
	goto tr354;
st16:
	if ( ++p == pe )
		goto _test_eof16;
case 16:
	if ( (*p) == 114 )
		goto st17;
	goto st0;
st17:
	if ( ++p == pe )
		goto _test_eof17;
case 17:
	if ( (*p) == 101 )
		goto st18;
	goto st0;
st18:
	if ( ++p == pe )
		goto _test_eof18;
case 18:
	if ( (*p) == 115 )
		goto st19;
	goto st0;
st19:
	if ( ++p == pe )
		goto _test_eof19;
case 19:
	if ( (*p) == 101 )
		goto st20;
	goto st0;
st20:
	if ( ++p == pe )
		goto _test_eof20;
case 20:
	if ( (*p) == 116 )
		goto st21;
	goto st0;
st21:
	if ( ++p == pe )
		goto _test_eof21;
case 21:
	if ( (*p) == 32 )
		goto st22;
	goto st0;
st22:
	if ( ++p == pe )
		goto _test_eof22;
case 22:
	switch( (*p) ) {
		case 32: goto st22;
		case 48: goto tr41;
	}
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr42;
	goto st0;
tr41:
#line 266 "lex/parser_timeline.rl"
	{ragel_num.vint = 0;}
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
#line 266 "lex/parser_timeline.rl"
	{ fsm.act_preset.idx = ragel_num.vint; }
	goto st271;
st271:
	if ( ++p == pe )
		goto _test_eof271;
case 271:
#line 870 "lex/parser_timeline.cpp"
	goto st0;
tr42:
#line 266 "lex/parser_timeline.rl"
	{ragel_num.vint = 0;}
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
#line 266 "lex/parser_timeline.rl"
	{ fsm.act_preset.idx = ragel_num.vint; }
	goto st272;
tr355:
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
#line 266 "lex/parser_timeline.rl"
	{ fsm.act_preset.idx = ragel_num.vint; }
	goto st272;
st272:
	if ( ++p == pe )
		goto _test_eof272;
case 272:
#line 890 "lex/parser_timeline.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr355;
	goto st0;
st23:
	if ( ++p == pe )
		goto _test_eof23;
case 23:
	if ( (*p) == 101 )
		goto st24;
	goto st0;
st24:
	if ( ++p == pe )
		goto _test_eof24;
case 24:
	if ( (*p) == 110 )
		goto st25;
	goto st0;
st25:
	if ( ++p == pe )
		goto _test_eof25;
case 25:
	if ( (*p) == 100 )
		goto st26;
	goto st0;
st26:
	if ( ++p == pe )
		goto _test_eof26;
case 26:
	if ( (*p) == 32 )
		goto st27;
	goto st0;
st27:
	if ( ++p == pe )
		goto _test_eof27;
case 27:
	switch( (*p) ) {
		case 32: goto st27;
		case 43: goto tr48;
		case 45: goto tr48;
		case 48: goto tr49;
	}
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr50;
	goto tr47;
tr47:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st273;
st273:
	if ( ++p == pe )
		goto _test_eof273;
case 273:
#line 943 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr356;
	goto tr47;
tr357:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st274;
tr356:
#line 123 "lex/parser_timeline.rl"
	{ fsm.onSendTarget(); }
	goto st274;
st274:
	if ( ++p == pe )
		goto _test_eof274;
case 274:
#line 959 "lex/parser_timeline.cpp"
	goto tr357;
tr48:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 21 "lex/ragel_numeric.rl"
	{ ragel_num.sign = ((*p)=='-') ? -1 : 1; }
	goto st275;
st275:
	if ( ++p == pe )
		goto _test_eof275;
case 275:
#line 971 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr356;
		case 48: goto tr49;
	}
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr50;
	goto tr47;
tr49:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
	goto st28;
st28:
	if ( ++p == pe )
		goto _test_eof28;
case 28:
#line 989 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto st0;
		case 46: goto tr51;
	}
	goto tr47;
tr51:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
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
	goto st276;
st276:
	if ( ++p == pe )
		goto _test_eof276;
case 276:
#line 1014 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr356;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr52;
	goto tr47;
tr52:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 54 "lex/ragel_numeric.rl"
	{
        (ragel_num.ratio.num *= 10) += ((*p) - '0');
        ragel_num.ratio.den *= 10;
    }
	goto st29;
st29:
	if ( ++p == pe )
		goto _test_eof29;
case 29:
#line 1033 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto st0;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr52;
	goto tr47;
tr50:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
	goto st30;
st30:
	if ( ++p == pe )
		goto _test_eof30;
case 30:
#line 1049 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto st0;
		case 46: goto tr51;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr50;
	goto tr47;
tr29:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st277;
st277:
	if ( ++p == pe )
		goto _test_eof277;
case 277:
#line 1065 "lex/parser_timeline.cpp"
	if ( (*p) == 95 )
		goto tr29;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr29;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr29;
	} else
		goto tr29;
	goto st0;
st31:
	if ( ++p == pe )
		goto _test_eof31;
case 31:
	if ( (*p) == 97 )
		goto st32;
	goto st0;
st32:
	if ( ++p == pe )
		goto _test_eof32;
case 32:
	if ( (*p) == 114 )
		goto st33;
	goto st0;
st33:
	if ( ++p == pe )
		goto _test_eof33;
case 33:
	if ( (*p) == 32 )
		goto st34;
	goto st0;
st34:
	if ( ++p == pe )
		goto _test_eof34;
case 34:
	switch( (*p) ) {
		case 32: goto st34;
		case 36: goto st35;
	}
	goto st0;
st35:
	if ( ++p == pe )
		goto _test_eof35;
case 35:
	if ( (*p) == 95 )
		goto tr57;
	if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr57;
	} else if ( (*p) >= 65 )
		goto tr57;
	goto st0;
tr57:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st36;
st36:
	if ( ++p == pe )
		goto _test_eof36;
case 36:
#line 1127 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto st37;
		case 95: goto tr57;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr57;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr57;
	} else
		goto tr57;
	goto st0;
st37:
	if ( ++p == pe )
		goto _test_eof37;
case 37:
	switch( (*p) ) {
		case 32: goto st37;
		case 33: goto st38;
	}
	goto st0;
st38:
	if ( ++p == pe )
		goto _test_eof38;
case 38:
	switch( (*p) ) {
		case 108: goto st39;
		case 115: goto st67;
	}
	goto st0;
st39:
	if ( ++p == pe )
		goto _test_eof39;
case 39:
	if ( (*p) == 105 )
		goto st40;
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
	if ( (*p) == 101 )
		goto st42;
	goto st0;
st42:
	if ( ++p == pe )
		goto _test_eof42;
case 42:
	switch( (*p) ) {
		case 32: goto st43;
		case 116: goto st65;
	}
	goto st0;
st43:
	if ( ++p == pe )
		goto _test_eof43;
case 43:
	switch( (*p) ) {
		case 32: goto st43;
		case 43: goto tr67;
		case 45: goto tr67;
		case 48: goto tr68;
	}
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr69;
	goto st0;
tr67:
#line 21 "lex/ragel_numeric.rl"
	{ ragel_num.sign = ((*p)=='-') ? -1 : 1; }
	goto st44;
st44:
	if ( ++p == pe )
		goto _test_eof44;
case 44:
#line 1210 "lex/parser_timeline.cpp"
	if ( (*p) == 48 )
		goto tr68;
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr69;
	goto st0;
tr68:
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
	goto st45;
st45:
	if ( ++p == pe )
		goto _test_eof45;
case 45:
#line 1224 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr70;
		case 46: goto tr71;
	}
	goto st0;
tr70:
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 7 "lex/ragel_units.rl"
	{ ragel_num.vdouble = ragel_num.vint; }
	goto st46;
tr97:
#line 58 "lex/ragel_numeric.rl"
	{
        const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
        ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
        ragel_type = TYPE_FLOAT;
        ragel_cat  = CAT_NUMBER;
    }
	goto st46;
st46:
	if ( ++p == pe )
		goto _test_eof46;
case 46:
#line 1253 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto st46;
		case 43: goto tr73;
		case 45: goto tr73;
		case 48: goto tr74;
	}
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr75;
	goto st0;
tr73:
#line 21 "lex/ragel_numeric.rl"
	{ ragel_num.sign = ((*p)=='-') ? -1 : 1; }
	goto st47;
st47:
	if ( ++p == pe )
		goto _test_eof47;
case 47:
#line 1271 "lex/parser_timeline.cpp"
	if ( (*p) == 48 )
		goto tr74;
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr75;
	goto st0;
tr74:
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
	goto st48;
st48:
	if ( ++p == pe )
		goto _test_eof48;
case 48:
#line 1285 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr76;
		case 46: goto tr77;
	}
	goto st0;
tr76:
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 7 "lex/ragel_units.rl"
	{ ragel_num.vdouble = ragel_num.vint; }
	goto st49;
tr95:
#line 58 "lex/ragel_numeric.rl"
	{
        const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
        ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
        ragel_type = TYPE_FLOAT;
        ragel_cat  = CAT_NUMBER;
    }
	goto st49;
st49:
	if ( ++p == pe )
		goto _test_eof49;
case 49:
#line 1314 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto st49;
		case 48: goto tr79;
	}
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr80;
	goto st0;
tr79:
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
	goto st50;
st50:
	if ( ++p == pe )
		goto _test_eof50;
case 50:
#line 1330 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 46: goto tr81;
		case 104: goto tr82;
		case 109: goto tr83;
		case 115: goto tr84;
	}
	goto st0;
tr81:
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
	goto st51;
st51:
	if ( ++p == pe )
		goto _test_eof51;
case 51:
#line 1355 "lex/parser_timeline.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr85;
	goto st0;
tr85:
#line 54 "lex/ragel_numeric.rl"
	{
        (ragel_num.ratio.num *= 10) += ((*p) - '0');
        ragel_num.ratio.den *= 10;
    }
	goto st52;
st52:
	if ( ++p == pe )
		goto _test_eof52;
case 52:
#line 1370 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 104: goto tr86;
		case 109: goto tr87;
		case 115: goto tr88;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr85;
	goto st0;
tr82:
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 7 "lex/ragel_units.rl"
	{ ragel_num.vdouble = ragel_num.vint; }
	goto st278;
tr86:
#line 58 "lex/ragel_numeric.rl"
	{
        const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
        ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
        ragel_type = TYPE_FLOAT;
        ragel_cat  = CAT_NUMBER;
    }
	goto st278;
st278:
	if ( ++p == pe )
		goto _test_eof278;
case 278:
#line 1402 "lex/parser_timeline.cpp"
	if ( (*p) == 111 )
		goto st53;
	goto st0;
st53:
	if ( ++p == pe )
		goto _test_eof53;
case 53:
	if ( (*p) == 117 )
		goto st54;
	goto st0;
st54:
	if ( ++p == pe )
		goto _test_eof54;
case 54:
	if ( (*p) == 114 )
		goto st279;
	goto st0;
st279:
	if ( ++p == pe )
		goto _test_eof279;
case 279:
	goto st0;
tr83:
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 7 "lex/ragel_units.rl"
	{ ragel_num.vdouble = ragel_num.vint; }
	goto st280;
tr87:
#line 58 "lex/ragel_numeric.rl"
	{
        const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
        ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
        ragel_type = TYPE_FLOAT;
        ragel_cat  = CAT_NUMBER;
    }
	goto st280;
st280:
	if ( ++p == pe )
		goto _test_eof280;
case 280:
#line 1448 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 105: goto st55;
		case 115: goto st282;
	}
	goto st0;
st55:
	if ( ++p == pe )
		goto _test_eof55;
case 55:
	if ( (*p) == 110 )
		goto st281;
	goto st0;
st281:
	if ( ++p == pe )
		goto _test_eof281;
case 281:
	goto st0;
st282:
	if ( ++p == pe )
		goto _test_eof282;
case 282:
	if ( (*p) == 101 )
		goto st56;
	goto st0;
st56:
	if ( ++p == pe )
		goto _test_eof56;
case 56:
	if ( (*p) == 99 )
		goto st283;
	goto st0;
st283:
	if ( ++p == pe )
		goto _test_eof283;
case 283:
	goto st0;
tr84:
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 7 "lex/ragel_units.rl"
	{ ragel_num.vdouble = ragel_num.vint; }
	goto st284;
tr88:
#line 58 "lex/ragel_numeric.rl"
	{
        const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
        ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
        ragel_type = TYPE_FLOAT;
        ragel_cat  = CAT_NUMBER;
    }
	goto st284;
st284:
	if ( ++p == pe )
		goto _test_eof284;
case 284:
#line 1508 "lex/parser_timeline.cpp"
	if ( (*p) == 101 )
		goto st57;
	goto st0;
st57:
	if ( ++p == pe )
		goto _test_eof57;
case 57:
	if ( (*p) == 99 )
		goto st285;
	goto st0;
st285:
	if ( ++p == pe )
		goto _test_eof285;
case 285:
	goto st0;
tr80:
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
	goto st58;
st58:
	if ( ++p == pe )
		goto _test_eof58;
case 58:
#line 1532 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 46: goto tr81;
		case 104: goto tr82;
		case 109: goto tr83;
		case 115: goto tr84;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr80;
	goto st0;
tr77:
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
	goto st59;
st59:
	if ( ++p == pe )
		goto _test_eof59;
case 59:
#line 1559 "lex/parser_timeline.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr94;
	goto st0;
tr94:
#line 54 "lex/ragel_numeric.rl"
	{
        (ragel_num.ratio.num *= 10) += ((*p) - '0');
        ragel_num.ratio.den *= 10;
    }
	goto st60;
st60:
	if ( ++p == pe )
		goto _test_eof60;
case 60:
#line 1574 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr95;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr94;
	goto st0;
tr75:
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
	goto st61;
st61:
	if ( ++p == pe )
		goto _test_eof61;
case 61:
#line 1588 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr76;
		case 46: goto tr77;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr75;
	goto st0;
tr71:
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
	goto st62;
st62:
	if ( ++p == pe )
		goto _test_eof62;
case 62:
#line 1613 "lex/parser_timeline.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr96;
	goto st0;
tr96:
#line 54 "lex/ragel_numeric.rl"
	{
        (ragel_num.ratio.num *= 10) += ((*p) - '0');
        ragel_num.ratio.den *= 10;
    }
	goto st63;
st63:
	if ( ++p == pe )
		goto _test_eof63;
case 63:
#line 1628 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr97;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr96;
	goto st0;
tr69:
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
	goto st64;
st64:
	if ( ++p == pe )
		goto _test_eof64;
case 64:
#line 1642 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr70;
		case 46: goto tr71;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr69;
	goto st0;
st65:
	if ( ++p == pe )
		goto _test_eof65;
case 65:
	if ( (*p) == 111 )
		goto st66;
	goto st0;
st66:
	if ( ++p == pe )
		goto _test_eof66;
case 66:
	if ( (*p) == 32 )
		goto st46;
	goto st0;
st67:
	if ( ++p == pe )
		goto _test_eof67;
case 67:
	if ( (*p) == 101 )
		goto st68;
	goto st0;
st68:
	if ( ++p == pe )
		goto _test_eof68;
case 68:
	if ( (*p) == 116 )
		goto st69;
	goto st0;
st69:
	if ( ++p == pe )
		goto _test_eof69;
case 69:
	if ( (*p) == 32 )
		goto st70;
	goto st0;
st70:
	if ( ++p == pe )
		goto _test_eof70;
case 70:
	switch( (*p) ) {
		case 32: goto st70;
		case 43: goto tr102;
		case 45: goto tr102;
		case 48: goto tr103;
	}
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr104;
	goto st0;
tr102:
#line 21 "lex/ragel_numeric.rl"
	{ ragel_num.sign = ((*p)=='-') ? -1 : 1; }
	goto st71;
st71:
	if ( ++p == pe )
		goto _test_eof71;
case 71:
#line 1706 "lex/parser_timeline.cpp"
	if ( (*p) == 48 )
		goto tr103;
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr104;
	goto st0;
tr103:
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
	goto st286;
st286:
	if ( ++p == pe )
		goto _test_eof286;
case 286:
#line 1720 "lex/parser_timeline.cpp"
	if ( (*p) == 46 )
		goto tr363;
	goto st0;
tr363:
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
	goto st72;
st72:
	if ( ++p == pe )
		goto _test_eof72;
case 72:
#line 1741 "lex/parser_timeline.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr105;
	goto st0;
tr105:
#line 54 "lex/ragel_numeric.rl"
	{
        (ragel_num.ratio.num *= 10) += ((*p) - '0');
        ragel_num.ratio.den *= 10;
    }
	goto st287;
st287:
	if ( ++p == pe )
		goto _test_eof287;
case 287:
#line 1756 "lex/parser_timeline.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr105;
	goto st0;
tr104:
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
	goto st288;
st288:
	if ( ++p == pe )
		goto _test_eof288;
case 288:
#line 1768 "lex/parser_timeline.cpp"
	if ( (*p) == 46 )
		goto tr363;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr104;
	goto st0;
tr18:
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 177 "lex/parser_timeline.rl"
	{
        event_time.bar = ragel_num.vint;
        ragel_num.vint = 0;
    }
#line 292 "lex/parser_timeline.rl"
	{ event_time.sign = (*p == '+') ? +1 : -1; }
	goto st73;
tr133:
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 182 "lex/parser_timeline.rl"
	{
        event_time.beat = ragel_num.vint;
        ragel_num.vint = 0;
    }
#line 292 "lex/parser_timeline.rl"
	{ event_time.sign = (*p == '+') ? +1 : -1; }
	goto st73;
st73:
	if ( ++p == pe )
		goto _test_eof73;
case 73:
#line 1808 "lex/parser_timeline.cpp"
	if ( (*p) == 48 )
		goto tr106;
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr107;
	goto st0;
tr106:
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
	goto st74;
st74:
	if ( ++p == pe )
		goto _test_eof74;
case 74:
#line 1822 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 46: goto tr108;
		case 104: goto tr109;
		case 109: goto tr110;
		case 115: goto tr111;
	}
	goto st0;
tr108:
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
	goto st75;
st75:
	if ( ++p == pe )
		goto _test_eof75;
case 75:
#line 1847 "lex/parser_timeline.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr112;
	goto st0;
tr112:
#line 54 "lex/ragel_numeric.rl"
	{
        (ragel_num.ratio.num *= 10) += ((*p) - '0');
        ragel_num.ratio.den *= 10;
    }
	goto st76;
st76:
	if ( ++p == pe )
		goto _test_eof76;
case 76:
#line 1862 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 104: goto tr113;
		case 109: goto tr114;
		case 115: goto tr115;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr112;
	goto st0;
tr109:
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 7 "lex/ragel_units.rl"
	{ ragel_num.vdouble = ragel_num.vint; }
	goto st77;
tr113:
#line 58 "lex/ragel_numeric.rl"
	{
        const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
        ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
        ragel_type = TYPE_FLOAT;
        ragel_cat  = CAT_NUMBER;
    }
	goto st77;
st77:
	if ( ++p == pe )
		goto _test_eof77;
case 77:
#line 1894 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr116;
		case 111: goto st78;
	}
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
	if ( (*p) == 114 )
		goto st80;
	goto st0;
st80:
	if ( ++p == pe )
		goto _test_eof80;
case 80:
	if ( (*p) == 32 )
		goto tr116;
	goto st0;
tr110:
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 7 "lex/ragel_units.rl"
	{ ragel_num.vdouble = ragel_num.vint; }
	goto st81;
tr114:
#line 58 "lex/ragel_numeric.rl"
	{
        const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
        ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
        ragel_type = TYPE_FLOAT;
        ragel_cat  = CAT_NUMBER;
    }
	goto st81;
st81:
	if ( ++p == pe )
		goto _test_eof81;
case 81:
#line 1944 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr120;
		case 105: goto st82;
		case 115: goto st84;
	}
	goto st0;
st82:
	if ( ++p == pe )
		goto _test_eof82;
case 82:
	if ( (*p) == 110 )
		goto st83;
	goto st0;
st83:
	if ( ++p == pe )
		goto _test_eof83;
case 83:
	if ( (*p) == 32 )
		goto tr120;
	goto st0;
st84:
	if ( ++p == pe )
		goto _test_eof84;
case 84:
	switch( (*p) ) {
		case 32: goto tr124;
		case 101: goto st85;
	}
	goto st0;
st85:
	if ( ++p == pe )
		goto _test_eof85;
case 85:
	if ( (*p) == 99 )
		goto st86;
	goto st0;
st86:
	if ( ++p == pe )
		goto _test_eof86;
case 86:
	if ( (*p) == 32 )
		goto tr124;
	goto st0;
tr111:
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 7 "lex/ragel_units.rl"
	{ ragel_num.vdouble = ragel_num.vint; }
	goto st87;
tr115:
#line 58 "lex/ragel_numeric.rl"
	{
        const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
        ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
        ragel_type = TYPE_FLOAT;
        ragel_cat  = CAT_NUMBER;
    }
	goto st87;
st87:
	if ( ++p == pe )
		goto _test_eof87;
case 87:
#line 2011 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr127;
		case 101: goto st88;
	}
	goto st0;
st88:
	if ( ++p == pe )
		goto _test_eof88;
case 88:
	if ( (*p) == 99 )
		goto st89;
	goto st0;
st89:
	if ( ++p == pe )
		goto _test_eof89;
case 89:
	if ( (*p) == 32 )
		goto tr127;
	goto st0;
tr107:
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
	goto st90;
st90:
	if ( ++p == pe )
		goto _test_eof90;
case 90:
#line 2039 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 46: goto tr108;
		case 104: goto tr109;
		case 109: goto tr110;
		case 115: goto tr111;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr107;
	goto st0;
tr19:
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 177 "lex/parser_timeline.rl"
	{
        event_time.bar = ragel_num.vint;
        ragel_num.vint = 0;
    }
	goto st91;
st91:
	if ( ++p == pe )
		goto _test_eof91;
case 91:
#line 2066 "lex/parser_timeline.cpp"
	if ( (*p) == 48 )
		goto tr130;
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr131;
	goto st0;
tr130:
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
	goto st92;
st92:
	if ( ++p == pe )
		goto _test_eof92;
case 92:
#line 2080 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr132;
		case 43: goto tr133;
		case 45: goto tr133;
	}
	goto st0;
tr131:
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
	goto st93;
st93:
	if ( ++p == pe )
		goto _test_eof93;
case 93:
#line 2095 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr132;
		case 43: goto tr133;
		case 45: goto tr133;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr131;
	goto st0;
tr15:
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
	goto st94;
st94:
	if ( ++p == pe )
		goto _test_eof94;
case 94:
#line 2112 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr17;
		case 43: goto tr18;
		case 45: goto tr18;
		case 46: goto tr19;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr15;
	goto st0;
tr16:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st95;
st95:
	if ( ++p == pe )
		goto _test_eof95;
case 95:
#line 2130 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr134;
		case 43: goto tr135;
		case 45: goto tr135;
		case 95: goto tr16;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr16;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr16;
	} else
		goto tr16;
	goto st0;
tr135:
#line 161 "lex/parser_timeline.rl"
	{
       event_time.rel_event = fsm.gensym();
       fsm.symbol.clear();
    }
#line 292 "lex/parser_timeline.rl"
	{ event_time.sign = (*p == '+') ? +1 : -1; }
	goto st96;
st96:
	if ( ++p == pe )
		goto _test_eof96;
case 96:
#line 2159 "lex/parser_timeline.cpp"
	if ( (*p) == 48 )
		goto tr136;
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr137;
	goto st0;
tr136:
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
	goto st97;
st97:
	if ( ++p == pe )
		goto _test_eof97;
case 97:
#line 2173 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 46: goto tr138;
		case 104: goto tr139;
		case 109: goto tr140;
		case 115: goto tr141;
	}
	goto st0;
tr138:
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
	goto st98;
st98:
	if ( ++p == pe )
		goto _test_eof98;
case 98:
#line 2198 "lex/parser_timeline.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr142;
	goto st0;
tr142:
#line 54 "lex/ragel_numeric.rl"
	{
        (ragel_num.ratio.num *= 10) += ((*p) - '0');
        ragel_num.ratio.den *= 10;
    }
	goto st99;
st99:
	if ( ++p == pe )
		goto _test_eof99;
case 99:
#line 2213 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 104: goto tr143;
		case 109: goto tr144;
		case 115: goto tr145;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr142;
	goto st0;
tr139:
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 7 "lex/ragel_units.rl"
	{ ragel_num.vdouble = ragel_num.vint; }
	goto st100;
tr143:
#line 58 "lex/ragel_numeric.rl"
	{
        const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
        ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
        ragel_type = TYPE_FLOAT;
        ragel_cat  = CAT_NUMBER;
    }
	goto st100;
st100:
	if ( ++p == pe )
		goto _test_eof100;
case 100:
#line 2245 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr146;
		case 111: goto st101;
	}
	goto st0;
st101:
	if ( ++p == pe )
		goto _test_eof101;
case 101:
	if ( (*p) == 117 )
		goto st102;
	goto st0;
st102:
	if ( ++p == pe )
		goto _test_eof102;
case 102:
	if ( (*p) == 114 )
		goto st103;
	goto st0;
st103:
	if ( ++p == pe )
		goto _test_eof103;
case 103:
	if ( (*p) == 32 )
		goto tr146;
	goto st0;
tr140:
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 7 "lex/ragel_units.rl"
	{ ragel_num.vdouble = ragel_num.vint; }
	goto st104;
tr144:
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
#line 2295 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr150;
		case 105: goto st105;
		case 115: goto st107;
	}
	goto st0;
st105:
	if ( ++p == pe )
		goto _test_eof105;
case 105:
	if ( (*p) == 110 )
		goto st106;
	goto st0;
st106:
	if ( ++p == pe )
		goto _test_eof106;
case 106:
	if ( (*p) == 32 )
		goto tr150;
	goto st0;
st107:
	if ( ++p == pe )
		goto _test_eof107;
case 107:
	switch( (*p) ) {
		case 32: goto tr154;
		case 101: goto st108;
	}
	goto st0;
st108:
	if ( ++p == pe )
		goto _test_eof108;
case 108:
	if ( (*p) == 99 )
		goto st109;
	goto st0;
st109:
	if ( ++p == pe )
		goto _test_eof109;
case 109:
	if ( (*p) == 32 )
		goto tr154;
	goto st0;
tr141:
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 7 "lex/ragel_units.rl"
	{ ragel_num.vdouble = ragel_num.vint; }
	goto st110;
tr145:
#line 58 "lex/ragel_numeric.rl"
	{
        const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
        ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
        ragel_type = TYPE_FLOAT;
        ragel_cat  = CAT_NUMBER;
    }
	goto st110;
st110:
	if ( ++p == pe )
		goto _test_eof110;
case 110:
#line 2362 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr157;
		case 101: goto st111;
	}
	goto st0;
st111:
	if ( ++p == pe )
		goto _test_eof111;
case 111:
	if ( (*p) == 99 )
		goto st112;
	goto st0;
st112:
	if ( ++p == pe )
		goto _test_eof112;
case 112:
	if ( (*p) == 32 )
		goto tr157;
	goto st0;
tr137:
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
	goto st113;
st113:
	if ( ++p == pe )
		goto _test_eof113;
case 113:
#line 2390 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 46: goto tr138;
		case 104: goto tr139;
		case 109: goto tr140;
		case 115: goto tr141;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr137;
	goto st0;
tr3:
#line 301 "lex/parser_timeline.rl"
	{ event_time.relative = true; }
#line 79 "lex/ragel_units.rl"
	{smpte.np = 0;}
#line 74 "lex/ragel_units.rl"
	{smpte.sign = ((*p)=='-')?-1:1;}
	goto st114;
st114:
	if ( ++p == pe )
		goto _test_eof114;
case 114:
#line 2412 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 43: goto tr4;
		case 45: goto tr4;
		case 48: goto tr6;
	}
	if ( (*p) > 53 ) {
		if ( 54 <= (*p) && (*p) <= 57 )
			goto tr8;
	} else if ( (*p) >= 49 )
		goto tr7;
	goto st0;
tr4:
#line 79 "lex/ragel_units.rl"
	{smpte.np = 0;}
#line 74 "lex/ragel_units.rl"
	{smpte.sign = ((*p)=='-')?-1:1;}
	goto st115;
st115:
	if ( ++p == pe )
		goto _test_eof115;
case 115:
#line 2434 "lex/parser_timeline.cpp"
	if ( 48 <= (*p) && (*p) <= 53 )
		goto tr160;
	goto st0;
tr160:
#line 67 "lex/ragel_units.rl"
	{smpte.hour = 0; smpte.np++;}
#line 67 "lex/ragel_units.rl"
	{(smpte.hour *= 10) += ((*p) - '0');}
	goto st116;
st116:
	if ( ++p == pe )
		goto _test_eof116;
case 116:
#line 2448 "lex/parser_timeline.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr161;
	goto st0;
tr161:
#line 67 "lex/ragel_units.rl"
	{(smpte.hour *= 10) += ((*p) - '0');}
	goto st117;
st117:
	if ( ++p == pe )
		goto _test_eof117;
case 117:
#line 2460 "lex/parser_timeline.cpp"
	if ( (*p) == 58 )
		goto st118;
	goto st0;
st118:
	if ( ++p == pe )
		goto _test_eof118;
case 118:
	if ( 48 <= (*p) && (*p) <= 53 )
		goto tr163;
	goto st0;
tr163:
#line 68 "lex/ragel_units.rl"
	{smpte.min = 0; smpte.np++;}
#line 68 "lex/ragel_units.rl"
	{(smpte.min *= 10) += ((*p) - '0');}
	goto st119;
st119:
	if ( ++p == pe )
		goto _test_eof119;
case 119:
#line 2481 "lex/parser_timeline.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr164;
	goto st0;
tr164:
#line 68 "lex/ragel_units.rl"
	{(smpte.min *= 10) += ((*p) - '0');}
	goto st120;
st120:
	if ( ++p == pe )
		goto _test_eof120;
case 120:
#line 2493 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr165;
		case 46: goto st121;
		case 58: goto st124;
	}
	goto st0;
st121:
	if ( ++p == pe )
		goto _test_eof121;
case 121:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr168;
	goto st0;
tr168:
#line 70 "lex/ragel_units.rl"
	{smpte.frame = 0;}
#line 70 "lex/ragel_units.rl"
	{(smpte.frame *= 10) += ((*p) - '0');}
	goto st122;
st122:
	if ( ++p == pe )
		goto _test_eof122;
case 122:
#line 2517 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr165;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr169;
	goto st0;
tr169:
#line 70 "lex/ragel_units.rl"
	{(smpte.frame *= 10) += ((*p) - '0');}
	goto st123;
st123:
	if ( ++p == pe )
		goto _test_eof123;
case 123:
#line 2531 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr165;
	goto st0;
st124:
	if ( ++p == pe )
		goto _test_eof124;
case 124:
	if ( 48 <= (*p) && (*p) <= 53 )
		goto tr170;
	goto st0;
tr170:
#line 69 "lex/ragel_units.rl"
	{smpte.sec = 0; smpte.np++;}
#line 69 "lex/ragel_units.rl"
	{(smpte.sec *= 10) += ((*p) - '0');}
	goto st125;
st125:
	if ( ++p == pe )
		goto _test_eof125;
case 125:
#line 2552 "lex/parser_timeline.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr171;
	goto st0;
tr171:
#line 69 "lex/ragel_units.rl"
	{(smpte.sec *= 10) += ((*p) - '0');}
	goto st126;
st126:
	if ( ++p == pe )
		goto _test_eof126;
case 126:
#line 2564 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr165;
		case 46: goto st121;
	}
	goto st0;
tr6:
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
#line 79 "lex/ragel_units.rl"
	{smpte.np = 0;}
#line 67 "lex/ragel_units.rl"
	{smpte.hour = 0; smpte.np++;}
#line 67 "lex/ragel_units.rl"
	{(smpte.hour *= 10) += ((*p) - '0');}
	goto st127;
st127:
	if ( ++p == pe )
		goto _test_eof127;
case 127:
#line 2584 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 46: goto tr172;
		case 104: goto tr173;
		case 109: goto tr174;
		case 115: goto tr175;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr161;
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
	goto st128;
st128:
	if ( ++p == pe )
		goto _test_eof128;
case 128:
#line 2611 "lex/parser_timeline.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr176;
	goto st0;
tr176:
#line 54 "lex/ragel_numeric.rl"
	{
        (ragel_num.ratio.num *= 10) += ((*p) - '0');
        ragel_num.ratio.den *= 10;
    }
	goto st129;
st129:
	if ( ++p == pe )
		goto _test_eof129;
case 129:
#line 2626 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 104: goto tr177;
		case 109: goto tr178;
		case 115: goto tr179;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr176;
	goto st0;
tr173:
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 7 "lex/ragel_units.rl"
	{ ragel_num.vdouble = ragel_num.vint; }
	goto st130;
tr177:
#line 58 "lex/ragel_numeric.rl"
	{
        const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
        ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
        ragel_type = TYPE_FLOAT;
        ragel_cat  = CAT_NUMBER;
    }
	goto st130;
st130:
	if ( ++p == pe )
		goto _test_eof130;
case 130:
#line 2658 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr180;
		case 111: goto st131;
	}
	goto st0;
st131:
	if ( ++p == pe )
		goto _test_eof131;
case 131:
	if ( (*p) == 117 )
		goto st132;
	goto st0;
st132:
	if ( ++p == pe )
		goto _test_eof132;
case 132:
	if ( (*p) == 114 )
		goto st133;
	goto st0;
st133:
	if ( ++p == pe )
		goto _test_eof133;
case 133:
	if ( (*p) == 32 )
		goto tr180;
	goto st0;
tr174:
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 7 "lex/ragel_units.rl"
	{ ragel_num.vdouble = ragel_num.vint; }
	goto st134;
tr178:
#line 58 "lex/ragel_numeric.rl"
	{
        const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
        ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
        ragel_type = TYPE_FLOAT;
        ragel_cat  = CAT_NUMBER;
    }
	goto st134;
st134:
	if ( ++p == pe )
		goto _test_eof134;
case 134:
#line 2708 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr184;
		case 105: goto st135;
		case 115: goto st137;
	}
	goto st0;
st135:
	if ( ++p == pe )
		goto _test_eof135;
case 135:
	if ( (*p) == 110 )
		goto st136;
	goto st0;
st136:
	if ( ++p == pe )
		goto _test_eof136;
case 136:
	if ( (*p) == 32 )
		goto tr184;
	goto st0;
st137:
	if ( ++p == pe )
		goto _test_eof137;
case 137:
	switch( (*p) ) {
		case 32: goto tr188;
		case 101: goto st138;
	}
	goto st0;
st138:
	if ( ++p == pe )
		goto _test_eof138;
case 138:
	if ( (*p) == 99 )
		goto st139;
	goto st0;
st139:
	if ( ++p == pe )
		goto _test_eof139;
case 139:
	if ( (*p) == 32 )
		goto tr188;
	goto st0;
tr175:
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 7 "lex/ragel_units.rl"
	{ ragel_num.vdouble = ragel_num.vint; }
	goto st140;
tr179:
#line 58 "lex/ragel_numeric.rl"
	{
        const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
        ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
        ragel_type = TYPE_FLOAT;
        ragel_cat  = CAT_NUMBER;
    }
	goto st140;
st140:
	if ( ++p == pe )
		goto _test_eof140;
case 140:
#line 2775 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr191;
		case 101: goto st141;
	}
	goto st0;
st141:
	if ( ++p == pe )
		goto _test_eof141;
case 141:
	if ( (*p) == 99 )
		goto st142;
	goto st0;
st142:
	if ( ++p == pe )
		goto _test_eof142;
case 142:
	if ( (*p) == 32 )
		goto tr191;
	goto st0;
tr7:
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
#line 79 "lex/ragel_units.rl"
	{smpte.np = 0;}
#line 67 "lex/ragel_units.rl"
	{smpte.hour = 0; smpte.np++;}
#line 67 "lex/ragel_units.rl"
	{(smpte.hour *= 10) += ((*p) - '0');}
	goto st143;
st143:
	if ( ++p == pe )
		goto _test_eof143;
case 143:
#line 2809 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 46: goto tr172;
		case 104: goto tr173;
		case 109: goto tr174;
		case 115: goto tr175;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr194;
	goto st0;
tr194:
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
#line 67 "lex/ragel_units.rl"
	{(smpte.hour *= 10) += ((*p) - '0');}
	goto st144;
st144:
	if ( ++p == pe )
		goto _test_eof144;
case 144:
#line 2829 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 46: goto tr172;
		case 58: goto st118;
		case 104: goto tr173;
		case 109: goto tr174;
		case 115: goto tr175;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr8;
	goto st0;
tr8:
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
	goto st145;
st145:
	if ( ++p == pe )
		goto _test_eof145;
case 145:
#line 2848 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 46: goto tr172;
		case 104: goto tr173;
		case 109: goto tr174;
		case 115: goto tr175;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr8;
	goto st0;
st146:
	if ( ++p == pe )
		goto _test_eof146;
case 146:
	if ( (*p) == 112 )
		goto st147;
	goto st0;
st147:
	if ( ++p == pe )
		goto _test_eof147;
case 147:
	if ( (*p) == 109 )
		goto st148;
	goto st0;
st148:
	if ( ++p == pe )
		goto _test_eof148;
case 148:
	if ( (*p) == 32 )
		goto st149;
	goto st0;
st149:
	if ( ++p == pe )
		goto _test_eof149;
case 149:
	switch( (*p) ) {
		case 32: goto st149;
		case 35: goto st150;
	}
	goto st0;
st150:
	if ( ++p == pe )
		goto _test_eof150;
case 150:
	if ( (*p) == 48 )
		goto tr199;
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr200;
	goto st0;
tr199:
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
	goto st151;
st151:
	if ( ++p == pe )
		goto _test_eof151;
case 151:
#line 2905 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr201;
	goto st0;
tr201:
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 346 "lex/parser_timeline.rl"
	{ fsm.bpm_bar = ragel_num.vint; ragel_num.vint = 0; }
	goto st152;
st152:
	if ( ++p == pe )
		goto _test_eof152;
case 152:
#line 2923 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto st152;
		case 48: goto tr203;
	}
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr204;
	goto st0;
tr203:
#line 9 "lex/ragel_music.rl"
	{ bpm.dur_num = 1; bpm.dur_den = 4; }
#line 6 "lex/ragel_music.rl"
	{ (bpm.ival *= 10) += ((*p) - '0'); }
	goto st289;
st289:
	if ( ++p == pe )
		goto _test_eof289;
case 289:
#line 2941 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 46: goto st153;
		case 95: goto st291;
		case 98: goto st154;
		case 124: goto st156;
	}
	goto st0;
st153:
	if ( ++p == pe )
		goto _test_eof153;
case 153:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr205;
	goto st0;
tr205:
#line 7 "lex/ragel_music.rl"
	{ (bpm.fnum *= 10) += ((*p) - '0'); bpm.fden *= 10; }
	goto st290;
st290:
	if ( ++p == pe )
		goto _test_eof290;
case 290:
#line 2964 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 95: goto st291;
		case 98: goto st154;
		case 124: goto st156;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr205;
	goto st0;
tr371:
#line 24 "lex/ragel_music.rl"
	{bpm.dur_num = 1;}
	goto st291;
st291:
	if ( ++p == pe )
		goto _test_eof291;
case 291:
#line 2981 "lex/parser_timeline.cpp"
	if ( (*p) == 98 )
		goto st154;
	goto st0;
tr372:
#line 24 "lex/ragel_music.rl"
	{bpm.dur_num = 1;}
	goto st154;
st154:
	if ( ++p == pe )
		goto _test_eof154;
case 154:
#line 2993 "lex/parser_timeline.cpp"
	if ( (*p) == 112 )
		goto st155;
	goto st0;
st155:
	if ( ++p == pe )
		goto _test_eof155;
case 155:
	if ( (*p) == 109 )
		goto st292;
	goto st0;
st292:
	if ( ++p == pe )
		goto _test_eof292;
case 292:
	goto st0;
st156:
	if ( ++p == pe )
		goto _test_eof156;
case 156:
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr208;
	goto st0;
tr208:
#line 11 "lex/ragel_music.rl"
	{ bpm.dur_num = 0; }
#line 12 "lex/ragel_music.rl"
	{ (bpm.dur_num *= 10) += ((*p) - '0'); }
#line 13 "lex/ragel_music.rl"
	{ bpm.dur_den = 0;}
#line 14 "lex/ragel_music.rl"
	{ (bpm.dur_den *= 10) += ((*p) - '0'); }
	goto st293;
tr370:
#line 12 "lex/ragel_music.rl"
	{ (bpm.dur_num *= 10) += ((*p) - '0'); }
#line 14 "lex/ragel_music.rl"
	{ (bpm.dur_den *= 10) += ((*p) - '0'); }
	goto st293;
st293:
	if ( ++p == pe )
		goto _test_eof293;
case 293:
#line 3036 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 46: goto tr368;
		case 47: goto st157;
		case 95: goto tr371;
		case 98: goto tr372;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr370;
	goto st0;
tr368:
#line 24 "lex/ragel_music.rl"
	{bpm.dur_num = 1;}
#line 8 "lex/ragel_music.rl"
	{ bpm.dur_num *= 3; bpm.dur_den *= 2; }
	goto st294;
tr373:
#line 8 "lex/ragel_music.rl"
	{ bpm.dur_num *= 3; bpm.dur_den *= 2; }
	goto st294;
st294:
	if ( ++p == pe )
		goto _test_eof294;
case 294:
#line 3060 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 95: goto st291;
		case 98: goto st154;
	}
	goto st0;
st157:
	if ( ++p == pe )
		goto _test_eof157;
case 157:
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr209;
	goto st0;
tr209:
#line 13 "lex/ragel_music.rl"
	{ bpm.dur_den = 0;}
#line 14 "lex/ragel_music.rl"
	{ (bpm.dur_den *= 10) += ((*p) - '0'); }
	goto st295;
tr374:
#line 14 "lex/ragel_music.rl"
	{ (bpm.dur_den *= 10) += ((*p) - '0'); }
	goto st295;
st295:
	if ( ++p == pe )
		goto _test_eof295;
case 295:
#line 3087 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 46: goto tr373;
		case 95: goto st291;
		case 98: goto st154;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr374;
	goto st0;
tr204:
#line 9 "lex/ragel_music.rl"
	{ bpm.dur_num = 1; bpm.dur_den = 4; }
#line 6 "lex/ragel_music.rl"
	{ (bpm.ival *= 10) += ((*p) - '0'); }
	goto st296;
tr375:
#line 6 "lex/ragel_music.rl"
	{ (bpm.ival *= 10) += ((*p) - '0'); }
	goto st296;
st296:
	if ( ++p == pe )
		goto _test_eof296;
case 296:
#line 3110 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 46: goto st153;
		case 95: goto st291;
		case 98: goto st154;
		case 124: goto st156;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr375;
	goto st0;
tr200:
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
	goto st158;
st158:
	if ( ++p == pe )
		goto _test_eof158;
case 158:
#line 3128 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr201;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr200;
	goto st0;
st159:
	if ( ++p == pe )
		goto _test_eof159;
case 159:
	if ( (*p) == 118 )
		goto st160;
	goto st0;
st160:
	if ( ++p == pe )
		goto _test_eof160;
case 160:
	if ( (*p) == 101 )
		goto st161;
	goto st0;
st161:
	if ( ++p == pe )
		goto _test_eof161;
case 161:
	if ( (*p) == 110 )
		goto st162;
	goto st0;
st162:
	if ( ++p == pe )
		goto _test_eof162;
case 162:
	if ( (*p) == 116 )
		goto st163;
	goto st0;
st163:
	if ( ++p == pe )
		goto _test_eof163;
case 163:
	if ( (*p) == 32 )
		goto st164;
	goto st0;
st164:
	if ( ++p == pe )
		goto _test_eof164;
case 164:
	switch( (*p) ) {
		case 32: goto st164;
		case 95: goto tr215;
	}
	if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr215;
	} else if ( (*p) >= 65 )
		goto tr215;
	goto st0;
tr215:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st297;
st297:
	if ( ++p == pe )
		goto _test_eof297;
case 297:
#line 3191 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr376;
		case 95: goto tr215;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr215;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr215;
	} else
		goto tr215;
	goto st0;
tr378:
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
	goto st165;
tr376:
#line 120 "lex/parser_timeline.rl"
	{ fsm.onEventIdDone(); tl.addEvent(fsm.event_id); }
	goto st165;
tr746:
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 125 "lex/parser_timeline.rl"
	{ tl.addEventAction(fsm.event_id, fsm.act_preset); }
	goto st165;
st165:
	if ( ++p == pe )
		goto _test_eof165;
case 165:
#line 3231 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto st165;
		case 33: goto st166;
	}
	goto st0;
st166:
	if ( ++p == pe )
		goto _test_eof166;
case 166:
	switch( (*p) ) {
		case 109: goto st167;
		case 111: goto st190;
		case 112: goto st193;
		case 115: goto st200;
	}
	goto st0;
st167:
	if ( ++p == pe )
		goto _test_eof167;
case 167:
	if ( (*p) == 105 )
		goto st168;
	goto st0;
st168:
	if ( ++p == pe )
		goto _test_eof168;
case 168:
	if ( (*p) == 100 )
		goto st169;
	goto st0;
st169:
	if ( ++p == pe )
		goto _test_eof169;
case 169:
	if ( (*p) == 105 )
		goto st170;
	goto st0;
st170:
	if ( ++p == pe )
		goto _test_eof170;
case 170:
	if ( (*p) == 58 )
		goto st171;
	goto st0;
st171:
	if ( ++p == pe )
		goto _test_eof171;
case 171:
	switch( (*p) ) {
		case 99: goto st172;
		case 110: goto st180;
		case 112: goto st183;
		case 115: goto st186;
	}
	goto st0;
st172:
	if ( ++p == pe )
		goto _test_eof172;
case 172:
	if ( (*p) == 116 )
		goto st173;
	goto st0;
st173:
	if ( ++p == pe )
		goto _test_eof173;
case 173:
	if ( (*p) == 108 )
		goto st174;
	goto st0;
st174:
	if ( ++p == pe )
		goto _test_eof174;
case 174:
	if ( (*p) == 32 )
		goto st175;
	goto st0;
st175:
	if ( ++p == pe )
		goto _test_eof175;
case 175:
	switch( (*p) ) {
		case 32: goto st175;
		case 48: goto tr233;
	}
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr234;
	goto st0;
tr233:
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
	goto st176;
st176:
	if ( ++p == pe )
		goto _test_eof176;
case 176:
#line 3327 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr235;
	goto st0;
tr235:
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
	goto st177;
st177:
	if ( ++p == pe )
		goto _test_eof177;
case 177:
#line 3343 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto st177;
		case 48: goto tr237;
	}
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr238;
	goto st0;
tr237:
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
	goto st298;
st298:
	if ( ++p == pe )
		goto _test_eof298;
case 298:
#line 3359 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr377;
	goto st0;
tr377:
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
	goto st178;
st178:
	if ( ++p == pe )
		goto _test_eof178;
case 178:
#line 3375 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto st178;
		case 33: goto st166;
		case 48: goto tr240;
	}
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr241;
	goto st0;
tr240:
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
	goto st299;
st299:
	if ( ++p == pe )
		goto _test_eof299;
case 299:
#line 3392 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr378;
	goto st0;
tr241:
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
	goto st300;
st300:
	if ( ++p == pe )
		goto _test_eof300;
case 300:
#line 3404 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr378;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr241;
	goto st0;
tr238:
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
	goto st301;
st301:
	if ( ++p == pe )
		goto _test_eof301;
case 301:
#line 3418 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr377;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr238;
	goto st0;
tr234:
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
	goto st179;
st179:
	if ( ++p == pe )
		goto _test_eof179;
case 179:
#line 3432 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr235;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr234;
	goto st0;
st180:
	if ( ++p == pe )
		goto _test_eof180;
case 180:
	if ( (*p) == 111 )
		goto st181;
	goto st0;
st181:
	if ( ++p == pe )
		goto _test_eof181;
case 181:
	if ( (*p) == 116 )
		goto st182;
	goto st0;
st182:
	if ( ++p == pe )
		goto _test_eof182;
case 182:
	if ( (*p) == 101 )
		goto st174;
	goto st0;
st183:
	if ( ++p == pe )
		goto _test_eof183;
case 183:
	if ( (*p) == 103 )
		goto st184;
	goto st0;
st184:
	if ( ++p == pe )
		goto _test_eof184;
case 184:
	if ( (*p) == 109 )
		goto st185;
	goto st0;
st185:
	if ( ++p == pe )
		goto _test_eof185;
case 185:
	if ( (*p) == 32 )
		goto st177;
	goto st0;
st186:
	if ( ++p == pe )
		goto _test_eof186;
case 186:
	if ( (*p) == 121 )
		goto st187;
	goto st0;
st187:
	if ( ++p == pe )
		goto _test_eof187;
case 187:
	if ( (*p) == 115 )
		goto st188;
	goto st0;
st188:
	if ( ++p == pe )
		goto _test_eof188;
case 188:
	if ( (*p) == 101 )
		goto st189;
	goto st0;
st189:
	if ( ++p == pe )
		goto _test_eof189;
case 189:
	if ( (*p) == 120 )
		goto st302;
	goto st0;
st302:
	if ( ++p == pe )
		goto _test_eof302;
case 302:
	if ( (*p) == 32 )
		goto st303;
	goto st0;
tr381:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 124 "lex/parser_timeline.rl"
	{ fsm.onArgsDone(); }
	goto st303;
tr407:
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 124 "lex/parser_timeline.rl"
	{ fsm.onArgsDone(); }
	goto st303;
tr733:
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 125 "lex/parser_timeline.rl"
	{ tl.addEventAction(fsm.event_id, fsm.act_preset); }
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 124 "lex/parser_timeline.rl"
	{ fsm.onArgsDone(); }
	goto st303;
st303:
	if ( ++p == pe )
		goto _test_eof303;
case 303:
#line 3551 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr381;
		case 33: goto tr382;
	}
	goto tr380;
tr380:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st304;
st304:
	if ( ++p == pe )
		goto _test_eof304;
case 304:
#line 3565 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr381;
	goto tr380;
tr382:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st305;
st305:
	if ( ++p == pe )
		goto _test_eof305;
case 305:
#line 3577 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr381;
		case 109: goto tr383;
		case 111: goto tr384;
		case 112: goto tr385;
		case 115: goto tr386;
	}
	goto tr380;
tr383:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st306;
st306:
	if ( ++p == pe )
		goto _test_eof306;
case 306:
#line 3594 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr381;
		case 105: goto tr387;
	}
	goto tr380;
tr387:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st307;
st307:
	if ( ++p == pe )
		goto _test_eof307;
case 307:
#line 3608 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr381;
		case 100: goto tr388;
	}
	goto tr380;
tr388:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st308;
st308:
	if ( ++p == pe )
		goto _test_eof308;
case 308:
#line 3622 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr381;
		case 105: goto tr389;
	}
	goto tr380;
tr389:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st309;
st309:
	if ( ++p == pe )
		goto _test_eof309;
case 309:
#line 3636 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr381;
		case 58: goto tr390;
	}
	goto tr380;
tr390:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st310;
st310:
	if ( ++p == pe )
		goto _test_eof310;
case 310:
#line 3650 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr381;
		case 99: goto tr391;
		case 110: goto tr392;
		case 112: goto tr393;
	}
	goto tr380;
tr391:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st311;
st311:
	if ( ++p == pe )
		goto _test_eof311;
case 311:
#line 3666 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr381;
		case 116: goto tr394;
	}
	goto tr380;
tr394:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st312;
st312:
	if ( ++p == pe )
		goto _test_eof312;
case 312:
#line 3680 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr381;
		case 108: goto tr395;
	}
	goto tr380;
tr395:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st313;
st313:
	if ( ++p == pe )
		goto _test_eof313;
case 313:
#line 3694 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr396;
	goto tr380;
tr396:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 124 "lex/parser_timeline.rl"
	{ fsm.onArgsDone(); }
	goto st314;
st314:
	if ( ++p == pe )
		goto _test_eof314;
case 314:
#line 3708 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr396;
		case 33: goto tr382;
		case 48: goto tr397;
	}
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr398;
	goto tr380;
tr397:
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st315;
st315:
	if ( ++p == pe )
		goto _test_eof315;
case 315:
#line 3727 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr399;
	goto tr380;
tr400:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 124 "lex/parser_timeline.rl"
	{ fsm.onArgsDone(); }
	goto st316;
tr399:
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 124 "lex/parser_timeline.rl"
	{ fsm.onArgsDone(); }
	goto st316;
st316:
	if ( ++p == pe )
		goto _test_eof316;
case 316:
#line 3753 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr400;
		case 33: goto tr382;
		case 48: goto tr401;
	}
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr402;
	goto tr380;
tr401:
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st317;
st317:
	if ( ++p == pe )
		goto _test_eof317;
case 317:
#line 3772 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr403;
	goto tr380;
tr404:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 124 "lex/parser_timeline.rl"
	{ fsm.onArgsDone(); }
	goto st318;
tr403:
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 124 "lex/parser_timeline.rl"
	{ fsm.onArgsDone(); }
	goto st318;
st318:
	if ( ++p == pe )
		goto _test_eof318;
case 318:
#line 3798 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr404;
		case 33: goto tr382;
		case 48: goto tr405;
	}
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr406;
	goto tr380;
tr405:
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st319;
st319:
	if ( ++p == pe )
		goto _test_eof319;
case 319:
#line 3817 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr407;
	goto tr380;
tr406:
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st320;
st320:
	if ( ++p == pe )
		goto _test_eof320;
case 320:
#line 3831 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr407;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr406;
	goto tr380;
tr402:
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st321;
st321:
	if ( ++p == pe )
		goto _test_eof321;
case 321:
#line 3847 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr403;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr402;
	goto tr380;
tr398:
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st322;
st322:
	if ( ++p == pe )
		goto _test_eof322;
case 322:
#line 3863 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr399;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr398;
	goto tr380;
tr392:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st323;
st323:
	if ( ++p == pe )
		goto _test_eof323;
case 323:
#line 3877 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr381;
		case 111: goto tr408;
	}
	goto tr380;
tr408:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st324;
st324:
	if ( ++p == pe )
		goto _test_eof324;
case 324:
#line 3891 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr381;
		case 116: goto tr409;
	}
	goto tr380;
tr409:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st325;
st325:
	if ( ++p == pe )
		goto _test_eof325;
case 325:
#line 3905 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr381;
		case 101: goto tr395;
	}
	goto tr380;
tr393:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st326;
st326:
	if ( ++p == pe )
		goto _test_eof326;
case 326:
#line 3919 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr381;
		case 103: goto tr410;
	}
	goto tr380;
tr410:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st327;
st327:
	if ( ++p == pe )
		goto _test_eof327;
case 327:
#line 3933 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr381;
		case 109: goto tr411;
	}
	goto tr380;
tr411:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st328;
st328:
	if ( ++p == pe )
		goto _test_eof328;
case 328:
#line 3947 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr400;
	goto tr380;
tr384:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st329;
st329:
	if ( ++p == pe )
		goto _test_eof329;
case 329:
#line 3959 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr381;
		case 115: goto tr412;
		case 117: goto tr413;
	}
	goto tr380;
tr412:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st330;
st330:
	if ( ++p == pe )
		goto _test_eof330;
case 330:
#line 3974 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr381;
		case 99: goto tr414;
	}
	goto tr380;
tr414:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st331;
st331:
	if ( ++p == pe )
		goto _test_eof331;
case 331:
#line 3988 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr415;
	goto tr380;
tr415:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 124 "lex/parser_timeline.rl"
	{ fsm.onArgsDone(); }
	goto st332;
tr442:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 124 "lex/parser_timeline.rl"
	{ fsm.onArgsDone(); }
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
	goto st332;
tr589:
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 125 "lex/parser_timeline.rl"
	{ tl.addEventAction(fsm.event_id, fsm.act_preset); }
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 124 "lex/parser_timeline.rl"
	{ fsm.onArgsDone(); }
	goto st332;
st332:
	if ( ++p == pe )
		goto _test_eof332;
case 332:
#line 4028 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr415;
		case 33: goto tr417;
	}
	goto tr416;
tr416:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st333;
st333:
	if ( ++p == pe )
		goto _test_eof333;
case 333:
#line 4042 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr415;
	goto tr416;
tr417:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st334;
st334:
	if ( ++p == pe )
		goto _test_eof334;
case 334:
#line 4054 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr415;
		case 109: goto tr418;
		case 111: goto tr419;
		case 112: goto tr420;
		case 115: goto tr421;
	}
	goto tr416;
tr418:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st335;
st335:
	if ( ++p == pe )
		goto _test_eof335;
case 335:
#line 4071 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr415;
		case 105: goto tr422;
	}
	goto tr416;
tr422:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st336;
st336:
	if ( ++p == pe )
		goto _test_eof336;
case 336:
#line 4085 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr415;
		case 100: goto tr423;
	}
	goto tr416;
tr423:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st337;
st337:
	if ( ++p == pe )
		goto _test_eof337;
case 337:
#line 4099 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr415;
		case 105: goto tr424;
	}
	goto tr416;
tr424:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st338;
st338:
	if ( ++p == pe )
		goto _test_eof338;
case 338:
#line 4113 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr415;
		case 58: goto tr425;
	}
	goto tr416;
tr425:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st339;
st339:
	if ( ++p == pe )
		goto _test_eof339;
case 339:
#line 4127 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr415;
		case 99: goto tr426;
		case 110: goto tr427;
		case 112: goto tr428;
	}
	goto tr416;
tr426:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st340;
st340:
	if ( ++p == pe )
		goto _test_eof340;
case 340:
#line 4143 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr415;
		case 116: goto tr429;
	}
	goto tr416;
tr429:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st341;
st341:
	if ( ++p == pe )
		goto _test_eof341;
case 341:
#line 4157 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr415;
		case 108: goto tr430;
	}
	goto tr416;
tr430:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st342;
st342:
	if ( ++p == pe )
		goto _test_eof342;
case 342:
#line 4171 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr431;
	goto tr416;
tr431:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 124 "lex/parser_timeline.rl"
	{ fsm.onArgsDone(); }
	goto st343;
st343:
	if ( ++p == pe )
		goto _test_eof343;
case 343:
#line 4185 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr431;
		case 33: goto tr417;
		case 48: goto tr432;
	}
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr433;
	goto tr416;
tr432:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
	goto st344;
st344:
	if ( ++p == pe )
		goto _test_eof344;
case 344:
#line 4204 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr434;
	goto tr416;
tr435:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 124 "lex/parser_timeline.rl"
	{ fsm.onArgsDone(); }
	goto st345;
tr434:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 124 "lex/parser_timeline.rl"
	{ fsm.onArgsDone(); }
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
	goto st345;
st345:
	if ( ++p == pe )
		goto _test_eof345;
case 345:
#line 4230 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr435;
		case 33: goto tr417;
		case 48: goto tr436;
	}
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr437;
	goto tr416;
tr436:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
	goto st346;
st346:
	if ( ++p == pe )
		goto _test_eof346;
case 346:
#line 4249 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr438;
	goto tr416;
tr439:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 124 "lex/parser_timeline.rl"
	{ fsm.onArgsDone(); }
	goto st347;
tr438:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 124 "lex/parser_timeline.rl"
	{ fsm.onArgsDone(); }
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
	goto st347;
st347:
	if ( ++p == pe )
		goto _test_eof347;
case 347:
#line 4275 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr439;
		case 33: goto tr417;
		case 48: goto tr440;
	}
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr441;
	goto tr416;
tr440:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
	goto st348;
st348:
	if ( ++p == pe )
		goto _test_eof348;
case 348:
#line 4294 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr442;
	goto tr416;
tr441:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
	goto st349;
st349:
	if ( ++p == pe )
		goto _test_eof349;
case 349:
#line 4308 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr442;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr441;
	goto tr416;
tr437:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
	goto st350;
st350:
	if ( ++p == pe )
		goto _test_eof350;
case 350:
#line 4324 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr438;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr437;
	goto tr416;
tr433:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
	goto st351;
st351:
	if ( ++p == pe )
		goto _test_eof351;
case 351:
#line 4340 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr434;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr433;
	goto tr416;
tr427:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st352;
st352:
	if ( ++p == pe )
		goto _test_eof352;
case 352:
#line 4354 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr415;
		case 111: goto tr443;
	}
	goto tr416;
tr443:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st353;
st353:
	if ( ++p == pe )
		goto _test_eof353;
case 353:
#line 4368 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr415;
		case 116: goto tr444;
	}
	goto tr416;
tr444:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st354;
st354:
	if ( ++p == pe )
		goto _test_eof354;
case 354:
#line 4382 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr415;
		case 101: goto tr430;
	}
	goto tr416;
tr428:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st355;
st355:
	if ( ++p == pe )
		goto _test_eof355;
case 355:
#line 4396 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr415;
		case 103: goto tr445;
	}
	goto tr416;
tr445:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st356;
st356:
	if ( ++p == pe )
		goto _test_eof356;
case 356:
#line 4410 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr415;
		case 109: goto tr446;
	}
	goto tr416;
tr446:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st357;
st357:
	if ( ++p == pe )
		goto _test_eof357;
case 357:
#line 4424 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr435;
	goto tr416;
tr419:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st358;
st358:
	if ( ++p == pe )
		goto _test_eof358;
case 358:
#line 4436 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr415;
		case 117: goto tr447;
	}
	goto tr416;
tr447:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st359;
st359:
	if ( ++p == pe )
		goto _test_eof359;
case 359:
#line 4450 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr415;
		case 116: goto tr448;
	}
	goto tr416;
tr448:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st360;
st360:
	if ( ++p == pe )
		goto _test_eof360;
case 360:
#line 4464 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr449;
	goto tr416;
tr449:
#line 122 "lex/parser_timeline.rl"
	{ fsm.act_out.args = fsm.args;  tl.addEventAction(fsm.event_id, fsm.act_out); }
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 124 "lex/parser_timeline.rl"
	{ fsm.onArgsDone(); }
	goto st361;
tr451:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 124 "lex/parser_timeline.rl"
	{ fsm.onArgsDone(); }
#line 122 "lex/parser_timeline.rl"
	{ fsm.act_out.args = fsm.args;  tl.addEventAction(fsm.event_id, fsm.act_out); }
	goto st361;
tr476:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 124 "lex/parser_timeline.rl"
	{ fsm.onArgsDone(); }
#line 122 "lex/parser_timeline.rl"
	{ fsm.act_out.args = fsm.args;  tl.addEventAction(fsm.event_id, fsm.act_out); }
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
	goto st361;
tr489:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 124 "lex/parser_timeline.rl"
	{ fsm.onArgsDone(); }
#line 122 "lex/parser_timeline.rl"
	{ fsm.act_out.args = fsm.args;  tl.addEventAction(fsm.event_id, fsm.act_out); }
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 125 "lex/parser_timeline.rl"
	{ tl.addEventAction(fsm.event_id, fsm.act_preset); }
	goto st361;
tr745:
#line 122 "lex/parser_timeline.rl"
	{ fsm.act_out.args = fsm.args;  tl.addEventAction(fsm.event_id, fsm.act_out); }
	goto st361;
st361:
	if ( ++p == pe )
		goto _test_eof361;
case 361:
#line 4522 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr451;
		case 33: goto tr452;
	}
	goto tr450;
tr450:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st362;
st362:
	if ( ++p == pe )
		goto _test_eof362;
case 362:
#line 4536 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr451;
	goto tr450;
tr452:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st363;
st363:
	if ( ++p == pe )
		goto _test_eof363;
case 363:
#line 4548 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr451;
		case 109: goto tr453;
		case 112: goto tr454;
		case 115: goto tr455;
	}
	goto tr450;
tr453:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st364;
st364:
	if ( ++p == pe )
		goto _test_eof364;
case 364:
#line 4564 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr451;
		case 105: goto tr456;
	}
	goto tr450;
tr456:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st365;
st365:
	if ( ++p == pe )
		goto _test_eof365;
case 365:
#line 4578 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr451;
		case 100: goto tr457;
	}
	goto tr450;
tr457:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st366;
st366:
	if ( ++p == pe )
		goto _test_eof366;
case 366:
#line 4592 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr451;
		case 105: goto tr458;
	}
	goto tr450;
tr458:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st367;
st367:
	if ( ++p == pe )
		goto _test_eof367;
case 367:
#line 4606 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr451;
		case 58: goto tr459;
	}
	goto tr450;
tr459:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st368;
st368:
	if ( ++p == pe )
		goto _test_eof368;
case 368:
#line 4620 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr451;
		case 99: goto tr460;
		case 110: goto tr461;
		case 112: goto tr462;
	}
	goto tr450;
tr460:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st369;
st369:
	if ( ++p == pe )
		goto _test_eof369;
case 369:
#line 4636 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr451;
		case 116: goto tr463;
	}
	goto tr450;
tr463:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st370;
st370:
	if ( ++p == pe )
		goto _test_eof370;
case 370:
#line 4650 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr451;
		case 108: goto tr464;
	}
	goto tr450;
tr464:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st371;
st371:
	if ( ++p == pe )
		goto _test_eof371;
case 371:
#line 4664 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr465;
	goto tr450;
tr465:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 124 "lex/parser_timeline.rl"
	{ fsm.onArgsDone(); }
#line 122 "lex/parser_timeline.rl"
	{ fsm.act_out.args = fsm.args;  tl.addEventAction(fsm.event_id, fsm.act_out); }
	goto st372;
st372:
	if ( ++p == pe )
		goto _test_eof372;
case 372:
#line 4680 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr465;
		case 33: goto tr452;
		case 48: goto tr466;
	}
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr467;
	goto tr450;
tr466:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
	goto st373;
st373:
	if ( ++p == pe )
		goto _test_eof373;
case 373:
#line 4699 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr468;
	goto tr450;
tr469:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 124 "lex/parser_timeline.rl"
	{ fsm.onArgsDone(); }
#line 122 "lex/parser_timeline.rl"
	{ fsm.act_out.args = fsm.args;  tl.addEventAction(fsm.event_id, fsm.act_out); }
	goto st374;
tr468:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 124 "lex/parser_timeline.rl"
	{ fsm.onArgsDone(); }
#line 122 "lex/parser_timeline.rl"
	{ fsm.act_out.args = fsm.args;  tl.addEventAction(fsm.event_id, fsm.act_out); }
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
	goto st374;
st374:
	if ( ++p == pe )
		goto _test_eof374;
case 374:
#line 4729 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr469;
		case 33: goto tr452;
		case 48: goto tr470;
	}
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr471;
	goto tr450;
tr470:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
	goto st375;
st375:
	if ( ++p == pe )
		goto _test_eof375;
case 375:
#line 4748 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr472;
	goto tr450;
tr473:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 124 "lex/parser_timeline.rl"
	{ fsm.onArgsDone(); }
#line 122 "lex/parser_timeline.rl"
	{ fsm.act_out.args = fsm.args;  tl.addEventAction(fsm.event_id, fsm.act_out); }
	goto st376;
tr472:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 124 "lex/parser_timeline.rl"
	{ fsm.onArgsDone(); }
#line 122 "lex/parser_timeline.rl"
	{ fsm.act_out.args = fsm.args;  tl.addEventAction(fsm.event_id, fsm.act_out); }
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
	goto st376;
st376:
	if ( ++p == pe )
		goto _test_eof376;
case 376:
#line 4778 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr473;
		case 33: goto tr452;
		case 48: goto tr474;
	}
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr475;
	goto tr450;
tr474:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
	goto st377;
st377:
	if ( ++p == pe )
		goto _test_eof377;
case 377:
#line 4797 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr476;
	goto tr450;
tr475:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
	goto st378;
st378:
	if ( ++p == pe )
		goto _test_eof378;
case 378:
#line 4811 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr476;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr475;
	goto tr450;
tr471:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
	goto st379;
st379:
	if ( ++p == pe )
		goto _test_eof379;
case 379:
#line 4827 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr472;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr471;
	goto tr450;
tr467:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
	goto st380;
st380:
	if ( ++p == pe )
		goto _test_eof380;
case 380:
#line 4843 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr468;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr467;
	goto tr450;
tr461:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st381;
st381:
	if ( ++p == pe )
		goto _test_eof381;
case 381:
#line 4857 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr451;
		case 111: goto tr477;
	}
	goto tr450;
tr477:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st382;
st382:
	if ( ++p == pe )
		goto _test_eof382;
case 382:
#line 4871 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr451;
		case 116: goto tr478;
	}
	goto tr450;
tr478:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st383;
st383:
	if ( ++p == pe )
		goto _test_eof383;
case 383:
#line 4885 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr451;
		case 101: goto tr464;
	}
	goto tr450;
tr462:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st384;
st384:
	if ( ++p == pe )
		goto _test_eof384;
case 384:
#line 4899 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr451;
		case 103: goto tr479;
	}
	goto tr450;
tr479:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st385;
st385:
	if ( ++p == pe )
		goto _test_eof385;
case 385:
#line 4913 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr451;
		case 109: goto tr480;
	}
	goto tr450;
tr480:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st386;
st386:
	if ( ++p == pe )
		goto _test_eof386;
case 386:
#line 4927 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr469;
	goto tr450;
tr454:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st387;
st387:
	if ( ++p == pe )
		goto _test_eof387;
case 387:
#line 4939 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr451;
		case 114: goto tr481;
	}
	goto tr450;
tr481:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st388;
st388:
	if ( ++p == pe )
		goto _test_eof388;
case 388:
#line 4953 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr451;
		case 101: goto tr482;
	}
	goto tr450;
tr482:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st389;
st389:
	if ( ++p == pe )
		goto _test_eof389;
case 389:
#line 4967 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr451;
		case 115: goto tr483;
	}
	goto tr450;
tr483:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st390;
st390:
	if ( ++p == pe )
		goto _test_eof390;
case 390:
#line 4981 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr451;
		case 101: goto tr484;
	}
	goto tr450;
tr484:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st391;
st391:
	if ( ++p == pe )
		goto _test_eof391;
case 391:
#line 4995 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr451;
		case 116: goto tr485;
	}
	goto tr450;
tr485:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st392;
st392:
	if ( ++p == pe )
		goto _test_eof392;
case 392:
#line 5009 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr486;
	goto tr450;
tr486:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 124 "lex/parser_timeline.rl"
	{ fsm.onArgsDone(); }
#line 122 "lex/parser_timeline.rl"
	{ fsm.act_out.args = fsm.args;  tl.addEventAction(fsm.event_id, fsm.act_out); }
	goto st393;
st393:
	if ( ++p == pe )
		goto _test_eof393;
case 393:
#line 5025 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr486;
		case 33: goto tr452;
		case 48: goto tr487;
	}
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr488;
	goto tr450;
tr487:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 266 "lex/parser_timeline.rl"
	{ragel_num.vint = 0;}
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
#line 266 "lex/parser_timeline.rl"
	{ fsm.act_preset.idx = ragel_num.vint; }
	goto st394;
st394:
	if ( ++p == pe )
		goto _test_eof394;
case 394:
#line 5048 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr489;
	goto tr450;
tr488:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 266 "lex/parser_timeline.rl"
	{ragel_num.vint = 0;}
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
#line 266 "lex/parser_timeline.rl"
	{ fsm.act_preset.idx = ragel_num.vint; }
	goto st395;
tr490:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
#line 266 "lex/parser_timeline.rl"
	{ fsm.act_preset.idx = ragel_num.vint; }
	goto st395;
st395:
	if ( ++p == pe )
		goto _test_eof395;
case 395:
#line 5074 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr489;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr490;
	goto tr450;
tr455:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st396;
st396:
	if ( ++p == pe )
		goto _test_eof396;
case 396:
#line 5088 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr451;
		case 101: goto tr491;
	}
	goto tr450;
tr491:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st397;
st397:
	if ( ++p == pe )
		goto _test_eof397;
case 397:
#line 5102 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr451;
		case 110: goto tr492;
	}
	goto tr450;
tr492:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st398;
st398:
	if ( ++p == pe )
		goto _test_eof398;
case 398:
#line 5116 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr451;
		case 100: goto tr493;
	}
	goto tr450;
tr493:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st399;
st399:
	if ( ++p == pe )
		goto _test_eof399;
case 399:
#line 5130 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr494;
	goto tr450;
tr494:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 124 "lex/parser_timeline.rl"
	{ fsm.onArgsDone(); }
#line 122 "lex/parser_timeline.rl"
	{ fsm.act_out.args = fsm.args;  tl.addEventAction(fsm.event_id, fsm.act_out); }
	goto st400;
st400:
	if ( ++p == pe )
		goto _test_eof400;
case 400:
#line 5146 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr494;
		case 33: goto tr496;
		case 43: goto tr497;
		case 45: goto tr497;
		case 48: goto tr498;
	}
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr499;
	goto tr495;
tr495:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st401;
st401:
	if ( ++p == pe )
		goto _test_eof401;
case 401:
#line 5165 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr500;
	goto tr495;
tr500:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 124 "lex/parser_timeline.rl"
	{ fsm.onArgsDone(); }
#line 122 "lex/parser_timeline.rl"
	{ fsm.act_out.args = fsm.args;  tl.addEventAction(fsm.event_id, fsm.act_out); }
#line 123 "lex/parser_timeline.rl"
	{ fsm.onSendTarget(); }
#line 121 "lex/parser_timeline.rl"
	{ fsm.act_send.args = fsm.args; tl.addEventAction(fsm.event_id, fsm.act_send); }
	goto st402;
tr502:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 124 "lex/parser_timeline.rl"
	{ fsm.onArgsDone(); }
#line 122 "lex/parser_timeline.rl"
	{ fsm.act_out.args = fsm.args;  tl.addEventAction(fsm.event_id, fsm.act_out); }
#line 121 "lex/parser_timeline.rl"
	{ fsm.act_send.args = fsm.args; tl.addEventAction(fsm.event_id, fsm.act_send); }
	goto st402;
tr527:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 124 "lex/parser_timeline.rl"
	{ fsm.onArgsDone(); }
#line 122 "lex/parser_timeline.rl"
	{ fsm.act_out.args = fsm.args;  tl.addEventAction(fsm.event_id, fsm.act_out); }
#line 121 "lex/parser_timeline.rl"
	{ fsm.act_send.args = fsm.args; tl.addEventAction(fsm.event_id, fsm.act_send); }
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
	goto st402;
tr540:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 124 "lex/parser_timeline.rl"
	{ fsm.onArgsDone(); }
#line 122 "lex/parser_timeline.rl"
	{ fsm.act_out.args = fsm.args;  tl.addEventAction(fsm.event_id, fsm.act_out); }
#line 121 "lex/parser_timeline.rl"
	{ fsm.act_send.args = fsm.args; tl.addEventAction(fsm.event_id, fsm.act_send); }
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 125 "lex/parser_timeline.rl"
	{ tl.addEventAction(fsm.event_id, fsm.act_preset); }
	goto st402;
tr635:
#line 122 "lex/parser_timeline.rl"
	{ fsm.act_out.args = fsm.args;  tl.addEventAction(fsm.event_id, fsm.act_out); }
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 124 "lex/parser_timeline.rl"
	{ fsm.onArgsDone(); }
#line 121 "lex/parser_timeline.rl"
	{ fsm.act_send.args = fsm.args; tl.addEventAction(fsm.event_id, fsm.act_send); }
	goto st402;
tr677:
#line 122 "lex/parser_timeline.rl"
	{ fsm.act_out.args = fsm.args;  tl.addEventAction(fsm.event_id, fsm.act_out); }
#line 123 "lex/parser_timeline.rl"
	{ fsm.onSendTarget(); }
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 124 "lex/parser_timeline.rl"
	{ fsm.onArgsDone(); }
#line 121 "lex/parser_timeline.rl"
	{ fsm.act_send.args = fsm.args; tl.addEventAction(fsm.event_id, fsm.act_send); }
	goto st402;
tr711:
#line 122 "lex/parser_timeline.rl"
	{ fsm.act_out.args = fsm.args;  tl.addEventAction(fsm.event_id, fsm.act_out); }
#line 123 "lex/parser_timeline.rl"
	{ fsm.onSendTarget(); }
#line 121 "lex/parser_timeline.rl"
	{ fsm.act_send.args = fsm.args; tl.addEventAction(fsm.event_id, fsm.act_send); }
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 124 "lex/parser_timeline.rl"
	{ fsm.onArgsDone(); }
	goto st402;
st402:
	if ( ++p == pe )
		goto _test_eof402;
case 402:
#line 5263 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr502;
		case 33: goto tr503;
	}
	goto tr501;
tr501:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st403;
st403:
	if ( ++p == pe )
		goto _test_eof403;
case 403:
#line 5277 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr502;
	goto tr501;
tr503:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st404;
st404:
	if ( ++p == pe )
		goto _test_eof404;
case 404:
#line 5289 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr502;
		case 109: goto tr504;
		case 112: goto tr505;
		case 115: goto tr506;
	}
	goto tr501;
tr504:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st405;
st405:
	if ( ++p == pe )
		goto _test_eof405;
case 405:
#line 5305 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr502;
		case 105: goto tr507;
	}
	goto tr501;
tr507:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st406;
st406:
	if ( ++p == pe )
		goto _test_eof406;
case 406:
#line 5319 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr502;
		case 100: goto tr508;
	}
	goto tr501;
tr508:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st407;
st407:
	if ( ++p == pe )
		goto _test_eof407;
case 407:
#line 5333 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr502;
		case 105: goto tr509;
	}
	goto tr501;
tr509:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st408;
st408:
	if ( ++p == pe )
		goto _test_eof408;
case 408:
#line 5347 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr502;
		case 58: goto tr510;
	}
	goto tr501;
tr510:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st409;
st409:
	if ( ++p == pe )
		goto _test_eof409;
case 409:
#line 5361 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr502;
		case 99: goto tr511;
		case 110: goto tr512;
		case 112: goto tr513;
	}
	goto tr501;
tr511:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st410;
st410:
	if ( ++p == pe )
		goto _test_eof410;
case 410:
#line 5377 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr502;
		case 116: goto tr514;
	}
	goto tr501;
tr514:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st411;
st411:
	if ( ++p == pe )
		goto _test_eof411;
case 411:
#line 5391 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr502;
		case 108: goto tr515;
	}
	goto tr501;
tr515:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st412;
st412:
	if ( ++p == pe )
		goto _test_eof412;
case 412:
#line 5405 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr516;
	goto tr501;
tr561:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 124 "lex/parser_timeline.rl"
	{ fsm.onArgsDone(); }
#line 122 "lex/parser_timeline.rl"
	{ fsm.act_out.args = fsm.args;  tl.addEventAction(fsm.event_id, fsm.act_out); }
#line 123 "lex/parser_timeline.rl"
	{ fsm.onSendTarget(); }
#line 121 "lex/parser_timeline.rl"
	{ fsm.act_send.args = fsm.args; tl.addEventAction(fsm.event_id, fsm.act_send); }
	goto st413;
tr516:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 124 "lex/parser_timeline.rl"
	{ fsm.onArgsDone(); }
#line 122 "lex/parser_timeline.rl"
	{ fsm.act_out.args = fsm.args;  tl.addEventAction(fsm.event_id, fsm.act_out); }
#line 121 "lex/parser_timeline.rl"
	{ fsm.act_send.args = fsm.args; tl.addEventAction(fsm.event_id, fsm.act_send); }
	goto st413;
st413:
	if ( ++p == pe )
		goto _test_eof413;
case 413:
#line 5435 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr516;
		case 33: goto tr503;
		case 48: goto tr517;
	}
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr518;
	goto tr501;
tr517:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
	goto st414;
st414:
	if ( ++p == pe )
		goto _test_eof414;
case 414:
#line 5454 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr519;
	goto tr501;
tr566:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 124 "lex/parser_timeline.rl"
	{ fsm.onArgsDone(); }
#line 122 "lex/parser_timeline.rl"
	{ fsm.act_out.args = fsm.args;  tl.addEventAction(fsm.event_id, fsm.act_out); }
#line 123 "lex/parser_timeline.rl"
	{ fsm.onSendTarget(); }
#line 121 "lex/parser_timeline.rl"
	{ fsm.act_send.args = fsm.args; tl.addEventAction(fsm.event_id, fsm.act_send); }
	goto st415;
tr520:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 124 "lex/parser_timeline.rl"
	{ fsm.onArgsDone(); }
#line 122 "lex/parser_timeline.rl"
	{ fsm.act_out.args = fsm.args;  tl.addEventAction(fsm.event_id, fsm.act_out); }
#line 121 "lex/parser_timeline.rl"
	{ fsm.act_send.args = fsm.args; tl.addEventAction(fsm.event_id, fsm.act_send); }
	goto st415;
tr519:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 124 "lex/parser_timeline.rl"
	{ fsm.onArgsDone(); }
#line 122 "lex/parser_timeline.rl"
	{ fsm.act_out.args = fsm.args;  tl.addEventAction(fsm.event_id, fsm.act_out); }
#line 121 "lex/parser_timeline.rl"
	{ fsm.act_send.args = fsm.args; tl.addEventAction(fsm.event_id, fsm.act_send); }
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
	goto st415;
st415:
	if ( ++p == pe )
		goto _test_eof415;
case 415:
#line 5500 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr520;
		case 33: goto tr503;
		case 48: goto tr521;
	}
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr522;
	goto tr501;
tr521:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
	goto st416;
st416:
	if ( ++p == pe )
		goto _test_eof416;
case 416:
#line 5519 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr523;
	goto tr501;
tr524:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 124 "lex/parser_timeline.rl"
	{ fsm.onArgsDone(); }
#line 122 "lex/parser_timeline.rl"
	{ fsm.act_out.args = fsm.args;  tl.addEventAction(fsm.event_id, fsm.act_out); }
#line 121 "lex/parser_timeline.rl"
	{ fsm.act_send.args = fsm.args; tl.addEventAction(fsm.event_id, fsm.act_send); }
	goto st417;
tr523:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 124 "lex/parser_timeline.rl"
	{ fsm.onArgsDone(); }
#line 122 "lex/parser_timeline.rl"
	{ fsm.act_out.args = fsm.args;  tl.addEventAction(fsm.event_id, fsm.act_out); }
#line 121 "lex/parser_timeline.rl"
	{ fsm.act_send.args = fsm.args; tl.addEventAction(fsm.event_id, fsm.act_send); }
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
	goto st417;
st417:
	if ( ++p == pe )
		goto _test_eof417;
case 417:
#line 5553 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr524;
		case 33: goto tr503;
		case 48: goto tr525;
	}
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr526;
	goto tr501;
tr525:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
	goto st418;
st418:
	if ( ++p == pe )
		goto _test_eof418;
case 418:
#line 5572 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr527;
	goto tr501;
tr526:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
	goto st419;
st419:
	if ( ++p == pe )
		goto _test_eof419;
case 419:
#line 5586 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr527;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr526;
	goto tr501;
tr522:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
	goto st420;
st420:
	if ( ++p == pe )
		goto _test_eof420;
case 420:
#line 5602 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr523;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr522;
	goto tr501;
tr518:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
	goto st421;
st421:
	if ( ++p == pe )
		goto _test_eof421;
case 421:
#line 5618 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr519;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr518;
	goto tr501;
tr512:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st422;
st422:
	if ( ++p == pe )
		goto _test_eof422;
case 422:
#line 5632 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr502;
		case 111: goto tr528;
	}
	goto tr501;
tr528:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st423;
st423:
	if ( ++p == pe )
		goto _test_eof423;
case 423:
#line 5646 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr502;
		case 116: goto tr529;
	}
	goto tr501;
tr529:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st424;
st424:
	if ( ++p == pe )
		goto _test_eof424;
case 424:
#line 5660 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr502;
		case 101: goto tr515;
	}
	goto tr501;
tr513:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st425;
st425:
	if ( ++p == pe )
		goto _test_eof425;
case 425:
#line 5674 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr502;
		case 103: goto tr530;
	}
	goto tr501;
tr530:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st426;
st426:
	if ( ++p == pe )
		goto _test_eof426;
case 426:
#line 5688 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr502;
		case 109: goto tr531;
	}
	goto tr501;
tr531:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st427;
st427:
	if ( ++p == pe )
		goto _test_eof427;
case 427:
#line 5702 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr520;
	goto tr501;
tr505:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st428;
st428:
	if ( ++p == pe )
		goto _test_eof428;
case 428:
#line 5714 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr502;
		case 114: goto tr532;
	}
	goto tr501;
tr532:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st429;
st429:
	if ( ++p == pe )
		goto _test_eof429;
case 429:
#line 5728 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr502;
		case 101: goto tr533;
	}
	goto tr501;
tr533:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st430;
st430:
	if ( ++p == pe )
		goto _test_eof430;
case 430:
#line 5742 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr502;
		case 115: goto tr534;
	}
	goto tr501;
tr534:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st431;
st431:
	if ( ++p == pe )
		goto _test_eof431;
case 431:
#line 5756 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr502;
		case 101: goto tr535;
	}
	goto tr501;
tr535:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st432;
st432:
	if ( ++p == pe )
		goto _test_eof432;
case 432:
#line 5770 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr502;
		case 116: goto tr536;
	}
	goto tr501;
tr536:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st433;
st433:
	if ( ++p == pe )
		goto _test_eof433;
case 433:
#line 5784 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr537;
	goto tr501;
tr572:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 124 "lex/parser_timeline.rl"
	{ fsm.onArgsDone(); }
#line 122 "lex/parser_timeline.rl"
	{ fsm.act_out.args = fsm.args;  tl.addEventAction(fsm.event_id, fsm.act_out); }
#line 123 "lex/parser_timeline.rl"
	{ fsm.onSendTarget(); }
#line 121 "lex/parser_timeline.rl"
	{ fsm.act_send.args = fsm.args; tl.addEventAction(fsm.event_id, fsm.act_send); }
	goto st434;
tr537:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 124 "lex/parser_timeline.rl"
	{ fsm.onArgsDone(); }
#line 122 "lex/parser_timeline.rl"
	{ fsm.act_out.args = fsm.args;  tl.addEventAction(fsm.event_id, fsm.act_out); }
#line 121 "lex/parser_timeline.rl"
	{ fsm.act_send.args = fsm.args; tl.addEventAction(fsm.event_id, fsm.act_send); }
	goto st434;
st434:
	if ( ++p == pe )
		goto _test_eof434;
case 434:
#line 5814 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr537;
		case 33: goto tr503;
		case 48: goto tr538;
	}
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr539;
	goto tr501;
tr538:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 266 "lex/parser_timeline.rl"
	{ragel_num.vint = 0;}
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
#line 266 "lex/parser_timeline.rl"
	{ fsm.act_preset.idx = ragel_num.vint; }
	goto st435;
st435:
	if ( ++p == pe )
		goto _test_eof435;
case 435:
#line 5837 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr540;
	goto tr501;
tr539:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 266 "lex/parser_timeline.rl"
	{ragel_num.vint = 0;}
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
#line 266 "lex/parser_timeline.rl"
	{ fsm.act_preset.idx = ragel_num.vint; }
	goto st436;
tr541:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
#line 266 "lex/parser_timeline.rl"
	{ fsm.act_preset.idx = ragel_num.vint; }
	goto st436;
st436:
	if ( ++p == pe )
		goto _test_eof436;
case 436:
#line 5863 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr540;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr541;
	goto tr501;
tr506:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st437;
st437:
	if ( ++p == pe )
		goto _test_eof437;
case 437:
#line 5877 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr502;
		case 101: goto tr542;
	}
	goto tr501;
tr542:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st438;
st438:
	if ( ++p == pe )
		goto _test_eof438;
case 438:
#line 5891 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr502;
		case 110: goto tr543;
	}
	goto tr501;
tr543:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st439;
st439:
	if ( ++p == pe )
		goto _test_eof439;
case 439:
#line 5905 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr502;
		case 100: goto tr544;
	}
	goto tr501;
tr544:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st440;
st440:
	if ( ++p == pe )
		goto _test_eof440;
case 440:
#line 5919 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr545;
	goto tr501;
tr576:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 124 "lex/parser_timeline.rl"
	{ fsm.onArgsDone(); }
#line 122 "lex/parser_timeline.rl"
	{ fsm.act_out.args = fsm.args;  tl.addEventAction(fsm.event_id, fsm.act_out); }
#line 123 "lex/parser_timeline.rl"
	{ fsm.onSendTarget(); }
#line 121 "lex/parser_timeline.rl"
	{ fsm.act_send.args = fsm.args; tl.addEventAction(fsm.event_id, fsm.act_send); }
	goto st441;
tr545:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 124 "lex/parser_timeline.rl"
	{ fsm.onArgsDone(); }
#line 122 "lex/parser_timeline.rl"
	{ fsm.act_out.args = fsm.args;  tl.addEventAction(fsm.event_id, fsm.act_out); }
#line 121 "lex/parser_timeline.rl"
	{ fsm.act_send.args = fsm.args; tl.addEventAction(fsm.event_id, fsm.act_send); }
	goto st441;
st441:
	if ( ++p == pe )
		goto _test_eof441;
case 441:
#line 5949 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr545;
		case 33: goto tr496;
		case 43: goto tr546;
		case 45: goto tr546;
		case 48: goto tr547;
	}
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr548;
	goto tr495;
tr496:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st442;
st442:
	if ( ++p == pe )
		goto _test_eof442;
case 442:
#line 5968 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr500;
		case 109: goto tr549;
		case 112: goto tr550;
		case 115: goto tr551;
	}
	goto tr495;
tr549:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st443;
st443:
	if ( ++p == pe )
		goto _test_eof443;
case 443:
#line 5984 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr500;
		case 105: goto tr552;
	}
	goto tr495;
tr552:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st444;
st444:
	if ( ++p == pe )
		goto _test_eof444;
case 444:
#line 5998 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr500;
		case 100: goto tr553;
	}
	goto tr495;
tr553:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st445;
st445:
	if ( ++p == pe )
		goto _test_eof445;
case 445:
#line 6012 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr500;
		case 105: goto tr554;
	}
	goto tr495;
tr554:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st446;
st446:
	if ( ++p == pe )
		goto _test_eof446;
case 446:
#line 6026 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr500;
		case 58: goto tr555;
	}
	goto tr495;
tr555:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st447;
st447:
	if ( ++p == pe )
		goto _test_eof447;
case 447:
#line 6040 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr500;
		case 99: goto tr556;
		case 110: goto tr557;
		case 112: goto tr558;
	}
	goto tr495;
tr556:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st448;
st448:
	if ( ++p == pe )
		goto _test_eof448;
case 448:
#line 6056 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr500;
		case 116: goto tr559;
	}
	goto tr495;
tr559:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st449;
st449:
	if ( ++p == pe )
		goto _test_eof449;
case 449:
#line 6070 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr500;
		case 108: goto tr560;
	}
	goto tr495;
tr560:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st450;
st450:
	if ( ++p == pe )
		goto _test_eof450;
case 450:
#line 6084 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr561;
	goto tr495;
tr557:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st451;
st451:
	if ( ++p == pe )
		goto _test_eof451;
case 451:
#line 6096 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr500;
		case 111: goto tr562;
	}
	goto tr495;
tr562:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st452;
st452:
	if ( ++p == pe )
		goto _test_eof452;
case 452:
#line 6110 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr500;
		case 116: goto tr563;
	}
	goto tr495;
tr563:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st453;
st453:
	if ( ++p == pe )
		goto _test_eof453;
case 453:
#line 6124 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr500;
		case 101: goto tr560;
	}
	goto tr495;
tr558:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st454;
st454:
	if ( ++p == pe )
		goto _test_eof454;
case 454:
#line 6138 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr500;
		case 103: goto tr564;
	}
	goto tr495;
tr564:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st455;
st455:
	if ( ++p == pe )
		goto _test_eof455;
case 455:
#line 6152 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr500;
		case 109: goto tr565;
	}
	goto tr495;
tr565:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st456;
st456:
	if ( ++p == pe )
		goto _test_eof456;
case 456:
#line 6166 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr566;
	goto tr495;
tr550:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st457;
st457:
	if ( ++p == pe )
		goto _test_eof457;
case 457:
#line 6178 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr500;
		case 114: goto tr567;
	}
	goto tr495;
tr567:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st458;
st458:
	if ( ++p == pe )
		goto _test_eof458;
case 458:
#line 6192 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr500;
		case 101: goto tr568;
	}
	goto tr495;
tr568:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st459;
st459:
	if ( ++p == pe )
		goto _test_eof459;
case 459:
#line 6206 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr500;
		case 115: goto tr569;
	}
	goto tr495;
tr569:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st460;
st460:
	if ( ++p == pe )
		goto _test_eof460;
case 460:
#line 6220 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr500;
		case 101: goto tr570;
	}
	goto tr495;
tr570:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st461;
st461:
	if ( ++p == pe )
		goto _test_eof461;
case 461:
#line 6234 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr500;
		case 116: goto tr571;
	}
	goto tr495;
tr571:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st462;
st462:
	if ( ++p == pe )
		goto _test_eof462;
case 462:
#line 6248 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr572;
	goto tr495;
tr551:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st463;
st463:
	if ( ++p == pe )
		goto _test_eof463;
case 463:
#line 6260 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr500;
		case 101: goto tr573;
	}
	goto tr495;
tr573:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st464;
st464:
	if ( ++p == pe )
		goto _test_eof464;
case 464:
#line 6274 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr500;
		case 110: goto tr574;
	}
	goto tr495;
tr574:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st465;
st465:
	if ( ++p == pe )
		goto _test_eof465;
case 465:
#line 6288 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr500;
		case 100: goto tr575;
	}
	goto tr495;
tr575:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st466;
st466:
	if ( ++p == pe )
		goto _test_eof466;
case 466:
#line 6302 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr576;
	goto tr495;
tr546:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 21 "lex/ragel_numeric.rl"
	{ ragel_num.sign = ((*p)=='-') ? -1 : 1; }
	goto st467;
st467:
	if ( ++p == pe )
		goto _test_eof467;
case 467:
#line 6316 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr500;
		case 48: goto tr547;
	}
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr548;
	goto tr495;
tr547:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
	goto st468;
st468:
	if ( ++p == pe )
		goto _test_eof468;
case 468:
#line 6334 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr502;
		case 46: goto tr577;
	}
	goto tr495;
tr577:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
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
	goto st469;
st469:
	if ( ++p == pe )
		goto _test_eof469;
case 469:
#line 6359 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr500;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr578;
	goto tr495;
tr578:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 54 "lex/ragel_numeric.rl"
	{
        (ragel_num.ratio.num *= 10) += ((*p) - '0');
        ragel_num.ratio.den *= 10;
    }
	goto st470;
st470:
	if ( ++p == pe )
		goto _test_eof470;
case 470:
#line 6378 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr502;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr578;
	goto tr495;
tr548:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
	goto st471;
st471:
	if ( ++p == pe )
		goto _test_eof471;
case 471:
#line 6394 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr502;
		case 46: goto tr577;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr548;
	goto tr495;
tr497:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 21 "lex/ragel_numeric.rl"
	{ ragel_num.sign = ((*p)=='-') ? -1 : 1; }
	goto st472;
st472:
	if ( ++p == pe )
		goto _test_eof472;
case 472:
#line 6412 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr500;
		case 48: goto tr498;
	}
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr499;
	goto tr495;
tr498:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
	goto st473;
st473:
	if ( ++p == pe )
		goto _test_eof473;
case 473:
#line 6430 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr451;
		case 46: goto tr579;
	}
	goto tr495;
tr579:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
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
	goto st474;
st474:
	if ( ++p == pe )
		goto _test_eof474;
case 474:
#line 6455 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr500;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr580;
	goto tr495;
tr580:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 54 "lex/ragel_numeric.rl"
	{
        (ragel_num.ratio.num *= 10) += ((*p) - '0');
        ragel_num.ratio.den *= 10;
    }
	goto st475;
st475:
	if ( ++p == pe )
		goto _test_eof475;
case 475:
#line 6474 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr451;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr580;
	goto tr495;
tr499:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
	goto st476;
st476:
	if ( ++p == pe )
		goto _test_eof476;
case 476:
#line 6490 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr451;
		case 46: goto tr579;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr499;
	goto tr495;
tr420:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st477;
st477:
	if ( ++p == pe )
		goto _test_eof477;
case 477:
#line 6506 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr415;
		case 114: goto tr581;
	}
	goto tr416;
tr581:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st478;
st478:
	if ( ++p == pe )
		goto _test_eof478;
case 478:
#line 6520 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr415;
		case 101: goto tr582;
	}
	goto tr416;
tr582:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st479;
st479:
	if ( ++p == pe )
		goto _test_eof479;
case 479:
#line 6534 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr415;
		case 115: goto tr583;
	}
	goto tr416;
tr583:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st480;
st480:
	if ( ++p == pe )
		goto _test_eof480;
case 480:
#line 6548 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr415;
		case 101: goto tr584;
	}
	goto tr416;
tr584:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st481;
st481:
	if ( ++p == pe )
		goto _test_eof481;
case 481:
#line 6562 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr415;
		case 116: goto tr585;
	}
	goto tr416;
tr585:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st482;
st482:
	if ( ++p == pe )
		goto _test_eof482;
case 482:
#line 6576 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr586;
	goto tr416;
tr586:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 124 "lex/parser_timeline.rl"
	{ fsm.onArgsDone(); }
	goto st483;
st483:
	if ( ++p == pe )
		goto _test_eof483;
case 483:
#line 6590 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr586;
		case 33: goto tr417;
		case 48: goto tr587;
	}
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr588;
	goto tr416;
tr587:
#line 266 "lex/parser_timeline.rl"
	{ragel_num.vint = 0;}
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
#line 266 "lex/parser_timeline.rl"
	{ fsm.act_preset.idx = ragel_num.vint; }
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st484;
st484:
	if ( ++p == pe )
		goto _test_eof484;
case 484:
#line 6613 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr589;
	goto tr416;
tr588:
#line 266 "lex/parser_timeline.rl"
	{ragel_num.vint = 0;}
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
#line 266 "lex/parser_timeline.rl"
	{ fsm.act_preset.idx = ragel_num.vint; }
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st485;
tr590:
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
#line 266 "lex/parser_timeline.rl"
	{ fsm.act_preset.idx = ragel_num.vint; }
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st485;
st485:
	if ( ++p == pe )
		goto _test_eof485;
case 485:
#line 6639 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr589;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr590;
	goto tr416;
tr421:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st486;
st486:
	if ( ++p == pe )
		goto _test_eof486;
case 486:
#line 6653 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr415;
		case 101: goto tr591;
	}
	goto tr416;
tr591:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st487;
st487:
	if ( ++p == pe )
		goto _test_eof487;
case 487:
#line 6667 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr415;
		case 110: goto tr592;
	}
	goto tr416;
tr592:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st488;
st488:
	if ( ++p == pe )
		goto _test_eof488;
case 488:
#line 6681 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr415;
		case 100: goto tr593;
	}
	goto tr416;
tr593:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st489;
st489:
	if ( ++p == pe )
		goto _test_eof489;
case 489:
#line 6695 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr594;
	goto tr416;
tr594:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 124 "lex/parser_timeline.rl"
	{ fsm.onArgsDone(); }
	goto st490;
st490:
	if ( ++p == pe )
		goto _test_eof490;
case 490:
#line 6709 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr594;
		case 33: goto tr596;
		case 43: goto tr597;
		case 45: goto tr597;
		case 48: goto tr598;
	}
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr599;
	goto tr595;
tr595:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st491;
st491:
	if ( ++p == pe )
		goto _test_eof491;
case 491:
#line 6728 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr600;
	goto tr595;
tr600:
#line 123 "lex/parser_timeline.rl"
	{ fsm.onSendTarget(); }
#line 121 "lex/parser_timeline.rl"
	{ fsm.act_send.args = fsm.args; tl.addEventAction(fsm.event_id, fsm.act_send); }
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 124 "lex/parser_timeline.rl"
	{ fsm.onArgsDone(); }
	goto st492;
tr602:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 124 "lex/parser_timeline.rl"
	{ fsm.onArgsDone(); }
#line 121 "lex/parser_timeline.rl"
	{ fsm.act_send.args = fsm.args; tl.addEventAction(fsm.event_id, fsm.act_send); }
	goto st492;
tr628:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 124 "lex/parser_timeline.rl"
	{ fsm.onArgsDone(); }
#line 121 "lex/parser_timeline.rl"
	{ fsm.act_send.args = fsm.args; tl.addEventAction(fsm.event_id, fsm.act_send); }
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
	goto st492;
tr644:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 124 "lex/parser_timeline.rl"
	{ fsm.onArgsDone(); }
#line 121 "lex/parser_timeline.rl"
	{ fsm.act_send.args = fsm.args; tl.addEventAction(fsm.event_id, fsm.act_send); }
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 125 "lex/parser_timeline.rl"
	{ tl.addEventAction(fsm.event_id, fsm.act_preset); }
	goto st492;
tr655:
#line 123 "lex/parser_timeline.rl"
	{ fsm.onSendTarget(); }
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 124 "lex/parser_timeline.rl"
	{ fsm.onArgsDone(); }
#line 121 "lex/parser_timeline.rl"
	{ fsm.act_send.args = fsm.args; tl.addEventAction(fsm.event_id, fsm.act_send); }
	goto st492;
tr748:
#line 123 "lex/parser_timeline.rl"
	{ fsm.onSendTarget(); }
#line 121 "lex/parser_timeline.rl"
	{ fsm.act_send.args = fsm.args; tl.addEventAction(fsm.event_id, fsm.act_send); }
	goto st492;
st492:
	if ( ++p == pe )
		goto _test_eof492;
case 492:
#line 6800 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr602;
		case 33: goto tr603;
	}
	goto tr601;
tr601:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st493;
st493:
	if ( ++p == pe )
		goto _test_eof493;
case 493:
#line 6814 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr602;
	goto tr601;
tr603:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st494;
st494:
	if ( ++p == pe )
		goto _test_eof494;
case 494:
#line 6826 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr602;
		case 109: goto tr604;
		case 111: goto tr605;
		case 112: goto tr606;
		case 115: goto tr607;
	}
	goto tr601;
tr604:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st495;
st495:
	if ( ++p == pe )
		goto _test_eof495;
case 495:
#line 6843 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr602;
		case 105: goto tr608;
	}
	goto tr601;
tr608:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st496;
st496:
	if ( ++p == pe )
		goto _test_eof496;
case 496:
#line 6857 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr602;
		case 100: goto tr609;
	}
	goto tr601;
tr609:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st497;
st497:
	if ( ++p == pe )
		goto _test_eof497;
case 497:
#line 6871 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr602;
		case 105: goto tr610;
	}
	goto tr601;
tr610:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st498;
st498:
	if ( ++p == pe )
		goto _test_eof498;
case 498:
#line 6885 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr602;
		case 58: goto tr611;
	}
	goto tr601;
tr611:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st499;
st499:
	if ( ++p == pe )
		goto _test_eof499;
case 499:
#line 6899 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr602;
		case 99: goto tr612;
		case 110: goto tr613;
		case 112: goto tr614;
	}
	goto tr601;
tr612:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st500;
st500:
	if ( ++p == pe )
		goto _test_eof500;
case 500:
#line 6915 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr602;
		case 116: goto tr615;
	}
	goto tr601;
tr615:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st501;
st501:
	if ( ++p == pe )
		goto _test_eof501;
case 501:
#line 6929 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr602;
		case 108: goto tr616;
	}
	goto tr601;
tr616:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st502;
st502:
	if ( ++p == pe )
		goto _test_eof502;
case 502:
#line 6943 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr617;
	goto tr601;
tr703:
#line 123 "lex/parser_timeline.rl"
	{ fsm.onSendTarget(); }
#line 121 "lex/parser_timeline.rl"
	{ fsm.act_send.args = fsm.args; tl.addEventAction(fsm.event_id, fsm.act_send); }
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 124 "lex/parser_timeline.rl"
	{ fsm.onArgsDone(); }
	goto st503;
tr617:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 124 "lex/parser_timeline.rl"
	{ fsm.onArgsDone(); }
#line 121 "lex/parser_timeline.rl"
	{ fsm.act_send.args = fsm.args; tl.addEventAction(fsm.event_id, fsm.act_send); }
	goto st503;
tr669:
#line 123 "lex/parser_timeline.rl"
	{ fsm.onSendTarget(); }
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 124 "lex/parser_timeline.rl"
	{ fsm.onArgsDone(); }
#line 121 "lex/parser_timeline.rl"
	{ fsm.act_send.args = fsm.args; tl.addEventAction(fsm.event_id, fsm.act_send); }
	goto st503;
st503:
	if ( ++p == pe )
		goto _test_eof503;
case 503:
#line 6979 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr617;
		case 33: goto tr603;
		case 48: goto tr618;
	}
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr619;
	goto tr601;
tr618:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
	goto st504;
st504:
	if ( ++p == pe )
		goto _test_eof504;
case 504:
#line 6998 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr620;
	goto tr601;
tr708:
#line 123 "lex/parser_timeline.rl"
	{ fsm.onSendTarget(); }
#line 121 "lex/parser_timeline.rl"
	{ fsm.act_send.args = fsm.args; tl.addEventAction(fsm.event_id, fsm.act_send); }
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 124 "lex/parser_timeline.rl"
	{ fsm.onArgsDone(); }
	goto st505;
tr621:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 124 "lex/parser_timeline.rl"
	{ fsm.onArgsDone(); }
#line 121 "lex/parser_timeline.rl"
	{ fsm.act_send.args = fsm.args; tl.addEventAction(fsm.event_id, fsm.act_send); }
	goto st505;
tr620:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 124 "lex/parser_timeline.rl"
	{ fsm.onArgsDone(); }
#line 121 "lex/parser_timeline.rl"
	{ fsm.act_send.args = fsm.args; tl.addEventAction(fsm.event_id, fsm.act_send); }
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
	goto st505;
tr674:
#line 123 "lex/parser_timeline.rl"
	{ fsm.onSendTarget(); }
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 124 "lex/parser_timeline.rl"
	{ fsm.onArgsDone(); }
#line 121 "lex/parser_timeline.rl"
	{ fsm.act_send.args = fsm.args; tl.addEventAction(fsm.event_id, fsm.act_send); }
	goto st505;
st505:
	if ( ++p == pe )
		goto _test_eof505;
case 505:
#line 7048 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr621;
		case 33: goto tr603;
		case 48: goto tr622;
	}
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr623;
	goto tr601;
tr622:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
	goto st506;
st506:
	if ( ++p == pe )
		goto _test_eof506;
case 506:
#line 7067 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr624;
	goto tr601;
tr625:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 124 "lex/parser_timeline.rl"
	{ fsm.onArgsDone(); }
#line 121 "lex/parser_timeline.rl"
	{ fsm.act_send.args = fsm.args; tl.addEventAction(fsm.event_id, fsm.act_send); }
	goto st507;
tr624:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 124 "lex/parser_timeline.rl"
	{ fsm.onArgsDone(); }
#line 121 "lex/parser_timeline.rl"
	{ fsm.act_send.args = fsm.args; tl.addEventAction(fsm.event_id, fsm.act_send); }
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
	goto st507;
st507:
	if ( ++p == pe )
		goto _test_eof507;
case 507:
#line 7097 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr625;
		case 33: goto tr603;
		case 48: goto tr626;
	}
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr627;
	goto tr601;
tr626:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
	goto st508;
st508:
	if ( ++p == pe )
		goto _test_eof508;
case 508:
#line 7116 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr628;
	goto tr601;
tr627:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
	goto st509;
st509:
	if ( ++p == pe )
		goto _test_eof509;
case 509:
#line 7130 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr628;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr627;
	goto tr601;
tr623:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
	goto st510;
st510:
	if ( ++p == pe )
		goto _test_eof510;
case 510:
#line 7146 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr624;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr623;
	goto tr601;
tr619:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
	goto st511;
st511:
	if ( ++p == pe )
		goto _test_eof511;
case 511:
#line 7162 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr620;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr619;
	goto tr601;
tr613:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st512;
st512:
	if ( ++p == pe )
		goto _test_eof512;
case 512:
#line 7176 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr602;
		case 111: goto tr629;
	}
	goto tr601;
tr629:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st513;
st513:
	if ( ++p == pe )
		goto _test_eof513;
case 513:
#line 7190 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr602;
		case 116: goto tr630;
	}
	goto tr601;
tr630:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st514;
st514:
	if ( ++p == pe )
		goto _test_eof514;
case 514:
#line 7204 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr602;
		case 101: goto tr616;
	}
	goto tr601;
tr614:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st515;
st515:
	if ( ++p == pe )
		goto _test_eof515;
case 515:
#line 7218 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr602;
		case 103: goto tr631;
	}
	goto tr601;
tr631:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st516;
st516:
	if ( ++p == pe )
		goto _test_eof516;
case 516:
#line 7232 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr602;
		case 109: goto tr632;
	}
	goto tr601;
tr632:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st517;
st517:
	if ( ++p == pe )
		goto _test_eof517;
case 517:
#line 7246 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr621;
	goto tr601;
tr605:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st518;
st518:
	if ( ++p == pe )
		goto _test_eof518;
case 518:
#line 7258 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr602;
		case 117: goto tr633;
	}
	goto tr601;
tr633:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st519;
st519:
	if ( ++p == pe )
		goto _test_eof519;
case 519:
#line 7272 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr602;
		case 116: goto tr634;
	}
	goto tr601;
tr634:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st520;
st520:
	if ( ++p == pe )
		goto _test_eof520;
case 520:
#line 7286 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr635;
	goto tr601;
tr606:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st521;
st521:
	if ( ++p == pe )
		goto _test_eof521;
case 521:
#line 7298 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr602;
		case 114: goto tr636;
	}
	goto tr601;
tr636:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st522;
st522:
	if ( ++p == pe )
		goto _test_eof522;
case 522:
#line 7312 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr602;
		case 101: goto tr637;
	}
	goto tr601;
tr637:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st523;
st523:
	if ( ++p == pe )
		goto _test_eof523;
case 523:
#line 7326 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr602;
		case 115: goto tr638;
	}
	goto tr601;
tr638:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st524;
st524:
	if ( ++p == pe )
		goto _test_eof524;
case 524:
#line 7340 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr602;
		case 101: goto tr639;
	}
	goto tr601;
tr639:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st525;
st525:
	if ( ++p == pe )
		goto _test_eof525;
case 525:
#line 7354 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr602;
		case 116: goto tr640;
	}
	goto tr601;
tr640:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st526;
st526:
	if ( ++p == pe )
		goto _test_eof526;
case 526:
#line 7368 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr641;
	goto tr601;
tr717:
#line 123 "lex/parser_timeline.rl"
	{ fsm.onSendTarget(); }
#line 121 "lex/parser_timeline.rl"
	{ fsm.act_send.args = fsm.args; tl.addEventAction(fsm.event_id, fsm.act_send); }
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 124 "lex/parser_timeline.rl"
	{ fsm.onArgsDone(); }
	goto st527;
tr641:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 124 "lex/parser_timeline.rl"
	{ fsm.onArgsDone(); }
#line 121 "lex/parser_timeline.rl"
	{ fsm.act_send.args = fsm.args; tl.addEventAction(fsm.event_id, fsm.act_send); }
	goto st527;
tr683:
#line 123 "lex/parser_timeline.rl"
	{ fsm.onSendTarget(); }
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 124 "lex/parser_timeline.rl"
	{ fsm.onArgsDone(); }
#line 121 "lex/parser_timeline.rl"
	{ fsm.act_send.args = fsm.args; tl.addEventAction(fsm.event_id, fsm.act_send); }
	goto st527;
st527:
	if ( ++p == pe )
		goto _test_eof527;
case 527:
#line 7404 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr641;
		case 33: goto tr603;
		case 48: goto tr642;
	}
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr643;
	goto tr601;
tr642:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 266 "lex/parser_timeline.rl"
	{ragel_num.vint = 0;}
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
#line 266 "lex/parser_timeline.rl"
	{ fsm.act_preset.idx = ragel_num.vint; }
	goto st528;
st528:
	if ( ++p == pe )
		goto _test_eof528;
case 528:
#line 7427 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr644;
	goto tr601;
tr643:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 266 "lex/parser_timeline.rl"
	{ragel_num.vint = 0;}
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
#line 266 "lex/parser_timeline.rl"
	{ fsm.act_preset.idx = ragel_num.vint; }
	goto st529;
tr645:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
#line 266 "lex/parser_timeline.rl"
	{ fsm.act_preset.idx = ragel_num.vint; }
	goto st529;
st529:
	if ( ++p == pe )
		goto _test_eof529;
case 529:
#line 7453 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr644;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr645;
	goto tr601;
tr607:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st530;
st530:
	if ( ++p == pe )
		goto _test_eof530;
case 530:
#line 7467 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr602;
		case 101: goto tr646;
	}
	goto tr601;
tr646:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st531;
st531:
	if ( ++p == pe )
		goto _test_eof531;
case 531:
#line 7481 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr602;
		case 110: goto tr647;
	}
	goto tr601;
tr647:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st532;
st532:
	if ( ++p == pe )
		goto _test_eof532;
case 532:
#line 7495 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr602;
		case 100: goto tr648;
	}
	goto tr601;
tr648:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st533;
st533:
	if ( ++p == pe )
		goto _test_eof533;
case 533:
#line 7509 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr649;
	goto tr601;
tr721:
#line 123 "lex/parser_timeline.rl"
	{ fsm.onSendTarget(); }
#line 121 "lex/parser_timeline.rl"
	{ fsm.act_send.args = fsm.args; tl.addEventAction(fsm.event_id, fsm.act_send); }
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 124 "lex/parser_timeline.rl"
	{ fsm.onArgsDone(); }
	goto st534;
tr649:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 124 "lex/parser_timeline.rl"
	{ fsm.onArgsDone(); }
#line 121 "lex/parser_timeline.rl"
	{ fsm.act_send.args = fsm.args; tl.addEventAction(fsm.event_id, fsm.act_send); }
	goto st534;
tr687:
#line 123 "lex/parser_timeline.rl"
	{ fsm.onSendTarget(); }
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 124 "lex/parser_timeline.rl"
	{ fsm.onArgsDone(); }
#line 121 "lex/parser_timeline.rl"
	{ fsm.act_send.args = fsm.args; tl.addEventAction(fsm.event_id, fsm.act_send); }
	goto st534;
st534:
	if ( ++p == pe )
		goto _test_eof534;
case 534:
#line 7545 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr649;
		case 33: goto tr651;
		case 43: goto tr652;
		case 45: goto tr652;
		case 48: goto tr653;
	}
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr654;
	goto tr650;
tr650:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st535;
st535:
	if ( ++p == pe )
		goto _test_eof535;
case 535:
#line 7564 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr655;
	goto tr650;
tr651:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st536;
st536:
	if ( ++p == pe )
		goto _test_eof536;
case 536:
#line 7576 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr655;
		case 109: goto tr656;
		case 111: goto tr657;
		case 112: goto tr658;
		case 115: goto tr659;
	}
	goto tr650;
tr656:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st537;
st537:
	if ( ++p == pe )
		goto _test_eof537;
case 537:
#line 7593 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr655;
		case 105: goto tr660;
	}
	goto tr650;
tr660:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st538;
st538:
	if ( ++p == pe )
		goto _test_eof538;
case 538:
#line 7607 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr655;
		case 100: goto tr661;
	}
	goto tr650;
tr661:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st539;
st539:
	if ( ++p == pe )
		goto _test_eof539;
case 539:
#line 7621 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr655;
		case 105: goto tr662;
	}
	goto tr650;
tr662:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st540;
st540:
	if ( ++p == pe )
		goto _test_eof540;
case 540:
#line 7635 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr655;
		case 58: goto tr663;
	}
	goto tr650;
tr663:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st541;
st541:
	if ( ++p == pe )
		goto _test_eof541;
case 541:
#line 7649 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr655;
		case 99: goto tr664;
		case 110: goto tr665;
		case 112: goto tr666;
	}
	goto tr650;
tr664:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st542;
st542:
	if ( ++p == pe )
		goto _test_eof542;
case 542:
#line 7665 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr655;
		case 116: goto tr667;
	}
	goto tr650;
tr667:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st543;
st543:
	if ( ++p == pe )
		goto _test_eof543;
case 543:
#line 7679 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr655;
		case 108: goto tr668;
	}
	goto tr650;
tr668:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st544;
st544:
	if ( ++p == pe )
		goto _test_eof544;
case 544:
#line 7693 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr669;
	goto tr650;
tr665:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st545;
st545:
	if ( ++p == pe )
		goto _test_eof545;
case 545:
#line 7705 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr655;
		case 111: goto tr670;
	}
	goto tr650;
tr670:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st546;
st546:
	if ( ++p == pe )
		goto _test_eof546;
case 546:
#line 7719 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr655;
		case 116: goto tr671;
	}
	goto tr650;
tr671:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st547;
st547:
	if ( ++p == pe )
		goto _test_eof547;
case 547:
#line 7733 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr655;
		case 101: goto tr668;
	}
	goto tr650;
tr666:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st548;
st548:
	if ( ++p == pe )
		goto _test_eof548;
case 548:
#line 7747 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr655;
		case 103: goto tr672;
	}
	goto tr650;
tr672:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st549;
st549:
	if ( ++p == pe )
		goto _test_eof549;
case 549:
#line 7761 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr655;
		case 109: goto tr673;
	}
	goto tr650;
tr673:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st550;
st550:
	if ( ++p == pe )
		goto _test_eof550;
case 550:
#line 7775 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr674;
	goto tr650;
tr657:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st551;
st551:
	if ( ++p == pe )
		goto _test_eof551;
case 551:
#line 7787 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr655;
		case 117: goto tr675;
	}
	goto tr650;
tr675:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st552;
st552:
	if ( ++p == pe )
		goto _test_eof552;
case 552:
#line 7801 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr655;
		case 116: goto tr676;
	}
	goto tr650;
tr676:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st553;
st553:
	if ( ++p == pe )
		goto _test_eof553;
case 553:
#line 7815 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr677;
	goto tr650;
tr658:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st554;
st554:
	if ( ++p == pe )
		goto _test_eof554;
case 554:
#line 7827 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr655;
		case 114: goto tr678;
	}
	goto tr650;
tr678:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st555;
st555:
	if ( ++p == pe )
		goto _test_eof555;
case 555:
#line 7841 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr655;
		case 101: goto tr679;
	}
	goto tr650;
tr679:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st556;
st556:
	if ( ++p == pe )
		goto _test_eof556;
case 556:
#line 7855 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr655;
		case 115: goto tr680;
	}
	goto tr650;
tr680:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st557;
st557:
	if ( ++p == pe )
		goto _test_eof557;
case 557:
#line 7869 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr655;
		case 101: goto tr681;
	}
	goto tr650;
tr681:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st558;
st558:
	if ( ++p == pe )
		goto _test_eof558;
case 558:
#line 7883 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr655;
		case 116: goto tr682;
	}
	goto tr650;
tr682:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st559;
st559:
	if ( ++p == pe )
		goto _test_eof559;
case 559:
#line 7897 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr683;
	goto tr650;
tr659:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st560;
st560:
	if ( ++p == pe )
		goto _test_eof560;
case 560:
#line 7909 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr655;
		case 101: goto tr684;
	}
	goto tr650;
tr684:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st561;
st561:
	if ( ++p == pe )
		goto _test_eof561;
case 561:
#line 7923 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr655;
		case 110: goto tr685;
	}
	goto tr650;
tr685:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st562;
st562:
	if ( ++p == pe )
		goto _test_eof562;
case 562:
#line 7937 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr655;
		case 100: goto tr686;
	}
	goto tr650;
tr686:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st563;
st563:
	if ( ++p == pe )
		goto _test_eof563;
case 563:
#line 7951 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr687;
	goto tr650;
tr652:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 21 "lex/ragel_numeric.rl"
	{ ragel_num.sign = ((*p)=='-') ? -1 : 1; }
	goto st564;
st564:
	if ( ++p == pe )
		goto _test_eof564;
case 564:
#line 7965 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr655;
		case 48: goto tr653;
	}
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr654;
	goto tr650;
tr653:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
	goto st565;
st565:
	if ( ++p == pe )
		goto _test_eof565;
case 565:
#line 7983 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr602;
		case 46: goto tr688;
	}
	goto tr650;
tr688:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
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
	goto st566;
st566:
	if ( ++p == pe )
		goto _test_eof566;
case 566:
#line 8008 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr655;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr689;
	goto tr650;
tr689:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 54 "lex/ragel_numeric.rl"
	{
        (ragel_num.ratio.num *= 10) += ((*p) - '0');
        ragel_num.ratio.den *= 10;
    }
	goto st567;
st567:
	if ( ++p == pe )
		goto _test_eof567;
case 567:
#line 8027 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr602;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr689;
	goto tr650;
tr654:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
	goto st568;
st568:
	if ( ++p == pe )
		goto _test_eof568;
case 568:
#line 8043 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr602;
		case 46: goto tr688;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr654;
	goto tr650;
tr596:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st569;
st569:
	if ( ++p == pe )
		goto _test_eof569;
case 569:
#line 8059 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr600;
		case 109: goto tr690;
		case 111: goto tr691;
		case 112: goto tr692;
		case 115: goto tr693;
	}
	goto tr595;
tr690:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st570;
st570:
	if ( ++p == pe )
		goto _test_eof570;
case 570:
#line 8076 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr600;
		case 105: goto tr694;
	}
	goto tr595;
tr694:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st571;
st571:
	if ( ++p == pe )
		goto _test_eof571;
case 571:
#line 8090 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr600;
		case 100: goto tr695;
	}
	goto tr595;
tr695:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st572;
st572:
	if ( ++p == pe )
		goto _test_eof572;
case 572:
#line 8104 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr600;
		case 105: goto tr696;
	}
	goto tr595;
tr696:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st573;
st573:
	if ( ++p == pe )
		goto _test_eof573;
case 573:
#line 8118 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr600;
		case 58: goto tr697;
	}
	goto tr595;
tr697:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st574;
st574:
	if ( ++p == pe )
		goto _test_eof574;
case 574:
#line 8132 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr600;
		case 99: goto tr698;
		case 110: goto tr699;
		case 112: goto tr700;
	}
	goto tr595;
tr698:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st575;
st575:
	if ( ++p == pe )
		goto _test_eof575;
case 575:
#line 8148 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr600;
		case 116: goto tr701;
	}
	goto tr595;
tr701:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st576;
st576:
	if ( ++p == pe )
		goto _test_eof576;
case 576:
#line 8162 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr600;
		case 108: goto tr702;
	}
	goto tr595;
tr702:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st577;
st577:
	if ( ++p == pe )
		goto _test_eof577;
case 577:
#line 8176 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr703;
	goto tr595;
tr699:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st578;
st578:
	if ( ++p == pe )
		goto _test_eof578;
case 578:
#line 8188 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr600;
		case 111: goto tr704;
	}
	goto tr595;
tr704:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st579;
st579:
	if ( ++p == pe )
		goto _test_eof579;
case 579:
#line 8202 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr600;
		case 116: goto tr705;
	}
	goto tr595;
tr705:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st580;
st580:
	if ( ++p == pe )
		goto _test_eof580;
case 580:
#line 8216 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr600;
		case 101: goto tr702;
	}
	goto tr595;
tr700:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st581;
st581:
	if ( ++p == pe )
		goto _test_eof581;
case 581:
#line 8230 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr600;
		case 103: goto tr706;
	}
	goto tr595;
tr706:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st582;
st582:
	if ( ++p == pe )
		goto _test_eof582;
case 582:
#line 8244 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr600;
		case 109: goto tr707;
	}
	goto tr595;
tr707:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st583;
st583:
	if ( ++p == pe )
		goto _test_eof583;
case 583:
#line 8258 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr708;
	goto tr595;
tr691:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st584;
st584:
	if ( ++p == pe )
		goto _test_eof584;
case 584:
#line 8270 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr600;
		case 117: goto tr709;
	}
	goto tr595;
tr709:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st585;
st585:
	if ( ++p == pe )
		goto _test_eof585;
case 585:
#line 8284 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr600;
		case 116: goto tr710;
	}
	goto tr595;
tr710:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st586;
st586:
	if ( ++p == pe )
		goto _test_eof586;
case 586:
#line 8298 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr711;
	goto tr595;
tr692:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st587;
st587:
	if ( ++p == pe )
		goto _test_eof587;
case 587:
#line 8310 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr600;
		case 114: goto tr712;
	}
	goto tr595;
tr712:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st588;
st588:
	if ( ++p == pe )
		goto _test_eof588;
case 588:
#line 8324 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr600;
		case 101: goto tr713;
	}
	goto tr595;
tr713:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st589;
st589:
	if ( ++p == pe )
		goto _test_eof589;
case 589:
#line 8338 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr600;
		case 115: goto tr714;
	}
	goto tr595;
tr714:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st590;
st590:
	if ( ++p == pe )
		goto _test_eof590;
case 590:
#line 8352 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr600;
		case 101: goto tr715;
	}
	goto tr595;
tr715:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st591;
st591:
	if ( ++p == pe )
		goto _test_eof591;
case 591:
#line 8366 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr600;
		case 116: goto tr716;
	}
	goto tr595;
tr716:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st592;
st592:
	if ( ++p == pe )
		goto _test_eof592;
case 592:
#line 8380 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr717;
	goto tr595;
tr693:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st593;
st593:
	if ( ++p == pe )
		goto _test_eof593;
case 593:
#line 8392 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr600;
		case 101: goto tr718;
	}
	goto tr595;
tr718:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st594;
st594:
	if ( ++p == pe )
		goto _test_eof594;
case 594:
#line 8406 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr600;
		case 110: goto tr719;
	}
	goto tr595;
tr719:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st595;
st595:
	if ( ++p == pe )
		goto _test_eof595;
case 595:
#line 8420 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr600;
		case 100: goto tr720;
	}
	goto tr595;
tr720:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st596;
st596:
	if ( ++p == pe )
		goto _test_eof596;
case 596:
#line 8434 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr721;
	goto tr595;
tr597:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 21 "lex/ragel_numeric.rl"
	{ ragel_num.sign = ((*p)=='-') ? -1 : 1; }
	goto st597;
st597:
	if ( ++p == pe )
		goto _test_eof597;
case 597:
#line 8448 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr600;
		case 48: goto tr598;
	}
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr599;
	goto tr595;
tr598:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
	goto st598;
st598:
	if ( ++p == pe )
		goto _test_eof598;
case 598:
#line 8466 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr415;
		case 46: goto tr722;
	}
	goto tr595;
tr722:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
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
	goto st599;
st599:
	if ( ++p == pe )
		goto _test_eof599;
case 599:
#line 8491 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr600;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr723;
	goto tr595;
tr723:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 54 "lex/ragel_numeric.rl"
	{
        (ragel_num.ratio.num *= 10) += ((*p) - '0');
        ragel_num.ratio.den *= 10;
    }
	goto st600;
st600:
	if ( ++p == pe )
		goto _test_eof600;
case 600:
#line 8510 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr415;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr723;
	goto tr595;
tr599:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
	goto st601;
st601:
	if ( ++p == pe )
		goto _test_eof601;
case 601:
#line 8526 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr415;
		case 46: goto tr722;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr599;
	goto tr595;
tr413:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st602;
st602:
	if ( ++p == pe )
		goto _test_eof602;
case 602:
#line 8542 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr381;
		case 116: goto tr724;
	}
	goto tr380;
tr724:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st603;
st603:
	if ( ++p == pe )
		goto _test_eof603;
case 603:
#line 8556 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr449;
	goto tr380;
tr385:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st604;
st604:
	if ( ++p == pe )
		goto _test_eof604;
case 604:
#line 8568 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr381;
		case 114: goto tr725;
	}
	goto tr380;
tr725:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st605;
st605:
	if ( ++p == pe )
		goto _test_eof605;
case 605:
#line 8582 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr381;
		case 101: goto tr726;
	}
	goto tr380;
tr726:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st606;
st606:
	if ( ++p == pe )
		goto _test_eof606;
case 606:
#line 8596 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr381;
		case 115: goto tr727;
	}
	goto tr380;
tr727:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st607;
st607:
	if ( ++p == pe )
		goto _test_eof607;
case 607:
#line 8610 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr381;
		case 101: goto tr728;
	}
	goto tr380;
tr728:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st608;
st608:
	if ( ++p == pe )
		goto _test_eof608;
case 608:
#line 8624 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr381;
		case 116: goto tr729;
	}
	goto tr380;
tr729:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st609;
st609:
	if ( ++p == pe )
		goto _test_eof609;
case 609:
#line 8638 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr730;
	goto tr380;
tr730:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 124 "lex/parser_timeline.rl"
	{ fsm.onArgsDone(); }
	goto st610;
st610:
	if ( ++p == pe )
		goto _test_eof610;
case 610:
#line 8652 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr730;
		case 33: goto tr382;
		case 48: goto tr731;
	}
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr732;
	goto tr380;
tr731:
#line 266 "lex/parser_timeline.rl"
	{ragel_num.vint = 0;}
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
#line 266 "lex/parser_timeline.rl"
	{ fsm.act_preset.idx = ragel_num.vint; }
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st611;
st611:
	if ( ++p == pe )
		goto _test_eof611;
case 611:
#line 8675 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr733;
	goto tr380;
tr732:
#line 266 "lex/parser_timeline.rl"
	{ragel_num.vint = 0;}
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
#line 266 "lex/parser_timeline.rl"
	{ fsm.act_preset.idx = ragel_num.vint; }
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st612;
tr734:
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
#line 266 "lex/parser_timeline.rl"
	{ fsm.act_preset.idx = ragel_num.vint; }
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st612;
st612:
	if ( ++p == pe )
		goto _test_eof612;
case 612:
#line 8701 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr733;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr734;
	goto tr380;
tr386:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st613;
st613:
	if ( ++p == pe )
		goto _test_eof613;
case 613:
#line 8715 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr381;
		case 101: goto tr735;
	}
	goto tr380;
tr735:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st614;
st614:
	if ( ++p == pe )
		goto _test_eof614;
case 614:
#line 8729 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr381;
		case 110: goto tr736;
	}
	goto tr380;
tr736:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st615;
st615:
	if ( ++p == pe )
		goto _test_eof615;
case 615:
#line 8743 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr381;
		case 100: goto tr737;
	}
	goto tr380;
tr737:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st616;
st616:
	if ( ++p == pe )
		goto _test_eof616;
case 616:
#line 8757 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr738;
	goto tr380;
tr738:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 124 "lex/parser_timeline.rl"
	{ fsm.onArgsDone(); }
	goto st617;
st617:
	if ( ++p == pe )
		goto _test_eof617;
case 617:
#line 8771 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr738;
		case 33: goto tr596;
		case 43: goto tr739;
		case 45: goto tr739;
		case 48: goto tr740;
	}
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr741;
	goto tr595;
tr739:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 21 "lex/ragel_numeric.rl"
	{ ragel_num.sign = ((*p)=='-') ? -1 : 1; }
	goto st618;
st618:
	if ( ++p == pe )
		goto _test_eof618;
case 618:
#line 8792 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr600;
		case 48: goto tr740;
	}
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr741;
	goto tr595;
tr740:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
	goto st619;
st619:
	if ( ++p == pe )
		goto _test_eof619;
case 619:
#line 8810 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr381;
		case 46: goto tr742;
	}
	goto tr595;
tr742:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
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
	goto st620;
st620:
	if ( ++p == pe )
		goto _test_eof620;
case 620:
#line 8835 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr600;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr743;
	goto tr595;
tr743:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 54 "lex/ragel_numeric.rl"
	{
        (ragel_num.ratio.num *= 10) += ((*p) - '0');
        ragel_num.ratio.den *= 10;
    }
	goto st621;
st621:
	if ( ++p == pe )
		goto _test_eof621;
case 621:
#line 8854 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr381;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr743;
	goto tr595;
tr741:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
	goto st622;
st622:
	if ( ++p == pe )
		goto _test_eof622;
case 622:
#line 8870 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr381;
		case 46: goto tr742;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr741;
	goto tr595;
st190:
	if ( ++p == pe )
		goto _test_eof190;
case 190:
	switch( (*p) ) {
		case 115: goto st191;
		case 117: goto st192;
	}
	goto st0;
st191:
	if ( ++p == pe )
		goto _test_eof191;
case 191:
	if ( (*p) == 99 )
		goto st623;
	goto st0;
st623:
	if ( ++p == pe )
		goto _test_eof623;
case 623:
	if ( (*p) == 32 )
		goto st332;
	goto st0;
st192:
	if ( ++p == pe )
		goto _test_eof192;
case 192:
	if ( (*p) == 116 )
		goto st624;
	goto st0;
st624:
	if ( ++p == pe )
		goto _test_eof624;
case 624:
	if ( (*p) == 32 )
		goto tr745;
	goto st0;
st193:
	if ( ++p == pe )
		goto _test_eof193;
case 193:
	if ( (*p) == 114 )
		goto st194;
	goto st0;
st194:
	if ( ++p == pe )
		goto _test_eof194;
case 194:
	if ( (*p) == 101 )
		goto st195;
	goto st0;
st195:
	if ( ++p == pe )
		goto _test_eof195;
case 195:
	if ( (*p) == 115 )
		goto st196;
	goto st0;
st196:
	if ( ++p == pe )
		goto _test_eof196;
case 196:
	if ( (*p) == 101 )
		goto st197;
	goto st0;
st197:
	if ( ++p == pe )
		goto _test_eof197;
case 197:
	if ( (*p) == 116 )
		goto st198;
	goto st0;
st198:
	if ( ++p == pe )
		goto _test_eof198;
case 198:
	if ( (*p) == 32 )
		goto st199;
	goto st0;
st199:
	if ( ++p == pe )
		goto _test_eof199;
case 199:
	switch( (*p) ) {
		case 32: goto st199;
		case 48: goto tr260;
	}
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr261;
	goto st0;
tr260:
#line 266 "lex/parser_timeline.rl"
	{ragel_num.vint = 0;}
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
#line 266 "lex/parser_timeline.rl"
	{ fsm.act_preset.idx = ragel_num.vint; }
	goto st625;
st625:
	if ( ++p == pe )
		goto _test_eof625;
case 625:
#line 8980 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr746;
	goto st0;
tr261:
#line 266 "lex/parser_timeline.rl"
	{ragel_num.vint = 0;}
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
#line 266 "lex/parser_timeline.rl"
	{ fsm.act_preset.idx = ragel_num.vint; }
	goto st626;
tr747:
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
#line 266 "lex/parser_timeline.rl"
	{ fsm.act_preset.idx = ragel_num.vint; }
	goto st626;
st626:
	if ( ++p == pe )
		goto _test_eof626;
case 626:
#line 9002 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr746;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr747;
	goto st0;
st200:
	if ( ++p == pe )
		goto _test_eof200;
case 200:
	if ( (*p) == 101 )
		goto st201;
	goto st0;
st201:
	if ( ++p == pe )
		goto _test_eof201;
case 201:
	if ( (*p) == 110 )
		goto st202;
	goto st0;
st202:
	if ( ++p == pe )
		goto _test_eof202;
case 202:
	if ( (*p) == 100 )
		goto st203;
	goto st0;
st203:
	if ( ++p == pe )
		goto _test_eof203;
case 203:
	if ( (*p) == 32 )
		goto st204;
	goto st0;
st204:
	if ( ++p == pe )
		goto _test_eof204;
case 204:
	switch( (*p) ) {
		case 32: goto st204;
		case 43: goto tr267;
		case 45: goto tr267;
		case 48: goto tr268;
	}
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr269;
	goto tr266;
tr266:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st627;
st627:
	if ( ++p == pe )
		goto _test_eof627;
case 627:
#line 9057 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr748;
	goto tr266;
tr267:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 21 "lex/ragel_numeric.rl"
	{ ragel_num.sign = ((*p)=='-') ? -1 : 1; }
	goto st628;
st628:
	if ( ++p == pe )
		goto _test_eof628;
case 628:
#line 9071 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr748;
		case 48: goto tr268;
	}
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr269;
	goto tr266;
tr268:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
	goto st205;
st205:
	if ( ++p == pe )
		goto _test_eof205;
case 205:
#line 9089 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto st0;
		case 46: goto tr270;
	}
	goto tr266;
tr270:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
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
	goto st629;
st629:
	if ( ++p == pe )
		goto _test_eof629;
case 629:
#line 9114 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr748;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr271;
	goto tr266;
tr271:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 54 "lex/ragel_numeric.rl"
	{
        (ragel_num.ratio.num *= 10) += ((*p) - '0');
        ragel_num.ratio.den *= 10;
    }
	goto st206;
st206:
	if ( ++p == pe )
		goto _test_eof206;
case 206:
#line 9133 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto st0;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr271;
	goto tr266;
tr269:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
	goto st207;
st207:
	if ( ++p == pe )
		goto _test_eof207;
case 207:
#line 9149 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto st0;
		case 46: goto tr270;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr269;
	goto tr266;
st208:
	if ( ++p == pe )
		goto _test_eof208;
case 208:
	if ( (*p) == 108 )
		goto st209;
	goto st0;
st209:
	if ( ++p == pe )
		goto _test_eof209;
case 209:
	if ( (*p) == 32 )
		goto st210;
	goto st0;
st210:
	if ( ++p == pe )
		goto _test_eof210;
case 210:
	switch( (*p) ) {
		case 32: goto st210;
		case 42: goto tr274;
		case 43: goto tr275;
		case 45: goto tr275;
		case 48: goto tr276;
	}
	if ( (*p) > 53 ) {
		if ( 54 <= (*p) && (*p) <= 57 )
			goto tr278;
	} else if ( (*p) >= 49 )
		goto tr277;
	goto st0;
tr274:
#line 116 "lex/parser_timeline.rl"
	{ fsm.onTimeInfDone(); }
#line 339 "lex/parser_timeline.rl"
	{ bar.inf = true; tl.bars.clear(); }
	goto st630;
st630:
	if ( ++p == pe )
		goto _test_eof630;
case 630:
#line 9198 "lex/parser_timeline.cpp"
	if ( (*p) == 124 )
		goto st211;
	goto st0;
st211:
	if ( ++p == pe )
		goto _test_eof211;
case 211:
	if ( (*p) == 48 )
		goto tr279;
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr280;
	goto st0;
tr279:
#line 325 "lex/parser_timeline.rl"
	{ ragel_num.vint = 0; }
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
	goto st212;
st212:
	if ( ++p == pe )
		goto _test_eof212;
case 212:
#line 9221 "lex/parser_timeline.cpp"
	if ( (*p) == 47 )
		goto tr281;
	goto st0;
tr281:
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 325 "lex/parser_timeline.rl"
	{ bar.num = ragel_num.vint; ragel_num.vint = 0; }
	goto st213;
st213:
	if ( ++p == pe )
		goto _test_eof213;
case 213:
#line 9239 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 49: goto tr282;
		case 50: goto tr283;
		case 51: goto st221;
		case 52: goto tr285;
		case 54: goto st222;
		case 56: goto tr287;
	}
	goto st0;
tr282:
#line 326 "lex/parser_timeline.rl"
	{ bar.div = 1; }
	goto st214;
st214:
	if ( ++p == pe )
		goto _test_eof214;
case 214:
#line 9257 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 50: goto st215;
		case 54: goto tr289;
		case 124: goto st631;
	}
	goto st0;
st215:
	if ( ++p == pe )
		goto _test_eof215;
case 215:
	if ( (*p) == 56 )
		goto tr291;
	goto st0;
tr283:
#line 327 "lex/parser_timeline.rl"
	{ bar.div = 2; }
	goto st216;
tr285:
#line 328 "lex/parser_timeline.rl"
	{ bar.div = 4; }
	goto st216;
tr287:
#line 329 "lex/parser_timeline.rl"
	{ bar.div = 8; }
	goto st216;
tr289:
#line 330 "lex/parser_timeline.rl"
	{ bar.div = 16; }
	goto st216;
tr291:
#line 333 "lex/parser_timeline.rl"
	{ bar.div = 128; }
	goto st216;
tr298:
#line 331 "lex/parser_timeline.rl"
	{ bar.div = 32; }
	goto st216;
tr299:
#line 332 "lex/parser_timeline.rl"
	{ bar.div = 64; }
	goto st216;
st216:
	if ( ++p == pe )
		goto _test_eof216;
case 216:
#line 9303 "lex/parser_timeline.cpp"
	if ( (*p) == 124 )
		goto st631;
	goto st0;
st631:
	if ( ++p == pe )
		goto _test_eof631;
case 631:
	if ( (*p) == 32 )
		goto tr749;
	goto st0;
tr749:
#line 219 "lex/parser_timeline.rl"
	{
        if (bar.inf) {
            tl.duration = std::numeric_limits<t_float>::max();
        } else {
            TimeLineBar b;
            b.count = bar.count;
            b.sig.set(bar.num, bar.div);
            tl.bars.push_back(b);
        }
    }
	goto st217;
st217:
	if ( ++p == pe )
		goto _test_eof217;
case 217:
#line 9331 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto st217;
		case 42: goto st218;
		case 48: goto tr294;
	}
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr295;
	goto st0;
tr297:
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 336 "lex/parser_timeline.rl"
	{ bar.inf = false; bar.count = ragel_num.vint; }
	goto st218;
st218:
	if ( ++p == pe )
		goto _test_eof218;
case 218:
#line 9354 "lex/parser_timeline.cpp"
	if ( (*p) == 124 )
		goto st211;
	goto st0;
tr294:
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
	goto st219;
st219:
	if ( ++p == pe )
		goto _test_eof219;
case 219:
#line 9366 "lex/parser_timeline.cpp"
	if ( (*p) == 42 )
		goto tr297;
	goto st0;
tr295:
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
	goto st220;
st220:
	if ( ++p == pe )
		goto _test_eof220;
case 220:
#line 9378 "lex/parser_timeline.cpp"
	if ( (*p) == 42 )
		goto tr297;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr295;
	goto st0;
st221:
	if ( ++p == pe )
		goto _test_eof221;
case 221:
	if ( (*p) == 50 )
		goto tr298;
	goto st0;
st222:
	if ( ++p == pe )
		goto _test_eof222;
case 222:
	if ( (*p) == 52 )
		goto tr299;
	goto st0;
tr300:
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
	goto st223;
tr280:
#line 325 "lex/parser_timeline.rl"
	{ ragel_num.vint = 0; }
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
	goto st223;
st223:
	if ( ++p == pe )
		goto _test_eof223;
case 223:
#line 9412 "lex/parser_timeline.cpp"
	if ( (*p) == 47 )
		goto tr281;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr300;
	goto st0;
tr275:
#line 79 "lex/ragel_units.rl"
	{smpte.np = 0;}
#line 74 "lex/ragel_units.rl"
	{smpte.sign = ((*p)=='-')?-1:1;}
	goto st224;
st224:
	if ( ++p == pe )
		goto _test_eof224;
case 224:
#line 9428 "lex/parser_timeline.cpp"
	if ( 48 <= (*p) && (*p) <= 53 )
		goto tr301;
	goto st0;
tr301:
#line 67 "lex/ragel_units.rl"
	{smpte.hour = 0; smpte.np++;}
#line 67 "lex/ragel_units.rl"
	{(smpte.hour *= 10) += ((*p) - '0');}
	goto st225;
st225:
	if ( ++p == pe )
		goto _test_eof225;
case 225:
#line 9442 "lex/parser_timeline.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr302;
	goto st0;
tr302:
#line 67 "lex/ragel_units.rl"
	{(smpte.hour *= 10) += ((*p) - '0');}
	goto st226;
st226:
	if ( ++p == pe )
		goto _test_eof226;
case 226:
#line 9454 "lex/parser_timeline.cpp"
	if ( (*p) == 58 )
		goto st227;
	goto st0;
st227:
	if ( ++p == pe )
		goto _test_eof227;
case 227:
	if ( 48 <= (*p) && (*p) <= 53 )
		goto tr304;
	goto st0;
tr304:
#line 68 "lex/ragel_units.rl"
	{smpte.min = 0; smpte.np++;}
#line 68 "lex/ragel_units.rl"
	{(smpte.min *= 10) += ((*p) - '0');}
	goto st228;
st228:
	if ( ++p == pe )
		goto _test_eof228;
case 228:
#line 9475 "lex/parser_timeline.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr305;
	goto st0;
tr305:
#line 68 "lex/ragel_units.rl"
	{(smpte.min *= 10) += ((*p) - '0');}
	goto st632;
st632:
	if ( ++p == pe )
		goto _test_eof632;
case 632:
#line 9487 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 46: goto st229;
		case 58: goto st230;
	}
	goto st0;
st229:
	if ( ++p == pe )
		goto _test_eof229;
case 229:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr306;
	goto st0;
tr306:
#line 70 "lex/ragel_units.rl"
	{smpte.frame = 0;}
#line 70 "lex/ragel_units.rl"
	{(smpte.frame *= 10) += ((*p) - '0');}
	goto st633;
st633:
	if ( ++p == pe )
		goto _test_eof633;
case 633:
#line 9510 "lex/parser_timeline.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr752;
	goto st0;
tr752:
#line 70 "lex/ragel_units.rl"
	{(smpte.frame *= 10) += ((*p) - '0');}
	goto st634;
st634:
	if ( ++p == pe )
		goto _test_eof634;
case 634:
#line 9522 "lex/parser_timeline.cpp"
	goto st0;
st230:
	if ( ++p == pe )
		goto _test_eof230;
case 230:
	if ( 48 <= (*p) && (*p) <= 53 )
		goto tr307;
	goto st0;
tr307:
#line 69 "lex/ragel_units.rl"
	{smpte.sec = 0; smpte.np++;}
#line 69 "lex/ragel_units.rl"
	{(smpte.sec *= 10) += ((*p) - '0');}
	goto st231;
st231:
	if ( ++p == pe )
		goto _test_eof231;
case 231:
#line 9541 "lex/parser_timeline.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr308;
	goto st0;
tr308:
#line 69 "lex/ragel_units.rl"
	{(smpte.sec *= 10) += ((*p) - '0');}
	goto st635;
st635:
	if ( ++p == pe )
		goto _test_eof635;
case 635:
#line 9553 "lex/parser_timeline.cpp"
	if ( (*p) == 46 )
		goto st229;
	goto st0;
tr276:
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
#line 79 "lex/ragel_units.rl"
	{smpte.np = 0;}
#line 67 "lex/ragel_units.rl"
	{smpte.hour = 0; smpte.np++;}
#line 67 "lex/ragel_units.rl"
	{(smpte.hour *= 10) += ((*p) - '0');}
#line 339 "lex/parser_timeline.rl"
	{ bar.inf = true; tl.bars.clear(); }
	goto st232;
st232:
	if ( ++p == pe )
		goto _test_eof232;
case 232:
#line 9573 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 42: goto tr297;
		case 46: goto tr309;
		case 104: goto tr310;
		case 109: goto tr311;
		case 115: goto tr312;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr302;
	goto st0;
tr309:
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
	goto st233;
st233:
	if ( ++p == pe )
		goto _test_eof233;
case 233:
#line 9601 "lex/parser_timeline.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr313;
	goto st0;
tr313:
#line 54 "lex/ragel_numeric.rl"
	{
        (ragel_num.ratio.num *= 10) += ((*p) - '0');
        ragel_num.ratio.den *= 10;
    }
	goto st234;
st234:
	if ( ++p == pe )
		goto _test_eof234;
case 234:
#line 9616 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 104: goto tr314;
		case 109: goto tr315;
		case 115: goto tr316;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr313;
	goto st0;
tr310:
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 7 "lex/ragel_units.rl"
	{ ragel_num.vdouble = ragel_num.vint; }
	goto st636;
tr314:
#line 58 "lex/ragel_numeric.rl"
	{
        const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
        ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
        ragel_type = TYPE_FLOAT;
        ragel_cat  = CAT_NUMBER;
    }
	goto st636;
st636:
	if ( ++p == pe )
		goto _test_eof636;
case 636:
#line 9648 "lex/parser_timeline.cpp"
	if ( (*p) == 111 )
		goto st235;
	goto st0;
st235:
	if ( ++p == pe )
		goto _test_eof235;
case 235:
	if ( (*p) == 117 )
		goto st236;
	goto st0;
st236:
	if ( ++p == pe )
		goto _test_eof236;
case 236:
	if ( (*p) == 114 )
		goto st637;
	goto st0;
st637:
	if ( ++p == pe )
		goto _test_eof637;
case 637:
	goto st0;
tr311:
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 7 "lex/ragel_units.rl"
	{ ragel_num.vdouble = ragel_num.vint; }
	goto st638;
tr315:
#line 58 "lex/ragel_numeric.rl"
	{
        const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
        ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
        ragel_type = TYPE_FLOAT;
        ragel_cat  = CAT_NUMBER;
    }
	goto st638;
st638:
	if ( ++p == pe )
		goto _test_eof638;
case 638:
#line 9694 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 105: goto st237;
		case 115: goto st640;
	}
	goto st0;
st237:
	if ( ++p == pe )
		goto _test_eof237;
case 237:
	if ( (*p) == 110 )
		goto st639;
	goto st0;
st639:
	if ( ++p == pe )
		goto _test_eof639;
case 639:
	goto st0;
st640:
	if ( ++p == pe )
		goto _test_eof640;
case 640:
	if ( (*p) == 101 )
		goto st238;
	goto st0;
st238:
	if ( ++p == pe )
		goto _test_eof238;
case 238:
	if ( (*p) == 99 )
		goto st641;
	goto st0;
st641:
	if ( ++p == pe )
		goto _test_eof641;
case 641:
	goto st0;
tr312:
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 7 "lex/ragel_units.rl"
	{ ragel_num.vdouble = ragel_num.vint; }
	goto st642;
tr316:
#line 58 "lex/ragel_numeric.rl"
	{
        const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
        ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
        ragel_type = TYPE_FLOAT;
        ragel_cat  = CAT_NUMBER;
    }
	goto st642;
st642:
	if ( ++p == pe )
		goto _test_eof642;
case 642:
#line 9754 "lex/parser_timeline.cpp"
	if ( (*p) == 101 )
		goto st239;
	goto st0;
st239:
	if ( ++p == pe )
		goto _test_eof239;
case 239:
	if ( (*p) == 99 )
		goto st643;
	goto st0;
st643:
	if ( ++p == pe )
		goto _test_eof643;
case 643:
	goto st0;
tr277:
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
#line 79 "lex/ragel_units.rl"
	{smpte.np = 0;}
#line 67 "lex/ragel_units.rl"
	{smpte.hour = 0; smpte.np++;}
#line 67 "lex/ragel_units.rl"
	{(smpte.hour *= 10) += ((*p) - '0');}
#line 339 "lex/parser_timeline.rl"
	{ bar.inf = true; tl.bars.clear(); }
	goto st240;
st240:
	if ( ++p == pe )
		goto _test_eof240;
case 240:
#line 9786 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 42: goto tr297;
		case 46: goto tr309;
		case 104: goto tr310;
		case 109: goto tr311;
		case 115: goto tr312;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr322;
	goto st0;
tr322:
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
#line 67 "lex/ragel_units.rl"
	{(smpte.hour *= 10) += ((*p) - '0');}
	goto st241;
st241:
	if ( ++p == pe )
		goto _test_eof241;
case 241:
#line 9807 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 42: goto tr297;
		case 46: goto tr309;
		case 58: goto st227;
		case 104: goto tr310;
		case 109: goto tr311;
		case 115: goto tr312;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr323;
	goto st0;
tr323:
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
	goto st242;
tr278:
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
#line 339 "lex/parser_timeline.rl"
	{ bar.inf = true; tl.bars.clear(); }
	goto st242;
st242:
	if ( ++p == pe )
		goto _test_eof242;
case 242:
#line 9833 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 42: goto tr297;
		case 46: goto tr309;
		case 104: goto tr310;
		case 109: goto tr311;
		case 115: goto tr312;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr323;
	goto st0;
st243:
	if ( ++p == pe )
		goto _test_eof243;
case 243:
	if ( (*p) == 97 )
		goto st244;
	goto st0;
st244:
	if ( ++p == pe )
		goto _test_eof244;
case 244:
	if ( (*p) == 114 )
		goto st245;
	goto st0;
st245:
	if ( ++p == pe )
		goto _test_eof245;
case 245:
	if ( (*p) == 32 )
		goto st246;
	goto st0;
st246:
	if ( ++p == pe )
		goto _test_eof246;
case 246:
	switch( (*p) ) {
		case 32: goto st246;
		case 95: goto tr327;
	}
	if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr327;
	} else if ( (*p) >= 65 )
		goto tr327;
	goto st0;
tr759:
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st644;
tr327:
#line 284 "lex/parser_timeline.rl"
	{ fsm.symbol.clear(); }
#line 118 "lex/parser_timeline.rl"
	{ fsm.symbol += *p; }
	goto st644;
st644:
	if ( ++p == pe )
		goto _test_eof644;
case 644:
#line 9893 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr758;
		case 95: goto tr759;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr759;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr759;
	} else
		goto tr759;
	goto st0;
tr758:
#line 230 "lex/parser_timeline.rl"
	{
        TimeLineVarDef var;
        var.name = gensym(fsm.symbol.c_str());
        tl.var_defs.push_back(var);
    }
	goto st247;
tr760:
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 7 "lex/ragel_units.rl"
	{ ragel_num.vdouble = ragel_num.vint; }
#line 285 "lex/parser_timeline.rl"
	{ tl.var_defs.back().def = ragel_num.getFloat(); }
	goto st247;
tr762:
#line 58 "lex/ragel_numeric.rl"
	{
        const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
        ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
        ragel_type = TYPE_FLOAT;
        ragel_cat  = CAT_NUMBER;
    }
#line 285 "lex/parser_timeline.rl"
	{ tl.var_defs.back().def = ragel_num.getFloat(); }
	goto st247;
tr763:
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 7 "lex/ragel_units.rl"
	{ ragel_num.vdouble = ragel_num.vint; }
#line 286 "lex/parser_timeline.rl"
	{ tl.var_defs.back().preset = ragel_num.getFloat(); }
	goto st247;
tr765:
#line 58 "lex/ragel_numeric.rl"
	{
        const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
        ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
        ragel_type = TYPE_FLOAT;
        ragel_cat  = CAT_NUMBER;
    }
#line 286 "lex/parser_timeline.rl"
	{ tl.var_defs.back().preset = ragel_num.getFloat(); }
	goto st247;
st247:
	if ( ++p == pe )
		goto _test_eof247;
case 247:
#line 9965 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto st247;
		case 64: goto st248;
	}
	goto st0;
st248:
	if ( ++p == pe )
		goto _test_eof248;
case 248:
	switch( (*p) ) {
		case 100: goto st249;
		case 112: goto st259;
	}
	goto st0;
st249:
	if ( ++p == pe )
		goto _test_eof249;
case 249:
	if ( (*p) == 101 )
		goto st250;
	goto st0;
st250:
	if ( ++p == pe )
		goto _test_eof250;
case 250:
	if ( (*p) == 102 )
		goto st251;
	goto st0;
st251:
	if ( ++p == pe )
		goto _test_eof251;
case 251:
	if ( (*p) == 97 )
		goto st252;
	goto st0;
st252:
	if ( ++p == pe )
		goto _test_eof252;
case 252:
	if ( (*p) == 117 )
		goto st253;
	goto st0;
st253:
	if ( ++p == pe )
		goto _test_eof253;
case 253:
	if ( (*p) == 108 )
		goto st254;
	goto st0;
st254:
	if ( ++p == pe )
		goto _test_eof254;
case 254:
	if ( (*p) == 116 )
		goto st255;
	goto st0;
st255:
	if ( ++p == pe )
		goto _test_eof255;
case 255:
	if ( (*p) == 32 )
		goto st256;
	goto st0;
st256:
	if ( ++p == pe )
		goto _test_eof256;
case 256:
	switch( (*p) ) {
		case 32: goto st256;
		case 43: goto tr339;
		case 45: goto tr339;
		case 48: goto tr340;
	}
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr341;
	goto st0;
tr339:
#line 21 "lex/ragel_numeric.rl"
	{ ragel_num.sign = ((*p)=='-') ? -1 : 1; }
	goto st257;
st257:
	if ( ++p == pe )
		goto _test_eof257;
case 257:
#line 10050 "lex/parser_timeline.cpp"
	if ( (*p) == 48 )
		goto tr340;
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr341;
	goto st0;
tr340:
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
	goto st645;
st645:
	if ( ++p == pe )
		goto _test_eof645;
case 645:
#line 10064 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr760;
		case 46: goto tr761;
	}
	goto st0;
tr761:
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
	goto st258;
st258:
	if ( ++p == pe )
		goto _test_eof258;
case 258:
#line 10087 "lex/parser_timeline.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr342;
	goto st0;
tr342:
#line 54 "lex/ragel_numeric.rl"
	{
        (ragel_num.ratio.num *= 10) += ((*p) - '0');
        ragel_num.ratio.den *= 10;
    }
	goto st646;
st646:
	if ( ++p == pe )
		goto _test_eof646;
case 646:
#line 10102 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr762;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr342;
	goto st0;
tr341:
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
	goto st647;
st647:
	if ( ++p == pe )
		goto _test_eof647;
case 647:
#line 10116 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr760;
		case 46: goto tr761;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr341;
	goto st0;
st259:
	if ( ++p == pe )
		goto _test_eof259;
case 259:
	if ( (*p) == 114 )
		goto st260;
	goto st0;
st260:
	if ( ++p == pe )
		goto _test_eof260;
case 260:
	if ( (*p) == 101 )
		goto st261;
	goto st0;
st261:
	if ( ++p == pe )
		goto _test_eof261;
case 261:
	if ( (*p) == 115 )
		goto st262;
	goto st0;
st262:
	if ( ++p == pe )
		goto _test_eof262;
case 262:
	if ( (*p) == 101 )
		goto st263;
	goto st0;
st263:
	if ( ++p == pe )
		goto _test_eof263;
case 263:
	if ( (*p) == 116 )
		goto st264;
	goto st0;
st264:
	if ( ++p == pe )
		goto _test_eof264;
case 264:
	if ( (*p) == 32 )
		goto st265;
	goto st0;
st265:
	if ( ++p == pe )
		goto _test_eof265;
case 265:
	switch( (*p) ) {
		case 32: goto st265;
		case 43: goto tr349;
		case 45: goto tr349;
		case 48: goto tr350;
	}
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr351;
	goto st0;
tr349:
#line 21 "lex/ragel_numeric.rl"
	{ ragel_num.sign = ((*p)=='-') ? -1 : 1; }
	goto st266;
st266:
	if ( ++p == pe )
		goto _test_eof266;
case 266:
#line 10187 "lex/parser_timeline.cpp"
	if ( (*p) == 48 )
		goto tr350;
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr351;
	goto st0;
tr350:
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
	goto st648;
st648:
	if ( ++p == pe )
		goto _test_eof648;
case 648:
#line 10201 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr763;
		case 46: goto tr764;
	}
	goto st0;
tr764:
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
	goto st267;
st267:
	if ( ++p == pe )
		goto _test_eof267;
case 267:
#line 10224 "lex/parser_timeline.cpp"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr352;
	goto st0;
tr352:
#line 54 "lex/ragel_numeric.rl"
	{
        (ragel_num.ratio.num *= 10) += ((*p) - '0');
        ragel_num.ratio.den *= 10;
    }
	goto st649;
st649:
	if ( ++p == pe )
		goto _test_eof649;
case 649:
#line 10239 "lex/parser_timeline.cpp"
	if ( (*p) == 32 )
		goto tr765;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr352;
	goto st0;
tr351:
#line 29 "lex/ragel_numeric.rl"
	{ (ragel_num.vint *= 10) += ((*p)-'0'); }
	goto st650;
st650:
	if ( ++p == pe )
		goto _test_eof650;
case 650:
#line 10253 "lex/parser_timeline.cpp"
	switch( (*p) ) {
		case 32: goto tr763;
		case 46: goto tr764;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr351;
	goto st0;
	}
	_test_eof2: cs = 2; goto _test_eof; 
	_test_eof3: cs = 3; goto _test_eof; 
	_test_eof268: cs = 268; goto _test_eof; 
	_test_eof4: cs = 4; goto _test_eof; 
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
	_test_eof269: cs = 269; goto _test_eof; 
	_test_eof270: cs = 270; goto _test_eof; 
	_test_eof16: cs = 16; goto _test_eof; 
	_test_eof17: cs = 17; goto _test_eof; 
	_test_eof18: cs = 18; goto _test_eof; 
	_test_eof19: cs = 19; goto _test_eof; 
	_test_eof20: cs = 20; goto _test_eof; 
	_test_eof21: cs = 21; goto _test_eof; 
	_test_eof22: cs = 22; goto _test_eof; 
	_test_eof271: cs = 271; goto _test_eof; 
	_test_eof272: cs = 272; goto _test_eof; 
	_test_eof23: cs = 23; goto _test_eof; 
	_test_eof24: cs = 24; goto _test_eof; 
	_test_eof25: cs = 25; goto _test_eof; 
	_test_eof26: cs = 26; goto _test_eof; 
	_test_eof27: cs = 27; goto _test_eof; 
	_test_eof273: cs = 273; goto _test_eof; 
	_test_eof274: cs = 274; goto _test_eof; 
	_test_eof275: cs = 275; goto _test_eof; 
	_test_eof28: cs = 28; goto _test_eof; 
	_test_eof276: cs = 276; goto _test_eof; 
	_test_eof29: cs = 29; goto _test_eof; 
	_test_eof30: cs = 30; goto _test_eof; 
	_test_eof277: cs = 277; goto _test_eof; 
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
	_test_eof42: cs = 42; goto _test_eof; 
	_test_eof43: cs = 43; goto _test_eof; 
	_test_eof44: cs = 44; goto _test_eof; 
	_test_eof45: cs = 45; goto _test_eof; 
	_test_eof46: cs = 46; goto _test_eof; 
	_test_eof47: cs = 47; goto _test_eof; 
	_test_eof48: cs = 48; goto _test_eof; 
	_test_eof49: cs = 49; goto _test_eof; 
	_test_eof50: cs = 50; goto _test_eof; 
	_test_eof51: cs = 51; goto _test_eof; 
	_test_eof52: cs = 52; goto _test_eof; 
	_test_eof278: cs = 278; goto _test_eof; 
	_test_eof53: cs = 53; goto _test_eof; 
	_test_eof54: cs = 54; goto _test_eof; 
	_test_eof279: cs = 279; goto _test_eof; 
	_test_eof280: cs = 280; goto _test_eof; 
	_test_eof55: cs = 55; goto _test_eof; 
	_test_eof281: cs = 281; goto _test_eof; 
	_test_eof282: cs = 282; goto _test_eof; 
	_test_eof56: cs = 56; goto _test_eof; 
	_test_eof283: cs = 283; goto _test_eof; 
	_test_eof284: cs = 284; goto _test_eof; 
	_test_eof57: cs = 57; goto _test_eof; 
	_test_eof285: cs = 285; goto _test_eof; 
	_test_eof58: cs = 58; goto _test_eof; 
	_test_eof59: cs = 59; goto _test_eof; 
	_test_eof60: cs = 60; goto _test_eof; 
	_test_eof61: cs = 61; goto _test_eof; 
	_test_eof62: cs = 62; goto _test_eof; 
	_test_eof63: cs = 63; goto _test_eof; 
	_test_eof64: cs = 64; goto _test_eof; 
	_test_eof65: cs = 65; goto _test_eof; 
	_test_eof66: cs = 66; goto _test_eof; 
	_test_eof67: cs = 67; goto _test_eof; 
	_test_eof68: cs = 68; goto _test_eof; 
	_test_eof69: cs = 69; goto _test_eof; 
	_test_eof70: cs = 70; goto _test_eof; 
	_test_eof71: cs = 71; goto _test_eof; 
	_test_eof286: cs = 286; goto _test_eof; 
	_test_eof72: cs = 72; goto _test_eof; 
	_test_eof287: cs = 287; goto _test_eof; 
	_test_eof288: cs = 288; goto _test_eof; 
	_test_eof73: cs = 73; goto _test_eof; 
	_test_eof74: cs = 74; goto _test_eof; 
	_test_eof75: cs = 75; goto _test_eof; 
	_test_eof76: cs = 76; goto _test_eof; 
	_test_eof77: cs = 77; goto _test_eof; 
	_test_eof78: cs = 78; goto _test_eof; 
	_test_eof79: cs = 79; goto _test_eof; 
	_test_eof80: cs = 80; goto _test_eof; 
	_test_eof81: cs = 81; goto _test_eof; 
	_test_eof82: cs = 82; goto _test_eof; 
	_test_eof83: cs = 83; goto _test_eof; 
	_test_eof84: cs = 84; goto _test_eof; 
	_test_eof85: cs = 85; goto _test_eof; 
	_test_eof86: cs = 86; goto _test_eof; 
	_test_eof87: cs = 87; goto _test_eof; 
	_test_eof88: cs = 88; goto _test_eof; 
	_test_eof89: cs = 89; goto _test_eof; 
	_test_eof90: cs = 90; goto _test_eof; 
	_test_eof91: cs = 91; goto _test_eof; 
	_test_eof92: cs = 92; goto _test_eof; 
	_test_eof93: cs = 93; goto _test_eof; 
	_test_eof94: cs = 94; goto _test_eof; 
	_test_eof95: cs = 95; goto _test_eof; 
	_test_eof96: cs = 96; goto _test_eof; 
	_test_eof97: cs = 97; goto _test_eof; 
	_test_eof98: cs = 98; goto _test_eof; 
	_test_eof99: cs = 99; goto _test_eof; 
	_test_eof100: cs = 100; goto _test_eof; 
	_test_eof101: cs = 101; goto _test_eof; 
	_test_eof102: cs = 102; goto _test_eof; 
	_test_eof103: cs = 103; goto _test_eof; 
	_test_eof104: cs = 104; goto _test_eof; 
	_test_eof105: cs = 105; goto _test_eof; 
	_test_eof106: cs = 106; goto _test_eof; 
	_test_eof107: cs = 107; goto _test_eof; 
	_test_eof108: cs = 108; goto _test_eof; 
	_test_eof109: cs = 109; goto _test_eof; 
	_test_eof110: cs = 110; goto _test_eof; 
	_test_eof111: cs = 111; goto _test_eof; 
	_test_eof112: cs = 112; goto _test_eof; 
	_test_eof113: cs = 113; goto _test_eof; 
	_test_eof114: cs = 114; goto _test_eof; 
	_test_eof115: cs = 115; goto _test_eof; 
	_test_eof116: cs = 116; goto _test_eof; 
	_test_eof117: cs = 117; goto _test_eof; 
	_test_eof118: cs = 118; goto _test_eof; 
	_test_eof119: cs = 119; goto _test_eof; 
	_test_eof120: cs = 120; goto _test_eof; 
	_test_eof121: cs = 121; goto _test_eof; 
	_test_eof122: cs = 122; goto _test_eof; 
	_test_eof123: cs = 123; goto _test_eof; 
	_test_eof124: cs = 124; goto _test_eof; 
	_test_eof125: cs = 125; goto _test_eof; 
	_test_eof126: cs = 126; goto _test_eof; 
	_test_eof127: cs = 127; goto _test_eof; 
	_test_eof128: cs = 128; goto _test_eof; 
	_test_eof129: cs = 129; goto _test_eof; 
	_test_eof130: cs = 130; goto _test_eof; 
	_test_eof131: cs = 131; goto _test_eof; 
	_test_eof132: cs = 132; goto _test_eof; 
	_test_eof133: cs = 133; goto _test_eof; 
	_test_eof134: cs = 134; goto _test_eof; 
	_test_eof135: cs = 135; goto _test_eof; 
	_test_eof136: cs = 136; goto _test_eof; 
	_test_eof137: cs = 137; goto _test_eof; 
	_test_eof138: cs = 138; goto _test_eof; 
	_test_eof139: cs = 139; goto _test_eof; 
	_test_eof140: cs = 140; goto _test_eof; 
	_test_eof141: cs = 141; goto _test_eof; 
	_test_eof142: cs = 142; goto _test_eof; 
	_test_eof143: cs = 143; goto _test_eof; 
	_test_eof144: cs = 144; goto _test_eof; 
	_test_eof145: cs = 145; goto _test_eof; 
	_test_eof146: cs = 146; goto _test_eof; 
	_test_eof147: cs = 147; goto _test_eof; 
	_test_eof148: cs = 148; goto _test_eof; 
	_test_eof149: cs = 149; goto _test_eof; 
	_test_eof150: cs = 150; goto _test_eof; 
	_test_eof151: cs = 151; goto _test_eof; 
	_test_eof152: cs = 152; goto _test_eof; 
	_test_eof289: cs = 289; goto _test_eof; 
	_test_eof153: cs = 153; goto _test_eof; 
	_test_eof290: cs = 290; goto _test_eof; 
	_test_eof291: cs = 291; goto _test_eof; 
	_test_eof154: cs = 154; goto _test_eof; 
	_test_eof155: cs = 155; goto _test_eof; 
	_test_eof292: cs = 292; goto _test_eof; 
	_test_eof156: cs = 156; goto _test_eof; 
	_test_eof293: cs = 293; goto _test_eof; 
	_test_eof294: cs = 294; goto _test_eof; 
	_test_eof157: cs = 157; goto _test_eof; 
	_test_eof295: cs = 295; goto _test_eof; 
	_test_eof296: cs = 296; goto _test_eof; 
	_test_eof158: cs = 158; goto _test_eof; 
	_test_eof159: cs = 159; goto _test_eof; 
	_test_eof160: cs = 160; goto _test_eof; 
	_test_eof161: cs = 161; goto _test_eof; 
	_test_eof162: cs = 162; goto _test_eof; 
	_test_eof163: cs = 163; goto _test_eof; 
	_test_eof164: cs = 164; goto _test_eof; 
	_test_eof297: cs = 297; goto _test_eof; 
	_test_eof165: cs = 165; goto _test_eof; 
	_test_eof166: cs = 166; goto _test_eof; 
	_test_eof167: cs = 167; goto _test_eof; 
	_test_eof168: cs = 168; goto _test_eof; 
	_test_eof169: cs = 169; goto _test_eof; 
	_test_eof170: cs = 170; goto _test_eof; 
	_test_eof171: cs = 171; goto _test_eof; 
	_test_eof172: cs = 172; goto _test_eof; 
	_test_eof173: cs = 173; goto _test_eof; 
	_test_eof174: cs = 174; goto _test_eof; 
	_test_eof175: cs = 175; goto _test_eof; 
	_test_eof176: cs = 176; goto _test_eof; 
	_test_eof177: cs = 177; goto _test_eof; 
	_test_eof298: cs = 298; goto _test_eof; 
	_test_eof178: cs = 178; goto _test_eof; 
	_test_eof299: cs = 299; goto _test_eof; 
	_test_eof300: cs = 300; goto _test_eof; 
	_test_eof301: cs = 301; goto _test_eof; 
	_test_eof179: cs = 179; goto _test_eof; 
	_test_eof180: cs = 180; goto _test_eof; 
	_test_eof181: cs = 181; goto _test_eof; 
	_test_eof182: cs = 182; goto _test_eof; 
	_test_eof183: cs = 183; goto _test_eof; 
	_test_eof184: cs = 184; goto _test_eof; 
	_test_eof185: cs = 185; goto _test_eof; 
	_test_eof186: cs = 186; goto _test_eof; 
	_test_eof187: cs = 187; goto _test_eof; 
	_test_eof188: cs = 188; goto _test_eof; 
	_test_eof189: cs = 189; goto _test_eof; 
	_test_eof302: cs = 302; goto _test_eof; 
	_test_eof303: cs = 303; goto _test_eof; 
	_test_eof304: cs = 304; goto _test_eof; 
	_test_eof305: cs = 305; goto _test_eof; 
	_test_eof306: cs = 306; goto _test_eof; 
	_test_eof307: cs = 307; goto _test_eof; 
	_test_eof308: cs = 308; goto _test_eof; 
	_test_eof309: cs = 309; goto _test_eof; 
	_test_eof310: cs = 310; goto _test_eof; 
	_test_eof311: cs = 311; goto _test_eof; 
	_test_eof312: cs = 312; goto _test_eof; 
	_test_eof313: cs = 313; goto _test_eof; 
	_test_eof314: cs = 314; goto _test_eof; 
	_test_eof315: cs = 315; goto _test_eof; 
	_test_eof316: cs = 316; goto _test_eof; 
	_test_eof317: cs = 317; goto _test_eof; 
	_test_eof318: cs = 318; goto _test_eof; 
	_test_eof319: cs = 319; goto _test_eof; 
	_test_eof320: cs = 320; goto _test_eof; 
	_test_eof321: cs = 321; goto _test_eof; 
	_test_eof322: cs = 322; goto _test_eof; 
	_test_eof323: cs = 323; goto _test_eof; 
	_test_eof324: cs = 324; goto _test_eof; 
	_test_eof325: cs = 325; goto _test_eof; 
	_test_eof326: cs = 326; goto _test_eof; 
	_test_eof327: cs = 327; goto _test_eof; 
	_test_eof328: cs = 328; goto _test_eof; 
	_test_eof329: cs = 329; goto _test_eof; 
	_test_eof330: cs = 330; goto _test_eof; 
	_test_eof331: cs = 331; goto _test_eof; 
	_test_eof332: cs = 332; goto _test_eof; 
	_test_eof333: cs = 333; goto _test_eof; 
	_test_eof334: cs = 334; goto _test_eof; 
	_test_eof335: cs = 335; goto _test_eof; 
	_test_eof336: cs = 336; goto _test_eof; 
	_test_eof337: cs = 337; goto _test_eof; 
	_test_eof338: cs = 338; goto _test_eof; 
	_test_eof339: cs = 339; goto _test_eof; 
	_test_eof340: cs = 340; goto _test_eof; 
	_test_eof341: cs = 341; goto _test_eof; 
	_test_eof342: cs = 342; goto _test_eof; 
	_test_eof343: cs = 343; goto _test_eof; 
	_test_eof344: cs = 344; goto _test_eof; 
	_test_eof345: cs = 345; goto _test_eof; 
	_test_eof346: cs = 346; goto _test_eof; 
	_test_eof347: cs = 347; goto _test_eof; 
	_test_eof348: cs = 348; goto _test_eof; 
	_test_eof349: cs = 349; goto _test_eof; 
	_test_eof350: cs = 350; goto _test_eof; 
	_test_eof351: cs = 351; goto _test_eof; 
	_test_eof352: cs = 352; goto _test_eof; 
	_test_eof353: cs = 353; goto _test_eof; 
	_test_eof354: cs = 354; goto _test_eof; 
	_test_eof355: cs = 355; goto _test_eof; 
	_test_eof356: cs = 356; goto _test_eof; 
	_test_eof357: cs = 357; goto _test_eof; 
	_test_eof358: cs = 358; goto _test_eof; 
	_test_eof359: cs = 359; goto _test_eof; 
	_test_eof360: cs = 360; goto _test_eof; 
	_test_eof361: cs = 361; goto _test_eof; 
	_test_eof362: cs = 362; goto _test_eof; 
	_test_eof363: cs = 363; goto _test_eof; 
	_test_eof364: cs = 364; goto _test_eof; 
	_test_eof365: cs = 365; goto _test_eof; 
	_test_eof366: cs = 366; goto _test_eof; 
	_test_eof367: cs = 367; goto _test_eof; 
	_test_eof368: cs = 368; goto _test_eof; 
	_test_eof369: cs = 369; goto _test_eof; 
	_test_eof370: cs = 370; goto _test_eof; 
	_test_eof371: cs = 371; goto _test_eof; 
	_test_eof372: cs = 372; goto _test_eof; 
	_test_eof373: cs = 373; goto _test_eof; 
	_test_eof374: cs = 374; goto _test_eof; 
	_test_eof375: cs = 375; goto _test_eof; 
	_test_eof376: cs = 376; goto _test_eof; 
	_test_eof377: cs = 377; goto _test_eof; 
	_test_eof378: cs = 378; goto _test_eof; 
	_test_eof379: cs = 379; goto _test_eof; 
	_test_eof380: cs = 380; goto _test_eof; 
	_test_eof381: cs = 381; goto _test_eof; 
	_test_eof382: cs = 382; goto _test_eof; 
	_test_eof383: cs = 383; goto _test_eof; 
	_test_eof384: cs = 384; goto _test_eof; 
	_test_eof385: cs = 385; goto _test_eof; 
	_test_eof386: cs = 386; goto _test_eof; 
	_test_eof387: cs = 387; goto _test_eof; 
	_test_eof388: cs = 388; goto _test_eof; 
	_test_eof389: cs = 389; goto _test_eof; 
	_test_eof390: cs = 390; goto _test_eof; 
	_test_eof391: cs = 391; goto _test_eof; 
	_test_eof392: cs = 392; goto _test_eof; 
	_test_eof393: cs = 393; goto _test_eof; 
	_test_eof394: cs = 394; goto _test_eof; 
	_test_eof395: cs = 395; goto _test_eof; 
	_test_eof396: cs = 396; goto _test_eof; 
	_test_eof397: cs = 397; goto _test_eof; 
	_test_eof398: cs = 398; goto _test_eof; 
	_test_eof399: cs = 399; goto _test_eof; 
	_test_eof400: cs = 400; goto _test_eof; 
	_test_eof401: cs = 401; goto _test_eof; 
	_test_eof402: cs = 402; goto _test_eof; 
	_test_eof403: cs = 403; goto _test_eof; 
	_test_eof404: cs = 404; goto _test_eof; 
	_test_eof405: cs = 405; goto _test_eof; 
	_test_eof406: cs = 406; goto _test_eof; 
	_test_eof407: cs = 407; goto _test_eof; 
	_test_eof408: cs = 408; goto _test_eof; 
	_test_eof409: cs = 409; goto _test_eof; 
	_test_eof410: cs = 410; goto _test_eof; 
	_test_eof411: cs = 411; goto _test_eof; 
	_test_eof412: cs = 412; goto _test_eof; 
	_test_eof413: cs = 413; goto _test_eof; 
	_test_eof414: cs = 414; goto _test_eof; 
	_test_eof415: cs = 415; goto _test_eof; 
	_test_eof416: cs = 416; goto _test_eof; 
	_test_eof417: cs = 417; goto _test_eof; 
	_test_eof418: cs = 418; goto _test_eof; 
	_test_eof419: cs = 419; goto _test_eof; 
	_test_eof420: cs = 420; goto _test_eof; 
	_test_eof421: cs = 421; goto _test_eof; 
	_test_eof422: cs = 422; goto _test_eof; 
	_test_eof423: cs = 423; goto _test_eof; 
	_test_eof424: cs = 424; goto _test_eof; 
	_test_eof425: cs = 425; goto _test_eof; 
	_test_eof426: cs = 426; goto _test_eof; 
	_test_eof427: cs = 427; goto _test_eof; 
	_test_eof428: cs = 428; goto _test_eof; 
	_test_eof429: cs = 429; goto _test_eof; 
	_test_eof430: cs = 430; goto _test_eof; 
	_test_eof431: cs = 431; goto _test_eof; 
	_test_eof432: cs = 432; goto _test_eof; 
	_test_eof433: cs = 433; goto _test_eof; 
	_test_eof434: cs = 434; goto _test_eof; 
	_test_eof435: cs = 435; goto _test_eof; 
	_test_eof436: cs = 436; goto _test_eof; 
	_test_eof437: cs = 437; goto _test_eof; 
	_test_eof438: cs = 438; goto _test_eof; 
	_test_eof439: cs = 439; goto _test_eof; 
	_test_eof440: cs = 440; goto _test_eof; 
	_test_eof441: cs = 441; goto _test_eof; 
	_test_eof442: cs = 442; goto _test_eof; 
	_test_eof443: cs = 443; goto _test_eof; 
	_test_eof444: cs = 444; goto _test_eof; 
	_test_eof445: cs = 445; goto _test_eof; 
	_test_eof446: cs = 446; goto _test_eof; 
	_test_eof447: cs = 447; goto _test_eof; 
	_test_eof448: cs = 448; goto _test_eof; 
	_test_eof449: cs = 449; goto _test_eof; 
	_test_eof450: cs = 450; goto _test_eof; 
	_test_eof451: cs = 451; goto _test_eof; 
	_test_eof452: cs = 452; goto _test_eof; 
	_test_eof453: cs = 453; goto _test_eof; 
	_test_eof454: cs = 454; goto _test_eof; 
	_test_eof455: cs = 455; goto _test_eof; 
	_test_eof456: cs = 456; goto _test_eof; 
	_test_eof457: cs = 457; goto _test_eof; 
	_test_eof458: cs = 458; goto _test_eof; 
	_test_eof459: cs = 459; goto _test_eof; 
	_test_eof460: cs = 460; goto _test_eof; 
	_test_eof461: cs = 461; goto _test_eof; 
	_test_eof462: cs = 462; goto _test_eof; 
	_test_eof463: cs = 463; goto _test_eof; 
	_test_eof464: cs = 464; goto _test_eof; 
	_test_eof465: cs = 465; goto _test_eof; 
	_test_eof466: cs = 466; goto _test_eof; 
	_test_eof467: cs = 467; goto _test_eof; 
	_test_eof468: cs = 468; goto _test_eof; 
	_test_eof469: cs = 469; goto _test_eof; 
	_test_eof470: cs = 470; goto _test_eof; 
	_test_eof471: cs = 471; goto _test_eof; 
	_test_eof472: cs = 472; goto _test_eof; 
	_test_eof473: cs = 473; goto _test_eof; 
	_test_eof474: cs = 474; goto _test_eof; 
	_test_eof475: cs = 475; goto _test_eof; 
	_test_eof476: cs = 476; goto _test_eof; 
	_test_eof477: cs = 477; goto _test_eof; 
	_test_eof478: cs = 478; goto _test_eof; 
	_test_eof479: cs = 479; goto _test_eof; 
	_test_eof480: cs = 480; goto _test_eof; 
	_test_eof481: cs = 481; goto _test_eof; 
	_test_eof482: cs = 482; goto _test_eof; 
	_test_eof483: cs = 483; goto _test_eof; 
	_test_eof484: cs = 484; goto _test_eof; 
	_test_eof485: cs = 485; goto _test_eof; 
	_test_eof486: cs = 486; goto _test_eof; 
	_test_eof487: cs = 487; goto _test_eof; 
	_test_eof488: cs = 488; goto _test_eof; 
	_test_eof489: cs = 489; goto _test_eof; 
	_test_eof490: cs = 490; goto _test_eof; 
	_test_eof491: cs = 491; goto _test_eof; 
	_test_eof492: cs = 492; goto _test_eof; 
	_test_eof493: cs = 493; goto _test_eof; 
	_test_eof494: cs = 494; goto _test_eof; 
	_test_eof495: cs = 495; goto _test_eof; 
	_test_eof496: cs = 496; goto _test_eof; 
	_test_eof497: cs = 497; goto _test_eof; 
	_test_eof498: cs = 498; goto _test_eof; 
	_test_eof499: cs = 499; goto _test_eof; 
	_test_eof500: cs = 500; goto _test_eof; 
	_test_eof501: cs = 501; goto _test_eof; 
	_test_eof502: cs = 502; goto _test_eof; 
	_test_eof503: cs = 503; goto _test_eof; 
	_test_eof504: cs = 504; goto _test_eof; 
	_test_eof505: cs = 505; goto _test_eof; 
	_test_eof506: cs = 506; goto _test_eof; 
	_test_eof507: cs = 507; goto _test_eof; 
	_test_eof508: cs = 508; goto _test_eof; 
	_test_eof509: cs = 509; goto _test_eof; 
	_test_eof510: cs = 510; goto _test_eof; 
	_test_eof511: cs = 511; goto _test_eof; 
	_test_eof512: cs = 512; goto _test_eof; 
	_test_eof513: cs = 513; goto _test_eof; 
	_test_eof514: cs = 514; goto _test_eof; 
	_test_eof515: cs = 515; goto _test_eof; 
	_test_eof516: cs = 516; goto _test_eof; 
	_test_eof517: cs = 517; goto _test_eof; 
	_test_eof518: cs = 518; goto _test_eof; 
	_test_eof519: cs = 519; goto _test_eof; 
	_test_eof520: cs = 520; goto _test_eof; 
	_test_eof521: cs = 521; goto _test_eof; 
	_test_eof522: cs = 522; goto _test_eof; 
	_test_eof523: cs = 523; goto _test_eof; 
	_test_eof524: cs = 524; goto _test_eof; 
	_test_eof525: cs = 525; goto _test_eof; 
	_test_eof526: cs = 526; goto _test_eof; 
	_test_eof527: cs = 527; goto _test_eof; 
	_test_eof528: cs = 528; goto _test_eof; 
	_test_eof529: cs = 529; goto _test_eof; 
	_test_eof530: cs = 530; goto _test_eof; 
	_test_eof531: cs = 531; goto _test_eof; 
	_test_eof532: cs = 532; goto _test_eof; 
	_test_eof533: cs = 533; goto _test_eof; 
	_test_eof534: cs = 534; goto _test_eof; 
	_test_eof535: cs = 535; goto _test_eof; 
	_test_eof536: cs = 536; goto _test_eof; 
	_test_eof537: cs = 537; goto _test_eof; 
	_test_eof538: cs = 538; goto _test_eof; 
	_test_eof539: cs = 539; goto _test_eof; 
	_test_eof540: cs = 540; goto _test_eof; 
	_test_eof541: cs = 541; goto _test_eof; 
	_test_eof542: cs = 542; goto _test_eof; 
	_test_eof543: cs = 543; goto _test_eof; 
	_test_eof544: cs = 544; goto _test_eof; 
	_test_eof545: cs = 545; goto _test_eof; 
	_test_eof546: cs = 546; goto _test_eof; 
	_test_eof547: cs = 547; goto _test_eof; 
	_test_eof548: cs = 548; goto _test_eof; 
	_test_eof549: cs = 549; goto _test_eof; 
	_test_eof550: cs = 550; goto _test_eof; 
	_test_eof551: cs = 551; goto _test_eof; 
	_test_eof552: cs = 552; goto _test_eof; 
	_test_eof553: cs = 553; goto _test_eof; 
	_test_eof554: cs = 554; goto _test_eof; 
	_test_eof555: cs = 555; goto _test_eof; 
	_test_eof556: cs = 556; goto _test_eof; 
	_test_eof557: cs = 557; goto _test_eof; 
	_test_eof558: cs = 558; goto _test_eof; 
	_test_eof559: cs = 559; goto _test_eof; 
	_test_eof560: cs = 560; goto _test_eof; 
	_test_eof561: cs = 561; goto _test_eof; 
	_test_eof562: cs = 562; goto _test_eof; 
	_test_eof563: cs = 563; goto _test_eof; 
	_test_eof564: cs = 564; goto _test_eof; 
	_test_eof565: cs = 565; goto _test_eof; 
	_test_eof566: cs = 566; goto _test_eof; 
	_test_eof567: cs = 567; goto _test_eof; 
	_test_eof568: cs = 568; goto _test_eof; 
	_test_eof569: cs = 569; goto _test_eof; 
	_test_eof570: cs = 570; goto _test_eof; 
	_test_eof571: cs = 571; goto _test_eof; 
	_test_eof572: cs = 572; goto _test_eof; 
	_test_eof573: cs = 573; goto _test_eof; 
	_test_eof574: cs = 574; goto _test_eof; 
	_test_eof575: cs = 575; goto _test_eof; 
	_test_eof576: cs = 576; goto _test_eof; 
	_test_eof577: cs = 577; goto _test_eof; 
	_test_eof578: cs = 578; goto _test_eof; 
	_test_eof579: cs = 579; goto _test_eof; 
	_test_eof580: cs = 580; goto _test_eof; 
	_test_eof581: cs = 581; goto _test_eof; 
	_test_eof582: cs = 582; goto _test_eof; 
	_test_eof583: cs = 583; goto _test_eof; 
	_test_eof584: cs = 584; goto _test_eof; 
	_test_eof585: cs = 585; goto _test_eof; 
	_test_eof586: cs = 586; goto _test_eof; 
	_test_eof587: cs = 587; goto _test_eof; 
	_test_eof588: cs = 588; goto _test_eof; 
	_test_eof589: cs = 589; goto _test_eof; 
	_test_eof590: cs = 590; goto _test_eof; 
	_test_eof591: cs = 591; goto _test_eof; 
	_test_eof592: cs = 592; goto _test_eof; 
	_test_eof593: cs = 593; goto _test_eof; 
	_test_eof594: cs = 594; goto _test_eof; 
	_test_eof595: cs = 595; goto _test_eof; 
	_test_eof596: cs = 596; goto _test_eof; 
	_test_eof597: cs = 597; goto _test_eof; 
	_test_eof598: cs = 598; goto _test_eof; 
	_test_eof599: cs = 599; goto _test_eof; 
	_test_eof600: cs = 600; goto _test_eof; 
	_test_eof601: cs = 601; goto _test_eof; 
	_test_eof602: cs = 602; goto _test_eof; 
	_test_eof603: cs = 603; goto _test_eof; 
	_test_eof604: cs = 604; goto _test_eof; 
	_test_eof605: cs = 605; goto _test_eof; 
	_test_eof606: cs = 606; goto _test_eof; 
	_test_eof607: cs = 607; goto _test_eof; 
	_test_eof608: cs = 608; goto _test_eof; 
	_test_eof609: cs = 609; goto _test_eof; 
	_test_eof610: cs = 610; goto _test_eof; 
	_test_eof611: cs = 611; goto _test_eof; 
	_test_eof612: cs = 612; goto _test_eof; 
	_test_eof613: cs = 613; goto _test_eof; 
	_test_eof614: cs = 614; goto _test_eof; 
	_test_eof615: cs = 615; goto _test_eof; 
	_test_eof616: cs = 616; goto _test_eof; 
	_test_eof617: cs = 617; goto _test_eof; 
	_test_eof618: cs = 618; goto _test_eof; 
	_test_eof619: cs = 619; goto _test_eof; 
	_test_eof620: cs = 620; goto _test_eof; 
	_test_eof621: cs = 621; goto _test_eof; 
	_test_eof622: cs = 622; goto _test_eof; 
	_test_eof190: cs = 190; goto _test_eof; 
	_test_eof191: cs = 191; goto _test_eof; 
	_test_eof623: cs = 623; goto _test_eof; 
	_test_eof192: cs = 192; goto _test_eof; 
	_test_eof624: cs = 624; goto _test_eof; 
	_test_eof193: cs = 193; goto _test_eof; 
	_test_eof194: cs = 194; goto _test_eof; 
	_test_eof195: cs = 195; goto _test_eof; 
	_test_eof196: cs = 196; goto _test_eof; 
	_test_eof197: cs = 197; goto _test_eof; 
	_test_eof198: cs = 198; goto _test_eof; 
	_test_eof199: cs = 199; goto _test_eof; 
	_test_eof625: cs = 625; goto _test_eof; 
	_test_eof626: cs = 626; goto _test_eof; 
	_test_eof200: cs = 200; goto _test_eof; 
	_test_eof201: cs = 201; goto _test_eof; 
	_test_eof202: cs = 202; goto _test_eof; 
	_test_eof203: cs = 203; goto _test_eof; 
	_test_eof204: cs = 204; goto _test_eof; 
	_test_eof627: cs = 627; goto _test_eof; 
	_test_eof628: cs = 628; goto _test_eof; 
	_test_eof205: cs = 205; goto _test_eof; 
	_test_eof629: cs = 629; goto _test_eof; 
	_test_eof206: cs = 206; goto _test_eof; 
	_test_eof207: cs = 207; goto _test_eof; 
	_test_eof208: cs = 208; goto _test_eof; 
	_test_eof209: cs = 209; goto _test_eof; 
	_test_eof210: cs = 210; goto _test_eof; 
	_test_eof630: cs = 630; goto _test_eof; 
	_test_eof211: cs = 211; goto _test_eof; 
	_test_eof212: cs = 212; goto _test_eof; 
	_test_eof213: cs = 213; goto _test_eof; 
	_test_eof214: cs = 214; goto _test_eof; 
	_test_eof215: cs = 215; goto _test_eof; 
	_test_eof216: cs = 216; goto _test_eof; 
	_test_eof631: cs = 631; goto _test_eof; 
	_test_eof217: cs = 217; goto _test_eof; 
	_test_eof218: cs = 218; goto _test_eof; 
	_test_eof219: cs = 219; goto _test_eof; 
	_test_eof220: cs = 220; goto _test_eof; 
	_test_eof221: cs = 221; goto _test_eof; 
	_test_eof222: cs = 222; goto _test_eof; 
	_test_eof223: cs = 223; goto _test_eof; 
	_test_eof224: cs = 224; goto _test_eof; 
	_test_eof225: cs = 225; goto _test_eof; 
	_test_eof226: cs = 226; goto _test_eof; 
	_test_eof227: cs = 227; goto _test_eof; 
	_test_eof228: cs = 228; goto _test_eof; 
	_test_eof632: cs = 632; goto _test_eof; 
	_test_eof229: cs = 229; goto _test_eof; 
	_test_eof633: cs = 633; goto _test_eof; 
	_test_eof634: cs = 634; goto _test_eof; 
	_test_eof230: cs = 230; goto _test_eof; 
	_test_eof231: cs = 231; goto _test_eof; 
	_test_eof635: cs = 635; goto _test_eof; 
	_test_eof232: cs = 232; goto _test_eof; 
	_test_eof233: cs = 233; goto _test_eof; 
	_test_eof234: cs = 234; goto _test_eof; 
	_test_eof636: cs = 636; goto _test_eof; 
	_test_eof235: cs = 235; goto _test_eof; 
	_test_eof236: cs = 236; goto _test_eof; 
	_test_eof637: cs = 637; goto _test_eof; 
	_test_eof638: cs = 638; goto _test_eof; 
	_test_eof237: cs = 237; goto _test_eof; 
	_test_eof639: cs = 639; goto _test_eof; 
	_test_eof640: cs = 640; goto _test_eof; 
	_test_eof238: cs = 238; goto _test_eof; 
	_test_eof641: cs = 641; goto _test_eof; 
	_test_eof642: cs = 642; goto _test_eof; 
	_test_eof239: cs = 239; goto _test_eof; 
	_test_eof643: cs = 643; goto _test_eof; 
	_test_eof240: cs = 240; goto _test_eof; 
	_test_eof241: cs = 241; goto _test_eof; 
	_test_eof242: cs = 242; goto _test_eof; 
	_test_eof243: cs = 243; goto _test_eof; 
	_test_eof244: cs = 244; goto _test_eof; 
	_test_eof245: cs = 245; goto _test_eof; 
	_test_eof246: cs = 246; goto _test_eof; 
	_test_eof644: cs = 644; goto _test_eof; 
	_test_eof247: cs = 247; goto _test_eof; 
	_test_eof248: cs = 248; goto _test_eof; 
	_test_eof249: cs = 249; goto _test_eof; 
	_test_eof250: cs = 250; goto _test_eof; 
	_test_eof251: cs = 251; goto _test_eof; 
	_test_eof252: cs = 252; goto _test_eof; 
	_test_eof253: cs = 253; goto _test_eof; 
	_test_eof254: cs = 254; goto _test_eof; 
	_test_eof255: cs = 255; goto _test_eof; 
	_test_eof256: cs = 256; goto _test_eof; 
	_test_eof257: cs = 257; goto _test_eof; 
	_test_eof645: cs = 645; goto _test_eof; 
	_test_eof258: cs = 258; goto _test_eof; 
	_test_eof646: cs = 646; goto _test_eof; 
	_test_eof647: cs = 647; goto _test_eof; 
	_test_eof259: cs = 259; goto _test_eof; 
	_test_eof260: cs = 260; goto _test_eof; 
	_test_eof261: cs = 261; goto _test_eof; 
	_test_eof262: cs = 262; goto _test_eof; 
	_test_eof263: cs = 263; goto _test_eof; 
	_test_eof264: cs = 264; goto _test_eof; 
	_test_eof265: cs = 265; goto _test_eof; 
	_test_eof266: cs = 266; goto _test_eof; 
	_test_eof648: cs = 648; goto _test_eof; 
	_test_eof267: cs = 267; goto _test_eof; 
	_test_eof649: cs = 649; goto _test_eof; 
	_test_eof650: cs = 650; goto _test_eof; 

	_test_eof: {}
	if ( p == eof )
	{
	switch ( cs ) {
	case 298: 
	case 299: 
	case 300: 
	case 301: 
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
	break;
	case 287: 
#line 58 "lex/ragel_numeric.rl"
	{
        const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
        ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
        ragel_type = TYPE_FLOAT;
        ragel_cat  = CAT_NUMBER;
    }
	break;
	case 297: 
#line 120 "lex/parser_timeline.rl"
	{ fsm.onEventIdDone(); tl.addEvent(fsm.event_id); }
	break;
	case 624: 
#line 122 "lex/parser_timeline.rl"
	{ fsm.act_out.args = fsm.args;  tl.addEventAction(fsm.event_id, fsm.act_out); }
	break;
	case 303: 
	case 304: 
	case 305: 
	case 306: 
	case 307: 
	case 308: 
	case 309: 
	case 310: 
	case 311: 
	case 312: 
	case 313: 
	case 314: 
	case 315: 
	case 316: 
	case 318: 
	case 322: 
	case 323: 
	case 324: 
	case 325: 
	case 326: 
	case 327: 
	case 328: 
	case 329: 
	case 330: 
	case 331: 
	case 332: 
	case 333: 
	case 334: 
	case 335: 
	case 336: 
	case 337: 
	case 338: 
	case 339: 
	case 340: 
	case 341: 
	case 342: 
	case 343: 
	case 344: 
	case 345: 
	case 347: 
	case 351: 
	case 352: 
	case 353: 
	case 354: 
	case 355: 
	case 356: 
	case 357: 
	case 358: 
	case 359: 
	case 477: 
	case 478: 
	case 479: 
	case 480: 
	case 481: 
	case 482: 
	case 483: 
	case 486: 
	case 487: 
	case 488: 
	case 489: 
	case 490: 
	case 598: 
	case 600: 
	case 601: 
	case 602: 
	case 604: 
	case 605: 
	case 606: 
	case 607: 
	case 608: 
	case 609: 
	case 610: 
	case 613: 
	case 614: 
	case 615: 
	case 616: 
	case 617: 
	case 619: 
	case 621: 
	case 622: 
#line 124 "lex/parser_timeline.rl"
	{ fsm.onArgsDone(); }
	break;
	case 277: 
#line 127 "lex/parser_timeline.rl"
	{
        fsm.onEventIdDone();
        for (auto& t: event_time.times)
            tl.addEventAt(fsm.event_id, t, event_time.relative);

        event_time.reset();
    }
	break;
	case 269: 
#line 144 "lex/parser_timeline.rl"
	{
        auto x = fsm.genAnonEventName();
        tl.addEvent(x);
        fsm.act_out.args = fsm.args;
        tl.addEventAction(x, fsm.act_out);
        tl.addEventAt(x, event_time.time, event_time.relative);
        event_time.reset();
    }
	break;
	case 644: 
#line 230 "lex/parser_timeline.rl"
	{
        TimeLineVarDef var;
        var.name = gensym(fsm.symbol.c_str());
        tl.var_defs.push_back(var);
    }
	break;
	case 630: 
#line 342 "lex/parser_timeline.rl"
	{ tl.duration = fsm.time_unit; }
	break;
	case 289: 
	case 290: 
	case 291: 
	case 292: 
	case 294: 
	case 295: 
	case 296: 
#line 10 "lex/ragel_music.rl"
	{ ragel_cat = CAT_UNIT; ragel_type = TYPE_BPM; }
#line 348 "lex/parser_timeline.rl"
	{ tl.setBarBpm(fsm.bpm_bar, fromRagel(bpm)); tl.calcBarDuration(bar.inf); }
	break;
	case 286: 
	case 288: 
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 7 "lex/ragel_units.rl"
	{ ragel_num.vdouble = ragel_num.vint; }
	break;
	case 317: 
	case 319: 
	case 320: 
	case 321: 
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 124 "lex/parser_timeline.rl"
	{ fsm.onArgsDone(); }
	break;
	case 625: 
	case 626: 
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 125 "lex/parser_timeline.rl"
	{ tl.addEventAction(fsm.event_id, fsm.act_preset); }
	break;
	case 271: 
	case 272: 
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 153 "lex/parser_timeline.rl"
	{
        auto x = fsm.genAnonEventName();
        tl.addEvent(x);
        tl.addEventAction(x, fsm.act_preset);
        tl.addEventAt(x, event_time.time, event_time.relative);
        event_time.reset();
    }
	break;
	case 646: 
#line 58 "lex/ragel_numeric.rl"
	{
        const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
        ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
        ragel_type = TYPE_FLOAT;
        ragel_cat  = CAT_NUMBER;
    }
#line 285 "lex/parser_timeline.rl"
	{ tl.var_defs.back().def = ragel_num.getFloat(); }
	break;
	case 649: 
#line 58 "lex/ragel_numeric.rl"
	{
        const auto frac = double(ragel_num.ratio.num) / ragel_num.ratio.den;
        ragel_num.vdouble = ragel_num.vint + ragel_num.sign * frac;
        ragel_type = TYPE_FLOAT;
        ragel_cat  = CAT_NUMBER;
    }
#line 286 "lex/parser_timeline.rl"
	{ tl.var_defs.back().preset = ragel_num.getFloat(); }
	break;
	case 278: 
	case 279: 
#line 17 "lex/ragel_units.rl"
	{ragel_type = TYPE_HOUR;}
#line 103 "lex/parser_timeline.rl"
	{
        switch(ragel_type) {
        case TYPE_MSEC:  fsm.time_unit = ragel_num.getFloat();   break;
        case TYPE_SEC:   fsm.time_unit = ragel_num.getFloat() * 1000; break;
        case TYPE_MIN:   fsm.time_unit = ragel_num.getFloat() * 60000; break;
        case TYPE_HOUR:  fsm.time_unit = ragel_num.getFloat() * 3600000; break;
        default:
        break;
        }
    }
	break;
	case 280: 
	case 281: 
#line 18 "lex/ragel_units.rl"
	{ragel_type = TYPE_MIN;}
#line 103 "lex/parser_timeline.rl"
	{
        switch(ragel_type) {
        case TYPE_MSEC:  fsm.time_unit = ragel_num.getFloat();   break;
        case TYPE_SEC:   fsm.time_unit = ragel_num.getFloat() * 1000; break;
        case TYPE_MIN:   fsm.time_unit = ragel_num.getFloat() * 60000; break;
        case TYPE_HOUR:  fsm.time_unit = ragel_num.getFloat() * 3600000; break;
        default:
        break;
        }
    }
	break;
	case 282: 
	case 283: 
#line 19 "lex/ragel_units.rl"
	{ragel_type = TYPE_MSEC;}
#line 103 "lex/parser_timeline.rl"
	{
        switch(ragel_type) {
        case TYPE_MSEC:  fsm.time_unit = ragel_num.getFloat();   break;
        case TYPE_SEC:   fsm.time_unit = ragel_num.getFloat() * 1000; break;
        case TYPE_MIN:   fsm.time_unit = ragel_num.getFloat() * 60000; break;
        case TYPE_HOUR:  fsm.time_unit = ragel_num.getFloat() * 3600000; break;
        default:
        break;
        }
    }
	break;
	case 284: 
	case 285: 
#line 22 "lex/ragel_units.rl"
	{ragel_type = TYPE_SEC;}
#line 103 "lex/parser_timeline.rl"
	{
        switch(ragel_type) {
        case TYPE_MSEC:  fsm.time_unit = ragel_num.getFloat();   break;
        case TYPE_SEC:   fsm.time_unit = ragel_num.getFloat() * 1000; break;
        case TYPE_MIN:   fsm.time_unit = ragel_num.getFloat() * 60000; break;
        case TYPE_HOUR:  fsm.time_unit = ragel_num.getFloat() * 3600000; break;
        default:
        break;
        }
    }
	break;
	case 360: 
	case 603: 
#line 122 "lex/parser_timeline.rl"
	{ fsm.act_out.args = fsm.args;  tl.addEventAction(fsm.event_id, fsm.act_out); }
#line 124 "lex/parser_timeline.rl"
	{ fsm.onArgsDone(); }
	break;
	case 627: 
	case 628: 
	case 629: 
#line 123 "lex/parser_timeline.rl"
	{ fsm.onSendTarget(); }
#line 121 "lex/parser_timeline.rl"
	{ fsm.act_send.args = fsm.args; tl.addEventAction(fsm.event_id, fsm.act_send); }
	break;
	case 273: 
	case 275: 
	case 276: 
#line 123 "lex/parser_timeline.rl"
	{ fsm.onSendTarget(); }
#line 135 "lex/parser_timeline.rl"
	{
        auto x = fsm.genAnonEventName();
        tl.addEvent(x);
        fsm.act_send.args = fsm.args;
        tl.addEventAction(x, fsm.act_send);
        tl.addEventAt(x, event_time.time, event_time.relative);
        event_time.reset();
    }
	break;
	case 346: 
	case 348: 
	case 349: 
	case 350: 
#line 124 "lex/parser_timeline.rl"
	{ fsm.onArgsDone(); }
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
	break;
	case 492: 
	case 493: 
	case 494: 
	case 495: 
	case 496: 
	case 497: 
	case 498: 
	case 499: 
	case 500: 
	case 501: 
	case 502: 
	case 503: 
	case 504: 
	case 505: 
	case 507: 
	case 511: 
	case 512: 
	case 513: 
	case 514: 
	case 515: 
	case 516: 
	case 517: 
	case 518: 
	case 519: 
	case 521: 
	case 522: 
	case 523: 
	case 524: 
	case 525: 
	case 526: 
	case 527: 
	case 530: 
	case 531: 
	case 532: 
	case 533: 
	case 534: 
	case 565: 
	case 567: 
	case 568: 
#line 124 "lex/parser_timeline.rl"
	{ fsm.onArgsDone(); }
#line 121 "lex/parser_timeline.rl"
	{ fsm.act_send.args = fsm.args; tl.addEventAction(fsm.event_id, fsm.act_send); }
	break;
	case 361: 
	case 362: 
	case 363: 
	case 364: 
	case 365: 
	case 366: 
	case 367: 
	case 368: 
	case 369: 
	case 370: 
	case 371: 
	case 372: 
	case 373: 
	case 374: 
	case 376: 
	case 380: 
	case 381: 
	case 382: 
	case 383: 
	case 384: 
	case 385: 
	case 386: 
	case 387: 
	case 388: 
	case 389: 
	case 390: 
	case 391: 
	case 392: 
	case 393: 
	case 396: 
	case 397: 
	case 398: 
	case 399: 
	case 400: 
	case 473: 
	case 475: 
	case 476: 
#line 124 "lex/parser_timeline.rl"
	{ fsm.onArgsDone(); }
#line 122 "lex/parser_timeline.rl"
	{ fsm.act_out.args = fsm.args;  tl.addEventAction(fsm.event_id, fsm.act_out); }
	break;
	case 274: 
#line 124 "lex/parser_timeline.rl"
	{ fsm.onArgsDone(); }
#line 135 "lex/parser_timeline.rl"
	{
        auto x = fsm.genAnonEventName();
        tl.addEvent(x);
        fsm.act_send.args = fsm.args;
        tl.addEventAction(x, fsm.act_send);
        tl.addEventAt(x, event_time.time, event_time.relative);
        event_time.reset();
    }
	break;
	case 270: 
#line 124 "lex/parser_timeline.rl"
	{ fsm.onArgsDone(); }
#line 144 "lex/parser_timeline.rl"
	{
        auto x = fsm.genAnonEventName();
        tl.addEvent(x);
        fsm.act_out.args = fsm.args;
        tl.addEventAction(x, fsm.act_out);
        tl.addEventAt(x, event_time.time, event_time.relative);
        event_time.reset();
    }
	break;
	case 631: 
#line 219 "lex/parser_timeline.rl"
	{
        if (bar.inf) {
            tl.duration = std::numeric_limits<t_float>::max();
        } else {
            TimeLineBar b;
            b.count = bar.count;
            b.sig.set(bar.num, bar.div);
            tl.bars.push_back(b);
        }
    }
#line 340 "lex/parser_timeline.rl"
	{ tl.calcBarDuration(bar.inf); }
	break;
	case 293: 
#line 24 "lex/ragel_music.rl"
	{bpm.dur_num = 1;}
#line 10 "lex/ragel_music.rl"
	{ ragel_cat = CAT_UNIT; ragel_type = TYPE_BPM; }
#line 348 "lex/parser_timeline.rl"
	{ tl.setBarBpm(fsm.bpm_bar, fromRagel(bpm)); tl.calcBarDuration(bar.inf); }
	break;
	case 645: 
	case 647: 
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 7 "lex/ragel_units.rl"
	{ ragel_num.vdouble = ragel_num.vint; }
#line 285 "lex/parser_timeline.rl"
	{ tl.var_defs.back().def = ragel_num.getFloat(); }
	break;
	case 648: 
	case 650: 
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 7 "lex/ragel_units.rl"
	{ ragel_num.vdouble = ragel_num.vint; }
#line 286 "lex/parser_timeline.rl"
	{ tl.var_defs.back().preset = ragel_num.getFloat(); }
	break;
	case 484: 
	case 485: 
	case 611: 
	case 612: 
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 125 "lex/parser_timeline.rl"
	{ tl.addEventAction(fsm.event_id, fsm.act_preset); }
#line 124 "lex/parser_timeline.rl"
	{ fsm.onArgsDone(); }
	break;
	case 636: 
	case 637: 
#line 17 "lex/ragel_units.rl"
	{ragel_type = TYPE_HOUR;}
#line 103 "lex/parser_timeline.rl"
	{
        switch(ragel_type) {
        case TYPE_MSEC:  fsm.time_unit = ragel_num.getFloat();   break;
        case TYPE_SEC:   fsm.time_unit = ragel_num.getFloat() * 1000; break;
        case TYPE_MIN:   fsm.time_unit = ragel_num.getFloat() * 60000; break;
        case TYPE_HOUR:  fsm.time_unit = ragel_num.getFloat() * 3600000; break;
        default:
        break;
        }
    }
#line 342 "lex/parser_timeline.rl"
	{ tl.duration = fsm.time_unit; }
	break;
	case 638: 
	case 639: 
#line 18 "lex/ragel_units.rl"
	{ragel_type = TYPE_MIN;}
#line 103 "lex/parser_timeline.rl"
	{
        switch(ragel_type) {
        case TYPE_MSEC:  fsm.time_unit = ragel_num.getFloat();   break;
        case TYPE_SEC:   fsm.time_unit = ragel_num.getFloat() * 1000; break;
        case TYPE_MIN:   fsm.time_unit = ragel_num.getFloat() * 60000; break;
        case TYPE_HOUR:  fsm.time_unit = ragel_num.getFloat() * 3600000; break;
        default:
        break;
        }
    }
#line 342 "lex/parser_timeline.rl"
	{ tl.duration = fsm.time_unit; }
	break;
	case 640: 
	case 641: 
#line 19 "lex/ragel_units.rl"
	{ragel_type = TYPE_MSEC;}
#line 103 "lex/parser_timeline.rl"
	{
        switch(ragel_type) {
        case TYPE_MSEC:  fsm.time_unit = ragel_num.getFloat();   break;
        case TYPE_SEC:   fsm.time_unit = ragel_num.getFloat() * 1000; break;
        case TYPE_MIN:   fsm.time_unit = ragel_num.getFloat() * 60000; break;
        case TYPE_HOUR:  fsm.time_unit = ragel_num.getFloat() * 3600000; break;
        default:
        break;
        }
    }
#line 342 "lex/parser_timeline.rl"
	{ tl.duration = fsm.time_unit; }
	break;
	case 642: 
	case 643: 
#line 22 "lex/ragel_units.rl"
	{ragel_type = TYPE_SEC;}
#line 103 "lex/parser_timeline.rl"
	{
        switch(ragel_type) {
        case TYPE_MSEC:  fsm.time_unit = ragel_num.getFloat();   break;
        case TYPE_SEC:   fsm.time_unit = ragel_num.getFloat() * 1000; break;
        case TYPE_MIN:   fsm.time_unit = ragel_num.getFloat() * 60000; break;
        case TYPE_HOUR:  fsm.time_unit = ragel_num.getFloat() * 3600000; break;
        default:
        break;
        }
    }
#line 342 "lex/parser_timeline.rl"
	{ tl.duration = fsm.time_unit; }
	break;
	case 632: 
	case 633: 
	case 634: 
	case 635: 
#line 53 "lex/ragel_units.rl"
	{
    ragel_type = TYPE_SMPTE;
    smpte.hour *= smpte.sign;
    smpte.min *= smpte.sign;
    smpte.sec *= smpte.sign;
    smpte.frame *= smpte.sign;

    if (smpte.np == 2) {
        smpte.sec = smpte.min;
        smpte.min = smpte.hour;
        smpte.hour = 0;
    }
}
#line 114 "lex/parser_timeline.rl"
	{ fsm.onTimeSmpteDone(smpte); }
#line 342 "lex/parser_timeline.rl"
	{ tl.duration = fsm.time_unit; }
	break;
	case 520: 
#line 122 "lex/parser_timeline.rl"
	{ fsm.act_out.args = fsm.args;  tl.addEventAction(fsm.event_id, fsm.act_out); }
#line 124 "lex/parser_timeline.rl"
	{ fsm.onArgsDone(); }
#line 121 "lex/parser_timeline.rl"
	{ fsm.act_send.args = fsm.args; tl.addEventAction(fsm.event_id, fsm.act_send); }
	break;
	case 491: 
	case 569: 
	case 570: 
	case 571: 
	case 572: 
	case 573: 
	case 574: 
	case 575: 
	case 576: 
	case 577: 
	case 578: 
	case 579: 
	case 580: 
	case 581: 
	case 582: 
	case 583: 
	case 584: 
	case 585: 
	case 587: 
	case 588: 
	case 589: 
	case 590: 
	case 591: 
	case 592: 
	case 593: 
	case 594: 
	case 595: 
	case 596: 
	case 597: 
	case 599: 
	case 618: 
	case 620: 
#line 123 "lex/parser_timeline.rl"
	{ fsm.onSendTarget(); }
#line 121 "lex/parser_timeline.rl"
	{ fsm.act_send.args = fsm.args; tl.addEventAction(fsm.event_id, fsm.act_send); }
#line 124 "lex/parser_timeline.rl"
	{ fsm.onArgsDone(); }
	break;
	case 535: 
	case 536: 
	case 537: 
	case 538: 
	case 539: 
	case 540: 
	case 541: 
	case 542: 
	case 543: 
	case 544: 
	case 545: 
	case 546: 
	case 547: 
	case 548: 
	case 549: 
	case 550: 
	case 551: 
	case 552: 
	case 554: 
	case 555: 
	case 556: 
	case 557: 
	case 558: 
	case 559: 
	case 560: 
	case 561: 
	case 562: 
	case 563: 
	case 564: 
	case 566: 
#line 123 "lex/parser_timeline.rl"
	{ fsm.onSendTarget(); }
#line 124 "lex/parser_timeline.rl"
	{ fsm.onArgsDone(); }
#line 121 "lex/parser_timeline.rl"
	{ fsm.act_send.args = fsm.args; tl.addEventAction(fsm.event_id, fsm.act_send); }
	break;
	case 506: 
	case 508: 
	case 509: 
	case 510: 
#line 124 "lex/parser_timeline.rl"
	{ fsm.onArgsDone(); }
#line 121 "lex/parser_timeline.rl"
	{ fsm.act_send.args = fsm.args; tl.addEventAction(fsm.event_id, fsm.act_send); }
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
	break;
	case 375: 
	case 377: 
	case 378: 
	case 379: 
#line 124 "lex/parser_timeline.rl"
	{ fsm.onArgsDone(); }
#line 122 "lex/parser_timeline.rl"
	{ fsm.act_out.args = fsm.args;  tl.addEventAction(fsm.event_id, fsm.act_out); }
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
	break;
	case 402: 
	case 403: 
	case 404: 
	case 405: 
	case 406: 
	case 407: 
	case 408: 
	case 409: 
	case 410: 
	case 411: 
	case 412: 
	case 413: 
	case 414: 
	case 415: 
	case 417: 
	case 421: 
	case 422: 
	case 423: 
	case 424: 
	case 425: 
	case 426: 
	case 427: 
	case 428: 
	case 429: 
	case 430: 
	case 431: 
	case 432: 
	case 433: 
	case 434: 
	case 437: 
	case 438: 
	case 439: 
	case 440: 
	case 441: 
	case 468: 
	case 470: 
	case 471: 
#line 124 "lex/parser_timeline.rl"
	{ fsm.onArgsDone(); }
#line 122 "lex/parser_timeline.rl"
	{ fsm.act_out.args = fsm.args;  tl.addEventAction(fsm.event_id, fsm.act_out); }
#line 121 "lex/parser_timeline.rl"
	{ fsm.act_send.args = fsm.args; tl.addEventAction(fsm.event_id, fsm.act_send); }
	break;
	case 586: 
#line 122 "lex/parser_timeline.rl"
	{ fsm.act_out.args = fsm.args;  tl.addEventAction(fsm.event_id, fsm.act_out); }
#line 123 "lex/parser_timeline.rl"
	{ fsm.onSendTarget(); }
#line 121 "lex/parser_timeline.rl"
	{ fsm.act_send.args = fsm.args; tl.addEventAction(fsm.event_id, fsm.act_send); }
#line 124 "lex/parser_timeline.rl"
	{ fsm.onArgsDone(); }
	break;
	case 553: 
#line 122 "lex/parser_timeline.rl"
	{ fsm.act_out.args = fsm.args;  tl.addEventAction(fsm.event_id, fsm.act_out); }
#line 123 "lex/parser_timeline.rl"
	{ fsm.onSendTarget(); }
#line 124 "lex/parser_timeline.rl"
	{ fsm.onArgsDone(); }
#line 121 "lex/parser_timeline.rl"
	{ fsm.act_send.args = fsm.args; tl.addEventAction(fsm.event_id, fsm.act_send); }
	break;
	case 528: 
	case 529: 
#line 124 "lex/parser_timeline.rl"
	{ fsm.onArgsDone(); }
#line 121 "lex/parser_timeline.rl"
	{ fsm.act_send.args = fsm.args; tl.addEventAction(fsm.event_id, fsm.act_send); }
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 125 "lex/parser_timeline.rl"
	{ tl.addEventAction(fsm.event_id, fsm.act_preset); }
	break;
	case 394: 
	case 395: 
#line 124 "lex/parser_timeline.rl"
	{ fsm.onArgsDone(); }
#line 122 "lex/parser_timeline.rl"
	{ fsm.act_out.args = fsm.args;  tl.addEventAction(fsm.event_id, fsm.act_out); }
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 125 "lex/parser_timeline.rl"
	{ tl.addEventAction(fsm.event_id, fsm.act_preset); }
	break;
	case 416: 
	case 418: 
	case 419: 
	case 420: 
#line 124 "lex/parser_timeline.rl"
	{ fsm.onArgsDone(); }
#line 122 "lex/parser_timeline.rl"
	{ fsm.act_out.args = fsm.args;  tl.addEventAction(fsm.event_id, fsm.act_out); }
#line 121 "lex/parser_timeline.rl"
	{ fsm.act_send.args = fsm.args; tl.addEventAction(fsm.event_id, fsm.act_send); }
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
	break;
	case 401: 
	case 442: 
	case 443: 
	case 444: 
	case 445: 
	case 446: 
	case 447: 
	case 448: 
	case 449: 
	case 450: 
	case 451: 
	case 452: 
	case 453: 
	case 454: 
	case 455: 
	case 456: 
	case 457: 
	case 458: 
	case 459: 
	case 460: 
	case 461: 
	case 462: 
	case 463: 
	case 464: 
	case 465: 
	case 466: 
	case 467: 
	case 469: 
	case 472: 
	case 474: 
#line 124 "lex/parser_timeline.rl"
	{ fsm.onArgsDone(); }
#line 122 "lex/parser_timeline.rl"
	{ fsm.act_out.args = fsm.args;  tl.addEventAction(fsm.event_id, fsm.act_out); }
#line 123 "lex/parser_timeline.rl"
	{ fsm.onSendTarget(); }
#line 121 "lex/parser_timeline.rl"
	{ fsm.act_send.args = fsm.args; tl.addEventAction(fsm.event_id, fsm.act_send); }
	break;
	case 435: 
	case 436: 
#line 124 "lex/parser_timeline.rl"
	{ fsm.onArgsDone(); }
#line 122 "lex/parser_timeline.rl"
	{ fsm.act_out.args = fsm.args;  tl.addEventAction(fsm.event_id, fsm.act_out); }
#line 121 "lex/parser_timeline.rl"
	{ fsm.act_send.args = fsm.args; tl.addEventAction(fsm.event_id, fsm.act_send); }
#line 30 "lex/ragel_numeric.rl"
	{
        ragel_num.vint *= ragel_num.sign;
        ragel_type = TYPE_INT;
        ragel_cat  = CAT_NUMBER;
    }
#line 125 "lex/parser_timeline.rl"
	{ tl.addEventAction(fsm.event_id, fsm.act_preset); }
	break;
#line 11805 "lex/parser_timeline.cpp"
	}
	}

	_out: {}
	}

#line 378 "lex/parser_timeline.rl"

    const bool ok = cs >= 268;
    return ok;
}

}
}
