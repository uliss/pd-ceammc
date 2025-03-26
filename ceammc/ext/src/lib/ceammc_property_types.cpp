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
#include "ceammc_property_types.h"

#include <limits>

namespace ceammc {

const char* to_string(PropValueType t)
{
    static const char* STR[] = {
        "bool",
        "int",
        "float",
        "symbol",
        "atom",
        "list"
    };

    auto idx = static_cast<size_t>(t);
    return (idx < sizeof(STR) / (sizeof(const char*))) ? STR[idx] : "?";
}

const char* to_string(PropValueView v)
{
    static const char* STR[] = {
        "slider",
        "knob",
        "numbox",
        "spinbox",
        "toggle",
        "menu",
        "entry",
        "color",
        "file",
        "dir",
        "ipv4",
        "ipv6",
        "date",
        "time",
        "datetime",
        "calendar",
        "text",
    };

    auto idx = static_cast<size_t>(v);
    return (idx < sizeof(STR) / (sizeof(const char*))) ? STR[idx] : "?";
}

const char* to_string(PropValueUnits u)
{
    static const char* STR[] = {
        "unknown",
        "millisecond",
        "second",
        "sample",
        "decibel",
        "degree",
        "radian",
        "hertz",
        "percent",
        "cent",
        "semitone",
        "tone",
        "bpm",
        "smpte",
        "pixel",
        "microsecond",
        "nanosecond",
        "meter",
        "centimeter",
        "millimeter",
        "minute",
        "hour",
        "day",
    };

    size_t idx = 0;
    auto bits = static_cast<PropValueUnitsBase>(u);

#ifdef HAVE_FFS
    idx = ffs(bits);
#else

    for (size_t i = 0; i < std::numeric_limits<PropValueUnitsBase>::digits; i++) {
        if (bits & (1 << i)) {
            idx = i + 1;
            break;
        }
    }

#endif

    return (idx < sizeof(STR) / (sizeof(const char*))) ? STR[idx] : "?";
}

const char* to_string(PropValueAccess v)
{
    static const char* STR[] = {
        "readonly",
        "initonly",
        "readwrite"
    };

    auto idx = static_cast<size_t>(v);
    return (idx < sizeof(STR) / (sizeof(const char*))) ? STR[idx] : "?";
}

const char* to_string(PropValueVis v)
{
    static const char* STR[] = {
        "public",
        "hidden",
        "internal"
    };

    auto idx = static_cast<size_t>(v);
    return (idx < sizeof(STR) / (sizeof(const char*))) ? STR[idx] : "?";
}

const char* to_string(PropValueConstraints v)
{
    static const char* STR[] = {
        "",
        ">",
        ">=",
        "<",
        "<=",
        "[]",
        "()",
        "(]",
        "[)",
        "!=0",
        "enum",
        "min count",
        "max count",
        "range count",
        "..."
    };

    auto idx = static_cast<size_t>(v);
    return (idx < sizeof(STR) / (sizeof(const char*))) ? STR[idx] : "?";
}

t_symbol* to_symbol(PropValueType t)
{
    return gensym(to_string(t));
}

t_symbol* to_symbol(PropValueView v)
{
    return gensym(to_string(v));
}

t_symbol* to_symbol(PropValueUnits u)
{
    return gensym(to_string(u));
}

t_symbol* to_symbol(PropValueAccess v)
{
    return gensym(to_string(v));
}

t_symbol* to_symbol(PropValueVis v)
{
    return gensym(to_string(v));
}

t_symbol* to_symbol(PropValueConstraints v)
{
    return gensym(to_string(v));
}

} // namespace ceammc
