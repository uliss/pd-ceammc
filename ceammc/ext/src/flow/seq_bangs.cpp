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
#include "seq_bangs.h"
#include "ceammc_factory.h"

SeqBangs::SeqBangs(const PdArgs& args)
    : BaseObject(args)
    , pattern_(nullptr)
    , bpm_(nullptr)
    , division_(nullptr)
    , clock_([this]() {
        const auto N = pattern_->value().size();

        if (current_ < N) {
            output();
            schedNext();
        } else if (current_ == N) {
            bangTo(2);
        }
    })
{
    bpm_ = new FloatProperty("@bpm", 60);
    bpm_->checkClosedRange(1, 600);
    addProperty(bpm_);

    division_ = new IntProperty("@div", 4);
    division_->checkClosedRange(1, 512);
    addProperty(division_);

    pattern_ = new ListProperty("@pattern");
    pattern_->setArgIndex(0);
    pattern_->setFilterAtomFn([this](const Atom& a) -> bool {
        if (!a.isFloat() || a.asT<t_float>() < 0) {
            OBJ_ERR << "invalid pattern value: " << a;
            return false;
        }

        return true;
    });
    addProperty(pattern_);

    createInlet();
    createOutlet();
    createOutlet();
    createOutlet();
}

void SeqBangs::onBang()
{
    current_ = 0;
    clock_.unset();

    if (current_ >= pattern_->value().size()) {
        OBJ_ERR << "empty pattern";
        return;
    }

    output();
    schedNext();
}

void SeqBangs::onInlet(size_t n, const AtomList& l)
{
    pattern_->set(l);
}

void SeqBangs::schedNext()
{
    if (current_ >= pattern_->value().size())
        return;

    const auto dur = pattern_->value()[current_].asFloat(0);
    clock_.delay(calcDurationMs(dur));

    current_++;
}

void SeqBangs::output()
{
    if (current_ >= pattern_->value().size())
        return;

    const auto dur = pattern_->value()[current_].asFloat(0);

    floatTo(1, calcDurationMs(dur));
    bangTo(0);
}

t_float SeqBangs::calcDurationMs(t_float dur) const
{
    const auto beat_ms = 1000 * (4 * 60.0 / bpm_->value()) / (division_->value());
    return dur * beat_ms;
}

void setup_seq_bangs()
{
    ObjectFactory<SeqBangs> obj("seq.bangs");
    obj.addAlias("seq.b");

    obj.setXletsInfo({ "bang: start playing sequence", "list: set new pattern" },
        { "bang: output pattern", "float: time until next bang (in ms)", "bang: after last pattern is played" });
}
