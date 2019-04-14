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
#include "ceammc_convert.h"
#include "ceammc_factory.h"

#include <cmath>
#include <cstdint>

static const size_t HIST_MIN_SIZE = 2;
static const size_t HIST_DEFAULT_SIZE = 100;
static const size_t HIST_MAX_SIZE = 1000;

ArrayHist::ArrayHist(const PdArgs& args)
    : ArrayBase(args)
    , nbins_(nullptr)
    , min_(nullptr)
    , max_(nullptr)
{
    // checking for first symbol argument, that is array name
    int off = (positionalArguments().symbolAt(0, nullptr) == nullptr) ? 0 : 1;

    const size_t N = clip<t_float>(positionalFloatArgument(off, 100), HIST_MIN_SIZE, HIST_MAX_SIZE);
    nbins_ = new SizeTPropertyClosedRange("@bins", N, HIST_MIN_SIZE, HIST_MAX_SIZE);
    createProperty(nbins_);

    const t_float MIN = positionalFloatArgument(off + 1, -1);
    const t_float MAX = positionalFloatArgument(off + 2, 1);
    min_ = new FloatProperty("@min", MIN);
    max_ = new FloatProperty("@max", MAX);
    createProperty(min_);
    createProperty(max_);

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

    std::vector<uint32_t> hist(nbins_->value());
    const t_sample MIN = min_->value();
    const t_sample MAX = max_->value();

    if (MIN >= MAX) {
        OBJ_ERR << "invalid range values";
        return;
    }

    const t_sample NBINS = nbins_->value();
    for (auto samp : array_) {
        int n = std::round(convert::lin2lin(samp, MIN, MAX, t_sample(0), NBINS));
        if (n < 0 || n >= NBINS)
            continue;

        hist[n]++;
    }

    AtomList res;
    res.fill(0.f, hist.size());
    for (size_t i = 0; i < res.size(); i++)
        res[i].setFloat(hist[i]);

    listTo(0, res);
}

void setup_array_hist()
{
    ObjectFactory<ArrayHist> obj("array.hist");
}
