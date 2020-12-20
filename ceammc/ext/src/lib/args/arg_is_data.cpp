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
#include "arg_is_data.h"
#include "ceammc_datastorage.h"
#include "ceammc_format.h"
#include "ceammc_log.h"
#include "fmt/format.h"

#include <map>

namespace ceammc {

ArgIsData::ArgIsData()
    : ArgCheckerSingle()
    , type_(0)
    , name_(nullptr)
{
    setName("d");
}

bool ArgIsData::checkAtom(const Atom& a, CheckerContext& ctx) const
{
    if (type_ == 0) {
        bool rc = a.isData();
        if (!rc)
            ctx.error = "not a data value: " + to_string(a);

        return rc;
    } else {
        bool rc = a.isDataType(type_);
        if (!rc)
            ctx.error = fmt::format("not a data type: {}", name_->s_name);

        return rc;
    }
}

void ArgIsData::setType(const char* name)
{
    static const std::map<t_symbol*, uint16_t> types = {
        { gensym("String"), DataStorage::instance().typeByName("String") },
        { gensym("Tree"), DataStorage::instance().typeByName("Tree") },
        { gensym("Dict"), DataStorage::instance().typeByName("Dict") },
        { gensym("Set"), DataStorage::instance().typeByName("Set") },
        { gensym("Mlist"), DataStorage::instance().typeByName("MList") }
    };

    name_ = gensym(name);
    auto it = types.find(name_);
    if (it != types.end()) {
        type_ = it->second;
    } else {
        LIB_ERR << "unkknown datatype name: " << name_->s_name;
    }
}
}
