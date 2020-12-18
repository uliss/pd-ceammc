/*****************************************************************************
 * Copyright 2020 Serge Poltavsky. All rights reserved.
 *
 * This file may be distributed under the terms of GNU Public License version
 * 3 (GPL v3) as defined by the Free Software Foundation (FSF). A copy of the
 * license should have been included with this file, or the project in which
 * this file belongs to. You may also find the details of GPL v3 at:
 * http://www.gnu.org/licenses/gpl-3.0.txt
 *
 * If you have any questions regarding the use of this file, feel free to
 * contact the author of this file, or the owner of the project in which
 * this file belongs to.
 *****************************************************************************/
#include "grainprops.lexer.h"
#include "ceammc_format.h"
#include "ceammc_units.h"
#include "grain.h"

namespace ceammc {

t_symbol* GrainPropertiesLexer::PROP_AMP = nullptr;
t_symbol* GrainPropertiesLexer::PROP_AT = nullptr;
t_symbol* GrainPropertiesLexer::PROP_INTERP = nullptr;
t_symbol* GrainPropertiesLexer::PROP_INTERP2 = nullptr;
t_symbol* GrainPropertiesLexer::PROP_LENGTH = nullptr;
t_symbol* GrainPropertiesLexer::PROP_LENGTH2 = nullptr;
t_symbol* GrainPropertiesLexer::PROP_PAN = nullptr;
t_symbol* GrainPropertiesLexer::PROP_SPEED = nullptr;
t_symbol* GrainPropertiesLexer::PROP_WHEN = nullptr;

t_symbol* GrainPropertiesLexer::SYM_ADD = nullptr;
t_symbol* GrainPropertiesLexer::SYM_CLIP = nullptr;
t_symbol* GrainPropertiesLexer::SYM_CUBIC = nullptr;
t_symbol* GrainPropertiesLexer::SYM_EXPR = nullptr;
t_symbol* GrainPropertiesLexer::SYM_FOLD = nullptr;
t_symbol* GrainPropertiesLexer::SYM_LINEAR = nullptr;
t_symbol* GrainPropertiesLexer::SYM_MODE = nullptr;
t_symbol* GrainPropertiesLexer::SYM_MOTION = nullptr;
t_symbol* GrainPropertiesLexer::SYM_MSEC = nullptr;
t_symbol* GrainPropertiesLexer::SYM_MSEC2 = nullptr;
t_symbol* GrainPropertiesLexer::SYM_NONE = nullptr;
t_symbol* GrainPropertiesLexer::SYM_ONDONE = nullptr;
t_symbol* GrainPropertiesLexer::SYM_OVERFLOW = nullptr;
t_symbol* GrainPropertiesLexer::SYM_RANDOM = nullptr;
t_symbol* GrainPropertiesLexer::SYM_RANGE = nullptr;
t_symbol* GrainPropertiesLexer::SYM_SEC = nullptr;
t_symbol* GrainPropertiesLexer::SYM_SET = nullptr;
t_symbol* GrainPropertiesLexer::SYM_SQRT = nullptr;
t_symbol* GrainPropertiesLexer::SYM_WRAP = nullptr;

static inline bool looksLikeUnitValue(t_symbol* s)
{
    auto str = s->s_name;
    return isdigit(str[0]) || (str[0] == '-' && isdigit(str[1]));
}

GrainPropertiesLexer::GrainPropertiesLexer(const AtomList& src, Grain* grain)
    : src_(src)
    , idx_(0)
    , grain_(grain)
{
    if (!PROP_AMP)
        initSymTab();
}

void GrainPropertiesLexer::reset()
{
    idx_ = 0;
    mode_ = NORMAL;
}

void GrainPropertiesLexer::set(const AtomList& src)
{
    reset();
    src_ = src;
}

GrainPropertiesParser::symbol_type GrainPropertiesLexer::lex()
{
    if (idx_ >= src_.size())
        return GrainPropertiesParser::make_YYEOF();

#define CHECK_SYM(sym) else if (s == SYM_##sym) return GrainPropertiesParser::make_S_##sym();
#define CHECK_SYM2(sym0, sym1) else if (s == SYM_##sym0 || s == SYM_##sym1) return GrainPropertiesParser::make_S_##sym0();

    if (mode_ == EXPR)
        return lexExpr();

    const auto& atom = src_[idx_++];

    if (atom.isFloat())
        return GrainPropertiesParser::make_FLOAT(atom.asFloat());
    else if (atom.isSymbol()) {
        auto* s = atom.asT<t_symbol*>();
        units::TimeValue time(0);
        units::FractionValue frac;

        if (s == PROP_PAN)
            return GrainPropertiesParser::make_PROP_PAN();
        else if (s == PROP_AMP)
            return GrainPropertiesParser::make_PROP_AMP();
        else if (s == PROP_SPEED)
            return GrainPropertiesParser::make_PROP_SPEED();
        else if (s == PROP_AT)
            return GrainPropertiesParser::make_PROP_AT();
        else if (s == PROP_LENGTH || s == PROP_LENGTH2)
            return GrainPropertiesParser::make_PROP_LENGTH();
        else if (s == PROP_WHEN)
            return GrainPropertiesParser::make_PROP_WHEN();
        else if (s == PROP_INTERP || s == PROP_INTERP2)
            return GrainPropertiesParser::make_PROP_INTERP();
        // check first
        CHECK_SYM(SET)
        CHECK_SYM(ADD)
        CHECK_SYM(ONDONE)
        CHECK_SYM(RANDOM)
        // check alphabet
        CHECK_SYM(CLIP)
        CHECK_SYM(CUBIC)
        CHECK_SYM(FOLD)
        CHECK_SYM(LINEAR)
        CHECK_SYM(MODE)
        CHECK_SYM(MOTION)
        CHECK_SYM2(MSEC, MSEC2)
        CHECK_SYM(NONE)
        CHECK_SYM(OVERFLOW)
        CHECK_SYM(RANGE)
        CHECK_SYM(SEC)
        CHECK_SYM(SQRT)
        CHECK_SYM(WRAP)
        else if (s == SYM_EXPR)
        {
            mode_ = EXPR;
            return GrainPropertiesParser::make_S_EXPR();
        }
        else if (looksLikeUnitValue(s) && units::TimeValue::parse(atom).matchValue(time))
        {
            time.setSamplerate(sys_getsr());
            return GrainPropertiesParser::make_FLOAT(time.toSamples());
        }
        else if (looksLikeUnitValue(s) && units::FractionValue::parse(atom).matchValue(frac))
        {
            return GrainPropertiesParser::make_FLOAT(frac.calcPartOfTotal(arraySize()));
        }
        else
        {
            return GrainPropertiesParser::make_YYerror();
        }
    } else
        return GrainPropertiesParser::make_YYerror();

#undef CHECK_SYM
#undef CHECK_SYM2
}

GrainPropertiesParser::symbol_type GrainPropertiesLexer::lexExpr()
{
    std::string str;

    for (; idx_ < src_.size(); idx_++) {
        const auto& atom = src_[idx_++];
        if (atom.isProperty()) {
            if (str.empty())
                return GrainPropertiesParser::make_YYerror();

            mode_ = NORMAL;
            str.pop_back(); // remove trailing space
            return GrainPropertiesParser::make_STRING(str);
        } else if (atom.isSymbol()) {
            str += atom.asT<t_symbol*>()->s_name;
            str += ' ';
        } else {
            str += to_string(atom);
            str += ' ';
        }
    }

    if (str.empty())
        return GrainPropertiesParser::make_YYerror();

    mode_ = NORMAL;
    str.pop_back(); // remove trailing space
    return GrainPropertiesParser::make_STRING(str);
}

void GrainPropertiesLexer::initSymTab()
{
    PROP_AMP = gensym("@amp");
    PROP_AT = gensym("@at");
    PROP_INTERP = gensym("@i");
    PROP_INTERP2 = gensym("@interp");
    PROP_LENGTH = gensym("@l");
    PROP_LENGTH2 = gensym("@length");
    PROP_PAN = gensym("@pan");
    PROP_SPEED = gensym("@speed");
    PROP_WHEN = gensym("@when");

    SYM_ADD = gensym("add");
    SYM_CLIP = gensym("clip");
    SYM_CUBIC = gensym("cubic");
    SYM_EXPR = gensym("expr");
    SYM_FOLD = gensym("fold");
    SYM_LINEAR = gensym("linear");
    SYM_MODE = gensym("mode");
    SYM_MOTION = gensym("motion");
    SYM_MSEC = gensym("ms");
    SYM_MSEC2 = gensym("msec");
    SYM_NONE = gensym("none");
    SYM_ONDONE = gensym("ondone");
    SYM_OVERFLOW = gensym("overflow");
    SYM_RANDOM = gensym("random");
    SYM_RANGE = gensym("range");
    SYM_SEC = gensym("sec");
    SYM_SET = gensym("set");
    SYM_SQRT = gensym("sqrt");
    SYM_WRAP = gensym("wrap");
}

}
