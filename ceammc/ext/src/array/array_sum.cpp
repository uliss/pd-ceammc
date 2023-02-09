/*****************************************************************************
 * Copyright 2019 Serge Poltavsky. All rights reserved.
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
#include "array_sum.h"
#include "ceammc_factory.h"

#include <algorithm>
#include <numeric>

ArraySum::ArraySum(const PdArgs& args)
    : ArrayBase(args)
{
    createOutlet();
}

void ArraySum::onBang()
{
    if (!checkArray())
        return;

    auto sum = std::accumulate(array_.begin(), array_.end(), t_sample(0));
    floatTo(0, sum);
}

void setup_array_sum()
{
    ObjectFactory<ArraySum> obj("array.sum");

    obj.setDescription("calculates sum of array elements");
    obj.setCategory("array");
}
