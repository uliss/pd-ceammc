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
#include "proto_midi.h"
#include "ceammc_factory.h"

static t_symbol* SYM_NOTEON;
static t_symbol* SYM_NOTEOFF;
static t_symbol* SYM_CLOCK;
static t_symbol* SYM_TICK;
static t_symbol* SYM_START;
static t_symbol* SYM_STOP;
static t_symbol* SYM_CONTINUE;

ProtoMidi::ProtoMidi(const PdArgs& args)
    : BaseObject(args)
{
    createOutlet();

    using midi::Byte;

    parser_.setNoteOnFn([this](Byte b, Byte n, Byte v) {
        Atom msg[3] = { 0x0F & b, n, v };
        anyTo(0, SYM_NOTEON, AtomListView(msg, 3));
    });

    parser_.setNoteOffFn([this](Byte b, Byte n, Byte v) {
        Atom msg[3] = { 0x0F & b, n, v };
        anyTo(0, SYM_NOTEOFF, AtomListView(msg, 3));
    });

    parser_.setRealtimeCb([this](Byte msg) {
        switch (msg) {
        case midi::MIDI_TIMECLOCK:
            return anyTo(0, SYM_CLOCK, AtomListView());
        case midi::MIDI_TIMETICK:
            return anyTo(0, SYM_TICK, AtomListView());
        case midi::MIDI_START:
            return anyTo(0, SYM_START, AtomListView());
        case midi::MIDI_STOP:
            return anyTo(0, SYM_STOP, AtomListView());
        case midi::MIDI_CONTINUE:
            return anyTo(0, SYM_CONTINUE, AtomListView());
        default:
            break;
        }
    });
}

void ProtoMidi::onFloat(t_float f)
{
    if (f < 0 || f > 0xff) {
        OBJ_ERR << "byte value expected in 0-255 range, got: " << f;
        return;
    }

    auto res = parser_.push(f);
    if (res.err != midi::MidiParser::NO_ERROR) {
        OBJ_ERR << "";
    }
}

void ProtoMidi::onList(const AtomList& lst)
{
    for (auto& a : lst) {
        if (a.isFloat())
            onFloat(a.asT<t_float>());
    }
}

void setup_proto_midi()
{
    SYM_NOTEON = gensym("noteon");
    SYM_CLOCK = gensym("clock");
    SYM_TICK = gensym("tick");
    SYM_NOTEOFF = gensym("noteoff");
    SYM_START = gensym("start");
    SYM_STOP = gensym("stop");
    SYM_CONTINUE = gensym("continue");

    ObjectFactory<ProtoMidi> obj("proto.midi");
}
