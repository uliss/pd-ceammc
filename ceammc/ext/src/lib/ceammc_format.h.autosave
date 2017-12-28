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
#ifndef CEAMMC_FORMATTER_H
#define CEAMMC_FORMATTER_H

#include <string>

namespace ceammc {

class Atom;
class AtomList;
class Message;

std::string to_string(const Atom& a);
std::string to_float_string(const Atom& a, const std::string& defaultValue = "");
std::string to_float_range_string(const Atom& a, float min, float max, const std::string& defaultValue = "");
std::string to_hex_string(const Atom& a, const std::string& defaultValue = "");

std::string to_string(const AtomList& a, const std::string& separator = " ");
std::string to_string(const Message& msg, const std::string& separator = " ");

} // namespace ceammc

#endif // CEAMMC_FORMATTER_H
