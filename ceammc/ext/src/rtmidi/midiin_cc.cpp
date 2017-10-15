/*****************************************************************************
 * Copyright 2017 Serge Poltavsky. All rights reserved.
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
#include "midiin_cc.h"
#include "ceammc_factory.h"

#include <cassert>

MidiCtlin::MidiCtlin(const PdArgs& a)
    : PdMidiIn(a)
    , channel_(0)
    , cc_(0)
{
    channel_ = new IntProperty("@channel", int(positionalFloatArgument(1, -1)));
    cc_ = new IntProperty("@cc", int(positionalFloatArgument(0, -1)));
    createProperty(channel_);
    createProperty(cc_);

    setCallback(ccCallback);
}

bool MidiCtlin::listenChannel(unsigned char ch) const
{
    int c = channel_->value();
    if (c < 0)
        return true;

    return c == ch;
}

int MidiCtlin::controlNumber() const
{
    return cc_->value();
}

void MidiCtlin::ccCallback(double, std::vector<unsigned char>* message, void* userData)
{
    if (message->size() < 3)
        return;

    unsigned char ch = message->at(0) & 0xF;
    unsigned char cc = message->at(1);
    unsigned char v = message->at(2);

    MidiCtlin* obj = static_cast<MidiCtlin*>(reinterpret_cast<PdMidiIn*>(userData));

    assert(obj);

    if (!obj->listenChannel(ch))
        return;

    if (obj->controlNumber() < 0) {
        AtomList l(ch, cc);
        l.append(v);
        obj->listTo(0, l);
    } else if (obj->controlNumber() == cc) {
        obj->floatTo(0, v);
    }
}

extern "C" void setup_midi0x2ectlin()
{
    ObjectFactory<MidiCtlin> obj("midi.ctlin");
}
