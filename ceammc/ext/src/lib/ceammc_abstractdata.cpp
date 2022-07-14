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
#include "ceammc_atomlist_view.h"
#include "ceammc_data.h"
#include "ceammc_datastorage.h"
#include "ceammc_format.h"
#include "ceammc_log.h"
#include "fmt/format.h"

#include <algorithm>
#include <cstdio>
#include <typeinfo>

namespace ceammc {

AbstractData::~AbstractData() noexcept = default;

void AbstractData::dump() const
{
    LIB_DBG << toString();
}

std::string AbstractData::toListString() const
{
    return fmt::format("{}({})", typeName().c_str(), toListStringContent());
}

std::string AbstractData::toDictString() const
{
    return fmt::format("{}[{}]", typeName().c_str(), toDictStringContent());
}

std::string AbstractData::toString() const
{
    return toListString();
}

std::string AbstractData::toJsonObject() const
{
    return fmt::format(R"({{"datatype":"{}","value":{}}})", typeName().c_str(), toJsonString());
}

std::string AbstractData::toJsonString() const
{
    return "null";
}

bool AbstractData::setFromDataString(const std::string& str)
{
    Atom data;

    if (parse(str, type(), data)) {
        return set(data.asData());
    } else {
        return false;
    }
}

bool AbstractData::setFromDataList(const AtomListView& lv)
{
    if (lv.empty())
        return true;

    Atom data;

    if (parse(lv, type(), data)) {
        return set(data.asData());
    } else {
        return false;
    }
}

bool AbstractData::setFromAtom(const Atom& a)
{
    const auto dt = a.dataType();
    if (dt == type()) {
        return set(a.asData());
    } else if (dt != 0) {
        LIB_ERR << fmt::format("unexpected datatype: '{}'", findTypeName(dt).c_str());
        return false;
    } else {
        LIB_ERR << fmt::format("expected data, got '{}'", to_string(a));
        return false;
    }
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

DataTypeName AbstractData::typeName() const noexcept
{
    return DataStorage::instance().nameByType(type());
}

bool AbstractData::canInitWithList() const noexcept
{
    return DataStorage::instance().fromListFunction(typeName()) != nullptr;
}

bool AbstractData::canInitWithDict() const noexcept
{
    return DataStorage::instance().fromDictFunction(typeName()) != nullptr;
}

bool AbstractData::parse(const std::string& str, int dataType, Atom& res)
{
    auto p = parseDataString(str.c_str());
    if (!p) {
        LIB_ERR << fmt::format("[{}] can't parse string '{}': {}", findTypeName(dataType).c_str(), str, p.err());
        return false;
    }

    auto& data = p.result();
    if (data.size() == 1 && data[0].dataType() == dataType) {
        res = data[0];
        return true;
    } else {
        LIB_ERR << fmt::format("datatype expected: '{}'", findTypeName(dataType).c_str());
        return false;
    }
}

bool AbstractData::parse(const AtomListView& lv, int dataType, Atom& res)
{
    auto p = parseDataList(lv);
    if (!p) {
        LIB_ERR << fmt::format("[{}] can't parse string '{}': {}", findTypeName(dataType).c_str(), to_string(lv), p.err());
        return false;
    }

    auto& data = p.result();
    if (data.size() == 1 && data[0].dataType() == dataType) {
        res = data[0];
        return true;
    } else {
        LIB_ERR << fmt::format("datatype expected: '{}'", findTypeName(dataType).c_str());
        return false;
    }
}

DataTypeName AbstractData::findTypeName(DataTypeId dataType)
{
    return DataStorage::instance().nameByType(dataType);
}
}
