# include "parser_units.h"
# include "parser_numeric.h"
# include "ceammc_convert.h"
# include "ceammc_log.h"
# include "fmt/core.h"

# include <cstdint>
# include <cstring>
# include <cstdio>

namespace {
    using namespace ceammc;
    using namespace ceammc::music;

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
}

namespace ceammc {
namespace parser {

%%{
    machine units_full;
    include units_common "ragel_units.rl";

    main := units;
    write data;
}%%

UnitsFullMatch::UnitsFullMatch()
{
    reset();
}

void UnitsFullMatch::reset()
{
    unit_ = { };
}

bool UnitsFullMatch::parse(const Atom& a)
{
    reset();

    if (a.isSymbol())
        return parse(a.asT<t_symbol*>()->s_name);
    else if(a.isFloat()) {
        unit_.value = a.asT<t_float>();
        if(a.isInteger())
            unit_.type = TYPE_INT;
        else
            unit_.type = TYPE_FLOAT;

        return true;
    } else
        return false;
}

bool UnitsFullMatch::parseAs(const Atom& a, AtomType t)
{
    const bool ok = parse(a);
    return ok && (unit_.type == t || unit_.type == TYPE_INT || unit_.type == TYPE_FLOAT);
}

bool UnitsFullMatch::parse(const char* str)
{
    const auto len = strlen(str);
    if (len == 0)
        return false;

    const char* p = str;
    const char* pe = p + len;
    const char* eof = pe;

    PositionType pos_ = POSITION_ABS;

    DECLARE_RAGEL_COMMON_VARS;
    DECLARE_RAGEL_NUMERIC_VARS;

    RagelTempo bpm;
    fsm::SmpteData smpte;

    reset();

    %% write init;
    %% write exec;

    const bool ok = cs >= %%{ write first_final; }%%;
    if (ok) {
        unit_.value = ragel_num.vdouble;

        switch(ragel_type) {
        case TYPE_RATIO:
            if (ragel_num.ratio.den == 0) {
                fprintf(stderr, "division by zero: %s\n", str);
                return false;
            }

            unit_.value = ragel_num.getRatioAsFloat();
        break;
        case TYPE_INT:
            unit_.value = ragel_num.getInteger();
        break;
        default:
        break;
        }

        unit_.type = ragel_type;
        unit_.pos = pos_;
        unit_.smpte.hour = smpte.hour;
        unit_.smpte.min = smpte.min;
        unit_.smpte.sec = smpte.sec;
        unit_.smpte.frame = smpte.frame;
        unit_.tempo = fromRagel(bpm);
    }

    return ok;
}

size_t UnitsFullMatch::parse(const AtomListView& lv, UnitVec& out)
{
    const size_t N = lv.size();

    for (size_t i = 0; i < N; i++) {
        const auto& a = lv[i];
        if (!parse(a))
            return i;

        out.push_back(unit_);
    }

    return N;
}

%%{
    machine units_type;
    include units_common "ragel_units.rl";

    main := unit_suffixes | unit_bpm;
    write data;
}%%

bool UnitTypeFullMatch::parse(const char* str)
{
    reset();

    const auto len = strlen(str);
    if (len == 0)
        return false;

    const char* p = str;
    const char* pe = p + len;
    const char* eof = pe;

    %% write init;
    %% write exec;

    return cs >= %%{ write first_final; }%%;
}

bool UnitTypeFullMatch::parse(const Atom& a)
{
    if (a.isSymbol()) {
        return parse(a.asT<t_symbol*>()->s_name);
    } else if(a.isFloat()) {
        reset();
        return true;
    } else
        return false;
}

%%{
    machine parse_angles;
    include units_common "ragel_units.rl";

    unit_pi = ('pi' | 'π') %{ ragel_num.vdouble *= std::acos(-1); ragel_type = TYPE_RADIAN; };
    unit = unit_rad | unit_deg | unit_pi;
    angle_float = num_float | (num_int %{ ragel_num.vdouble = ragel_num.vint;});
    main := angle_float unit?;

    write data;
}%%

using AngleUnit = std::pair<float, parser::AtomType>;

Either<float> parse_angle_as(const Atom& a, AtomType type)
{
    if (a.isFloat())
        return a.asT<t_float>();
    else if(a.isSymbol()) {
        auto res = parse_angle(a.asT<t_symbol*>()->s_name);
        if (res.isOk()) {
            auto& x = res.value();
            switch (x.second) {
            case TYPE_RADIAN:
                if (type == TYPE_DEGREE)
                    return convert::rad2degree(x.first);
                else
                    return x.first;
            case TYPE_DEGREE:
                if (type == TYPE_RADIAN)
                    return convert::degree2rad(x.first);
                else
                    return x.first;
            case TYPE_INT:
            case TYPE_FLOAT:
                return x.first;
            default:
                return RuntimeError(fmt::format("invalid unit type: {}", (int)x.second));
            }
        } else
            return res.error();
    } else
        return RuntimeError("invalid atom type");
}

Either<AngleUnit> parse_angle(const Atom &a, AtomType def)
{
    if (a.isFloat())
        return AngleUnit(a.asT<t_float>(), def);
    else if(a.isSymbol())
        return parse_angle(a.asT<t_symbol*>()->s_name);
    else
        return RuntimeError("invalid atom type");
}

Either<AngleUnit> parse_angle(const char* str)
{
    if (!str)
        return RuntimeError("NULL string pointer");

    const auto len = strlen(str);
    if (len == 0)
        return RuntimeError("empty string");

    int cs = 0;
    const char* p = str;
    const char* pe = p + len;
    const char* eof = pe;
    DECLARE_RAGEL_COMMON_VARS;
    DECLARE_RAGEL_NUMERIC_VARS;

    %% write init;
    %% write exec;

    if (cs >= %%{ write first_final; }%%) {
        if (ragel_type == TYPE_INT)
            return AngleUnit(ragel_num.getInteger(), TYPE_FLOAT);
        else if (ragel_type == TYPE_FLOAT)
            return AngleUnit(ragel_num.getFloat(), ragel_type);
        else
            return AngleUnit(ragel_num.getFloat(), ragel_type);
    } else
        return RuntimeError(fmt::format("can't parse string: '{}'", str));
}

}
}


