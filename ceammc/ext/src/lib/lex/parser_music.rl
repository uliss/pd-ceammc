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

%%{
    machine bpm;
    include music_common "ragel_music.rl";

    main := bpm;
    write data;
}%%

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

    %% write init;
    %% write exec;

    const bool ok = cs >= %%{ write first_final; }%%;

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

%%{
    machine spn;
    include music_common "ragel_music.rl";

    main := spn;
    write data;
}%%

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

    %% write init;
    %% write exec;

    const bool ok = cs >= %%{ write first_final; }%%;

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

%%{
    machine pitch;
    include music_common "ragel_music.rl";

    main := pitch;
    write data;
}%%


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

    %% write init;
    %% write exec;

    const bool ok = cs >= %%{ write first_final; }%%;

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

%%{
    machine notation;
    include music_common "ragel_music.rl";

    main := note_single_seq;
    write data;
}%%

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

    %% write init;
    %% write exec;

    const bool ok = cs >= %%{ write first_final; }%%;

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

%%{
    machine duration;
    include music_common "ragel_music.rl";

    main := dur_sequence;
    write data;
}%%

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

    %% write init;
    %% write exec;

    const bool ok = cs >= %%{ write first_final; }%%;

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

%%{
    machine time_signature;
    include music_common "ragel_music.rl";

    main := time_signature;
    write data;
}%%

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

    %% write init;
    %% write exec;

    const bool ok = cs >= %%{ write first_final; }%%;

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

%%{
    machine chord_names;
    include music_common "ragel_music.rl";

    main := chord_name;
    write data;
}%%

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

    %% write init;
    %% write exec;

    const bool ok = cs >= %%{ write first_final; }%%;

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


