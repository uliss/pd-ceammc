/*****************************************************************************
 * Copyright 2020 Serge Poltavsky. All rights reserved.
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
#include "flt_freqz_tilde.h"
#include "ceammc_factory.h"
#include "flt_common.h"

FltFreqZTilde::FltFreqZTilde(const PdArgs& args)
    : SoundExternal(args)
    , ca_(nullptr)
    , cb_(nullptr)
    , use_sr_(nullptr)
    , db_scale_(nullptr)
    , kb_ { 1 }
    , ka_ { 1 }
{
    createSignalOutlet();
    createSignalOutlet();

    cb_ = new ListProperty("@b", { 1 });
    cb_->setListCheckFn([](const AtomList& l) -> bool { return l.size() > 0 && l.allOf(isFloat); }, "invalid list");
    cb_->setSuccessFn([this](Property*) {
        kb_.clear();
        kb_.reserve(cb_->value().size());

        for (auto& b : cb_->value()) {
            kb_.push_back(b.toT<t_sample>(0));
        }
    });
    addProperty(cb_);

    ca_ = new ListProperty("@a");
    ca_->setListCheckFn([](const AtomList& l) -> bool { return l.empty() || l.allOf(isFloat); }, "invalid list");
    ca_->setSuccessFn([this](Property*) {
        ka_.clear();
        ka_.reserve(ca_->value().size() + 1);
        ka_.push_back(1);

        for (auto& a : ca_->value()) {
            ka_.push_back(a.toT<t_float>(0));
        }
    });

    addProperty(ca_);

    use_sr_ = new BoolProperty("@sr", false);
    addProperty(use_sr_);

    db_scale_ = new BoolProperty("@db", false);
    addProperty(db_scale_);
}

void FltFreqZTilde::processBlock(const t_sample** in, t_sample** out)
{
    const size_t BS = blockSize();
    const t_float norm = use_sr_->value() ? (flt::m_2pi / (t_sample)samplerate()) : 1;
    bool db = db_scale_->value();

    for (size_t i = 0; i < BS; i++) {
        t_sample w = norm * in[0][i];
        auto Hw = flt::calcHw<t_sample>(w, kb_.begin(), kb_.end(), ka_.begin(), ka_.end());
        auto m = std::abs(Hw);
        out[0][i] = db ? 20 * std::log(m) : m;
        out[1][i] = std::arg(Hw);
    }
}

void setup_flt_freqz_tilde()
{
    SoundExternalFactory<FltFreqZTilde> obj("flt.freqz~");

    obj.setDescription("compute the frequency response of a digital filter");
    obj.addAuthor("Serge Poltavsky");
    obj.setKeywords({ "filter", "calc", "bode", "frequence", "response", "phase", "plot" });
    obj.setCategory("flt");
    obj.setSinceVersion(0, 9);

    FltFreqZTilde::setInletsInfo(obj.classPointer(), { "input freq in \\[0..π\\] or \\[0..sr/2\\] range" });
    FltFreqZTilde::setOutletsInfo(obj.classPointer(), { "amplitude response", "phase response" });
}
