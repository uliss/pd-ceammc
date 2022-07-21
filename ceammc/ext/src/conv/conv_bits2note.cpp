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
#include "conv_bits2note.h"
#include "ceammc_factory.h"

static t_symbol* SYM_OFF;
static t_symbol* SYM_TIE;
static t_symbol* SYM_RAW;

ConvBits2Note::ConvBits2Note(const PdArgs& args)
    : BaseObject(args)
    , poly_(nullptr)
    , keys_(nullptr)
    , vel_(nullptr)
    , mode_(nullptr)
{
    poly_ = new FlagProperty("@poly");
    addProperty(poly_);

    keys_ = new ListProperty("@keys");
    keys_->setInitOnly();
    keys_->setArgIndex(0);
    addProperty(keys_);

    vel_ = new FloatProperty("@vel", 90);
    vel_->checkClosedRange(0, 127);
    addProperty(vel_);

    mode_ = new SymbolEnumProperty("@mode", { SYM_OFF, SYM_TIE, SYM_RAW });
    addProperty(mode_);
}

void ConvBits2Note::initDone()
{
    if (poly_->value()) {
        createOutlet();
    } else {
        for (size_t i = 0; i < keys_->value().size(); i++)
            createOutlet();
    }
}

void ConvBits2Note::onList(const AtomListView& lv)
{
    const size_t N = keys_->value().size();
    if (lv.size() != N) {
        OBJ_ERR << "length of input list should be equal " << N << ", got: " << lv;
        return;
    }

    for (size_t i = 0; i < N; i++) {
        const t_float key = keys_->value()[i].asFloat(0);

        const auto& a = lv[i];
        const bool bit_on = a.isFloat() && a.asT<t_float>() != 0;

        processNote(i, key, bit_on);
    }
}

void ConvBits2Note::noteTo(size_t n, t_float note, t_float vel)
{
    Atom atoms[3];

    if (poly_->value()) {
        atoms[0] = n;
        atoms[1] = note;
        atoms[2] = vel;
        listTo(0, AtomListView(atoms, 3));
    } else {
        atoms[0] = note;
        atoms[1] = vel;
        listTo(n, AtomListView(atoms, 2));
    }
}

void ConvBits2Note::processNote(size_t n, t_float note, bool on)
{
    auto st = notes_.test(n);
    notes_.set(n, on);
    auto m = mode_->value();

    if (m == SYM_TIE) {
        if (on && !st) // note on
            noteTo(n, note, vel_->value());
        else if (!on && st) // note off
            noteTo(n, note, 0);
        else // ignore repeating events
            return;
    } else if (m == SYM_OFF) {
        if (on && !st) // note on
            noteTo(n, note, vel_->value());
        else if (!on && st) // note off
            noteTo(n, note, 0);
        else if (on && st) { // repeated on
            noteTo(n, note, 0); // first - off
            noteTo(n, note, vel_->value());
        } else // ignore repeated offs
            return;
    } else if (m == SYM_RAW) {
        noteTo(n, note, on ? vel_->value() : 0);
    } else
        OBJ_ERR << "unknown mode: " << m;
}

void setup_conv_bits2note()
{
    SYM_OFF = gensym("off");
    SYM_TIE = gensym("tie");
    SYM_RAW = gensym("raw");

    ObjectFactory<ConvBits2Note> obj("conv.bits2note");
    obj.addAlias("bits->note");
}
