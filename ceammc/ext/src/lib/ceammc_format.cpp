/*****************************************************************************
 * Copyright 2016 Serge Poltavsky. All rights reserved.
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
#include "ceammc_format.h"
#include "ceammc_atom.h"
#include "ceammc_atomlist.h"
#include "ceammc_controlvalue.h"
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <sstream>

namespace ceammc {

template <class T>
static inline T clip(T v, T min, T max)
{
    return std::min(max, std::max(min, v));
}

std::string to_string(const Atom& a)
{
    std::ostringstream ss;
    ss << a;
    return ss.str();
}

std::string to_float_string(const Atom& a)
{
    if (!a.isFloat())
        return "";

    std::ostringstream ss;
    ss << a.asFloat();
    return ss.str();
}

std::string to_hex_string(const Atom& a)
{
    if (!a.isFloat())
        return "";

    std::ostringstream ss;
    ss << std::hex << std::uppercase << static_cast<long>(a.asFloat());
    return ss.str();
}

std::string to_float_range_string(const Atom& a, float min, float max)
{
    if (!a.isFloat())
        return "";

    std::ostringstream ss;
    ss << clip(a.asFloat(), min, max);
    return ss.str();
}

} // namespace ceammc
