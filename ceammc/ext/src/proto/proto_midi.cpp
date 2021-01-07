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
static t_symbol* SYM_ACTIVESENSE;
static t_symbol* SYM_SYSRESET;
static t_symbol* SYM_AFTOUCH_MONO;
static t_symbol* SYM_AFTOUCH_POLY;

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

    parser_.setAfterTouchFn([this](Byte b, Byte v) {
        Atom msg[2] = { 0x0F & b, v };
        anyTo(0, SYM_AFTOUCH_MONO, AtomListView(msg, 2));
    });

    parser_.setRealtimeFn([this](Byte msg) {
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
        case midi::MIDI_ACTIVE_SENSE:
            return anyTo(0, SYM_ACTIVESENSE, AtomListView());
        case midi::MIDI_SYSTEM_RESET:
            return anyTo(0, SYM_SYSRESET, AtomListView());
        default:
            OBJ_ERR << "unknown realtime message: " << (int)msg;
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

void ProtoMidi::m_noteOn(t_symbol* s, const AtomListView& lv)
{
    if (!checkMethodByte3(s, lv)) {
        METHOD_ERR(s) << "usage: CHAN NOTE VEL";
        return;
    }

    byteStatus(midi::MIDI_NOTEON, lv[0].asT<int>());
    byteData(lv[1].asT<int>());
    byteData(lv[2].asT<int>());
}

void ProtoMidi::m_noteOff(t_symbol* s, const AtomListView& lv)
{
    if (!checkMethodByte3(s, lv)) {
        METHOD_ERR(s) << "usage: CHAN NOTE VEL";
        return;
    }

    byteStatus(midi::MIDI_NOTEOFF, lv[0].asT<int>());
    byteData(lv[1].asT<int>());
    byteData(lv[2].asT<int>());
}

void ProtoMidi::m_afterTouchMono(t_symbol* s, const AtomListView& lv)
{
    if (!checkMethodByte2(s, lv)) {
        METHOD_ERR(s) << "usage: CHAN DATA";
        return;
    }

    byteStatus(midi::MIDI_AFTERTOUCH, lv[0].asT<int>());
    byteData(lv[1].asT<int>());
}

bool ProtoMidi::checkMethodByte2(t_symbol* m, const AtomListView& lv)
{
    if (lv.size() != 2) {
        METHOD_ERR(m) << "invalid arg count: " << lv.size();
        return false;
    }

    const auto chan = lv[0].asInt(-1);
    const auto byte0 = lv[1].asInt(-1);

    if (chan < 0 || chan > 15) {
        METHOD_ERR(m) << "channel value in [0-15] range expected, got: " << lv[0];
        return false;
    }

    if (byte0 < 0 || byte0 > 127) {
        METHOD_ERR(m) << "byte value in [0-127] range expected, got: " << lv[1];
        return false;
    }

    return true;
}

bool ProtoMidi::checkMethodByte3(t_symbol* m, const AtomListView& lv)
{
    if (lv.size() != 3) {
        METHOD_ERR(m) << "invalid arg count: " << lv.size();
        return false;
    }

    const auto chan = lv[0].asInt(-1);
    const auto byte0 = lv[1].asInt(-1);
    const auto byte1 = lv[2].asInt(-1);

    if (chan < 0 || chan > 15) {
        METHOD_ERR(m) << "channel value in [0-15] range expected, got: " << lv[0];
        return false;
    }

    if (byte0 < 0 || byte0 > 127) {
        METHOD_ERR(m) << "byte value in [0-127] range expected, got: " << lv[1];
        return false;
    }

    if (byte1 < 0 || byte1 > 127) {
        METHOD_ERR(m) << "byte value in [0-127] range expected, got: " << lv[2];
        return false;
    }

    return true;
}

void setup_proto_midi()
{
    SYM_ACTIVESENSE = gensym("activesense");
    SYM_AFTOUCH_MONO = gensym("aftertouch");
    SYM_AFTOUCH_POLY = gensym("polytouch");
    SYM_CLOCK = gensym("clock");
    SYM_CONTINUE = gensym("continue");
    SYM_NOTEOFF = gensym("noteoff");
    SYM_NOTEON = gensym("noteon");
    SYM_START = gensym("start");
    SYM_STOP = gensym("stop");
    SYM_SYSRESET = gensym("sysreset");
    SYM_TICK = gensym("tick");

    ObjectFactory<ProtoMidi> obj("proto.midi");

    obj.addMethod(SYM_NOTEON->s_name, &ProtoMidi::m_noteOn);
    obj.addMethod("note", &ProtoMidi::m_noteOn);
    obj.addMethod(SYM_NOTEOFF->s_name, &ProtoMidi::m_noteOff);
    obj.addMethod(SYM_AFTOUCH_MONO->s_name, &ProtoMidi::m_afterTouchMono);
}
