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
#include "datatype_color.h"
#include "ceammc_containers.h"
#include "ceammc_datastorage.h"
#include "ceammc_string.h"
#include "colorm/colorm.h"
#include "fmt/core.h"
#include "lex/parser_color.h"

//#include <algorithm>
#include <boost/algorithm/string/predicate.hpp>
//#include <boost/functional/hash.hpp>
#include <ctime>
#include <random>

namespace {
using namespace ceammc;

constexpr const char* TYPE_NAME = "Color";

DataTypeId initType()
{
    DataTypeId id = DataStorage::instance().typeByName(TYPE_NAME);
    if (id == data::DATA_INVALID)
        id = DataStorage::instance().registerNewType(TYPE_NAME,
            [](const AtomListView& lv) -> Atom { return new DataTypeColor(lv); });

    return id;
}

colorm::Srgb toRGB(const DataTypeColor& color)
{
    return colorm::Srgb(color.dataAt(0), color.dataAt(1), color.dataAt(2), color.dataAt(3));
}

}

namespace ceammc {

const DataTypeId DataTypeColor::dataType = initType();

DataTypeColor::DataTypeColor()
    : data_ { 0, 0, 0, 1 }
{
}

DataTypeColor::DataTypeColor(std::uint32_t value)
    : data_ {
        float((0xFF & (value >> 24)) / 255.0),
        float((0xFF & (value >> 16)) / 255.0),
        float((0xFF & (value >> 8)) / 255.0),
        float((0xFF & value) / 255.0),
    }
{
}

DataTypeColor::DataTypeColor(const AtomListView& lv)
    : data_ { 0, 0, 0, 0 }
{
    if (lv.isSymbol()) {
        parser::ColorFullMatch p;
        if (p.parse(lv[0])) {
            data_[0] = p.norm_red();
            data_[1] = p.norm_green();
            data_[2] = p.norm_blue();
            data_[3] = p.norm_alpha();
        }
    } else {
        data_[0] = clip01<float>(lv.floatAt(0, 0));
        data_[1] = clip01<float>(lv.floatAt(1, 0));
        data_[2] = clip01<float>(lv.floatAt(2, 0));
        data_[3] = clip01<float>(lv.floatAt(3, 1));
    }
}

DataTypeId DataTypeColor::type() const noexcept
{
    return dataType;
}

bool DataTypeColor::isEqual(const AbstractData* d) const noexcept
{
    if (!d || d->type() != dataType)
        return false;

    const DataTypeColor* ds = d->as<DataTypeColor>();
    return operator==(*ds);
}

AtomList DataTypeColor::toList(bool sorted) const
{
    return { data_[0], data_[1], data_[2], data_[3] };
}

std::string DataTypeColor::toString() const noexcept
{
    try {
        if (data_[3] == 1)
            return fmt::format("#{:02X}{:02X}{:02X}", red8(), green8(), blue8());
        else
            return fmt::format("#{:02X}{:02X}{:02X}{:02X}", red8(), green8(), blue8(), alpha8());
    } catch (std::exception& e) {
        return {};
    }
}

AbstractData* DataTypeColor::clone() const
{
    return new DataTypeColor(*this);
}

bool DataTypeColor::operator==(const DataTypeColor& c) const noexcept
{
    return std::memcmp(data_, c.data_, sizeof(c.data_)) == 0;
}

DataTypeColor& DataTypeColor::operator=(const DataTypeColor& c)
{
    if (this == &c)
        return *this;

    std::memcpy(data_, c.data_, sizeof(c.data_));
    return *this;
}

DataTypeColor::DataTypeColor(const DataTypeColor& c)
{
    std::memcpy(data_, c.data_, sizeof(c.data_));
}

std::string DataTypeColor::toListStringContent() const noexcept
{
    SmallAtomListN<16> lst;
    for (auto& a : data_)
        lst.push_back(a);

    string::MediumString str;
    string::parsed_list_to_string(lst.view(), str);
    return std::string(str.data(), str.size());
}

std::string DataTypeColor::toDictStringContent() const noexcept
{
    return fmt::format("hex: {}", toString());
}

bool DataTypeColor::set(const AbstractData* d) noexcept
{
    return setDataT<DataTypeColor>(d);
}

bool DataTypeColor::looksLikeCtor(const AtomListView& lv) noexcept
{
    if (lv.empty() || !lv[0].isSymbol())
        return false;

    auto name = lv[0].asT<t_symbol*>()->s_name;
    return boost::starts_with(name, TYPE_NAME);
}

std::ostream& operator<<(std::ostream& os, const DataTypeColor& color)
{
    os << color.toString();
    return os;
}

}
