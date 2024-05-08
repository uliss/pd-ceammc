/*****************************************************************************
 * Copyright 2024 Serge Poltavski. All rights reserved.
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
#include "flow_histogram.h"
#include "ceammc_factory.h"

#include <cassert>
#include <numeric>

FlowHistogram::FlowHistogram(const PdArgs& args)
    : BaseObject(args)
{
    normalize_ = new BoolProperty("@norm", true);
    addProperty(normalize_);

    inner_bins_ = new BoolProperty("@inner_bins", true);
    addProperty(inner_bins_);

    sync_ = new BoolProperty("@sync", true);
    addProperty(sync_);

    bins_ = new ListProperty("@bins", AtomList { 0. });
    bins_->setArgIndex(0);
    bins_->setListCheckFn([this](const AtomListView& lv) {
        return lv.size() > 0 && lv.allOf(isFloat);
    });
    bins_->setSuccessFn([this](Property* p) {
        auto& val = bins_->value();
        val.sort();

        counters_.assign(val.size() + 1, 0.0);
        output_.resizePad(counters_.size(), 0.);
        output_.fill(0.0);

        fbins_.resize(val.size() + 2);
        fbins_.front() = std::numeric_limits<t_float>::lowest();
        fbins_.back() = std::numeric_limits<t_float>::max();

        for (size_t i = 0; i < val.size(); i++) {
            fbins_[i + 1] = val[i].asT<t_float>();
        }
    });
    addProperty(bins_);

    createOutlet();
}

void FlowHistogram::onBang()
{
    output();
}

void FlowHistogram::onFloat(t_float f)
{
    if (fbins_.empty()) {
        return;
    }

    // find bin index
    // O=log2(N)+O(1)
    auto it = std::upper_bound(fbins_.begin(), fbins_.end(), f);
    // should not happen
    if (it == fbins_.end())
        return;

    // bin index
    auto idx = (it - fbins_.begin()) - 1;

    // should not happen
    if (idx < 0 || idx >= counters_.size())
        return;

    counters_[idx]++;
    output_[idx] = counters_[idx];

    if (sync_->value())
        output();
}

void FlowHistogram::m_clear(t_symbol*, const AtomListView&)
{
    clear();
}

void FlowHistogram::clear()
{
    std::fill(counters_.begin(), counters_.end(), 0.0);
    output_.fill(0.0);
}

void FlowHistogram::output()
{
    if (normalize_->value()) {
        // O=O(N) max(N−1,0)
        std::uint64_t max = 0;
        if (inner_bins_->value()) {
            if (counters_.size() >= 2) {
                auto it = std::max_element(counters_.begin() + 1, counters_.end() - 1);
                if (it != counters_.end())
                    max = *it;
            }
        } else {
            auto it = std::max_element(counters_.begin(), counters_.end());
            if (it != counters_.end())
                max = *it;
        }

        if (max > 0) {
            // O=O(N)
            for (size_t i = 0; i < counters_.size(); i++)
                output_[i] = static_cast<double>(counters_[i]) / max;
        }
    }

    if (inner_bins_->value())
        listTo(0, output_.view(1, output_.size() - 2));
    else
        listTo(0, output_);
}

void setup_flow_histogram()
{
    ObjectFactory<FlowHistogram> obj("flow.histogram");
    obj.addAlias("flow.hist");

    obj.addMethod("clear", &FlowHistogram::m_clear);
}
