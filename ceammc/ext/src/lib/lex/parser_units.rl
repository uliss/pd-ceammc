# include "parser_units.h"

# include <cstdint>
# include <cstring>
# include <cstdio>

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
    int cat_ = 0;
    AtomType type_ = TYPE_UNKNOWN;
    PositionType pos_ = POSITION_ABS;
    NumericRagelData num_;
    fsm::BpmData bpm;
    fsm::SmpteData smpte;

    reset();

    %% write init;
    %% write exec;

    const bool ok = cs >= %%{ write first_final; }%%;
    if (ok) {
        unit_.value = num_.fval;
        if  (type_ == TYPE_RATIO) {
            if (num_.iden == 0) {
                fprintf(stderr, "division by zero: %s\n", str);
                return false;
            }

            unit_.value = double(num_.inum) / double(num_.iden);
        }
        else if(type_ == TYPE_INT)
            unit_.value = num_.ival;

        unit_.type = type_;
        unit_.pos = pos_;
        unit_.smpte.hour = smpte.hour;
        unit_.smpte.min = smpte.min;
        unit_.smpte.sec = smpte.sec;
        unit_.smpte.frame = smpte.frame;
        unit_.bpm = bpm;
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

    main := unit_suffixes;
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

}
}


