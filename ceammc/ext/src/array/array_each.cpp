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
#include "array_each.h"
#include "ceammc_factory.h"

ArrayEach::ArrayEach(const PdArgs& a)
    : ArrayMod(a)
    , idx_(0)
{
    createInlet();
    createOutlet();
    createOutlet();
}

void ArrayEach::onBang()
{
    if (!checkArray())
        return;

    outputEach();
    finish();
}

void ArrayEach::onSymbol(t_symbol* s)
{
    if (!setArray(s))
        return;

    outputEach();
    finish();
}

void ArrayEach::onInlet(size_t n, const AtomList& l)
{
    if (n != 1 || l.empty())
        return;

    array_[idx_] = l[0].asFloat();
}

void ArrayEach::outputEach()
{
    for (idx_ = 0; idx_ < array_.size(); idx_++)
        floatTo(1, array_[idx_]);
}

void ArrayEach::finish()
{

    array_.redraw();
    bangTo(0);
}

void setup_array_each()
{
    ObjectFactory<ArrayEach> obj("array.each");
}
