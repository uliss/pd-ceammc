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
#include "grain.h"

namespace ceammc {

t_symbol* GrainPropertiesLexer::PROP_AMP = nullptr;
t_symbol* GrainPropertiesLexer::PROP_PAN = nullptr;
t_symbol* GrainPropertiesLexer::PROP_SPEED = nullptr;
t_symbol* GrainPropertiesLexer::PROP_AT = nullptr;
t_symbol* GrainPropertiesLexer::PROP_LENGTH = nullptr;
t_symbol* GrainPropertiesLexer::PROP_LENGTH2 = nullptr;
t_symbol* GrainPropertiesLexer::PROP_WHEN = nullptr;
t_symbol* GrainPropertiesLexer::SYM_RANDOM = nullptr;
t_symbol* GrainPropertiesLexer::SYM_OVERFLOW = nullptr;
t_symbol* GrainPropertiesLexer::SYM_ONDONE = nullptr;
t_symbol* GrainPropertiesLexer::SYM_ADD = nullptr;
t_symbol* GrainPropertiesLexer::SYM_SET = nullptr;
t_symbol* GrainPropertiesLexer::SYM_MOTION = nullptr;
t_symbol* GrainPropertiesLexer::SYM_EXPR = nullptr;
t_symbol* GrainPropertiesLexer::SYM_RANGE = nullptr;
t_symbol* GrainPropertiesLexer::SYM_MSEC = nullptr;
t_symbol* GrainPropertiesLexer::SYM_MSEC2 = nullptr;
t_symbol* GrainPropertiesLexer::SYM_SEC = nullptr;
t_symbol* GrainPropertiesLexer::SYM_CLIP = nullptr;
t_symbol* GrainPropertiesLexer::SYM_WRAP = nullptr;
t_symbol* GrainPropertiesLexer::SYM_FOLD = nullptr;
t_symbol* GrainPropertiesLexer::SYM_MODE = nullptr;
t_symbol* GrainPropertiesLexer::SYM_LINEAR = nullptr;
t_symbol* GrainPropertiesLexer::SYM_SQRT = nullptr;
t_symbol* GrainPropertiesLexer::SYM_NONE = nullptr;

GrainPropertiesLexer::GrainPropertiesLexer(const AtomList& src, Grain* grain)
    : src_(src)
    , idx_(0)
    , grain_(grain)
{
    if (!PROP_AMP)
        initSymTab();
}

void GrainPropertiesLexer::set(const AtomList& src)
{
    src_ = src;
    idx_ = 0;
}

GrainPropertiesParser::symbol_type GrainPropertiesLexer::lex()
{
    if (idx_ >= src_.size())
        return GrainPropertiesParser::make_YYEOF();

    auto& atom = src_[idx_++];

    if (atom.isFloat())
        return GrainPropertiesParser::make_FLOAT(atom.asFloat());
    else if (atom.isSymbol()) {
        auto* s = atom.asT<t_symbol*>();
        if (s == PROP_PAN)
            return GrainPropertiesParser::make_PROP_PAN();
        else if (s == PROP_AMP)
            return GrainPropertiesParser::make_PROP_AMP();
        else if (s == PROP_SPEED)
            return GrainPropertiesParser::make_PROP_SPEED();
        if (s == PROP_AT)
            return GrainPropertiesParser::make_PROP_AT();
        else if (s == PROP_LENGTH || s == PROP_LENGTH2)
            return GrainPropertiesParser::make_PROP_LENGTH();
        else if (s == PROP_WHEN)
            return GrainPropertiesParser::make_PROP_WHEN();
        else if (s == SYM_MSEC || s == SYM_MSEC2)
            return GrainPropertiesParser::make_MSEC();
        else if (s == SYM_SEC)
            return GrainPropertiesParser::make_SEC();
        else if (s == SYM_RANDOM)
            return GrainPropertiesParser::make_RANDOM();
        else if (s == SYM_OVERFLOW)
            return GrainPropertiesParser::make_TOVERFLOW();
        else if (s == SYM_ONDONE)
            return GrainPropertiesParser::make_ONDONE();
        else if (s == SYM_ADD)
            return GrainPropertiesParser::make_ADD();
        else if (s == SYM_SET)
            return GrainPropertiesParser::make_SET();
        else if (s == SYM_MOTION)
            return GrainPropertiesParser::make_MOTION();
        else if (s == SYM_RANGE)
            return GrainPropertiesParser::make_RANGE();
        else if (s == SYM_MODE)
            return GrainPropertiesParser::make_MODE();
        else if (s == SYM_EXPR)
            return GrainPropertiesParser::make_EXPR();
        else if (s == SYM_CLIP)
            return GrainPropertiesParser::make_OVERFLOW_CLIP(Grain::PAN_OVERFLOW_CLIP);
        else if (s == SYM_WRAP)
            return GrainPropertiesParser::make_OVERFLOW_WRAP(Grain::PAN_OVERFLOW_WRAP);
        else if (s == SYM_FOLD)
            return GrainPropertiesParser::make_OVERFLOW_FOLD(Grain::PAN_OVERFLOW_FOLD);
        else if (s == SYM_NONE)
            return GrainPropertiesParser::make_PAN_MODE(Grain::PAN_MODE_NONE);
        else if (s == SYM_LINEAR)
            return GrainPropertiesParser::make_PAN_MODE(Grain::PAN_MODE_LINEAR);
        else if (s == SYM_SQRT)
            return GrainPropertiesParser::make_PAN_MODE(Grain::PAN_MODE_SQRT);
        else {
            auto str = to_string(src_.view(idx_ - 1));
            return GrainPropertiesParser::make_STRING(str);
        }
    } else
        return GrainPropertiesParser::make_YYerror();
}

void GrainPropertiesLexer::initSymTab()
{
    PROP_AMP = gensym("@amp");
    PROP_PAN = gensym("@pan");
    PROP_SPEED = gensym("@speed");
    PROP_AT = gensym("@at");
    PROP_LENGTH = gensym("@l");
    PROP_LENGTH2 = gensym("@length");
    PROP_WHEN = gensym("@when");

    SYM_RANDOM = gensym("random");
    SYM_OVERFLOW = gensym("overflow");
    SYM_ONDONE = gensym("ondone");
    SYM_ADD = gensym("add");
    SYM_SET = gensym("set");
    SYM_MOTION = gensym("motion");
    SYM_EXPR = gensym("expr");
    SYM_RANGE = gensym("range");

    SYM_MSEC = gensym("ms");
    SYM_MSEC2 = gensym("msec");
    SYM_SEC = gensym("sec");
    SYM_CLIP = gensym("clip");
    SYM_WRAP = gensym("wrap");
    SYM_FOLD = gensym("fold");

    SYM_MODE = gensym("mode");
    SYM_LINEAR = gensym("none");
    SYM_SQRT = gensym("linear");
    SYM_NONE = gensym("sqrt");
}

}
