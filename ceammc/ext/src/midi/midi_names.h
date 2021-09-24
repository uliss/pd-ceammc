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
#ifndef MIDI_NAMES_H
#define MIDI_NAMES_H

namespace ceammc {

constexpr const char* M_ALL_NOTES_OFF = "notes_off";
constexpr const char* M_ALL_SOUND_OFF = "sounds_off";

constexpr const char* M_CC_VOLUME_COARSE = "volume~";
constexpr const char* M_CC_VOLUME_FINE = "volume.";
constexpr const char* M_CC_VOLUME_FLOAT = "volume:f";
constexpr const char* M_CC_VOLUME_INT = "volume:i";

constexpr const char* M_BANK_SELECT_MSB = "bank:msb";
constexpr const char* M_BANK_SELECT_LSB = "bank:lsb";
constexpr const char* M_BANK_SELECT_INT = "bank:i";
constexpr const char* M_BANK_SELECT = "bank";

constexpr const char* M_PORTAMENTO_SWITCH = "portamento";

constexpr const char* M_MODWHEEL_COARSE = "mod~";
constexpr const char* M_MODWHEEL_FINE = "mod.";
constexpr const char* M_MODWHEEL_INT = "mod:i";
constexpr const char* M_MODWHEEL_FLOAT = "mod:f";

constexpr const char* M_EXPRESSION_COARSE = "exp~";
constexpr const char* M_EXPRESSION_FINE = "exp.";
constexpr const char* M_EXPRESSION_INT = "exp:i";
constexpr const char* M_EXPRESSION_FLOAT = "exp:f";

constexpr const char* M_HOLD_PEDAL = "hold";
constexpr const char* M_SOSTENUTO_PEDAL = "sostenuto";
constexpr const char* M_SOFT_PEDAL = "soft";

constexpr const char* M_PAN_POSITION_INT = "pan:i";
constexpr const char* M_PAN_POSITION_FLOAT = "pan:f";
constexpr const char* M_PAN_POSITION_COARSE = "pan~";
constexpr const char* M_PAN_POSITION_FINE = "pan.";

constexpr const char* M_BEND_SENSIVITY = "bendsens";

constexpr const char* M_TUNE_COARSE = "tune~";
constexpr const char* M_TUNE_FINE = "tune.";
constexpr const char* M_TUNE_SEMITONES = "tune:s";
constexpr const char* M_TUNE_CENTS = "tune:c";

}

#endif // MIDI_NAMES_H
