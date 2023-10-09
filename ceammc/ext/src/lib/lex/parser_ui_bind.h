/*****************************************************************************
 * Copyright 2023 Serge Poltavski. All rights reserved.
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
#ifndef PARSER_UI_BIND_H
#define PARSER_UI_BIND_H

#include <cstdint>

namespace ceammc {

enum UIBindType : std::uint8_t {
    UI_BIND_NONE,
    UI_BIND_MIDI_CC,
    UI_BIND_MIDI_PGM,
    UI_BIND_MIDI_NOTE,
    UI_BIND_KEY_CODE,
    UI_BIND_KEY_NAME,
    UI_BIND_OSC
};

enum UIBindCompare : std::uint8_t {
    UI_BIND_CMP_NONE,
    UI_BIND_CMP_EQ,
    UI_BIND_CMP_LT,
    UI_BIND_CMP_GT
};

enum UIBindKeyMode : std::uint8_t {
    UI_BIND_MODE_NONE = 0,
    UI_BIND_MODE_CTL = 1,
    UI_BIND_MODE_ALT = 1 << 1,
    UI_BIND_MODE_SHIFT = 1 << 2,
};

struct UIBindOptions {
    enum { ALL_MIDI_CHANNELS = 0 };

    UIBindType type { UI_BIND_NONE };
    UIBindCompare cmp { UI_BIND_CMP_NONE };
    std::uint8_t midi_chan { ALL_MIDI_CHANNELS };
    std::uint8_t midi_param { 0 };
    std::uint8_t midi_value { 0 };
    std::uint8_t key_code { 0 };
    std::uint8_t key_mode { 0 };
    char key_name[16] { 0 };
    std::uint8_t name_len { 0 };

    void reset() { type = UI_BIND_NONE; }

    bool checkMidiValue(int val) const
    {
        switch (cmp) {
        case UI_BIND_CMP_EQ:
            return val == midi_value;
        case UI_BIND_CMP_LT:
            return val < midi_value;
        case UI_BIND_CMP_GT:
            return val > midi_value;
        case UI_BIND_CMP_NONE:
            return true;
        }
    }

    bool checkMidiChan(int chan) const
    {
        if (midi_chan == ALL_MIDI_CHANNELS)
            return true;
        else
            return midi_chan == chan;
    }

    bool checkMidiParam(int param) const
    {
        return midi_param == param;
    }

    bool checkMidi(int chan, int param, int value) const
    {
        if (type == UI_BIND_MIDI_CC || type == UI_BIND_MIDI_NOTE) {
            return checkMidiChan(chan) && checkMidiParam(param) && checkMidiValue(value);
        } else if (type == UI_BIND_MIDI_PGM)
            return checkMidiChan(chan) && checkMidiParam(param);
        else
            return false;
    }
};

namespace parser {
    bool parse_ui_bind(const char* str, UIBindOptions& opts);
}
}

#endif // PARSER_UI_BIND_H
