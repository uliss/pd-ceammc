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
#include "ceammc_output.h"

#include <array>
#include <cstdlib>

constexpr int LCD_MAX_CHARS = 7;
constexpr int MAX_VU_DB = 0;
constexpr int MIN_VU_DB = -60;
constexpr int MAX_SCENES = 16;
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

void XTouchExtender::initDone()
{
    const auto N = num_scenes_->value();
    scenes_.resize(N);
    scene_->checkClosedRange(0, N - 1);

    for (int i = 0; i < N; i++) {
        const auto NCH = scenes_[i].NCHAN;

        char buf[64];

        // init faders
        for (int j = 0; j < NCH; j++) {
            const int idx = i * NCH + j;
            sprintf(buf, "@fader%d", idx);
            auto p = new FloatProperty(buf);
            p->checkClosedRange(0, 1);
            p->setSuccessFn([this, i, j](Property* p) { sendFader(i, j, static_cast<FloatProperty*>(p)->value()); });
            addProperty(p);
            scenes_[i].faders_[j] = p;
        }

        // init knobs
        for (int j = 0; j < NCH; j++) {
            const int idx = i * NCH + j;
            sprintf(buf, "@knob%d", idx);
            auto p = new FloatProperty(buf);
            p->checkClosedRange(0, 1);
            p->setSuccessFn([this, i, j](Property* p) { sendKnob(i, j, static_cast<FloatProperty*>(p)->value()); });
            addProperty(p);
            scenes_[i].knobs_[j] = p;
        }

        // init btn rec
        for (int j = 0; j < NCH; j++) {
            const int idx = i * NCH + j;
            sprintf(buf, "@rec%d", idx);
            auto p = new IntProperty(buf);
            p->checkClosedRange(-1, 1);
            p->setSuccessFn([this, i, j](Property* p) { sendRec(i, j, static_cast<IntProperty*>(p)->value()); });
            addProperty(p);
            scenes_[i].btn_rec_[j] = p;
        }

        // init btn rec toggle mode
        for (int j = 0; j < NCH; j++) {
            const int idx = i * NCH + j;
            sprintf(buf, "@trec%d", idx);
            auto p = new BoolProperty(buf, true);
            addProperty(p);
            scenes_[i].btn_rec_tgl_mode_[j] = p;
        }

        // init btn solo
        for (int j = 0; j < NCH; j++) {
            const int idx = i * NCH + j;
            sprintf(buf, "@solo%d", idx);
            auto p = new IntProperty(buf);
            p->checkClosedRange(-1, 1);
            p->setSuccessFn([this, i, j](Property* p) { sendSolo(i, j, static_cast<IntProperty*>(p)->value()); });
            addProperty(p);
            scenes_[i].btn_solo_[j] = p;
        }

        // init btn solo toggle mode
        for (int j = 0; j < NCH; j++) {
            const int idx = i * NCH + j;
            sprintf(buf, "@tsolo%d", idx);
            auto p = new BoolProperty(buf, true);
            addProperty(p);
            scenes_[i].btn_solo_tgl_mode_[j] = p;
        }

        // init btn mute
        for (int j = 0; j < NCH; j++) {
            const int idx = i * NCH + j;
            sprintf(buf, "@mute%d", idx);
            auto p = new IntProperty(buf);
            p->checkClosedRange(-1, 1);
            p->setSuccessFn([this, i, j](Property* p) { sendMute(i, j, static_cast<IntProperty*>(p)->value()); });
            addProperty(p);
            scenes_[i].btn_mute_[j] = p;
        }

        // init btn mute toggle mode
        for (int j = 0; j < NCH; j++) {
            const int idx = i * NCH + j;
            sprintf(buf, "@tmute%d", idx);
            auto p = new BoolProperty(buf, true);
            addProperty(p);
            scenes_[i].btn_mute_tgl_mode_[j] = p;
        }

        // init btn select
        for (int j = 0; j < NCH; j++) {
            const int idx = i * NCH + j;
            sprintf(buf, "@select%d", idx);
            auto p = new IntProperty(buf);
            p->checkClosedRange(-1, 1);
            p->setSuccessFn([this, i, j](Property* p) { sendSelect(i, j, static_cast<IntProperty*>(p)->value()); });
            addProperty(p);
            scenes_[i].btn_select_[j] = p;
        }

        // init btn select toggle mode
        for (int j = 0; j < NCH; j++) {
            const int idx = i * NCH + j;
            sprintf(buf, "@tselect%d", idx);
            auto p = new BoolProperty(buf, false);
            addProperty(p);
            scenes_[i].btn_select_tgl_mode_[j] = p;
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
            currentScene().faders_.at(ch)->setValue(val);
            sendFader(scene_->value(), ch, val);
        } else if (in_range(cc, XT_KNOB_FIRST, XT_KNOB_LAST)) {
            const t_float val = convert::lin2lin_clip<t_float, 0, 127>(parser_.data[2], 0, 1);
            const auto ch = cc - XT_KNOB_FIRST;
            currentScene().knobs_.at(ch)->setValue(val);
            sendKnob(scene_->value(), ch, val);
        } else {
            OBJ_ERR << "unknown CC: " << (int)cc;
        }
    } else if (parser_.isNoteOn()) {
        const auto note = parser_.data[1];
        if (in_range(note, XT_REC_FIRST, XT_REC_LAST)) {
            const auto ch = note - XT_REC_FIRST;
            const int velocity = parser_.data[2];

            int val = 0;
            if (currentScene().btn_rec_tgl_mode_.at(ch)->value()) {
                if (velocity == 0)
                    return;

                const auto current_v = currentScene().btn_rec_.at(ch)->value();
                if (current_v < 0)
                    val = 1;
                else
                    val = (1 - current_v);
            } else {
                val = (velocity > 64);
            }

            currentScene().btn_rec_.at(ch)->setValue(val);
            sendRec(scene_->value(), ch, val);
        } else if (in_range(note, XT_BTN_KNOB_FIRST, XT_BTN_KNOB_LAST)) {
            const auto ch = note - XT_BTN_KNOB_FIRST;
            const int velocity = parser_.data[2];
            sendKnobButton(scene_->value(), ch, velocity);
        } else if (in_range(note, XT_SOLO_FIRST, XT_SOLO_LAST)) {
            const auto ch = note - XT_SOLO_FIRST;
            const int velocity = parser_.data[2];

            int val = 0;
            if (currentScene().btn_solo_tgl_mode_.at(ch)->value()) {
                if (velocity == 0)
                    return;

                const auto current_v = currentScene().btn_solo_.at(ch)->value();
                if (current_v < 0)
                    val = 1;
                else
                    val = (1 - current_v);
            } else {
                val = (velocity > 64);
            }

            currentScene().btn_solo_.at(ch)->setValue(val);
            sendSolo(scene_->value(), ch, val);
        } else if (in_range(note, XT_MUTE_FIRST, XT_MUTE_LAST)) {
            const auto ch = note - XT_MUTE_FIRST;
            const int velocity = parser_.data[2];

            int val = 0;
            if (currentScene().btn_mute_tgl_mode_.at(ch)->value()) {
                if (velocity == 0)
                    return;

                const auto current_v = currentScene().btn_mute_.at(ch)->value();
                if (current_v < 0)
                    val = 1;
                else
                    val = (1 - current_v);
            } else {
                val = (velocity > 64);
            }

            currentScene().btn_mute_.at(ch)->setValue(val);
            sendMute(scene_->value(), ch, val);
        } else if (in_range(note, XT_SELECT_FIRST, XT_SELECT_LAST)) {
            const auto ch = note - XT_SELECT_FIRST;
            const int velocity = parser_.data[2];

            int val = 0;
            if (currentScene().btn_select_tgl_mode_.at(ch)->value()) {
                if (velocity == 0)
                    return;

                const auto current_v = currentScene().btn_select_.at(ch)->value();
                if (current_v < 0)
                    val = 1;
                else
                    val = (1 - current_v);
            } else {
                val = (velocity > 64);
            }

            currentScene().btn_select_.at(ch)->setValue(val);
            sendSelect(scene_->value(), ch, val);
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
    for (int i = 0; i < Scene::NCHAN; i++)
        sendVu(i, MIN_VU_DB);
}

void XTouchExtender::resetFaders()
{
    for (size_t scene_idx = 0; scene_idx < scenes_.size(); scene_idx++) {
        const auto& f = scenes_[scene_idx].faders_;
        for (size_t fader_idx = 0; fader_idx < f.size(); fader_idx++) {
            f[fader_idx]->setValue(0);
            sendFader(scene_idx, fader_idx, 0);
        }
    }
}

void XTouchExtender::resetKnobs()
{
    for (size_t scene_idx = 0; scene_idx < scenes_.size(); scene_idx++) {
        const auto& k = scenes_[scene_idx].knobs_;
        for (size_t knob_idx = 0; knob_idx < k.size(); knob_idx++) {
            k[knob_idx]->setValue(0);
            sendKnob(scene_idx, knob_idx, 0);
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
        sendFader(scene_idx, i, faders[i]->value());
        sendKnob(scene_idx, i, knobs[i]->value());
        sendRec(scene_idx, i, recs[i]->value());
        sendSolo(scene_idx, i, solo[i]->value());
        sendMute(scene_idx, i, mute[i]->value());
        sendSelect(scene_idx, i, select[i]->value());
        syncDisplay(scene_idx, i);
    }
}

void XTouchExtender::sendFader(uint8_t scene_idx, uint8_t ctl_idx, t_float v)
{
    OBJ_LOG << "sendFader: " << (int)scene_idx << ' ' << (int)ctl_idx << ' ' << v;

    if (proto_->value() == PROTO_XMIDI) {
        const int logic_idx = (scene_idx * Scene::NCHAN + ctl_idx) % MAX_CONTROLS;

        // output for all scenes
        anyTo(1, SYM_FADERS[logic_idx], AtomListView(Atom(v)));

        // send MIDI only for current scene
        if (scene_->value() == scene_idx) {
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

void XTouchExtender::setLogicLcdMode(uint8_t log_idx, int mode)
{
    using Mode = Display::Mode;

    if (log_idx >= numLogicChannels()) {
        OBJ_ERR << "channel is out of range: " << log_idx;
        return;
    }

    if (mode < Display::MODE_MIN || mode > Display::MODE_MAX) {
        OBJ_ERR << "mode in "
                << (int)Display::MODE_MIN << ".." << (int)Display::MODE_MAX
                << "range is expected, got: " << mode;
        return;
    }

    OBJ_LOG << "set lcd mode: " << (int)log_idx << ' ' << mode;

    sceneByLogicIdx(log_idx).display(log_idx).setMode(static_cast<Display::Mode>(mode));
    syncLogicDisplay(log_idx);
}

void XTouchExtender::setLogicLcdColor(uint8_t log_idx, const Atom& color)
{
    using Color = Display::Color;

    if (log_idx >= numLogicChannels()) {
        OBJ_ERR << "channel is out of range: " << log_idx;
        return;
    }

    Color new_color;

    if (color.isFloat()) { // set by color index
        const auto color_idx = color.asInt(-1);
        if (color_idx < Display::BLACK || color_idx > Display::WHITE) {
            OBJ_ERR << "expected color index in "
                    << Display::BLACK << ".." << Display::WHITE << " range, got: " << color_idx;
            return;
        }

        new_color = static_cast<Color>(color_idx);
    } else if (color.isSymbol()) { // set by color name
        const auto scolor = color.asT<t_symbol*>();

        if (scolor == SYM_RANDOM) { // set random color
            new_color = Display::randomColor();
        } else { // color names
            new_color = Display::namedColor(scolor);
            if (new_color == Color::UNKNOWN) { // not found
                OBJ_ERR << "unknown color: " << scolor;
                return;
            }
        }
    } else {
        OBJ_ERR << "unknown type: " << color;
        return;
    }

    OBJ_LOG << "set lcd color: " << (int)log_idx << ' ' << color;

    sceneByLogicIdx(log_idx).display(log_idx).setColor(new_color);
    syncLogicDisplay(log_idx);
}

void XTouchExtender::m_lcd_upper(t_symbol* s, const AtomListView& lv)
{
    if (lv.size() < 1) {
        METHOD_ERR(s) << "usage: IDX [TXT]...";
        return;
    }

    const auto ch = getLogicChannel(lv);
    if (ch < 0) {
        METHOD_ERR(s) << "invalid channel: " << lv[0];
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
        METHOD_ERR(s) << "invalid channel: " << lv[0];
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
        METHOD_ERR(s) << "invalid channel: " << lv[0];
        return;
    }

    setLogicDisplayUpperText(ch, lv.subView(1, 1));
    setLogicDisplayLowerText(ch, lv.subView(2, 1));

    syncLogicDisplay(ch);
}

void XTouchExtender::m_lcd_align(t_symbol* s, const AtomListView& lv)
{
    m_apply_fn(s, lv,
        [this](int idx, const Atom& a) {
            auto& d = sceneByLogicIdx(idx).display(idx);
            d.setUpperAlign(a);
            d.setLowerAlign(a);
        });
}

void XTouchExtender::m_lcd_upper_align(t_symbol* s, const AtomListView& lv)
{
    m_apply_fn(s, lv,
        [this](int idx, const Atom& a) { display(idx).setUpperAlign(a); });
}

void XTouchExtender::m_lcd_lower_align(t_symbol* s, const AtomListView& lv)
{
    m_apply_fn(s, lv,
        [this](int idx, const Atom& a) { display(idx).setLowerAlign(a); });
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
            METHOD_ERR(s) << "invalid channel: " << lv[0];
            return;
        }

        const int nmax = (Scene::NCHAN - ch % Scene::NCHAN);
        for (size_t i = 1; i < lv.size(); i++) {
            const int idx = ch + i - 1;
            // idx >= 0
            if (i >= nmax) {
                METHOD_ERR(s) << "ignoring extra values: " << lv.subView(i);
                break;
            }

            fn(calcLogicIdx(ch + i - 1), lv[i]);
        }
    }
}

void XTouchExtender::m_lcd_mode(t_symbol* s, const AtomListView& lv)
{
    if (lv.size() < 2) {
        METHOD_ERR(s) << "usage: IDX MODES...(0, 1, 2 or 3)";
        return;
    }

    const auto log_idx = lv.intAt(0, -1);

    if (lv[0].isSymbol() && lv[0].asT<t_symbol*>() == SYM_ALL) {
        for (int i = 0; i < Scene::NCHAN; i++)
            setLogicLcdMode(calcLogicIdx(i), lv[1].asInt());
    } else if (log_idx < 0 || log_idx >= numLogicChannels()) {
        METHOD_ERR(s) << "invalid index:" << log_idx;
        return;
    } else {
        int i = 0;
        for (auto& c : lv.subView(1))
            setLogicLcdMode(log_idx + (i++), c.asInt());
    }
}

void XTouchExtender::m_lcd_color(t_symbol* s, const AtomListView& lv)
{
    if (lv.size() < 2) {
        METHOD_ERR(s) << "usage: IDX COLOR_LIST(index, color name or random)...";
        return;
    }

    const auto log_idx = lv.intAt(0, -1);

    if (lv[0].isSymbol()) {
        const auto sym = lv[0].asT<t_symbol*>();

        if (sym == SYM_SCENE) {
            for (int i = 0; i < Scene::NCHAN; i++)
                setLogicLcdColor(calcLogicIdx(i), lv[1]);
        } else if (sym == SYM_ALL) {
            for (size_t i = 0; i < scenes_.size(); i++) {
                for (int j = 0; j < Scene::NCHAN; j++)
                    setLogicLcdColor(i * Scene::NCHAN + j, lv[1]);
            }
        }
    } else if (log_idx < 0 || log_idx >= numLogicChannels()) {
        METHOD_ERR(s) << "invalid index:" << log_idx;
        return;
    } else {
        int i = 0;
        for (auto& c : lv.subView(1))
            setLogicLcdColor(log_idx + (i++), c);
    }
}

void XTouchExtender::m_set(t_symbol* s, const AtomListView& lv)
{
    auto usage = [this, s](const char* msg) -> void {
        METHOD_ERR(s) << msg << ", usage: " << s->s_name
                      << " all|scene|scene_idx ctl_name(rec|solo|mute|select|btn) value";
    };

    if (lv.size() != 3)
        return usage("empty arguments");

    const auto ctrl = lv.symbolAt(1, nullptr);
    if (ctrl != gensym("rec")
        && ctrl != gensym("solo")
        && ctrl != gensym("mute")
        && ctrl != gensym("select")
        && ctrl != gensym("btn")) {
        return usage("invalid control name");
    }

    if (!lv[2].isFloat())
        return usage("float value expected");

    const auto val = lv.floatAt(2, 0);
    //    if (val <)
    const auto NCH = currentScene().NCHAN;

    if (lv[0].isSymbol()) {
        const auto target = lv[0].asT<t_symbol*>();
        if (target == gensym("scene")) {
            const auto btn = gensym("btn");

            if (ctrl == gensym("rec") || ctrl == btn) {
                for (int i = 0; i < NCH; i++)
                    sendRec(scene_->value(), i, val);
            }

            if (ctrl == gensym("solo") || ctrl == btn) {
                for (int i = 0; i < NCH; i++)
                    sendSolo(scene_->value(), i, val);
            }

            if (ctrl == gensym("mute") || ctrl == btn) {
                for (int i = 0; i < NCH; i++)
                    sendMute(scene_->value(), i, val);
            }

            if (ctrl == gensym("select") || ctrl == btn) {
                for (int i = 0; i < NCH; i++)
                    sendSelect(scene_->value(), i, val);
            }
        }
    } else if (lv[0].isFloat()) {
        const int scene_idx = lv[0].asInt();
        if (scene_idx < 0 || scene_idx >= num_scenes_->value()) {
            METHOD_ERR(s) << "invalid scene index: " << scene_idx;
            return usage("");
        }

        const auto btn = gensym("btn");

        if (ctrl == gensym("rec") || ctrl == btn) {
            for (auto p : scenes_[scene_idx].btn_rec_)
                p->setInt(val);
        }

        if (ctrl == gensym("solo") || ctrl == btn) {
            for (auto p : scenes_[scene_idx].btn_solo_)
                p->setInt(val);
        }

        if (ctrl == gensym("mute") || ctrl == btn) {
            for (auto p : scenes_[scene_idx].btn_mute_)
                p->setInt(val);
        }

        if (ctrl == gensym("select") || ctrl == btn) {
            for (auto p : scenes_[scene_idx].btn_select_)
                p->setInt(val);
        }
    }
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

void setup_proto_xtouch_ext()
{
    init_symbols();

    ObjectFactory<XTouchExtender> obj("proto.xtouch_ext");
    obj.addMethod("vu", &XTouchExtender::m_vu);
    obj.addMethod("reset", &XTouchExtender::m_reset);

    obj.addMethod("lcd", &XTouchExtender::m_lcd);
    obj.addMethod("lcd0", &XTouchExtender::m_lcd_upper);
    obj.addMethod("lcd1", &XTouchExtender::m_lcd_lower);
    obj.addMethod("lcd_color", &XTouchExtender::m_lcd_color);
    obj.addMethod("lcd_mode", &XTouchExtender::m_lcd_mode);
    obj.addMethod("lcd_align", &XTouchExtender::m_lcd_align);
    obj.addMethod("lcd_align0", &XTouchExtender::m_lcd_upper_align);
    obj.addMethod("lcd_align1", &XTouchExtender::m_lcd_lower_align);

    obj.addMethod("set", &XTouchExtender::m_set);
}
