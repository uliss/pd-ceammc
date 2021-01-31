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
#include "flt_freqz.h"
#include "ceammc_factory.h"
#include "ceammc_filter.h"

#include <cmath>

FltFreqZ::FltFreqZ(const PdArgs& args)
    : BaseObject(args)
    , ca_(nullptr)
    , cb_(nullptr)
    , n_(nullptr)
    , db_scale_(nullptr)
    , kb_ { 1 }
    , ka_ { 1 }
{
    createOutlet();
    createOutlet();
    createOutlet();

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

    n_ = new IntProperty("@n", 256);
    n_->checkClosedRange(4, 1024);
    n_->setArgIndex(0);
    addProperty(n_);

    db_scale_ = new BoolProperty("@db", false);
    addProperty(db_scale_);
}

void FltFreqZ::onBang()
{
    const size_t N = n_->value();
    const bool db = db_scale_->value();

    Atom amp[N];
    Atom phase[N];
    floatTo(2, N);

    for (size_t i = 0; i < N; i++) {
        t_float w = flt::freq2ang(i, 2 * N);
        const auto Hw = flt::freqz(w, kb_.begin(), kb_.end(), ka_.begin(), ka_.end());

        amp[i] = db ? 20 * std::log(std::abs(Hw)) : std::abs(Hw);
        phase[i] = std::arg(Hw);
    }

    listTo(1, AtomListView(phase, N));
    listTo(0, AtomListView(amp, N));
}

void FltFreqZ::dump() const
{
    BaseObject::dump();

    Post p(this);

    for (size_t i = 0; i < kb_.size(); i++)
        p << 'b' << i << '=' << kb_[i] << ' ';

    for (size_t i = 0; i < ka_.size(); i++)
        p << 'a' << i << '=' << ka_[i] << ' ';
}

void FltFreqZ::m_biquad(t_symbol* s, const AtomListView& lv)
{
    if (lv.size() != 5) {
        METHOD_ERR(s) << "b0 b1 b2 a1 a2 coeffs expected, got: " << lv;
        return;
    }

    cb_->set(lv.subView(0, 3));
    ca_->set(lv.subView(3, 2));
}

void setup_flt_freqz()
{
    ObjectFactory<FltFreqZ> obj("flt.freqz");
    obj.addMethod("biquad", &FltFreqZ::m_biquad);

    obj.setDescription("compute the frequency response of a digital filter");
    obj.addAuthor("Serge Poltavsky");
    obj.setKeywords({ "filter", "calc", "bode", "frequence", "response", "phase", "plot" });
    obj.setCategory("flt");
    obj.setSinceVersion(0, 9);

    obj.setXletsInfo({ "bang: start calculation" }, {
                                                        "float: freq response value",
                                                        "float: phase response value",
                                                        "int: plot size",
                                                    });
}
