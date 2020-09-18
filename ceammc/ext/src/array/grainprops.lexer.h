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
#ifndef GRAINPROPSLEXER_H
#define GRAINPROPSLEXER_H

#include "ceammc_atomlist.h"
#include "grainprops.parser.hpp"

class Grain;

namespace ceammc {

class GrainPropertiesLexer {
    AtomList src_;
    size_t idx_;
    size_t array_size_ = { 0 };
    Grain* grain_;

public:
    GrainPropertiesLexer(const AtomList& src, Grain* grain);

    void set(const AtomList& src);
    GrainPropertiesParser::symbol_type lex();

    Grain* grain() { return grain_; }

    size_t arraySize() const { return array_size_; }
    void setArraySize(size_t sz) { array_size_ = sz; }

private:
    static t_symbol* PROP_AMP; // 1
    static t_symbol* PROP_AT;
    static t_symbol* PROP_INTERP2;
    static t_symbol* PROP_INTERP;
    static t_symbol* PROP_LENGTH2;
    static t_symbol* PROP_LENGTH;
    static t_symbol* PROP_PAN;
    static t_symbol* PROP_SPEED;
    static t_symbol* PROP_WHEN; // 9

    static t_symbol* SYM_ADD; // 1
    static t_symbol* SYM_CLIP;
    static t_symbol* SYM_CUBIC;
    static t_symbol* SYM_EXPR;
    static t_symbol* SYM_FOLD;
    static t_symbol* SYM_LINEAR;
    static t_symbol* SYM_MODE;
    static t_symbol* SYM_MOTION;
    static t_symbol* SYM_MSEC2;
    static t_symbol* SYM_MSEC;
    static t_symbol* SYM_NONE;
    static t_symbol* SYM_ONDONE;
    static t_symbol* SYM_OVERFLOW;
    static t_symbol* SYM_RANDOM;
    static t_symbol* SYM_RANGE;
    static t_symbol* SYM_SEC;
    static t_symbol* SYM_SET;
    static t_symbol* SYM_SQRT;
    static t_symbol* SYM_WRAP; // 19

    static void initSymTab();
};

}

#endif // GRAINPROPSLEXER_H
