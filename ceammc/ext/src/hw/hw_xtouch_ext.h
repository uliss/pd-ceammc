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
#ifndef HW_XTOUCH_EXT_H
#define HW_XTOUCH_EXT_H

#include "ceammc_object.h"
#include "ceammc_property_enum.h"

#include <array>
#include <cstdint>
#include <string>

using namespace ceammc;

struct DisplayData {
    std::string text_upper;
    std::string text_lower;

    enum DisplayMode {
        INVERTED = 0,
        NORMAL = 1
    };

    enum DisplayColor {
        BLACK = 0,
        RED = 1,
        GREEN,
        YELLOW,
        BLUE,
        MAGENTA,
        CYAN,
        WHITE
    };

    DisplayColor color = { BLUE };
    DisplayMode mode = { INVERTED };
};

enum MidiFSMState {
    MIDI_FSM_STATE_INIT = 0,
    MIDI_FSM_STATE_NOTE_OFF = 0x80,
    MIDI_FSM_STATE_NOTE_ON = 0x90,
    MIDI_FSM_STATE_CONTROLCHANGE = 0xb0,
};

struct MidiFSMParser {
    MidiFSMState state = { MIDI_FSM_STATE_INIT };
    const static uint8_t max_data_len = { 3 };
    uint8_t data[max_data_len];
    uint8_t size = { 0 };

    void reset()
    {
        state = MIDI_FSM_STATE_INIT;
        size = 0;
    }

    void appendData(uint8_t v)
    {
        if (size >= max_data_len)
            return reset();

        data[size++] = v;
    }

    bool isReady() const { return size == max_data_len; }
    bool isCC() const { return isReady() && ((data[0] & 0xf0) == MIDI_FSM_STATE_CONTROLCHANGE); }
    bool isNoteOn() const { return isReady() && ((data[0] & 0xf0) == MIDI_FSM_STATE_NOTE_ON); }
    uint8_t channel() const { return data[0] & 0x0f; }
};

struct Scene {
    static constexpr int NCHAN = 8;
    std::array<FloatProperty*, NCHAN> faders_;
    std::array<FloatProperty*, NCHAN> knobs_;
    std::array<IntProperty*, NCHAN> btn_rec_;
    std::array<BoolProperty*, NCHAN> btn_rec_tgl_mode_;
    std::array<IntProperty*, NCHAN> btn_solo_;
    std::array<BoolProperty*, NCHAN> btn_solo_tgl_mode_;
    std::array<IntProperty*, NCHAN> btn_mute_;
    std::array<BoolProperty*, NCHAN> btn_mute_tgl_mode_;
    std::array<IntProperty*, NCHAN> btn_select_;
    std::array<BoolProperty*, NCHAN> btn_select_tgl_mode_;

    DisplayData& displayData(uint8_t n) { return display_data_[n % NCHAN]; }

private:
    DisplayData display_data_[NCHAN];
};

class XTouchExtender : public BaseObject {
    std::vector<Scene> scenes_;
    IntProperty* num_scenes_;
    IntProperty* scene_;
    SymbolEnumProperty* proto_;
    MidiFSMParser parser_;

public:
    XTouchExtender(const PdArgs& args);
    void onFloat(t_float f) override;
    void initDone() override;

    void m_vu(t_symbol* s, const AtomListView& lv);
    void m_reset(t_symbol* s, const AtomListView& lv);
    void m_lcd_upper(t_symbol* s, const AtomListView& lv);
    void m_lcd_lower(t_symbol* s, const AtomListView& lv);
    void m_lcd(t_symbol* s, const AtomListView& lv);
    void m_lcd_mode(t_symbol* s, const AtomListView& lv);
    void m_lcd_color(t_symbol* s, const AtomListView& lv);

private:
    void parseXMidi();
    void parseHui();
    void parseMcu();

    void sendVu(uint8_t idx, int db);
    void sendFader(uint8_t scene_idx, uint8_t ctl_idx, t_float v);
    void sendKnob(uint8_t scene_idx, uint8_t ctl_idx, t_float v);
    void sendKnobButton(uint8_t scene_idx, uint8_t ctl_idx, uint8_t v);
    void sendRec(uint8_t scene_idx, uint8_t ctl_idx, int v);
    void sendSolo(uint8_t scene_idx, uint8_t ctl_idx, int v);
    void sendMute(uint8_t scene_idx, uint8_t ctl_idx, int v);

    void sendCC(uint8_t cc, uint8_t value, uint8_t ch = 0);
    void sendNote(uint8_t note, uint8_t velocity, uint8_t ch = 0);

    void resetVu();
    void resetFaders();
    void resetKnobs();

    void syncScene();
    void syncDisplay(uint8_t scene_idx, uint8_t ctl_idx);
    void syncLogicDisplay(uint8_t log_idx) { syncDisplay(log_idx / Scene::NCHAN, log_idx % Scene::NCHAN); }

    int numLogicChannels() const { return scenes_.size() * Scene::NCHAN; }
    int numHWChannels() const { return Scene::NCHAN; }

    /**
     * @brief calcLogicIdx - calculates logic index for current scene
     * @param idx - hardware control index
     * @return
     */
    int calcLogicIdx(uint8_t idx) const { return scene_->value() * Scene::NCHAN + (idx % scene_->value()); }

    Scene& currentScene() { return scenes_[scene_->value()]; }
    Scene& sceneByIdx(uint8_t scene_idx) { return scenes_[scene_idx % scenes_.size()]; }
    Scene& sceneByLogicIdx(uint8_t log_ctl_idx) { return sceneByIdx(log_ctl_idx / Scene::NCHAN); }

    void setLogicDisplayUpperText(uint8_t log_idx, const std::string& txt);
    void setLogicDisplayLowerText(uint8_t log_idx, const std::string& txt);

    void setLogicLcdColor(uint8_t log_idx, const Atom& color);
};

void setup_hw_xtouch_ext();

#endif // HW_XTOUCH_EXT_H
