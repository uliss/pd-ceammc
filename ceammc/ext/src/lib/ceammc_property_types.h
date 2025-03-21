/*****************************************************************************
 * Copyright 2025 Serge Poltavski. All rights reserved.
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
#ifndef CEAMMC_PROPERTY_TYPES_H
#define CEAMMC_PROPERTY_TYPES_H

#include <cstdint>

#include "m_pd.h"

namespace ceammc {

// on order change/adding new type
// change also in to_string(PropValueType)
enum class PropValueType : uint8_t {
    BOOLEAN = 0,
    INTEGER,
    FLOAT,
    SYMBOL,
    ATOM,
    LIST
};

// on order change/adding new type
// change also in to_string(PropValueType)
enum class PropValueAccess : uint8_t {
    READONLY = 0,
    INITONLY,
    READWRITE
};

// on order change/adding new type
// change also in to_string(PropValueType)
enum class PropValueView : uint8_t {
    SLIDER = 0,
    KNOB,
    NUMBOX,
    SPINBOX,
    TOGGLE,
    MENU,
    ENTRY,
    COLOR,
    FILEPATH,
};

// on order change/adding new type
// change also in to_string(PropValueType)
using PropValueUnitsBase = std::uint32_t;
enum class PropValueUnits : PropValueUnitsBase {
    NONE = 0,
    MSEC = 1, // milliseconds
    SEC = 1 << 1, // seconds
    SAMP = 1 << 2, // samples
    DB = 1 << 3, // decibels
    DEG = 1 << 4, // degree
    RAD = 1 << 5, // radians
    HZ = 1 << 6, // herz
    PERCENT = 1 << 7, // percents
    CENT = 1 << 8, // cents
    SEMITONE = 1 << 9, // semitone
    TONE = 1 << 10, // tone
    BPM = 1 << 11, // bpm
    SMPTE = 1 << 12, // smpte
    PIXEL = 1 << 13, // pixels
    MICROSEC = 1 << 14, // microseconds
    NANOSEC = 1 << 15, // nanoseconds
    METER = 1 << 16, // meter
    CENTIMETER = 1 << 17, // centimeter
    MILLIMETER = 1 << 18, // millimeter
    MINUTE = 1 << 19, // minute
    HOUR = 1 << 20, // hour
    DAY = 1 << 21, // day
};

// on order change/adding new type
// change also in to_string(PropValueType)
enum class PropValueVis : uint8_t {
    PUBLIC = 0, // settable and shown in UI
    HIDDEN, // settable and hidden from UI
    INTERNAL // settable and hidden everywhere
};

// on order change/adding new type
// change also in to_string(PropValueType)
enum class PropValueConstraints : uint8_t {
    NONE = 0,
    GREATER_THEN,
    GREATER_EQUAL,
    LESS_THEN,
    LESS_EQUAL,
    CLOSED_RANGE,
    OPEN_RANGE,
    OPEN_CLOSED_RANGE,
    CLOSED_OPEN_RANGE,
    NON_ZERO,
    ENUM,
    MIN_ELEMENT_COUNT,
    MAX_ELEMENT_COUNT,
    RANGE_ELEMENT_COUNT,
    OTHER
};

t_symbol* to_symbol(PropValueType t);
t_symbol* to_symbol(PropValueView v);
t_symbol* to_symbol(PropValueUnits u);
t_symbol* to_symbol(PropValueAccess v);
t_symbol* to_symbol(PropValueVis v);
t_symbol* to_symbol(PropValueConstraints v);

const char* to_string(PropValueType v);
const char* to_string(PropValueView v);
const char* to_string(PropValueUnits v);
const char* to_string(PropValueAccess v);
const char* to_string(PropValueVis v);
const char* to_string(PropValueConstraints v);

} // namespace ceammc

#endif // CEAMMC_PROPERTY_TYPES_H
