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

DataPointer::DataPointer()
    : type_(0)
    , id_(0)
    , data_(0)
    , ref_count_(0)
{
}

DataPointer::DataPointer(Data* d)
    : type_(d->type())
    , id_(0)
    , data_(d)
    , ref_count_(new int(0))
{
    id_ = DataStorage::instance().generateId(type_);
    acquire();
}

DataPointer::DataPointer(const DataPointer& d)
    : type_(d.type_)
    , id_(d.id_)
    , data_(d.data_)
    , ref_count_(d.ref_count_)
{
    // self assignment check
    if (&d == this)
        return;

    acquire();
}

DataPointer& DataPointer::operator=(const DataPointer& d)
{
    // self assignment check
    if (&d == this)
        return *this;

    if (d.type_ != type_)
        return *this;

    release();

    id_ = d.id_;
    data_ = d.data_;
    ref_count_ = d.ref_count_;
    acquire();
    return *this;
}

DataPointer::~DataPointer()
{
    release();
}

Atom DataPointer::toAtom() const
{
    Atom res;
    res.setFloat(0, true);
    t_atom* a = reinterpret_cast<t_atom*>(&res);

    data_id_type id = id_ & ID_MASK;
    data_id_type type = (type_ << TYPE_SHIFT);
    data_id_type value = DATA_MAGIC | type | id;

    a->a_w.w_index = value;
    return res;
}

DataPointer DataPointer::fromAtom(const Atom& a)
{
    if (!isData(a))
        return DataPointer();

    const t_atom* atom = reinterpret_cast<const t_atom*>(&a);
    data_id_type value = static_cast<data_id_type>(atom->a_w.w_index);

    DataType t = (value & TYPE_MASK) >> TYPE_SHIFT;
    DataId id = value & ID_MASK;

    DataPointer* ptr = DataStorage::instance().get(t, id);
    return ptr == 0 ? DataPointer() : *ptr;
}

int DataPointer::refCount() const
{
    if (!ref_count_)
        return 0;

    return *ref_count_;
}

Data* DataPointer::data()
{
    return data_;
}

const Data* DataPointer::data() const
{
    return data_;
}

DataId DataPointer::id() const
{
    return id_;
}

DataType DataPointer::type() const
{
    return type_;
}

bool DataPointer::isData(const Atom& a)
{
    if (!a.isFloat())
        return false;

    const t_atom* atom = reinterpret_cast<const t_atom*>(&a);
    data_id_type value = static_cast<data_id_type>(atom->a_w.w_index);

    return DATA_MAGIC & value;
}

void DataPointer::acquire()
{
    if (!ref_count_)
        return;

    if ((*ref_count_) == 0) {
        if (!DataStorage::instance().add(type_, id_, this)) {
            std::cerr << "invalid add\n";
            return;
        }
    }

    (*ref_count_)++;
}

void DataPointer::release()
{
    if (!ref_count_)
        return;

    (*ref_count_)--;

    if ((*ref_count_) == 0) {
        delete data_;
        delete ref_count_;
        data_ = 0;
        if (!DataStorage::instance().remove(type_, id_)) {
            std::cerr << "can not remove data:\n";
        }
    }
}

bool Data::registerData(DataType type)
{
    return DataStorage::instance().addNewType(type);
}
