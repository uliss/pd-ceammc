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

BaseData::~BaseData() {}

AtomDataInfo data::toData(const Atom& a)
{
    if (!a.isFloat())
        return AtomDataInfo(0, 0);

    const t_atom* atom = reinterpret_cast<const t_atom*>(&a);
    data_id_type value = static_cast<data_id_type>(atom->a_w.w_index);

    if (!(DATA_MAGIC & value))
        return AtomDataInfo(0, 0);

    DataType t = (value & TYPE_MASK) >> TYPE_SHIFT;
    DataId id = value & ID_MASK;
    return AtomDataInfo(t, id);
}

Atom data::toAtom(const AtomDataInfo& v)
{
    Atom res(0.f);
    t_atom* a = reinterpret_cast<t_atom*>(&res);

    data_id_type t = static_cast<unsigned int>(v.first) << TYPE_SHIFT;
    data_id_type id = v.second & ID_MASK;
    data_id_type value = DATA_MAGIC | t | id;
    a->a_w.w_index = value;
    return res;
}

