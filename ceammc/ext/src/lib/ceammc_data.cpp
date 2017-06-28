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
#include "ceammc_data.h"
#include "ceammc_datatypes.h"
#include "ceammc_datastorage.h"

using namespace ceammc;

static const DataDesc INVALID(data::DATA_INVALID, DataId(-1));

DataPtr::DataPtr(AbstractData* data)
    : desc_(INVALID)
    , data_(data)
{
    desc_ = DataStorage::instance().add(data_);
}

DataPtr::DataPtr(const Atom& data)
    : desc_(INVALID)
    , data_(0)
{
    if (!data.isData())
        return;

    desc_ = data.getData();
    data_ = DataStorage::instance().acquire(desc_);
}

DataPtr::DataPtr(const DataPtr& d)
    : desc_(INVALID)
    , data_(0)
{
    desc_ = d.desc_;
    data_ = DataStorage::instance().acquire(desc_);
}

DataPtr& DataPtr::operator=(const DataPtr& d)
{
    if (this == &d)
        return *this;

    DataStorage::instance().release(desc_);

    desc_ = d.desc_;
    data_ = DataStorage::instance().acquire(desc_);
    return *this;
}

DataPtr::~DataPtr()
{
    DataStorage::instance().release(desc_);
}

bool DataPtr::isValid() const
{
    return desc_.type != data::DATA_INVALID
        && desc_.id != DataId(-1)
        && data_ != 0;
}

DataDesc DataPtr::desc() const
{
    return desc_;
}

size_t DataPtr::refCount() const
{
    return DataStorage::instance().refCount(desc_);
}

const AbstractData* DataPtr::data() const
{
    return data_;
}

const AbstractData* DataPtr::operator->() const
{
    return data_;
}

Atom DataPtr::asAtom() const
{
    return Atom(desc_);
}

bool DataPtr::operator==(const DataPtr& d) const
{
    if (data_ == d.data_)
        return true;

    if (isValid() && d.isValid())
        return data_->isEqual(d.data_);

    return false;
}

bool DataPtr::operator!=(const DataPtr& d) const
{
    return !this->operator==(d);
}

void DataPtr::invalidate()
{
    if (isNull())
        return;

    DataStorage::instance().release(desc_);
    data_ = 0;
    desc_ = INVALID;
}

bool ceammc::operator<(const DataPtr& d0, const DataPtr& d1)
{
    if (&d0 == &d1)
        return false;

    if (d0.data() == d1.data())
        return false;

    if (d0.isValid()) {
        return d1.isValid() ? d0.data()->isLess(d1.data()) : false;
    } else {
        return true;
    }
}

bool ceammc::DataPtr::isNull() const
{
    return !isValid();
}
