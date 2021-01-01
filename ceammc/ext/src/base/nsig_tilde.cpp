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
#include "nsig_tilde.h"
#include "ceammc_factory.h"

constexpr int MIN_INS = 1;
constexpr int MAX_INS = 12;
constexpr int DEF_INS = 1;

static_assert(MIN_INS <= DEF_INS && DEF_INS <= MAX_INS, "");

NSigTilde::NSigTilde(const PdArgs& args)
    : SoundExternal(args)
    , n_(nullptr)
{
    n_ = new IntProperty("@n", DEF_INS);
    n_->setInitOnly();
    n_->checkClosedRange(MIN_INS, MAX_INS);
    n_->setArgIndex(0);
    addProperty(n_);

    auto p = createCbListProperty(
        "@values", [this]() -> AtomList {
        AtomList res;
        res.reserve(samp_.size());
        for(auto f: samp_)
            res.append(Atom(f));

        return res; },
        [this](const AtomList& lst) -> bool {
            updateSamples();
            if (lst.size() > samp_.size())
                OBJ_ERR << "extra list values are ignored: " << lst.slice(samp_.size());

            const size_t N = std::min(samp_.size(), lst.size());
            for (size_t i = 0; i < N; i++)
                samp_[i] = lst[i].asFloat();

            return true;
        });
    p->setArgIndex(1);

    createSignalOutlet();
}

void NSigTilde::initDone()
{
    for (int i = 1; i < n_->value(); i++) {
        createInlet();
        createSignalOutlet();
    }

    updateSamples();
}

void NSigTilde::onFloat(t_float v)
{
    updateSamples();
    samp_[0] = v;
}

void NSigTilde::onInlet(size_t n, const AtomListView& l)
{
    updateSamples();
    samp_[n] = l.floatAt(0, 0);
}

void NSigTilde::onList(const AtomList& lst)
{
    setProperty("@values", lst);
}

void NSigTilde::processBlock(const t_sample** in, t_sample** out)
{
    const size_t BS = blockSize();
    const size_t NOUTS = numOutputChannels();
    for (size_t j = 0; j < NOUTS; j++) {
        const auto v = samp_[j];
        for (size_t i = 0; i < BS; i++) {
            out[j][i] = v;
        }
    }
}

void NSigTilde::updateSamples()
{
    if (samp_.size() < n_->value())
        samp_.resize(n_->value(), 0);
}

void setup_nsig_tilde()
{
    SoundExternalFactory<NSigTilde> obj("nsig~", OBJECT_FACTORY_DEFAULT);

    obj.setDescription("Mulitchannel converter from control to audio rate");
    obj.addAuthor("Serge Poltavsky");
    obj.setKeywords({"sig", "list"});
    obj.setCategory("base");
    obj.setSinceVersion(0, 9);
}
