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
#include "ceammc_data.h"
#include "ceammc_datastorage.h"
#include "ceammc_format.h"
#include "ceammc_json.h"
#include "ceammc_log.h"

#include <algorithm>
#include <cmath>
#include <cstring>

namespace ceammc {

static Atom newMList(const AtomList& args)
{
    return new DataTypeMList(args);
}

int DataTypeMList::dataType = DataStorage::instance().registerNewType("MList", newMList);

DataTypeMList::DataTypeMList() noexcept = default;

DataTypeMList::DataTypeMList(const AtomList& lst)
    : data_(lst)
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

int DataTypeMList::type() const noexcept
{
    return dataType;
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

std::string DataTypeMList::toString() const
{
    std::string res("(");

    for (size_t i = 0; i < data_.size(); i++) {
        res += to_string_quoted(data_[i]);
        res.push_back(' ');
    }

    if (!data_.empty())
        res.pop_back();

    res.push_back(')');
    return res;
}

std::string DataTypeMList::valueToJsonString() const
{
    return ceammc::json::to_json(*this);
}

void DataTypeMList::append(const Atom& a)
{
    data_.append(a);
}

void DataTypeMList::append(const AtomList& lst)
{
    data_.append(lst);
}

void DataTypeMList::clear()
{
    data_.clear();
}

bool DataTypeMList::insert(size_t idx, const AtomList& lst)
{
    return data_.insert(idx, lst);
}

void DataTypeMList::prepend(const Atom& a)
{
    data_.insert(0, a);
}

void DataTypeMList::prepend(const AtomList& lst)
{
    data_.insert(0, lst);
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

void DataTypeMList::setRaw(const AtomList& lst)
{
    data_ = lst;
}

void DataTypeMList::setParsed(const AtomList& lst)
{
    auto ml = parse(lst);
    if (ml)
        std::swap(data_, ml.value().data_);
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

DataTypeMList::MaybeList DataTypeMList::parse(const AtomListView& lv)
{
    if (lv.anyOf(isData)) {
        LIB_ERR << "only core atom types allowed for parsing....";
        return DataTypeMList {};
    }

    return parse(to_string(lv, " "));
}

DataTypeMList::MaybeList DataTypeMList::parse(const std::string& str)
{
    auto pos = str.find('(');

    if (str.empty() || pos == std::string::npos)
        return {};

    auto parse_result = parseDataString(str);
    if (!parse_result) {
        LIB_ERR << "parse error: " << parse_result.err();
        return {};
    }

    if (!parse_result.result().isA<DataTypeMList>()) {
        LIB_ERR << "not a MList: " << str;
        return {};
    }

    return *parse_result.result().asD<DataTypeMList>();
}

std::ostream& operator<<(std::ostream& os, const DataTypeMList& d)
{
    return os << d.toString();
}

}
