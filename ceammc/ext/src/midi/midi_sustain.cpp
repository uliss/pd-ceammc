/*****************************************************************************
 * Copyright 2021 Serge Poltavsky. All rights reserved.
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
#include "midi_sustain.h"
#include "ceammc_args.h"
#include "ceammc_factory.h"

#include <memory>

constexpr int CC_SUSTAIN = 64;
static t_symbol* SYM_CTLIN;

static std::unique_ptr<ArgChecker> onlist_chk;

MidiSustain::MidiSustain(const PdArgs& args)
    : BaseObject(args)
    , on_(nullptr)
    , ctlin_(nullptr)
    , proxy_(this, &MidiSustain::onCC)
{
    on_ = new BoolProperty("@on", false);
    on_->setSuccessFn([this](Property*) { if(!on_->value()) notesOff(); });
    addProperty(on_);

    ctlin_ = new BoolProperty("@ctlin", false);
    ctlin_->setSuccessFn([this](Property*) {
        if (ctlin_->value())
            proxy_.bind(SYM_CTLIN);
        else
            proxy_.unbind();
    });
    addProperty(ctlin_);

    createInlet();
    createOutlet();
}

void MidiSustain::onList(const AtomList& lst)
{
    if (!onlist_chk->check(lst.view())) {
        OBJ_ERR << "NOTE VEL expected, got: " << lst;
        return;
    }

    const auto note = lst[0].asT<int>();
    const auto vel = lst[1].asT<t_float>();

    if (on_->value()) {
        if (vel > 0) {
            notes_.set(note, true);
            listTo(0, lst);
        }
    } else {
        notes_.set(note, vel > 0);
        listTo(0, lst);
    }
}

void MidiSustain::onInlet(size_t n, const AtomListView& lv)
{
    on_->set(lv);
}

void MidiSustain::onCC(const AtomListView& lv)
{
    if (!ctlin_->value())
        return;

    if (lv.size() >= 2 && lv[0].asInt() == CC_SUSTAIN) {
        bool on = lv[1].asInt() >= 64;
        on_->setValue(on);
        if (!on)
            notesOff();
    }
}

void MidiSustain::notesOff()
{
    Atom msg[2] = { 0., 0. };
    AtomListView lv(msg, 2);

    for (size_t i = 0; i < notes_.size(); i++) {
        if (notes_.test(i)) {
            msg[0] = i;
            listTo(0, lv);
        }
    }
}

void setup_midi_sustain()
{
    onlist_chk.reset(new ArgChecker("f0..127 f0..127"));
    SYM_CTLIN = gensym("#ctlin");

    ObjectFactory<MidiSustain> obj("midi.sustain");
}
