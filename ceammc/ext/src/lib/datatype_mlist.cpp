/*****************************************************************************
 * Copyright 2018 Serge Poltavsky. All rights reserved.
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
#include "datatype_mlist.h"
#include "ceammc_datastorage.h"
#include "ceammc_json.h"
#include "ceammc_log.h"
#include "ceammc_string.h"
#include "fmt/core.h"

#include <algorithm>
#include <cmath>
#include <cstring>

constexpr const char* TYPE_NAME = "MList";

namespace ceammc {

DataTypeId DataTypeMList::staticType()
{
    CEAMMC_REGISTER_DATATYPE(TYPE_NAME, [](const AtomListView& args) -> Atom { return new DataTypeMList(args); }, {});
}

DataTypeMList::DataTypeMList() noexcept = default;

DataTypeMList::DataTypeMList(const AtomListView& lv)
    : data_(lv)
{
}

DataTypeMList::DataTypeMList(AtomList&& lst) noexcept
    : data_(std::move(lst))
{
}

DataTypeMList::DataTypeMList(const DataTypeMList& mlist)
    : data_(mlist.data_)
{
}

DataTypeMList::DataTypeMList(DataTypeMList&& mlist) noexcept
    : data_(std::move(mlist.data_))
{
}

DataTypeMList& DataTypeMList::operator=(const DataTypeMList& mlist)
{
    if (this == &mlist)
        return *this;

    data_ = mlist.data_;
    return *this;
}

DataTypeMList& DataTypeMList::operator=(DataTypeMList&& mlist)
{
    if (this == &mlist)
        return *this;

    data_ = std::move(mlist.data_);
    return *this;
}

DataTypeId DataTypeMList::type() const noexcept
{
    return staticType();
}

DataTypeMList* DataTypeMList::clone() const
{
    return new DataTypeMList(*this);
}

bool DataTypeMList::isEqual(const AbstractData* cmp) const noexcept
{
    auto mlist = cmp->as<DataTypeMList>();
    if (!mlist)
        return false;

    return mlist->data_ == data_;
}

std::string DataTypeMList::toJsonString() const
{
    return ceammc::json::to_json_string(*this);
}

std::string DataTypeMList::toListStringContent() const noexcept
{
    string::MediumString str;
    string::parsed_list_to_string(data_.view(), str);
    return std::string(str.data(), str.size());
}

std::string DataTypeMList::toDictStringContent() const noexcept
{
    return fmt::format("items: {}", toListStringContent());
}

bool DataTypeMList::set(const AbstractData* d) noexcept
{
    return setDataT<DataTypeMList>(d);
}

std::string DataTypeMList::toString() const
{
    return fmt::format("({})", toListStringContent());
}

void DataTypeMList::append(const Atom& a)
{
    data_.append(a);
}

void DataTypeMList::append(const AtomListView& lv)
{
    data_.append(lv);
}

void DataTypeMList::clear()
{
    data_.clear();
}

bool DataTypeMList::insert(size_t idx, const AtomListView& lv)
{
    return data_.insert(idx, lv);
}

void DataTypeMList::prepend(const Atom& a)
{
    data_.insert(0, a);
}

void DataTypeMList::prepend(const AtomListView& lv)
{
    data_.insert(0, lv);
}

bool DataTypeMList::pop()
{
    if (data_.empty())
        return false;

    data_.remove(data_.size() - 1);
    return true;
}

bool DataTypeMList::remove(size_t idx)
{
    return data_.remove(idx);
}

void DataTypeMList::reserve(size_t n)
{
    data_.reserve(n);
}

void DataTypeMList::setRaw(const AtomListView& lv)
{
    data_ = lv;
}

DataTypeMList DataTypeMList::rotateLeft(int steps) const
{
    if (size() < 2 || steps == 0)
        return *this;

    int sz = int(size());
    steps = steps % sz;
    if (steps < 0)
        steps += sz;

    DataTypeMList res(*this);
    std::rotate(res.begin(), res.begin() + steps, res.end());
    return res;
}

DataTypeMList DataTypeMList::flatten() const
{
    DataTypeMList res;
    res.data_.reserve(size());

    for (auto& x : data_) {
        if (x.isData()) {
            if (x.isA<DataTypeMList>()) {
                auto mlist = x.asD<DataTypeMList>();
                if (!mlist) {
                    LIB_ERR << "invalid mlist pointer";
                    continue;
                }

                // recursion call
                res.data_.append(mlist->flatten().data_);
            } else
                res.data_.append(x);
        } else
            res.data_.append(x);
    }

    return res;
}

DataTypeMList DataTypeMList::slice(int start, int end, size_t step) const
{
    return DataTypeMList(data_.slice(start, end, step));
}

void DataTypeMList::sort()
{
    data_.sort();
}

void DataTypeMList::reverse()
{
    data_.reverse();
}

void DataTypeMList::shuffle()
{
    data_.shuffle();
}

bool DataTypeMList::contains(const Atom& a) const
{
    return data_.contains(a);
}

bool DataTypeMList::contains(const DataTypeMList& l) const
{
    auto it = std::search(data_.begin(), data_.end(), l.begin(), l.end());
    return it != data_.end();
}

std::ostream& operator<<(std::ostream& os, const DataTypeMList& d)
{
    return os << d.toString();
}

}
