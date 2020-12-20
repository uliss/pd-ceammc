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
#ifndef CEAMMC_JSON_H
#define CEAMMC_JSON_H

#include "ceammc_atomlist.h"
#include "ceammc_data.h"

#include <string>

namespace ceammc {
namespace json {

    std::string to_json(const Atom& a, int indent = -1);
    std::string to_json(const AtomList& l, int indent = -1);
    std::string to_json(const DataTypeString& str, int indent = -1);
    std::string to_json(const DataTypeMList& ml, int indent = -1);
    std::string to_json(const DataTypeDict& dict, int indent = -1);
}

}

#endif // CEAMMC_JSON_H
