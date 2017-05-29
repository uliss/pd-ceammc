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
#include "ceammc_datapointer.h"
#include "ceammc_datastorage.h"

#include <boost/make_shared.hpp>
#include <limits>

using namespace ceammc;

typedef unsigned int data_id_type;
static const unsigned int MASK_BITS = 12;
static const unsigned int DATA_MAGIC = (data_id_type(2) << (std::numeric_limits<data_id_type>::digits - 2));

// on 32-bit uint - use 2**20 unique object id
// on 64-bit uint - use 2**52 unique object id
static const data_id_type ID_MASK = (std::numeric_limits<data_id_type>::max() >> MASK_BITS);

// use 2**12 unique data types
static const data_id_type TYPE_MASK = ~(DATA_MAGIC | ID_MASK);
static const unsigned int TYPE_SHIFT = std::numeric_limits<data_id_type>::digits - MASK_BITS;

Data::Data()
    : id_(0)
    , data_(0)
{
}

Data::Data(BaseData* d)
    : id_(0)
    , data_(d)
{
    if (data_) {
        id_ = DataStorage::instance().generateId(type());
        DataStorage::instance().add(type(), id_, this);
    }
}

Data::~Data()
{
    if (data_) {
        DataStorage::instance().remove(type(), id_);
        delete data_;
    }
}

Atom Data::toAtom() const
{
    Atom res;
    if (isNull())
        return res;

    res.setFloat(0, true);
    t_atom* a = reinterpret_cast<t_atom*>(&res);

    data_id_type id = id_ & ID_MASK;
    data_id_type t = (type() << TYPE_SHIFT);
    data_id_type value = DATA_MAGIC | t | id;

    a->a_w.w_index = value;
    return res;
}

DataPtr Data::fromAtom(const Atom& a)
{
    DataHandle dh = unpackAtom(a);
    if (!dh.first)
        return DataPtr();

    Data* ptr = DataStorage::instance().get(dh.first, dh.second);
    if (!ptr)
        return DataPtr();

    return DataPtr(ptr->clone());
}



bool Data::isData(const Atom& a)
{
    if (!a.isFloat())
        return false;

    const t_atom* atom = reinterpret_cast<const t_atom*>(&a);
    data_id_type value = static_cast<data_id_type>(atom->a_w.w_index);

    return DATA_MAGIC & value;
}

DataHandle Data::unpackAtom(const Atom& a)
{
    if (!a.isFloat())
        return DataHandle(0, 0);

    const t_atom* atom = reinterpret_cast<const t_atom*>(&a);
    data_id_type value = static_cast<data_id_type>(atom->a_w.w_index);

    if (!(DATA_MAGIC & value))
        return DataHandle(0, 0);

    DataType t = (value & TYPE_MASK) >> TYPE_SHIFT;
    DataId id = value & ID_MASK;
    return DataHandle(t, id);
}

Atom Data::packAtom(const DataHandle& v)
{
    Atom res(0.f);
    t_atom* a = reinterpret_cast<t_atom*>(&res);

    data_id_type t = static_cast<unsigned int>(v.first) << TYPE_SHIFT;
    data_id_type id = v.second & ID_MASK;
    data_id_type value = DATA_MAGIC | t | id;
    a->a_w.w_index = value;
    return res;
}

Data* Data::clone() const
{
    if (isNull())
        return 0;

    Data* ptr = new Data();
    ptr->data_ = data_->clone();
    ptr->id_ = DataStorage::instance().generateId(type());
    DataStorage::instance().add(type(), ptr->id_, ptr);
    return ptr;
}

BaseData::~BaseData() {}

bool BaseData::registerData(DataType type)
{
    return DataStorage::instance().addNewType(type);
}
