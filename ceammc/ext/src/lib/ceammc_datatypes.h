/*****************************************************************************
 * Copyright 2017 Serge Poltavsky. All rights reserved.
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
#ifndef CEAMMC_DATATYPES_H
#define CEAMMC_DATATYPES_H

#include <boost/static_string.hpp>

#include <cstdint>

namespace ceammc {

constexpr size_t DATA_TYPE_NAME_MAX_LENGTH = 14;

namespace data {
    constexpr uint16_t DATA_INVALID = uint16_t(-1);
    constexpr uint16_t DATA_PROPERTY = 31;
    constexpr uint16_t DATA_WRAPPER = 32;
}

using DataTypeName = boost::static_string<DATA_TYPE_NAME_MAX_LENGTH>;
using DataTypeId = uint16_t;

}

#endif // CEAMMC_DATATYPES_H
