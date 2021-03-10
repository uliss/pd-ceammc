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
#include "ceammc_convert.h"
#include "ceammc_factory.h"
#include "ceammc_units.h"

#include <tuple>

static t_symbol* SYM_ACTIVESENSE;
static t_symbol* SYM_AFTOUCH_MONO;
static t_symbol* SYM_AFTOUCH_POLY;
static t_symbol* SYM_CLOCK;
static t_symbol* SYM_CONTINUE;
static t_symbol* SYM_CONTROLCHANGE;
static t_symbol* SYM_NOTEOFF;
static t_symbol* SYM_NOTEON;
static t_symbol* SYM_PITCHWHEEL;
static t_symbol* SYM_PROGRAMCHANGE;
static t_symbol* SYM_SONGPOS;
static t_symbol* SYM_SONGSELECT;
static t_symbol* SYM_START;
static t_symbol* SYM_STOP;
static t_symbol* SYM_SYSRESET;
static t_symbol* SYM_SYSEX;
static t_symbol* SYM_TICK;
static t_symbol* SYM_TIMECODE;
static t_symbol* SYM_TUNEREQUEST;

static std::tuple<uint8_t, uint8_t> floatToBit14(t_float v)
{
    constexpr int16_t IN_MIN = -0x2000;
    constexpr int16_t IN_MAX = 0x1fff;

    const auto ival = clip<int16_t, IN_MIN, IN_MAX>(v);
    const auto uval = static_cast<uint16_t>(ival - IN_MIN);
    const uint8_t lsb = 0x7F & uval;
    const uint8_t msb = 0x7F & (uval >> 7);

    return { lsb, msb };
}

t_float MidiQuaterFrame::floatFps() const
{
    switch (fps_) {
    case FPS_24:
        return 24;
    case FPS_25:
        return 25;
    case FPS_30_DROP:
        return 29.97;
    case FPS_30:
        return 30;
    default:
        return 24;
    }
}

t_float MidiQuaterFrame::floatSeconds() const
{
    return frames() / floatFps() + seconds() + minutes() * 60 + hours() * 3600;
}

bool MidiQuaterFrame::pushByte(uint8_t byte)
{
    const uint8_t lnib = 0x0F & byte;
    const uint8_t unib = 0xF0 & (byte << 4);

    switch (0x70 & byte) {
    case 0x00: // lower frames nib
        data_[3] = (0xF0 & data_[3]) | lnib;
        break;
    case 0x10:
        data_[3] = (0x0F & data_[3]) | unib;
        break;
    case 0x20: // lower seconds nib
        data_[2] = (0xF0 & data_[2]) | lnib;
        break;
    case 0x30: // upper seconds nib
        data_[2] = (0x0F & data_[2]) | unib;
        break;
    case 0x40: // lower minutes nib
        data_[1] = (0xF0 & data_[1]) | lnib;
        break;
    case 0x50: // upper minutes nib
        data_[1] = (0x0F & data_[1]) | unib;
        break;
    case 0x60: // lower hours nib
        data_[0] = (0xF0 & data_[0]) | lnib;
        break;
    case 0x70: { // upper hours
        // HOUR BIT2 BIT1 UNUSED
        data_[0] = (0x0F & data_[0]) | ((byte & 0b1000) << 1);
        fps_ = static_cast<FPS>((byte >> 1) & 0b0011);
        return true;
    } break;
    default:
        break;
    }

    return false;
}

void MidiQuaterFrame::set(uint8_t h, uint8_t min, uint8_t sec, uint8_t f, MidiQuaterFrame::FPS fps)
{
    data_[3] = f;
    data_[2] = sec;
    data_[1] = min;
    data_[0] = h;
    fps_ = fps;
}

MidiQuaterFrame::Msg MidiQuaterFrame::get() const
{
    Msg out;
    out[0] = 0x00 | (0x0F & data_[3]);
    out[1] = 0x10 | (0x0F & (data_[3] >> 4));
    out[2] = 0x20 | (0x0F & data_[2]);
    out[3] = 0x30 | (0x0F & (data_[2] >> 4));
    out[4] = 0x40 | (0x0F & data_[1]);
    out[5] = 0x50 | (0x0F & (data_[1] >> 4));
    out[6] = 0x60 | (0x0F & data_[0]);
    const uint8_t uh = 0b1000 & (data_[0] >> 1);
    const uint8_t fr = 0b0110 & (fps_ << 1);
    out[7] = 0x70 | uh | fr;
    return out;
}

ProtoMidi::ProtoMidi(const PdArgs& args)
    : BaseObject(args)
{
    createOutlet();

    using midi::Byte;

    parser_.setNoteOnFn([this](Byte b, Byte n, Byte v) {
        Atom msg[3] = { 0x0F & b, n, v };
        msgTo(SYM_NOTEON, msg, 3);
    });

    parser_.setNoteOffFn([this](Byte b, Byte n, Byte v) {
        Atom msg[3] = { 0x0F & b, n, v };
        msgTo(SYM_NOTEOFF, msg, 3);
    });

    parser_.setAfterTouchFn([this](Byte b, Byte v) {
        Atom msg[2] = { 0x0F & b, v };
        msgTo(SYM_AFTOUCH_MONO, msg, 2);
    });

    parser_.setPolyTouchFn([this](Byte b, Byte n, Byte v) {
        Atom msg[3] = { 0x0F & b, n, v };
        msgTo(SYM_AFTOUCH_POLY, msg, 3);
    });

    parser_.setControlChangeFn([this](Byte b, Byte c, Byte v) {
        Atom msg[3] = { 0x0F & b, c, v };
        msgTo(SYM_CONTROLCHANGE, msg, 3);
    });

    parser_.setProgramChangeFn([this](Byte b, Byte v) {
        Atom msg[2] = { 0x0F & b, v };
        msgTo(SYM_PROGRAMCHANGE, msg, 2);
    });

    parser_.setPitchWheelFn([this](Byte b, Byte lsb, Byte msb) {
        const t_float v = ((msb << 7) | lsb) - 0x2000;
        Atom msg[2] = { 0x0F & b, v };
        msgTo(SYM_PITCHWHEEL, msg, 2);
    });

    parser_.setSysCommonFn([this](Byte b, Byte d0, Byte d1) {
        switch (b) {
        case midi::MIDI_TUNEREQUEST:
            return anyTo(0, SYM_TUNEREQUEST, AtomListView());
        case midi::MIDI_SONGSELECT:
            return anyTo(0, SYM_SONGSELECT, Atom(0x7F & d0));
        case midi::MIDI_SONGPOS:
            return anyTo(0, SYM_SONGPOS, Atom((d1 << 7) | d0));
        case midi::MIDI_TIMECODE:
            return handleTimecode(d0);
        default:
            OBJ_ERR << "unknown system common message: " << (int)b;
            break;
        }
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

    parser_.setSysExFn([this](size_t n, const Byte* b) {
        Atom msg[n];
        for (size_t i = 0; i < n; i++)
            msg[i] = b[i];

        msgTo(SYM_SYSEX, msg, n);
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

void ProtoMidi::m_activeSense(t_symbol*, const AtomListView&)
{
    floatTo(0, midi::MIDI_ACTIVE_SENSE);
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

void ProtoMidi::m_programChange(t_symbol* s, const AtomListView& lv)
{
    if (!checkMethodByte2(s, lv)) {
        METHOD_ERR(s) << "usage: CHAN PROG";
        return;
    }

    byteStatus(midi::MIDI_PROGRAMCHANGE, lv[0].asT<int>());
    byteData(lv[1].asT<int>());
}

void ProtoMidi::m_songPosition(t_symbol* s, const AtomListView& lv)
{
    if (lv.size() < 1) {
        METHOD_ERR(s) << "invalid arg count: " << lv.size() << ", usage: POS [DUR=16]";
        return;
    }

    const auto pos = lv[0].asInt(-1);
    const auto dur = lv.intAt(1, 16);
    int k = 1;

    switch (dur) {
    case 1:
    case 2:
    case 4:
    case 8:
    case 16:
        k = 16 / dur;
        break;
    default:
        METHOD_ERR(s) << "invalid position duration, valid values are: 1, 2, 4, 8, 16(default)";
        break;
    }

    const int ipos = pos * k;

    if (ipos < 0 || ipos > 0x3fff) {
        METHOD_ERR(s) << "song position in [0..0x3fff] range expected, got: " << ipos;
        return;
    }

    const uint16_t upos = ipos;

    floatTo(0, midi::MIDI_SONGPOS);
    byteData(upos & 0x7F);
    byteData(upos >> 7);
}

void ProtoMidi::m_songSelect(t_symbol* s, const AtomListView& lv)
{
    if (lv.size() != 1) {
        METHOD_ERR(s) << "invalid arg count: " << lv.size() << ", usage: SONG_IDX";
        return;
    }

    const auto idx = lv[0].asInt(-1);

    if (idx < 0 || idx > 0x7F) {
        METHOD_ERR(s) << "song index in [0..127] range expected, got: " << lv[0];
        return;
    }

    floatTo(0, midi::MIDI_SONGSELECT);
    byteData(idx);
}

void ProtoMidi::m_start(t_symbol*, const AtomListView&)
{
    floatTo(0, midi::MIDI_START);
}

void ProtoMidi::m_stop(t_symbol*, const AtomListView&)
{
    floatTo(0, midi::MIDI_STOP);
}

void ProtoMidi::m_sysex(t_symbol*, const AtomListView& lv)
{
    floatTo(0, midi::MIDI_SYSEX);
    for (auto& a : lv) {
        if (a.isFloat()) {
            auto b = 0x7F & int(a.asT<int>());
            floatTo(0, b);
        }
    }
    floatTo(0, midi::MIDI_SYSEXEND);
}

void ProtoMidi::m_sysReset(t_symbol*, const AtomListView&)
{
    floatTo(0, midi::MIDI_SYSTEM_RESET);
}

void ProtoMidi::m_tick(t_symbol*, const AtomListView&)
{
    floatTo(0, midi::MIDI_TIMETICK);
}

void ProtoMidi::m_tuneRequest(t_symbol*, const AtomListView&)
{
    floatTo(0, midi::MIDI_TUNEREQUEST);
}

void ProtoMidi::m_pitchWheel(t_symbol* s, const AtomListView& lv)
{
    constexpr int ZERO = 0x2000;
    constexpr int FROM = -ZERO;
    constexpr int TO = 0x3FFF - ZERO;

    if (!checkMethodByte2(s, lv, FROM, TO)) {
        METHOD_ERR(s) << "usage: CHAN VALUE(" << FROM << ".." << TO << ")";
        return;
    }

    byteStatus(midi::MIDI_PITCHBEND, lv[0].asT<int>());
    auto bb = floatToBit14(lv[1].asInt());
    byteData(std::get<0>(bb));
    byteData(std::get<1>(bb));
}

void ProtoMidi::m_clock(t_symbol*, const AtomListView&)
{
    floatTo(0, midi::MIDI_TIMECLOCK);
}

void ProtoMidi::m_continue(t_symbol*, const AtomListView&)
{
    floatTo(0, midi::MIDI_CONTINUE);
}

void ProtoMidi::m_timecode(t_symbol* s, const AtomListView& lv)
{
    if (lv.size() != 2) {
        METHOD_ERR(s) << "usage: TIME FPS";
        return;
    }

    auto is_equal = [](t_float f, t_float to) {
        return std::fabs(f - to) < 0.01;
    };

    const auto tm = lv.subView(0, 1);
    const auto fps = lv[1].asFloat();

    MidiQuaterFrame::FPS msg_fps;
    if (is_equal(fps, 24))
        msg_fps = MidiQuaterFrame::FPS_24;
    else if (is_equal(fps, 25))
        msg_fps = MidiQuaterFrame::FPS_25;
    else if (is_equal(fps, 29.97f))
        msg_fps = MidiQuaterFrame::FPS_30_DROP;
    else if (is_equal(fps, 30))
        msg_fps = MidiQuaterFrame::FPS_30;
    else {
        METHOD_ERR(s) << "expected FPS values: 24, 25, 29.97 or 30, got: " << lv[1];
        return;
    }

    auto res = units::TimeValue::parse(tm);
    units::TimeValue val(0);
    units::UnitParseError err;

    val.setFramerate(fps);

    if (res.matchValue(val)) {
        MidiQuaterFrame mqf;

        if (val.units() == units::TimeValue::SMPTE) {
            int v = val.value();
            int frames = v % 1000;
            int sec = v / 1000;
            int min = sec / 60;
            int hour = min / 60;

            mqf.set(hour % 32, min % 60, sec % 60, frames, msg_fps);
        } else {
            t_float ms = val.toMs();
            int frames = (fps * (int(ms) % 1000)) / 1000;
            int sec = int(ms / 1000);
            int min = sec / 60;
            int hour = min / 60;

            mqf.set(hour % 32, min % 60, sec % 60, frames, msg_fps);
        }

        for (auto b : mqf.get()) {
            floatTo(0, midi::MIDI_TIMECODE);
            byteData(b);
        }

    } else if (res.matchError(err)) {
        METHOD_ERR(s) << "parse error: " << err.msg;
        return;
    }
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

void ProtoMidi::m_afterTouchPoly(t_symbol* s, const AtomListView& lv)
{
    if (!checkMethodByte3(s, lv)) {
        METHOD_ERR(s) << "usage: CHAN NOTE VEL";
        return;
    }

    byteStatus(midi::MIDI_POLYAFTERTOUCH, lv[0].asT<int>());
    byteData(lv[1].asT<int>());
    byteData(lv[2].asT<int>());
}

void ProtoMidi::m_cc(t_symbol* s, const AtomListView& lv)
{
    if (!checkMethodByte3(s, lv)) {
        METHOD_ERR(s) << "usage: CHAN CC VALUE";
        return;
    }

    byteStatus(midi::MIDI_CONTROLCHANGE, lv[0].asT<int>());
    byteData(lv[1].asT<int>());
    byteData(lv[2].asT<int>());
}

bool ProtoMidi::checkMethodByte2(t_symbol* m, const AtomListView& lv, int from, int to)
{
    if (lv.size() != 2) {
        METHOD_ERR(m) << "invalid arg count: " << lv.size();
        return false;
    }

    const auto chan = lv[0].asInt(-1);
    const auto byte0 = lv[1].asInt(-1);

    if (chan < 0 || chan > 15) {
        METHOD_ERR(m) << "channel value in [0..15] range expected, got: " << lv[0];
        return false;
    }

    if (byte0 < from || byte0 > to) {
        METHOD_ERR(m) << "byte value in [" << from << ".." << to << "] range expected, got: " << lv[1];
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
        METHOD_ERR(m) << "channel value in [0..15] range expected, got: " << lv[0];
        return false;
    }

    if (byte0 < 0 || byte0 > 127) {
        METHOD_ERR(m) << "byte value in [0..127] range expected, got: " << lv[1];
        return false;
    }

    if (byte1 < 0 || byte1 > 127) {
        METHOD_ERR(m) << "byte value in [0..127] range expected, got: " << lv[2];
        return false;
    }

    return true;
}

void ProtoMidi::handleTimecode(uint8_t data)
{
    if (mqf_.pushByte(data)) {
        Atom msg[5] = { mqf_.hours(), mqf_.minutes(), mqf_.seconds(), mqf_.frames(), mqf_.floatFps() };
        msgTo(SYM_TIMECODE, msg, 5);
    }
}

void setup_proto_midi()
{
    SYM_ACTIVESENSE = gensym("activesense");
    SYM_AFTOUCH_MONO = gensym("aftertouch");
    SYM_AFTOUCH_POLY = gensym("polytouch");
    SYM_CLOCK = gensym("clock");
    SYM_CONTINUE = gensym("continue");
    SYM_CONTROLCHANGE = gensym("cc");
    SYM_NOTEOFF = gensym("noteoff");
    SYM_NOTEON = gensym("noteon");
    SYM_PITCHWHEEL = gensym("pitchwheel");
    SYM_PROGRAMCHANGE = gensym("program");
    SYM_SONGSELECT = gensym("songsel");
    SYM_SONGPOS = gensym("songpos");
    SYM_START = gensym("start");
    SYM_STOP = gensym("stop");
    SYM_SYSRESET = gensym("sysreset");
    SYM_SYSEX = gensym("sysex");
    SYM_TICK = gensym("tick");
    SYM_TIMECODE = gensym("timecode");
    SYM_TUNEREQUEST = gensym("tunerequest");

    ObjectFactory<ProtoMidi> obj("proto.midi");

    obj.addMethod("note", &ProtoMidi::m_noteOn);
    obj.addMethod(SYM_ACTIVESENSE->s_name, &ProtoMidi::m_activeSense);
    obj.addMethod(SYM_AFTOUCH_MONO->s_name, &ProtoMidi::m_afterTouchMono);
    obj.addMethod(SYM_AFTOUCH_POLY->s_name, &ProtoMidi::m_afterTouchPoly);
    obj.addMethod(SYM_CLOCK->s_name, &ProtoMidi::m_clock);
    obj.addMethod(SYM_CONTINUE->s_name, &ProtoMidi::m_continue);
    obj.addMethod(SYM_CONTROLCHANGE->s_name, &ProtoMidi::m_cc);
    obj.addMethod(SYM_NOTEOFF->s_name, &ProtoMidi::m_noteOff);
    obj.addMethod(SYM_NOTEON->s_name, &ProtoMidi::m_noteOn);
    obj.addMethod(SYM_PITCHWHEEL->s_name, &ProtoMidi::m_pitchWheel);
    obj.addMethod(SYM_PROGRAMCHANGE->s_name, &ProtoMidi::m_programChange);
    obj.addMethod(SYM_SONGPOS->s_name, &ProtoMidi::m_songPosition);
    obj.addMethod(SYM_SONGSELECT->s_name, &ProtoMidi::m_songSelect);
    obj.addMethod(SYM_START->s_name, &ProtoMidi::m_start);
    obj.addMethod(SYM_STOP->s_name, &ProtoMidi::m_stop);
    obj.addMethod(SYM_SYSEX->s_name, &ProtoMidi::m_sysex);
    obj.addMethod(SYM_SYSRESET->s_name, &ProtoMidi::m_sysReset);
    obj.addMethod(SYM_TICK->s_name, &ProtoMidi::m_tick);
    obj.addMethod(SYM_TIMECODE->s_name, &ProtoMidi::m_timecode);
    obj.addMethod(SYM_TUNEREQUEST->s_name, &ProtoMidi::m_tuneRequest);
}
