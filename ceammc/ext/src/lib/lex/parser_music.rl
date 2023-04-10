# include "parser_music.h"
# include "parser_numeric.h"

# include <cstdint>
# include <cstring>

namespace ceammc {
namespace parser {

%%{
    machine bpm;
    include music_common "ragel_music.rl";

    main := bpm;
    write data;
}%%

std::ostream& operator<<(std::ostream& os, const Duration& dur)
{
    if (dur.repeats > 1)
        os << (int)dur.repeats << '*';

    os << (int)dur.num << '/' << (int)dur.den;
    return os;
}

bool BpmFullMatch::parse(const Atom& a, Bpm& res)
{
    if (a.isSymbol())
        return parse(a.asT<t_symbol*>()->s_name, res);
    else if(a.isFloat()) {
        auto f = a.asT<t_float>();
        if (f < 0)
            return false;

        res = Bpm(a.asT<t_float>(), 1, 4);
        return true;
    } else
        return false;
}

bool BpmFullMatch::parse(const char* str, Bpm& res)
{
    const auto len = strlen(str);
    if (len == 0)
        return false;

    int cs = 0;
    const char* p = str;
    const char* pe = p + len;
    const char* eof = pe;

    DECLARE_RAGEL_COMMON_VARS;
    fsm::BpmData bpm;

    %% write init;
    %% write exec;

    const bool ok = cs >= %%{ write first_final; }%%;

    if (ok)
        res = bpm;

    return ok;
}

size_t BpmFullMatch::parse(const AtomListView& lv, SmallBpmVec& out)
{
    const size_t N = lv.size();

    Bpm bpm;

    for (size_t i = 0; i < N; i++) {
        const auto& a = lv[i];
        if (!parse(a, bpm))
            return i;

        out.push_back(bpm);
    }

    return N;
}

%%{
    machine spn;
    include music_common "ragel_music.rl";

    main := spn;
    write data;
}%%

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

    DECLARE_RAGEL_COMMON_VARS;
    fsm::SpnData spn;

    reset();

    %% write init;
    %% write exec;

    const bool ok = cs >= %%{ write first_final; }%%;

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

%%{
    machine pitch;
    include music_common "ragel_music.rl";

    main := pitch;
    write data;
}%%

PitchFullMatch::PitchFullMatch()
{
    reset();
}

void PitchFullMatch::reset()
{
    spn_ = { };
}

bool PitchFullMatch::parse(const char* str)
{
    const auto len = strlen(str);
    if (len == 0)
        return false;

    const char* p = str;
    const char* pe = p + len;
    const char* eof = pe;

    DECLARE_RAGEL_COMMON_VARS;
    fsm::SpnData spn;

    reset();

    %% write init;
    %% write exec;

    const bool ok = cs >= %%{ write first_final; }%%;

    if (ok)
        spn_ = spn;

    return ok;
}

bool PitchFullMatch::parse(const Atom& a)
{
    if (a.isSymbol())
        return parse(a.asT<t_symbol*>()->s_name);
    else if(a.isInteger() && a.asT<int>() >= 0 && a.asT<int>() < 12) {
        spn_.setSemitones(a.asT<int>());
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

    DECLARE_RAGEL_COMMON_VARS;
    fsm::NotationData note;
    fsm::SpnData spn;

    reset();

    %% write init;
    %% write exec;

    const bool ok = cs >= %%{ write first_final; }%%;

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
        if (note_.dur.repeats > 1) {
            std::cerr << note_.dur.repeats << "\n";
            out.insert(out.end(), note_.dur.repeats - 1, out.back());
            note_.dur.repeats = 1;
        }
    }

    return N;
}

%%{
    machine duration;
    include music_common "ragel_music.rl";

    main := dur_sequence;
    write data;
}%%

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

    int cs = 0;
    const char* p = str;
    const char* pe = p + len;
    const char* eof = pe;
    int cat_ = 0;
    AtomType type_;
    fsm::NotationData note;

    reset();

    %% write init;
    %% write exec;

    const bool ok = cs >= %%{ write first_final; }%%;

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

%%{
    machine time_signature;
    include music_common "ragel_music.rl";

    main := time_signature;
    write data;
}%%

bool TimeSignatureParser::parse(const char* str, music::TimeSignature& ts)
{
    const auto len = strlen(str);
    if (len == 0)
        return false;

    int cs = 0;
    const char* p = str;
    const char* pe = p + len;
    const char* eof = pe;

    fsm::TimeSignatureData ragel_ts;

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


}
}


