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

#include "ceammc_atom.h"
#include "ceammc_crc32.h"

#include <cstdint>
#include <cstring>

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
    UI_BIND_CMP_LE,
    UI_BIND_CMP_GT,
    UI_BIND_CMP_GE
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
    std::uint32_t key_name_hash { 0 };

    void reset() { type = UI_BIND_NONE; }

    bool checkMidiValue(int val) const
    {
        switch (cmp) {
        case UI_BIND_CMP_EQ:
            return val == midi_value;
        case UI_BIND_CMP_LT:
            return val < midi_value;
        case UI_BIND_CMP_LE:
            return val <= midi_value;
        case UI_BIND_CMP_GT:
            return val > midi_value;
        case UI_BIND_CMP_GE:
            return val >= midi_value;
        case UI_BIND_CMP_NONE:
            return true;
        default:
            return false;
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

    bool checkKeyCode(int key, int mode) const
    {
        if (type != UI_BIND_KEY_CODE)
            return false;

        if (key_mode != UI_BIND_MODE_NONE && !(key_mode & mode))
            return false;

        if (key_mode | UI_BIND_MODE_SHIFT) {
            auto key_low = std::tolower(key);
            return key_code == key || key_code == key_low;
        } else {
            return key_code == key;
        }
    }

    bool checkKeyName(const Atom& name, int mode) const
    {
        if (type != UI_BIND_KEY_NAME)
            return false;

        if (key_mode != UI_BIND_MODE_NONE && !(key_mode & mode))
            return false;

        std::uint32_t name_hash = 0;
        if (name.isSymbol()) {
            name_hash = crc32_case_hash(name.asT<t_symbol*>()->s_name);
        } else if (name.isInteger()) {
            auto x = name.asInt();
            if (x >= 0 && x <= 9)
                name_hash = crc32_append_char(crc32_hash_seed(), x + '0');
            else
                return false;
        } else
            return false;

        return key_name_hash == name_hash;
    }
};

namespace parser {
    bool parse_ui_bind(const char* str, UIBindOptions& opts);
}
}

#endif // PARSER_UI_BIND_H
