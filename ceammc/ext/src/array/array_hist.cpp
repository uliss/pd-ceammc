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
#include "array_hist.h"
#include "ceammc_containers.h"
#include "ceammc_convert.h"
#include "ceammc_factory.h"

#include <cmath>
#include <cstdint>

constexpr const size_t HIST_MIN_SIZE = 2;
constexpr const size_t HIST_DEFAULT_SIZE = 100;
constexpr const size_t HIST_MAX_SIZE = 1000;

ArrayHist::ArrayHist(const PdArgs& args)
    : ArrayBase(args)
    , nbins_(nullptr)
    , min_(nullptr)
    , max_(nullptr)
{
    nbins_ = new IntProperty("@bins", HIST_DEFAULT_SIZE);
    nbins_->setArgIndex(1);
    nbins_->checkClosedRange(HIST_MIN_SIZE, HIST_MAX_SIZE);
    addProperty(nbins_);

    min_ = new FloatProperty("@min", -1);
    min_->setArgIndexNext(nbins_);
    addProperty(min_);

    max_ = new FloatProperty("@max", 1);
    max_->setArgIndexNext(min_);
    addProperty(max_);

    createOutlet();
}

void ArrayHist::onBang()
{
    if (!checkArray())
        return;

    const size_t N = array_.size();
    if (N < 1) {
        OBJ_ERR << "array is empty";
        return;
    }

    const size_t NBINS = nbins_->value();
    uint32_t hist[NBINS];
    for (size_t i = 0; i < NBINS; i++)
        hist[i] = 0;

    const t_sample MIN = min_->value();
    const t_sample MAX = max_->value();

    if (MIN >= MAX) {
        OBJ_ERR << "invalid range values";
        return;
    }

    for (t_sample samp : array_) {
        // ignore out of range values
        if (samp < MIN || samp > MAX)
            continue;

        auto n = convert::lin2lin<t_sample>(samp, MIN, MAX, t_sample(0), NBINS);
        int idx = n;
        if (n == NBINS)
            idx--;

        hist[idx]++;
    }

    StaticAtomList<256> res;
    res.reserve(NBINS);

    for (size_t i = 0; i < NBINS; i++)
        res.push_back(hist[i]);

    listTo(0, res.view());
}

void setup_array_hist()
{
    ObjectFactory<ArrayHist> obj("array.hist");

    obj.setDescription("calculates array histogram");
    obj.setCategory("array");
}
