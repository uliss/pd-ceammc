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
#include "ceammc_abstractdata.h"
#include "ceammc_datastorage.h"
#include "ceammc_log.h"
#include "fmt/format.h"

#include <cstdio>
#include <typeinfo>

namespace ceammc {

AbstractData::~AbstractData() noexcept = default;

void AbstractData::dump() const
{
    LIB_DBG << toString();
}

std::string AbstractData::toString() const
{
    throw std::runtime_error("not implemented");
    return fmt::format("{[Data {} id: {}]}", typeid(*this).name(), type());
}

std::string AbstractData::objectToJsonString() const
{
    return fmt::format(R"({{"datatype":"{}","value":{}}})", typeName(), valueToJsonString());
}

std::string AbstractData::valueToJsonString() const
{
    return "null";
}

bool AbstractData::isEqual(const AbstractData* d) const noexcept
{
    if (type() != d->type())
        return false;

    return this == d;
}

bool AbstractData::isLess(const AbstractData* d) const noexcept
{
    if (type() != d->type())
        return type() < d->type();

    return false;
}

std::string AbstractData::typeName() const noexcept
{
    return DataStorage::instance().nameByType(type());
}

std::string AbstractData::toListConstructor() const noexcept
{
    return fmt::format("{}", typeName(), toString());
}

std::string AbstractData::toDictConstructor() const noexcept
{
     return fmt::format("{}", typeName(), toString());
}

bool AbstractData::canInitWithList() const noexcept
{
    return DataStorage::instance().fromListFunction(typeName()) != nullptr;
}

bool AbstractData::canInitWithDict() const noexcept
{
    return DataStorage::instance().fromDictFunction(typeName()) != nullptr;
}
}
