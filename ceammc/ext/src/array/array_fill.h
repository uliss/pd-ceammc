/*****************************************************************************
 * Copyright 2017 Serge Poltavsky. All rights reserved.
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
#ifndef ARRAY_FILL_H
#define ARRAY_FILL_H

#include "array_base.h"

using namespace ceammc;

class ArrayFill : public ArrayMod {

public:
    ArrayFill(const PdArgs& a);
    void onSymbol(t_symbol* s);
    void onFloat(float f);
    void onList(const AtomList& l);

    void m_fill(t_symbol* m, const AtomList& l);
    AtomList parseRange(const AtomList& args, size_t* from, size_t* to) const;

private:
    void fillRange(size_t from, size_t to, const AtomList& l);
    void finish();
};

#endif // ARRAY_FILL_H
