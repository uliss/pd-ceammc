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
#ifndef ARRAY_BASE_H
#define ARRAY_BASE_H

#include "ceammc_array.h"
#include "ceammc_object.h"

using namespace ceammc;

class ArrayBase : public BaseObject {
    t_symbol* array_name_;

public:
    ArrayBase(const PdArgs& a);

    bool setArray(t_symbol* s);
    bool check();

protected:
    Array array_;

private:
    AtomList propArray() const;
    void propSetArray(const AtomList& l);
};

class ArrayMod : public ArrayBase {
    BoolProperty* redraw_;

public:
    ArrayMod(const PdArgs& a);
    bool shouldRedraw() const;
};

#endif // ARRAY_BASE_H
