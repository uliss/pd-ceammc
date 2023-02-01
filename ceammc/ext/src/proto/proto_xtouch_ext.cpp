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
#include "proto_xtouch_ext.h"
#include "ceammc_convert.h"
#include "ceammc_factory.h"
#include "ceammc_format.h"

#include <array>
#include <cstdlib>

constexpr int MAX_VU_DB = 0;
constexpr int MIN_VU_DB = -60;
constexpr int MAX_SCENES = 32;
constexpr int MIN_SCENES = 1;

constexpr int MAX_CONTROLS = Scene::NCHAN * MAX_SCENES;
constexpr int XT_FADER_FIRST = 70;
constexpr int XT_FADER_LAST = XT_FADER_FIRST + Scene::NCHAN;
constexpr int XT_KNOB_FIRST = 80;
constexpr int XT_KNOB_LAST = XT_KNOB_FIRST + Scene::NCHAN;
constexpr int XT_VU_FIRST = 90;
constexpr int XT_VU_LAST = XT_KNOB_FIRST + Scene::NCHAN;
constexpr int XT_REC_FIRST = 8;
constexpr int XT_REC_LAST = XT_REC_FIRST + Scene::NCHAN;
constexpr int XT_BTN_KNOB_FIRST = 0;
constexpr int XT_BTN_KNOB_LAST = XT_BTN_KNOB_FIRST + Scene::NCHAN;
constexpr int XT_SOLO_FIRST = 16;
constexpr int XT_SOLO_LAST = XT_SOLO_FIRST + Scene::NCHAN;
constexpr int XT_MUTE_FIRST = 24;
constexpr int XT_MUTE_LAST = XT_MUTE_FIRST + Scene::NCHAN;
constexpr int XT_SELECT_FIRST = 32;
constexpr int XT_SELECT_LAST = XT_SELECT_FIRST + Scene::NCHAN;
constexpr int XT_FADER_MOVE_FIRST = 110;
constexpr int XT_FADER_MOVE_LAST = XT_FADER_MOVE_FIRST + Scene::NCHAN;

static std::array<t_symbol*, MAX_CONTROLS> SYM_FADERS;
static std::array<t_symbol*, MAX_CONTROLS> SYM_KNOBS;
static std::array<t_symbol*, MAX_CONTROLS> SYM_BTN_REC;
static std::array<t_symbol*, MAX_CONTROLS> SYM_BTN_SOLO;
static std::array<t_symbol*, MAX_CONTROLS> SYM_BTN_MUTE;
static std::array<t_symbol*, MAX_CONTROLS> SYM_BTN_SELECT;
static std::array<t_symbol*, MAX_CONTROLS> SYM_BTN_KNOB;

static t_symbol* PROTO_HUI;
static t_symbol* PROTO_MCU;
static t_symbol* PROTO_XMIDI;
static t_symbol* SYM_ALL;
static t_symbol* SYM_BLACK;
static t_symbol* SYM_BLUE;
static t_symbol* SYM_CYAN;
static t_symbol* SYM_GREEN;
static t_symbol* SYM_MAGENTA;
static t_symbol* SYM_RANDOM;
static t_symbol* SYM_RED;
static t_symbol* SYM_SCENE;
static t_symbol* SYM_WHITE;
static t_symbol* SYM_YELLOW;
static t_symbol* SYM_CENTER;
static t_symbol* SYM_LEFT;
static t_symbol* SYM_RIGHT;
static t_symbol* SYM_JUSTIFY;
static t_symbol* SYM_AUTO;
static t_symbol* SYM_SELECT;
static t_symbol* SYM_MUTE;
static t_symbol* SYM_SOLO;
static t_symbol* SYM_REC;
static t_symbol* SYM_BTN;
static t_symbol* SYM_TILDE;
static t_symbol* SYM_EXCLAM;

enum MidiMSG {
    /* channel voice messages */
    MIDI_NOTEOFF = 0x80,
    MIDI_NOTEON = 0x90,
    MIDI_POLYAFTERTOUCH = 0xa0,
    MIDI_CONTROLCHANGE = 0xb0,
    MIDI_PROGRAMCHANGE = 0xc0,
    MIDI_AFTERTOUCH = 0xd0,
    MIDI_PITCHBEND = 0xe0,
    /* system common messages */
    MIDI_SYSEX = 0xf0,
    MIDI_TIMECODE = 0xf1,
    MIDI_SONGPOS = 0xf2,
    MIDI_SONGSELECT = 0xf3,
    MIDI_TUNEREQUEST = 0xf6,
    MIDI_SYSEXEND = 0xf7
};

static void cc_set(Atom m[3], uint8_t n, uint8_t cc, uint8_t v)
{
    m[0].setFloat(MIDI_FSM_STATE_CONTROLCHANGE | (0x0F & n), true);
    m[1].setFloat(cc, true);
    m[2].setFloat(v, true);
}

static void note_set(Atom m[3], uint8_t n, uint8_t note, uint8_t vel)
{
    m[0].setFloat(MIDI_FSM_STATE_NOTE_ON | (0x0F & n), true);
    m[1].setFloat(note, true);
    m[2].setFloat(vel, true);
}

XTouchExtender::XTouchExtender(const PdArgs& args)
    : BaseObject(args)
    , num_scenes_(nullptr)
    , scene_(nullptr)
    , proto_(nullptr)
{
    createOutlet();
    createOutlet();

    proto_ = new SymbolEnumProperty("@proto", { PROTO_XMIDI, PROTO_HUI, PROTO_MCU });
    proto_->setArgIndex(0);
    addProperty(proto_);

    num_scenes_ = new IntProperty("@n", MIN_SCENES + 1, PropValueAccess::INITONLY);
    num_scenes_->checkClosedRange(MIN_SCENES, MAX_SCENES);
    addProperty(num_scenes_);

    scene_ = new IntProperty("@scene", 0);
    scene_->setSuccessFn([this](Property*) { syncScene(); });
    addProperty(scene_);

    parser_.reset();
}

void XTouchExtender::onFloat(t_float f)
{
    try {
        const bool ok = (0 <= f && f <= 256 && (f == static_cast<int>(f)));
        if (!ok) {
            OBJ_ERR << "integer in 0..256 range expected";
            return;
        }

        const uint8_t byte = f;
        switch (parser_.state) {
        case MIDI_FSM_STATE_INIT: {
            const bool is_status_byte = 0x80 & byte;
            if (!is_status_byte) {
                OBJ_ERR << "unexpected raw midi value: " << (int)byte;
                return;
            }

            const uint8_t status = byte & 0xf0;
            switch (status) {
            case MIDI_FSM_STATE_NOTE_ON:
            case MIDI_FSM_STATE_NOTE_OFF:
            case MIDI_FSM_STATE_CONTROLCHANGE:
                parser_.state = static_cast<MidiFSMState>(status); // new state
                parser_.appendData(byte);
                break;
            default:
                // not a status byte in init state
                OBJ_ERR << "unexpected midi value: " << (int)byte;
                parser_.reset();
                break;
            }
        } break;
        // data bytes
        case MIDI_FSM_STATE_NOTE_ON:
        case MIDI_FSM_STATE_NOTE_OFF:
        case MIDI_FSM_STATE_CONTROLCHANGE: {
            const bool is_status_byte = 0x80 & byte;
            if (is_status_byte) {
                OBJ_ERR << "unexpected status byte value: " << (int)byte;
                parser_.reset();
            } else {
                parser_.appendData(byte);
            }
        } break;
        default:
            break;
        }

        if (parser_.isReady()) {
            const auto* proto = proto_->value();
            if (proto == PROTO_XMIDI)
                parseXMidi();
            else if (proto == PROTO_HUI)
                parseHui();
            else if (proto == PROTO_MCU)
                parseMcu();

            parser_.reset();
        }
    } catch (std::exception& e) {
        OBJ_ERR << "exception: " << e.what();
    }
}

int XTouchExtender::msgToIndex(const char* msg)
{
    char* end = 0;
    const auto idx = std::strtol(msg, &end, 10);

    if (msg == end)
        return -1;

    if (idx < 0 || idx >= numLogicChannels())
        return -1;

    return idx;
}

void XTouchExtender::onAny(t_symbol* s, const AtomListView& l)
{
    typedef Button& (Scene::*ButtonGetFn)(uint8_t);
    typedef Fader& (Scene::*FaderGetFn)(uint8_t);

    auto set_btn = [this](const char* msg, const AtomListView& l, ButtonGetFn fn) {
        auto idx = msgToIndex(msg);
        if (idx < 0) {
            LIB_ERR << "invalid channel: " << idx;
            return;
        }

        if (l.size() != 1) {
            LIB_ERR << "single value expected, got: " << l;
            return;
        }

        auto& sc = sceneByLogicIdx(idx);
        auto& btn = (sc.*fn)(idx);
        btn.setState(l[0]);
    };

    auto set_fader = [this](const char* msg, const AtomListView& l, FaderGetFn fn) {
        auto idx = msgToIndex(msg);
        if (idx < 0) {
            LIB_ERR << "invalid channel: " << idx;
            return;
        }

        if (l.size() != 1) {
            LIB_ERR << "single value expected, got: " << l;
            return;
        }

        auto& sc = sceneByLogicIdx(idx);
        auto& f = (sc.*fn)(idx);
        f.setValue(l[0]);
    };

    auto equal = [](const char* msg, const char* pat) {
        return strncmp(msg, pat, strlen(pat)) == 0;
    };

    const char* msg = s->s_name;

    if (equal(msg, "rec"))
        set_btn(msg + 3, l, &Scene::rec);
    else if (equal(msg, "solo"))
        set_btn(msg + 4, l, &Scene::solo);
    else if (equal(msg, "mute"))
        set_btn(msg + 4, l, &Scene::mute);
    else if (equal(msg, "select"))
        set_btn(msg + 6, l, &Scene::select);
    else if (equal(msg, "fader"))
        set_fader(msg + 5, l, &Scene::fader);
    else if (equal(msg, "knob"))
        set_fader(msg + 4, l, &Scene::knob);
    else
        LIB_ERR << "unknown message: " << s;
}

void XTouchExtender::initDone()
{
    const auto N = num_scenes_->value();
    scenes_.resize(N);
    scene_->checkClosedRange(0, N - 1);

    for (int i = 0; i < N; i++) {
        const auto NCH = scenes_[i].NCHAN;

        // init faders
        for (int j = 0; j < NCH; j++) {
            auto& f = scenes_[i].fader(j);
            f.setFn([this, i, j](t_float v) { sendFader(i, j, v); });
        }

        // init knobs
        for (int j = 0; j < NCH; j++) {
            auto& k = scenes_[i].knob(j);
            k.setFn([this, i, j](t_float v) { sendKnob(i, j, v); });
        }

        // init btn rec
        for (int j = 0; j < NCH; j++) {
            auto& btn = scenes_[i].rec(j);
            btn.setMode(Button::TOGGLE);
            btn.setFn([this, i, j](int v) { sendRec(i, j, v); });
        }

        // init btn solo
        for (int j = 0; j < NCH; j++) {
            auto& btn = scenes_[i].solo(j);
            btn.setMode(Button::TOGGLE);
            btn.setFn([this, i, j](int v) { sendSolo(i, j, v); });
        }

        // init btn mute
        for (int j = 0; j < NCH; j++) {
            auto& btn = scenes_[i].mute(j);
            btn.setMode(Button::TOGGLE);
            btn.setFn([this, i, j](int v) { sendMute(i, j, v); });
        }

        // init btn select
        for (int j = 0; j < NCH; j++) {
            auto& btn = scenes_[i].select(j);
            btn.setMode(Button::BUTTON);
            btn.setFn([this, i, j](int v) { sendSelect(i, j, v); });
        }

        for (int j = 0; j < NCH; j++)
            scenes_[i].display(j).setDefault();
    }

    resetVu();
}

void XTouchExtender::m_vu(t_symbol* s, const AtomListView& lv)
{
    constexpr int NOT_FOUND = -1000;
    const auto idx = lv.intAt(0, NOT_FOUND);
    const auto db_val = lv.floatAt(1, NOT_FOUND);
    if (idx == NOT_FOUND || db_val == NOT_FOUND) {
        METHOD_ERR(s) << "usage: CHAN DB_VALUE";
        return;
    }

    if (idx >= MAX_CONTROLS) {
        METHOD_ERR(s) << "expected channel value in 0.." << (MAX_CONTROLS - 1) << " range, got: " << idx;
        return;
    }

    sendVu(idx, db_val);
}

void XTouchExtender::m_reset(t_symbol* s, const AtomListView& lv)
{
    resetVu();
    resetFaders();
    resetKnobs();
    resetButtons();

    parser_.reset();
}

static bool in_range(uint8_t v, uint8_t min, uint8_t max)
{
    return min <= v && v < max;
}

void XTouchExtender::parseXMidi()
{
    if (!parser_.isReady())
        return;

    if (parser_.isCC()) {
        const auto cc = parser_.data[1];
        if (in_range(cc, XT_FADER_FIRST, XT_FADER_LAST)) {
            const t_float val = convert::lin2lin_clip<t_float, 0, 127>(parser_.data[2], 0, 1);
            const auto ch = cc - XT_FADER_FIRST;
            currentScene().fader(ch).setValue(val);
        } else if (in_range(cc, XT_KNOB_FIRST, XT_KNOB_LAST)) {
            const t_float val = convert::lin2lin_clip<t_float, 0, 127>(parser_.data[2], 0, 1);
            const auto ch = cc - XT_KNOB_FIRST;
            currentScene().knob(ch).setValue(val);
        } else {
            OBJ_ERR << "unknown CC: " << (int)cc;
        }
    } else if (parser_.isNoteOn()) {
        const auto note = parser_.data[1];
        if (in_range(note, XT_BTN_KNOB_FIRST, XT_BTN_KNOB_LAST)) {
            const auto ch = note - XT_BTN_KNOB_FIRST;
            const int vel = parser_.data[2];
            sendKnobButton(scene_->value(), ch, vel);
        } else if (in_range(note, XT_REC_FIRST, XT_REC_LAST)) {
            const auto ch = note - XT_REC_FIRST;
            const int vel = parser_.data[2];
            currentScene().rec(ch).setMidi(vel);
        } else if (in_range(note, XT_SOLO_FIRST, XT_SOLO_LAST)) {
            const auto ch = note - XT_SOLO_FIRST;
            const int vel = parser_.data[2];
            currentScene().solo(ch).setMidi(vel);
        } else if (in_range(note, XT_MUTE_FIRST, XT_MUTE_LAST)) {
            const auto ch = note - XT_MUTE_FIRST;
            const int vel = parser_.data[2];
            currentScene().mute(ch).setMidi(vel);
        } else if (in_range(note, XT_SELECT_FIRST, XT_SELECT_LAST)) {
            const auto ch = note - XT_SELECT_FIRST;
            const int vel = parser_.data[2];
            currentScene().select(ch).setMidi(vel);
        } else if (in_range(note, XT_FADER_MOVE_FIRST, XT_FADER_MOVE_LAST)) {
            const auto ch = note - XT_FADER_MOVE_FIRST;
            const int velocity = parser_.data[2];
            currentScene().fader_is_moving_[ch] = (velocity > 0);
        }
    }
}

void XTouchExtender::parseHui()
{
}

void XTouchExtender::parseMcu()
{
}

void XTouchExtender::sendVu(uint8_t idx, int db)
{
    const uint8_t scene_idx = idx / Scene::NCHAN;
    if (scene_->value() != scene_idx)
        return;

    if (proto_->value() == PROTO_XMIDI) {
        const uint8_t cc = XT_VU_FIRST + (0x0f & (idx % Scene::NCHAN));
        const uint8_t val = std::round(convert::lin2lin_clip<t_float>(db, MIN_VU_DB, MAX_VU_DB, 0, 127));
        sendCC(cc, val);
    } else {
        OBJ_ERR << "not implemented yet: " << proto_->value() << " " << __FUNCTION__;
    }
}

void XTouchExtender::resetVu()
{
    int off = scene_->value() * Scene::NCHAN;
    for (int i = 0; i < Scene::NCHAN; i++)
        sendVu(off + i, MIN_VU_DB);
}

void XTouchExtender::resetFaders()
{
    for (size_t si = 0; si < scenes_.size(); si++) {
        auto& f = scenes_[si].faders_;
        for (size_t fi = 0; fi < f.size(); fi++) {
            f[fi].setValue(0);
            sendFader(si, fi, 0);
        }
    }
}

void XTouchExtender::resetKnobs()
{
    for (size_t si = 0; si < scenes_.size(); si++) {
        auto& k = scenes_[si].knobs_;
        for (size_t ki = 0; ki < k.size(); ki++) {
            k[ki].setValue(0);
            sendKnob(si, ki, 0);
        }
    }
}

void XTouchExtender::resetButtons()
{
    for (size_t scene_idx = 0; scene_idx < scenes_.size(); scene_idx++) {
        auto& sc = scenes_[scene_idx];
        auto& r = sc.btn_rec_;
        auto& s = sc.btn_solo_;
        auto& m = sc.btn_mute_;
        auto& sel = sc.btn_select_;

        for (size_t i = 0; i < r.size(); i++) {
            r[i].setState(Button::OFF);
            s[i].setState(Button::OFF);
            m[i].setState(Button::OFF);
            sel[i].setState(Button::OFF);
        }
    }
}

void XTouchExtender::syncScene()
{
    const auto scene_idx = scene_->value();
    auto& faders = currentScene().faders_;
    auto& knobs = currentScene().knobs_;
    auto& recs = currentScene().btn_rec_;
    auto& solo = currentScene().btn_solo_;
    auto& mute = currentScene().btn_mute_;
    auto& select = currentScene().btn_select_;

    OBJ_LOG << "sync scene: " << scene_idx;

    for (size_t i = 0; i < faders.size(); i++) {
        sendFader(scene_idx, i, faders[i].value());
        sendKnob(scene_idx, i, knobs[i].value());
        sendRec(scene_idx, i, recs[i].state());
        sendSolo(scene_idx, i, solo[i].state());
        sendMute(scene_idx, i, mute[i].state());
        sendSelect(scene_idx, i, select[i].state());
        syncDisplay(scene_idx, i);
    }
}

void XTouchExtender::sendFader(uint8_t scene_idx, uint8_t ctl_idx, t_float v)
{
    OBJ_LOG << "sendFader: " << (int)scene_idx << ' ' << (int)ctl_idx << ' ' << v;

    if (proto_->value() == PROTO_XMIDI) {
        const int logic_idx = (scene_idx * Scene::NCHAN + ctl_idx) % MAX_CONTROLS;

        // output for any scenes
        anyTo(1, SYM_FADERS[logic_idx], AtomListView(Atom(v)));

        const bool is_moving = sceneByIdx(scene_idx).fader_is_moving_[ctl_idx];
        // send MIDI only for current scene
        if (scene_->value() == scene_idx && !is_moving) {
            const uint8_t cc = XT_FADER_FIRST + (0x0F & ctl_idx);
            const uint8_t val = std::round(convert::lin2lin_clip<t_float, 0, 1>(v, 0, 127));
            sendCC(cc, val);
        }
    } else {
        OBJ_ERR << "not implemented yet: " << proto_->value() << " " << __FUNCTION__;
    }
}

void XTouchExtender::sendKnob(uint8_t scene_idx, uint8_t ctl_idx, t_float v)
{
    OBJ_LOG << "sendKnob: " << (int)scene_idx << ' ' << (int)ctl_idx << ' ' << v;

    if (proto_->value() == PROTO_XMIDI) {
        const int logic_idx = (scene_idx * Scene::NCHAN + ctl_idx) % MAX_CONTROLS;

        // output for all scenes
        anyTo(1, SYM_KNOBS.at(logic_idx), AtomListView(Atom(v)));

        // send MIDI only for current scene
        if (scene_->value() == scene_idx) {
            const uint8_t cc = XT_KNOB_FIRST + (0x0F & ctl_idx);
            const uint8_t val = std::round(convert::lin2lin_clip<t_float, 0, 1>(v, 0, 127));
            sendCC(cc, val);
        }

    } else {
        OBJ_ERR << "not implemented yet: " << proto_->value() << " " << __FUNCTION__;
    }
}

void XTouchExtender::sendKnobButton(uint8_t scene_idx, uint8_t ctl_idx, uint8_t v)
{
    OBJ_LOG << "sendKnobButton: " << (int)scene_idx << ' ' << (int)ctl_idx << ' ' << v;

    if (proto_->value() == PROTO_XMIDI) {
        const int logic_idx = (scene_idx * Scene::NCHAN + ctl_idx) % MAX_CONTROLS;
        // output for all scenes
        anyTo(1, SYM_BTN_KNOB.at(logic_idx), AtomListView(Atom(v > 0)));
    } else {
        OBJ_ERR << "not implemented yet: " << proto_->value() << " " << __FUNCTION__;
    }
}

void XTouchExtender::sendRec(uint8_t scene_idx, uint8_t ctl_idx, int v)
{
    OBJ_LOG << "sendRec: " << (int)scene_idx << ' ' << (int)ctl_idx << ' ' << v;

    if (proto_->value() == PROTO_XMIDI) {
        const int logic_idx = (scene_idx * Scene::NCHAN + ctl_idx) % MAX_CONTROLS;

        // output for all scenes
        anyTo(1, SYM_BTN_REC.at(logic_idx), AtomListView(Atom(v)));

        // send MIDI only for current scene
        if (scene_->value() == scene_idx) {
            const uint8_t note = XT_REC_FIRST + (0x0F & ctl_idx);
            const uint8_t val = (v < 0) ? 64 : (v > 0 ? 127 : 0);
            sendNote(note, val);
        }
    } else {
        OBJ_ERR << "not implemented yet: " << proto_->value() << " " << __FUNCTION__;
    }
}

void XTouchExtender::sendSolo(uint8_t scene_idx, uint8_t ctl_idx, int v)
{
    OBJ_LOG << "sendSolo: " << (int)scene_idx << ' ' << (int)ctl_idx << ' ' << v;

    if (proto_->value() == PROTO_XMIDI) {
        const int logic_idx = (scene_idx * Scene::NCHAN + ctl_idx) % MAX_CONTROLS;

        // output for all scenes
        anyTo(1, SYM_BTN_SOLO.at(logic_idx), AtomListView(Atom(v)));

        // send MIDI only for current scene
        if (scene_->value() == scene_idx) {
            const uint8_t note = XT_SOLO_FIRST + (0x0F & ctl_idx);
            const uint8_t val = (v < 0) ? 64 : (v > 0 ? 127 : 0);
            sendNote(note, val);
        }
    } else {
        OBJ_ERR << "not implemented yet: " << proto_->value() << " " << __FUNCTION__;
    }
}

void XTouchExtender::sendMute(uint8_t scene_idx, uint8_t ctl_idx, int v)
{
    OBJ_LOG << "sendMute: " << (int)scene_idx << ' ' << (int)ctl_idx << ' ' << v;

    if (proto_->value() == PROTO_XMIDI) {
        const int logic_idx = (scene_idx * Scene::NCHAN + ctl_idx) % MAX_CONTROLS;

        // output for all scenes
        anyTo(1, SYM_BTN_MUTE.at(logic_idx), AtomListView(Atom(v)));

        // send MIDI only for current scene
        if (scene_->value() == scene_idx) {
            const uint8_t note = XT_MUTE_FIRST + (0x0F & ctl_idx);
            const uint8_t val = (v < 0) ? 64 : (v > 0 ? 127 : 0);
            sendNote(note, val);
        }
    } else {
        OBJ_ERR << "not implemented yet: " << proto_->value() << " " << __FUNCTION__;
    }
}

void XTouchExtender::sendSelect(uint8_t scene_idx, uint8_t ctl_idx, int v)
{
    OBJ_LOG << "sendSelect: " << (int)scene_idx << ' ' << (int)ctl_idx << ' ' << v;

    if (proto_->value() == PROTO_XMIDI) {
        const int logic_idx = (scene_idx * Scene::NCHAN + ctl_idx) % MAX_CONTROLS;

        // output for all scenes
        anyTo(1, SYM_BTN_SELECT.at(logic_idx), AtomListView(Atom(v)));

        // send MIDI only for current scene
        if (scene_->value() == scene_idx) {
            const uint8_t note = XT_SELECT_FIRST + (0x0F & ctl_idx);
            const uint8_t val = (v < 0) ? 64 : (v > 0 ? 127 : 0);
            sendNote(note, val);
        }
    } else {
        OBJ_ERR << "not implemented yet: " << proto_->value() << " " << __FUNCTION__;
    }
}

void XTouchExtender::sendCC(uint8_t cc, uint8_t value, uint8_t ch)
{
    Atom m[3];
    cc_set(m, ch, cc, value);
    listTo(0, AtomListView(&m[0].atom(), 3));
}

void XTouchExtender::sendNote(uint8_t note, uint8_t velocity, uint8_t ch)
{
    Atom m[3];
    note_set(m, ch, note, velocity);
    listTo(0, AtomListView(&m[0].atom(), 3));
}

int XTouchExtender::getLogicChannel(const AtomListView& lv) const
{
    const auto idx = lv.intAt(0, -1);

    if (idx < 0 || idx >= numLogicChannels())
        return -1;

    return idx;
}

void XTouchExtender::syncDisplay(uint8_t scene_idx, uint8_t ctl_idx)
{
    if (scene_->value() != scene_idx)
        return;

    auto& dd = sceneByIdx(scene_idx).display(ctl_idx);

    // example from doc: F0 00 20 32 dd 4C nn cc c1 .. c14 F7
    constexpr int N = 23;
    Atom m[N];
    m[0].setFloat(0xf0, true);
    m[1].setFloat(0x00, true);
    m[2].setFloat(0x20, true);
    m[3].setFloat(0x32, true);
    m[4].setFloat(0x15, true);
    m[5].setFloat(0x4C, true);
    m[6].setFloat(ctl_idx, true);
    m[7].setFloat(dd.packedColorMode(), true);

    for (size_t i = 0; i < dd.MAX_CHARS; i++) {
        m[i + 8].setFloat(dd.upperCharAt(i), true);
        m[i + 15].setFloat(dd.lowerCharAt(i), true);
    }
    m[N - 1].setFloat(0xf7, true);

    listTo(0, AtomListView(&m[0].atom(), N));
}

void XTouchExtender::setLogicDisplayUpperText(uint8_t log_idx, const AtomListView& txt)
{
    auto& d = sceneByLogicIdx(log_idx).display(log_idx);
    d.setUpperText(txt);
}

void XTouchExtender::setLogicDisplayLowerText(uint8_t log_idx, const AtomListView& txt)
{
    auto& d = sceneByLogicIdx(log_idx).display(log_idx);
    d.setLowerText(txt);
}

void XTouchExtender::m_lcd_upper(t_symbol* s, const AtomListView& lv)
{
    if (lv.size() < 1) {
        METHOD_ERR(s) << "usage: IDX [TXT]...";
        return;
    }

    const auto ch = getLogicChannel(lv);
    if (ch < 0) {
        METHOD_ERR(s) << "invalid scene channel: " << lv[0];
        return;
    }

    setLogicDisplayUpperText(ch, lv.subView(1));
    syncLogicDisplay(ch);
}

void XTouchExtender::m_lcd_lower(t_symbol* s, const AtomListView& lv)
{
    if (lv.size() < 1) {
        METHOD_ERR(s) << "usage: IDX [TXT]...";
        return;
    }

    const auto ch = getLogicChannel(lv);
    if (ch < 0) {
        METHOD_ERR(s) << "invalid scene channel: " << lv[0];
        return;
    }

    setLogicDisplayLowerText(ch, lv.subView(1));
    syncLogicDisplay(ch);
}

void XTouchExtender::m_lcd(t_symbol* s, const AtomListView& lv)
{
    if (lv.size() != 3) {
        METHOD_ERR(s) << "usage: IDX UPPER_TEXT LOWER_TEXT";
        return;
    }

    const auto ch = getLogicChannel(lv);
    if (ch < 0) {
        METHOD_ERR(s) << "invalid scene channel: " << lv[0];
        return;
    }

    setLogicDisplayUpperText(ch, lv.subView(1, 1));
    setLogicDisplayLowerText(ch, lv.subView(2, 1));

    syncLogicDisplay(ch);
}

void XTouchExtender::m_ulcd(t_symbol* s, const AtomListView& lv)
{
    m_apply_fn(s, lv,
        [this](int idx, const Atom& a) {
            display(idx).setUpperText(a);
            syncLogicDisplay(idx);
        });
}

void XTouchExtender::m_llcd(t_symbol* s, const AtomListView& lv)
{
    m_apply_fn(s, lv,
        [this](int idx, const Atom& a) {
            display(idx).setLowerText(a);
            syncLogicDisplay(idx);
        });
}

void XTouchExtender::m_lcd_align(t_symbol* s, const AtomListView& lv)
{
    m_apply_fn(s, lv,
        [this, s](int idx, const Atom& a) {
            auto& d = sceneByLogicIdx(idx).display(idx);
            if (!d.setUpperAlign(a) || !d.setLowerAlign(a)) {
                METHOD_ERR(s) << "invalid align value: " << a;
                return;
            }

            syncLogicDisplay(idx);
        });
}

void XTouchExtender::m_lcd_upper_align(t_symbol* s, const AtomListView& lv)
{
    m_apply_fn(s, lv,
        [this, s](int idx, const Atom& a) {
            if (!display(idx).setUpperAlign(a)) {
                METHOD_ERR(s) << "invalid align value: " << a;
                return;
            }

            syncLogicDisplay(idx);
        });
}

void XTouchExtender::m_lcd_upper_enum(t_symbol* s, const AtomListView& lv)
{
    t_symbol* prefix = lv.symbolAt(0, &s_);
    char buf[12];

    for (int i = 0; i < numLogicChannels(); i++) {
        snprintf(buf, sizeof(buf) - 1, "%s%d", prefix->s_name, i);
        display(i).setUpperText(buf);
        syncLogicDisplay(i);
    }
}

void XTouchExtender::m_lcd_lower_enum(t_symbol* s, const AtomListView& lv)
{
    t_symbol* prefix = lv.symbolAt(0, &s_);
    char buf[12];

    for (int i = 0; i < numLogicChannels(); i++) {
        snprintf(buf, sizeof(buf) - 1, "%s%d", prefix->s_name, i);
        display(i).setLowerText(buf);
        syncLogicDisplay(i);
    }
}

void XTouchExtender::m_rec(t_symbol* s, const AtomListView& lv)
{
    m_apply_fn(s, lv,
        [this](int idx, const Atom& a) { sceneByLogicIdx(idx).rec(idx).setState(a); });
}

void XTouchExtender::m_solo(t_symbol* s, const AtomListView& lv)
{
    m_apply_fn(s, lv,
        [this](int idx, const Atom& a) { sceneByLogicIdx(idx).solo(idx).setState(a); });
}

void XTouchExtender::m_mute(t_symbol* s, const AtomListView& lv)
{
    m_apply_fn(s, lv,
        [this](int idx, const Atom& a) { sceneByLogicIdx(idx).mute(idx).setState(a); });
}

void XTouchExtender::m_select(t_symbol* s, const AtomListView& lv)
{
    m_apply_fn(s, lv,
        [this](int idx, const Atom& a) { sceneByLogicIdx(idx).select(idx).setState(a); });
}

void XTouchExtender::m_knob(t_symbol* s, const AtomListView& lv)
{
    m_apply_fn(s, lv,
        [this](int idx, const Atom& a) { sceneByLogicIdx(idx).knob(idx).setValue(a); });
}

void XTouchExtender::m_fader(t_symbol* s, const AtomListView& lv)
{
    m_apply_fn(s, lv,
        [this](int idx, const Atom& a) { sceneByLogicIdx(idx).fader(idx).setValue(a); });
}

void XTouchExtender::m_rec_get(t_symbol* s, const AtomListView& lv)
{
    m_get_button_fn(s, lv, &Scene::rec);
}

void XTouchExtender::m_solo_get(t_symbol* s, const AtomListView& lv)
{
    m_get_button_fn(s, lv, &Scene::solo);
}

void XTouchExtender::m_mute_get(t_symbol* s, const AtomListView& lv)
{
    m_get_button_fn(s, lv, &Scene::mute);
}

void XTouchExtender::m_select_get(t_symbol* s, const AtomListView& lv)
{
    m_get_button_fn(s, lv, &Scene::select);
}

void XTouchExtender::m_knob_get(t_symbol* s, const AtomListView& lv)
{
    m_get_fader_fn(s, lv, &Scene::knob);
}

void XTouchExtender::m_fader_get(t_symbol* s, const AtomListView& lv)
{
    m_get_fader_fn(s, lv, &Scene::fader);
}

void XTouchExtender::m_rec_mode(t_symbol* s, const AtomListView& lv)
{
    m_apply_fn(s, lv,
        [this](int idx, const Atom& a) { sceneByLogicIdx(idx).rec(idx).setMode(a); });
}

void XTouchExtender::m_solo_mode(t_symbol* s, const AtomListView& lv)
{
    m_apply_fn(s, lv,
        [this](int idx, const Atom& a) { sceneByLogicIdx(idx).solo(idx).setMode(a); });
}

void XTouchExtender::m_mute_mode(t_symbol* s, const AtomListView& lv)
{
    m_apply_fn(s, lv,
        [this](int idx, const Atom& a) { sceneByLogicIdx(idx).mute(idx).setMode(a); });
}

void XTouchExtender::m_select_mode(t_symbol* s, const AtomListView& lv)
{
    m_apply_fn(s, lv,
        [this](int idx, const Atom& a) { sceneByLogicIdx(idx).select(idx).setMode(a); });
}

void XTouchExtender::m_lcd_lower_align(t_symbol* s, const AtomListView& lv)
{
    m_apply_fn(s, lv,
        [this, s](int idx, const Atom& a) {
            if (!display(idx).setLowerAlign(a)) {
                METHOD_ERR(s) << "invalid align value: " << a;
                return;
            }

            syncLogicDisplay(idx);
        });
}

void XTouchExtender::m_lcd_mode(t_symbol* s, const AtomListView& lv)
{
    m_apply_fn(s, lv,
        [this, s](int idx, const Atom& a) {
            int m = a.asInt(-1);
            if (m < Display::MODE_MIN || m > Display::MODE_MAX) {
                METHOD_ERR(s) << "mode 0|1|2|3 expected, got: " << a;
                return;
            }

            display(idx).setMode(static_cast<Display::Mode>(m));
            syncLogicDisplay(idx);
        });
}

void XTouchExtender::m_lcd_color(t_symbol* s, const AtomListView& lv)
{
    m_apply_fn(s, lv,
        [this, s](int idx, const Atom& a) {
            if (!display(idx).setColor(a)) {
                METHOD_ERR(s) << "invalid color: " << a;
                return;
            }

            syncLogicDisplay(idx);
        });
}

void XTouchExtender::m_apply_fn(t_symbol* s, const AtomListView& lv, std::function<void(int, const Atom&)> fn)
{
    auto usage = [this, s]() -> void { METHOD_ERR(s) << "usage: IDX|all|scene ARGS..."; };

    if (lv.size() < 2)
        return usage();

    t_symbol* INVALID = nullptr;
    const auto sym_ch = lv[0].toT<t_symbol*>(INVALID);

    // set all channels on all scenes
    if (sym_ch == SYM_ALL) {
        for (int i = 0; i < numLogicChannels(); i++)
            fn(i, lv[1]);

        return;
    } else if (sym_ch == SYM_SCENE) { // set all channels on current scene
        for (int i = 0; i < Scene::NCHAN; i++)
            fn(calcLogicIdx(i), lv[1]);
    } else if (sym_ch != INVALID) {
        return usage();
    } else {
        const auto ch = getLogicChannel(lv);
        if (ch < 0) {
            METHOD_ERR(s) << "invalid scene channel: " << lv[0];
            return;
        }

        const size_t nmax = (Scene::NCHAN - (ch % Scene::NCHAN));
        for (size_t i = 1; i < lv.size(); i++) {

            if (i > nmax) {
                METHOD_ERR(s) << "ignoring extra values: " << lv.subView(i);
                break;
            }

            // idx >= 0
            const auto idx = ch + i - 1;
            fn(idx, lv[i]);
        }
    }
}

void XTouchExtender::m_get_button_fn(t_symbol* s, const AtomListView& lv, Button& (Scene::*fn)(uint8_t))
{
    int idx = lv.intAt(0, -1);
    if (idx < 0 || idx >= numLogicChannels()) {
        METHOD_ERR(s) << "expected value in [0.." << numLogicChannels() << ") range, got: " << lv;
        return;
    }

    auto& sc = sceneByLogicIdx(idx);
    Atom lout[2];
    lout[0] = lv[0];
    lout[1] = (sc.*fn)(idx).state();
    anyTo(1, s, AtomListView(lout, 2));
}

void XTouchExtender::m_get_fader_fn(t_symbol* s, const AtomListView& lv, Fader& (Scene::*fn)(uint8_t))
{
    int idx = lv.intAt(0, -1);
    if (idx < 0 || idx >= numLogicChannels()) {
        METHOD_ERR(s) << "expected value in [0.." << numLogicChannels() << ") range, got: " << lv;
        return;
    }

    auto& sc = sceneByLogicIdx(idx);
    Atom lout[2];
    lout[0] = lv[0];
    lout[1] = (sc.*fn)(idx).value();
    anyTo(1, s, AtomListView(lout, 2));
}

static void init_symbols()
{
    PROTO_XMIDI = gensym("xmidi");
    PROTO_HUI = gensym("hui");
    PROTO_MCU = gensym("mcu");

    char buf[64];
    for (int i = 0; i < MAX_CONTROLS; i++) {
        sprintf(buf, "fader%d", i);
        SYM_FADERS[i] = gensym(buf);
        sprintf(buf, "knob%d", i);
        SYM_KNOBS[i] = gensym(buf);
        sprintf(buf, "rec%d", i);
        SYM_BTN_REC[i] = gensym(buf);
        sprintf(buf, "solo%d", i);
        SYM_BTN_SOLO[i] = gensym(buf);
        sprintf(buf, "mute%d", i);
        SYM_BTN_MUTE[i] = gensym(buf);
        sprintf(buf, "select%d", i);
        SYM_BTN_SELECT[i] = gensym(buf);
        sprintf(buf, "knobb%d", i);
        SYM_BTN_KNOB[i] = gensym(buf);
    }

    SYM_ALL = gensym("all");
    SYM_BLACK = gensym("black");
    SYM_BLUE = gensym("blue");
    SYM_CYAN = gensym("cyan");
    SYM_GREEN = gensym("green");
    SYM_MAGENTA = gensym("magenta");
    SYM_RANDOM = gensym("random");
    SYM_RED = gensym("red");
    SYM_SCENE = gensym("scene");
    SYM_WHITE = gensym("white");
    SYM_YELLOW = gensym("yellow");
    SYM_CENTER = gensym("center");
    SYM_LEFT = gensym("left");
    SYM_RIGHT = gensym("right");
    SYM_JUSTIFY = gensym("justify");
    SYM_AUTO = gensym("auto");
    SYM_SELECT = gensym("select");
    SYM_MUTE = gensym("mute");
    SYM_SOLO = gensym("solo");
    SYM_REC = gensym("rec");
    SYM_BTN = gensym("btn");
    SYM_TILDE = gensym("~");
    SYM_EXCLAM = gensym("!");
}

void Display::setUpperText(const char* str)
{
    switch (upper_align_) {
    case ALIGN_LEFT:
        return setAlignedLeft(txt_, str);
    case ALIGN_RIGHT:
        return setAlignedRight(txt_, str);
    case ALIGN_JUSTIFY:
        return setJustified(txt_, str);
    case ALIGN_CENTER:
    default:
        return setCentered(txt_, str);
    }
}

void Display::setUpperText(const AtomListView& atoms)
{
    if (atoms.empty())
        clearUpper();
    else if (atoms.isSymbol())
        setUpperText(atoms[0].asT<t_symbol*>()->s_name);
    else if (upper_align_ == ALIGN_AUTO && atoms.size() == 2) {
        upper_align_ = ALIGN_JUSTIFY;
        setUpperText(to_string(atoms).c_str());
        upper_align_ = ALIGN_AUTO;
    } else
        setUpperText(to_string(atoms).c_str());
}

void Display::setLowerText(const AtomListView& atoms)
{
    if (atoms.empty())
        clearLower();
    else if (atoms.isSymbol())
        setLowerText(atoms[0].asT<t_symbol*>()->s_name);
    else if (upper_align_ == ALIGN_AUTO && atoms.size() == 2) {
        upper_align_ = ALIGN_JUSTIFY;
        setLowerText(to_string(atoms).c_str());
        upper_align_ = ALIGN_AUTO;
    } else
        setLowerText(to_string(atoms).c_str());
}

std::string Display::upperText() const
{
    int zi = -1;
    for (uint8_t i = 0; i < MAX_CHARS; i++) {
        if (txt_[i] == '\0') {
            zi = i;
            break;
        }
    }

    if (zi >= 0)
        return std::string(txt_, zi);
    else
        return std::string(txt_, MAX_CHARS);
}

std::string Display::lowerText() const
{
    int zi = -1;
    for (uint8_t i = 0; i < MAX_CHARS; i++) {
        if (txt_[MAX_CHARS + i] == '\0') {
            zi = i;
            break;
        }
    }

    if (zi >= 0)
        return std::string(txt_ + MAX_CHARS, zi);
    else
        return std::string(txt_ + MAX_CHARS, MAX_CHARS);
}

bool Display::setUpperAlign(const Atom& a)
{
    Align al = ALIGN_INVALID;
    if (a.isSymbol())
        al = namedAlign(a.asT<t_symbol*>());
    else if (a.isFloat()) {
        auto i = a.asInt(ALIGN_INVALID);
        if (i < ALIGN_MIN || i > ALIGN_MAX)
            return false;

        al = static_cast<Align>(i);
    }

    if (al == ALIGN_INVALID)
        return false;

    setUpperAlign(al);
    return true;
}

bool Display::setLowerAlign(const Atom& a)
{
    Align al = ALIGN_INVALID;

    if (a.isSymbol())
        al = namedAlign(a.asT<t_symbol*>());
    else if (a.isFloat()) {
        auto i = a.asInt(ALIGN_INVALID);
        if (i < ALIGN_MIN || i > ALIGN_MAX)
            return false;

        al = static_cast<Align>(i);
    }

    if (al == ALIGN_INVALID)
        return false;

    setLowerAlign(al);
    return true;
}

void Display::setLowerText(const char* str)
{
    switch (upper_align_) {
    case ALIGN_LEFT:
        return setAlignedLeft(txt_ + MAX_CHARS, str);
    case ALIGN_RIGHT:
        return setAlignedRight(txt_ + MAX_CHARS, str);
    case ALIGN_JUSTIFY:
        return setJustified(txt_ + MAX_CHARS, str);
    case ALIGN_CENTER:
    default:
        return setCentered(txt_ + MAX_CHARS, str);
    }
}

void Display::clearUpper()
{
    for (uint8_t i = 0; i < MAX_CHARS; i++)
        txt_[i] = 0;
}

void Display::clearLower()
{
    for (uint8_t i = 0; i < MAX_CHARS; i++)
        txt_[i + MAX_CHARS] = 0;
}

void Display::clearBoth()
{
    for (uint8_t i = 0; i < sizeof(txt_); i++)
        txt_[i] = 0;
}

void Display::setDefault()
{
    color_ = CYAN;
    mode_ = MODE_INVERTED;
    upper_align_ = ALIGN_AUTO;
    lower_align_ = ALIGN_AUTO;
    clearBoth();
}

bool Display::setColor(const Atom& a)
{
    if (a == SYM_RANDOM) {
        setColor(randomColor());
        return true;
    } else if (a.isSymbol()) {
        auto c = namedColor(a.asT<t_symbol*>());
        if (c == UNKNOWN)
            return false;

        setColor(c);
        return true;
    } else {
        int c = a.asInt(-1);
        if (c < COLOR_MIN || c > COLOR_MAX)
            return false;

        setColor(static_cast<Color>(c));
        return true;
    }
}

Display::Color Display::randomColor()
{
    return static_cast<Color>((std::rand() % WHITE) + 1);
}

Display::Color Display::namedColor(const t_symbol* c)
{
    const std::array<const t_symbol*, NUM_COLORS> all = {
        SYM_BLACK,
        SYM_RED,
        SYM_GREEN,
        SYM_YELLOW,
        SYM_BLUE,
        SYM_MAGENTA,
        SYM_CYAN,
        SYM_WHITE,
    };

    for (size_t i = 0; i < all.size(); i++) {
        if (all[i] == c)
            return static_cast<Color>(i);
    }

    return UNKNOWN;
}

Display::Align Display::namedAlign(const t_symbol* a)
{
    const std::array<const t_symbol*, ALIGN_MAX + 1> all = {
        SYM_CENTER,
        SYM_LEFT,
        SYM_RIGHT,
        SYM_JUSTIFY,
        SYM_AUTO
    };

    for (size_t i = 0; i < all.size(); i++) {
        if (all[i] == a)
            return static_cast<Align>(i);
    }

    return ALIGN_INVALID;
}

void Display::setCentered(char* dest, const char* txt)
{
    const char* pch = txt;

    uint8_t i = 0;
    for (; i < MAX_CHARS; i++, pch++) {
        if (*pch != 0)
            dest[i] = *pch;
        else
            break;
    }

    // pad with zeroes
    for (; i < MAX_CHARS; i++)
        dest[i] = 0;
}

void Display::setAlignedLeft(char* dest, const char* txt)
{
    const char* pch = txt;

    uint8_t i = 0;
    for (; i < MAX_CHARS; i++, pch++) {
        if (*pch != 0)
            dest[i] = *pch;
        else
            break;
    }

    // pad with spaces
    for (; i < MAX_CHARS; i++)
        dest[i] = ' ';
}

void Display::setAlignedRight(char* dest, const char* txt)
{
    const char* pch = txt;
    const auto len = strlen(txt);

    uint8_t i = 0;

    if (len < MAX_CHARS) {
        // pad with spaces
        for (; i < (MAX_CHARS - len); i++)
            dest[i] = ' ';
    }

    for (; i < MAX_CHARS; i++, pch++) {
        if (*pch != 0)
            dest[i] = *pch;
        else
            break;
    }
}

void Display::setJustified(char* dest, const char* txt)
{
    const char* pch = txt;
    const auto len = strlen(txt);

    uint8_t i = 0;

    for (; i < MAX_CHARS; i++) {
        switch (*pch) {
        case '\0':
            dest[i] = '\0';
            break;
        case ' ':
            pch++;
            goto on_space;
            break;
        default:
            dest[i] = *pch;
            pch++;
            break;
        }
    }

    return;

on_space:
    // i < len
    const auto nleft = len - (i + 1);
    // nleft >= 0
    if (nleft < MAX_CHARS) {
        // pad with spaces
        for (; i < (MAX_CHARS - nleft); i++)
            dest[i] = ' ';
    }

    for (; i < MAX_CHARS; i++, pch++) {
        if (*pch != 0)
            dest[i] = *pch;
        else
            break;
    }
}

bool Button::setMode(const Atom& a)
{
    if (a.isBool()) {
        mode_ = a.asBool(false) ? TOGGLE : BUTTON;
        return true;
    } else if (a == SYM_TILDE || a == SYM_EXCLAM) {
        mode_ = (mode_ == TOGGLE) ? BUTTON : TOGGLE;
        return true;
    } else
        return false;
}

void Button::setState(State st)
{
    state_ = st;

    if (fn_)
        fn_(st);
}

bool Button::setState(const Atom& a)
{
    if (a.isBool()) {
        setState(a.asBool(false) ? ON : OFF);
        return true;
    } else if (a == SYM_TILDE || a == SYM_EXCLAM) {
        toggle();
        return true;
    } else if (a == -1) {
        setState(ACTIVE);
        return true;
    } else
        return false;
}

void Button::setMidi(int vel)
{
    const bool on = vel > 63;
    if (mode_ == TOGGLE && on)
        toggle();
    else if (mode_ == BUTTON)
        setState(on ? ON : OFF);
}

void Button::toggle()
{
    if (state_ == ACTIVE)
        state_ = OFF;

    setState((state_ == ON) ? OFF : ON);
}

void Fader::setValue(t_float v)
{
    v_ = clip01<t_float>(v);

    if (fn_)
        fn_(v);
}

bool Fader::setValue(const Atom& a)
{
    if (a.isFloat()) {
        setValue(a.asT<t_float>());
        return true;
    }

    return false;
}

void setup_proto_xtouch_ext()
{
    init_symbols();

    ObjectFactory<XTouchExtender> obj("proto.xtouch_ext");
    obj.addMethod("vu", &XTouchExtender::m_vu);
    obj.addMethod("reset", &XTouchExtender::m_reset);

    obj.addMethod("lcd", &XTouchExtender::m_lcd);
    obj.addMethod("lcd0", &XTouchExtender::m_lcd_upper);
    obj.addMethod("lcd1", &XTouchExtender::m_lcd_lower);
    obj.addMethod("ulcd", &XTouchExtender::m_ulcd);
    obj.addMethod("llcd", &XTouchExtender::m_llcd);
    obj.addMethod("lcd_color", &XTouchExtender::m_lcd_color);
    obj.addMethod("lcd_mode", &XTouchExtender::m_lcd_mode);
    obj.addMethod("lcd_align", &XTouchExtender::m_lcd_align);
    obj.addMethod("lcd_align0", &XTouchExtender::m_lcd_upper_align);
    obj.addMethod("lcd_align1", &XTouchExtender::m_lcd_lower_align);
    obj.addMethod("lcd_enum0", &XTouchExtender::m_lcd_upper_enum);
    obj.addMethod("lcd_enum1", &XTouchExtender::m_lcd_lower_enum);

    obj.addMethod("rec", &XTouchExtender::m_rec);
    obj.addMethod("mute", &XTouchExtender::m_mute);
    obj.addMethod("solo", &XTouchExtender::m_solo);
    obj.addMethod("select", &XTouchExtender::m_select);
    obj.addMethod("knob", &XTouchExtender::m_knob);
    obj.addMethod("fader", &XTouchExtender::m_fader);

    obj.addMethod("rec?", &XTouchExtender::m_rec_get);
    obj.addMethod("mute?", &XTouchExtender::m_mute_get);
    obj.addMethod("solo?", &XTouchExtender::m_solo_get);
    obj.addMethod("select?", &XTouchExtender::m_select_get);
    obj.addMethod("knob?", &XTouchExtender::m_knob_get);
    obj.addMethod("fader?", &XTouchExtender::m_fader_get);

    obj.addMethod("rec_mode", &XTouchExtender::m_rec_mode);
    obj.addMethod("mute_mode", &XTouchExtender::m_mute_mode);
    obj.addMethod("solo_mode", &XTouchExtender::m_solo_mode);
    obj.addMethod("select_mode", &XTouchExtender::m_select_mode);
}
