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
#include "array_minmax.h"
#include "ceammc_factory.h"

#include <algorithm>

ArrayMinMax::ArrayMinMax(const PdArgs& a)
    : ArrayBase(a)
{
    createOutlet();
    createOutlet();
}

void ArrayMinMax::onBang()
{
    if (!checkArray())
        return;

    perform();
}

void ArrayMinMax::onSymbol(t_symbol* s)
{
    if (!setArray(s))
        return;

    perform();
}

void ArrayMinMax::perform()
{
    auto minmax = std::minmax_element(array_.begin(), array_.end());

    if (minmax.first == array_.end())
        return;

    Atom out0[2] = { minmax.first - array_.begin(), minmax.second - array_.begin() };
    listTo(1, AtomListView(out0, 2));

    Atom out1[2] = { *minmax.first, *minmax.second };
    listTo(0, AtomListView(out1, 2));
}

void setup_array_minmax()
{
    ObjectFactory<ArrayMinMax> obj("array.minmax");
}
