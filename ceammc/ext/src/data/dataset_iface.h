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
#ifndef DATASET_IFACE_H
#define DATASET_IFACE_H

#include "ceammc_object.h"
#include "data_protocol.h"
#include "datatype_set.h"

#include <ctime>
#include <random>

using namespace ceammc;

template <class T>
class DataSetIface : public CollectionIFace<T> {
public:
    DataSetIface(const PdArgs& args)
        : CollectionIFace<T>(args)
    {
    }

    void dump() const override
    {
        BaseObject::dump();
        OBJ_DBG << data().toString();
    }

    void onBang() override
    {
        this->atomTo(0, data().clone());
    }

    void onFloat(t_float f) override
    {
        data().add(Atom(f));
    }

    void onSymbol(t_symbol* s) override
    {
        data().add(Atom(s));
    }

    void onList(const AtomListView& lv) override
    {
        data().add(lv);
    }

    void onDataT(const SetAtom& s)
    {
        data() = *s;
        onBang();
    }

    void proto_add(const AtomListView& lv) override
    {
        data().add(lv);
    }

    void proto_clear() override
    {
        data().clear();
    }

    size_t proto_size() const override
    {
        return data().size();
    }

    bool proto_remove(const AtomListView& lv) override
    {
        data().remove(lv);
        return true;
    }

    void proto_set(const AtomListView& lv) override
    {
        data() = DataTypeSet(lv);
    }

    bool proto_choose(Atom& res) const override
    {
        return data().choose(res);
    }

public:
    virtual DataTypeSet& data() = 0;
    virtual const DataTypeSet& data() const = 0;
};

#endif // DATASET_IFACE_H
