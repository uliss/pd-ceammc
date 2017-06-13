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
#include "ceammc_datastorage.h"
#include "ceammc_log.h"

namespace ceammc {
Data::Data()
    : data_(0)
    , desc_(0, 0)
{
}

Data::Data(AbstractData* d)
    : data_(d)
    , desc_(0, 0)
{
    if (data_) {
        desc_ = DataStorage::instance().generateNewDesc(data_);
        DataStorage::instance().add(desc_, this);
    }
}

Data::~Data()
{
    if (data_) {
        DataStorage::instance().remove(desc_);
        delete data_;
    }
}

Atom Data::toAtom() const
{
    Atom res;
    if (isNull())
        return res;

    res.setData(desc());
    return res;
}

SharedDataPtr Data::fromAtom(const Atom& a)
{
    if (!a.isData()) {
        std::cerr << "invalid atom: " << a;
        return SharedDataPtr();
    }

    DataDesc desc = a.getData();

    Data* ptr = DataStorage::instance().get(desc);
    if (!ptr) {
        LIB_DBG << "data not found: " << desc;
        return SharedDataPtr();
    }

    if (ptr->type() != a.dataType()) {
        LIB_DBG << "invalid data type: " << desc;
        return SharedDataPtr();
    }

    return SharedDataPtr(ptr->clone());
}

Data* Data::getTypedData(const DataDesc& d)
{
    return DataStorage::instance().get(d);
}

DataType Data::type() const
{
    return data_ ? data_->type() : 0;
}

DataDesc Data::desc() const
{
    return desc_;
}

bool Data::operator==(const Data& d) const
{
    if (this == &d)
        return true;

    if (!data_ && !d.data_)
        return true;

    if (!data_ || !d.data_)
        return false;

    return data_->isEqual(d.data_);
}

Data* Data::clone() const
{
    if (isNull())
        return 0;

    return new Data(data_->clone());
}

void Data::setData(AbstractData* d)
{
    if (data_) {
        DataStorage::instance().remove(desc_);
        delete data_;
        data_ = 0;
    }

    if (d) {
        data_ = d;
        desc_ = DataStorage::instance().generateNewDesc(data_);
        DataStorage::instance().add(desc_, this);
    }
}

void Data::setData(const Atom& a)
{
    if (!a.isData())
        return;

    Data* p = Data::getTypedData(a.getData());
    if (!p || !p->data())
        return;

    setData(p->data()->clone());
}
}
