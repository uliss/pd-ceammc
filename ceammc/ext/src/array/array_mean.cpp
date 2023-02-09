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
#include "array_mean.h"
#include "ceammc_factory.h"

#include <numeric>

ArrayMean::ArrayMean(const PdArgs& args)
    : ArrayBase(args)
{
    createOutlet();
}

void ArrayMean::onBang()
{
    if (!checkArray())
        return;

    const size_t N = array_.size();
    if (N < 1) {
        OBJ_ERR << "array is empty";
        return;
    }

    t_sample sum = std::accumulate(array_.begin(), array_.end(), t_sample(0),
        [](t_sample accum, t_sample x) { return accum + x; });

    floatTo(0, sum / N);
}

void setup_array_mean()
{
    ObjectFactory<ArrayMean> obj("array.mean");

    obj.setDescription("calculates array arithmetic mean value");
    obj.setCategory("array");
    obj.setKeywords({"array", "mean"});
}
