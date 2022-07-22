# include "parser_numeric.h"

# include <cstdint>
# include <cstring>
# include <limits>

namespace ceammc {
namespace parser {

%%{
    machine numeric_full;
    include numeric_common "ragel_numeric.rl";

    main := number;
    write data;
}%%

NumericFullMatch::NumericFullMatch()
{
    reset();
}

void NumericFullMatch::reset()
{
    res_ = {};
}

bool NumericFullMatch::parse(const Atom& a)
{
    reset();

    if (a.isSymbol())
        return parse(a.asT<t_symbol*>()->s_name);
    else if(a.isFloat()) {
        if(a.isInteger()) {
            res_.t = TYPE_INT;
            res_.f = a.asT<t_float>();
            res_.i0 = res_.f;
        } else {
            res_.t = TYPE_FLOAT;
            res_.f = a.asT<t_float>();
        }

        return true;
    } else
        return false;
}

bool NumericFullMatch::parse(const char* str)
{
    const auto len = strlen(str);
    if (len == 0)
        return false;

    const char* p = str;
    const char* pe = p + len;
    const char* eof = pe;

    DECLARE_RAGEL_COMMON_VARS;
    DECLARE_RAGEL_NUMERIC_VARS;

    reset();

    %% write init;
    %% write exec;

    switch(ragel_type) {
    case TYPE_PHASE:
    case TYPE_PERCENT:
    case TYPE_FLOAT:
        res_.f = ragel_num.getFloat();
        break;
    case TYPE_INT:
    case TYPE_HEX:
    case TYPE_BIN:
        res_.i0 = ragel_num.getInteger();
        break;
    case TYPE_RATIO:
        res_.i0 = ragel_num.ratio.num;
        res_.i1 = ragel_num.ratio.den;
        break;
    case TYPE_INF:
        res_.f = std::numeric_limits<typeof(res_.f)>::infinity();
        break;
    default:
        break;
    }

    res_.t = ragel_type;

    return cs >= %%{ write first_final; }%%;
}

bool NumericFullMatch::getBin(t_int& b) const
{
    if(!isBin()) return false;
    b = res_.i0;
    return true;
}

bool NumericFullMatch::getInt(t_int& i) const
{
    if(!isInt()) return false;
    i = res_.i0;
    return true;
}

bool NumericFullMatch::getHex(t_int& x) const
{
    if(!isHex()) return false;
    x = res_.i0;
    return true;
}

bool NumericFullMatch::getFloat(t_float& f) const
{
    if(!isFloat()) return false;
    f = res_.f;
    return true;
}

bool NumericFullMatch::getRatio(t_int& num, t_int& den) const
{
    if(!isRatio()) return false;
    num = res_.i0;
    den = res_.i1;
    return true;
}

bool NumericFullMatch::parseAll(const AtomListView& lv, SmallFVec& out)
{
    return parseSome(lv, out) == lv.size();
}

size_t NumericFullMatch::parseSome(const AtomListView& lv, SmallFVec& out)
{
    const size_t N = lv.size();

    for (size_t i = 0; i < N; i++) {
        const auto& a = lv[i];
        if (!parse(a))
            return i;

        out.push_back(asFloat());
    }

    return N;
}

%%{
    machine numeric_some;
    include numeric_common "ragel_numeric.rl";

    action non_space {if(fc!=' '&&fc!='\t'){fhold;fbreak;}}

    ws = [ \t];
    tail = [] | (ws+ any* >*non_space);
    number_sp = ws* number tail;
    main := number_sp;
    write data;
}%%

NumericMatchSome::NumericMatchSome()
{
    reset();
}

void NumericMatchSome::reset()
{
    %% write init;
    res_.clear();
}

bool NumericMatchSome::parseSingle(const char* str, const char*& endptr)
{
    const auto len = strlen(str);
    if (len == 0)
        return false;

    const char* p = str;
    const char* pe = p + len;
    const char* eof = pe;

    DECLARE_RAGEL_COMMON_VARS;
    DECLARE_RAGEL_NUMERIC_VARS;

    bool ok = false;

    %% write init;
    %% write exec;

    ok = cs >= %%{ write first_final; }%%;

    if(ok) {
        switch(ragel_type) {
        case TYPE_PHASE:
        case TYPE_PERCENT:
        case TYPE_FLOAT:
            res_.push_back({});
            res_.back().f = ragel_num.getFloat();
            break;
        case TYPE_INT:
        case TYPE_HEX:
        case TYPE_BIN:
            res_.push_back({});
            res_.back().i0 = ragel_num.getInteger();
            break;
        case TYPE_RATIO:
            res_.push_back({});
            res_.back().i0 = ragel_num.ratio.num;
            res_.back().i1 = ragel_num.ratio.den;
            break;
        case TYPE_INF:
            res_.push_back({});
            res_.back().f = std::numeric_limits<typeof(res_[0].f)>::infinity();
            break;
        default:
            return false;
            break;
        }

        res_.back().t = ragel_type;
    }

    endptr = p;
    return ok;
}

size_t NumericMatchSome::parseMany(const char *str, const char *&endptr)
{
    reset();

    auto pstr = str;
    const char* end = nullptr;
    while(parseSingle(pstr, end))
        pstr = end;

    endptr = end;
    return res_.size();
}

bool NumericMatchSome::getBinAt(size_t idx, t_int& b) const
{
    if(!isBinAt(idx)) return false;
    b = res_[idx].i0;
    return true;
}

bool NumericMatchSome::getIntAt(size_t idx, t_int& i) const
{
    if(!isIntAt(idx)) return false;
    i = res_[idx].i0;
    return true;
}

bool NumericMatchSome::getHexAt(size_t idx, t_int& x) const
{
    if(!isHexAt(idx)) return false;
    x = res_[idx].i0;
    return true;
}

bool NumericMatchSome::getFloatAt(size_t idx, t_float& f) const
{
    if(!isFloatAt(idx)) return false;
    f = res_[idx].f;
    return true;
}

bool NumericMatchSome::getRatioAt(size_t idx, t_int& num, t_int& den) const
{
    if(!isRatioAt(idx)) return false;
    num = res_[idx].i0;
    den = res_[idx].i1;
    return true;
}

double NumericMatchSome::asFloatAt(size_t idx, double def) const
{
    if(idx >= res_.size())
        return def;

    auto& n = res_[idx];
    return n.asFloat(def);
}

}
}


