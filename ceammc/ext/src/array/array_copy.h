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
#ifndef ARRAY_COPY_H
#define ARRAY_COPY_H

#include "ceammc_object.h"

using namespace ceammc;

namespace ceammc {
class Array;
}

struct Range {
    size_t from, to;
    Range(size_t f, size_t t)
        : from(f)
        , to(t)
    {
    }

    bool isValid() const { return from <= to; }
    Range clip(size_t max) const;
    size_t length() const { return to - from; }
};

class ArrayCopy : public BaseObject {
    BoolProperty* resize_;

public:
    ArrayCopy(const PdArgs& a);
    void onList(const AtomListView& lv) override;
    void m_copy(t_symbol*, const AtomListView& lv);

private:
    void copy(t_symbol* src, t_symbol* dest);
    void copyRange(t_symbol* src, const Range& range, t_symbol* dest, size_t destpos);
    bool checkArrays(const Array& src, const Array& dest) const;
};

void setup_array_copy();

#endif // ARRAY_COPY_H
