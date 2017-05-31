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
#ifndef DATA_STRING_FACTORY_H
#define DATA_STRING_FACTORY_H

#include "ceammc_datapointer.h"
#include "ceammc_factory.h"

using namespace ceammc;

template <class External, class CustomData>
class DataStringFactory : public ObjectFactory<External> {
public:
    typedef typename Data<CustomData>::DataPtr DataPtr;

public:
    DataStringFactory(const char* name, int flags = 0)
        : ObjectFactory<External>(name, flags)
    {
        ObjectFactory<External>::setListFn(processTypedData);
    }

    void processAnyData()
    {
        ObjectFactory<External>::setListFn(processAnyData);
    }

    static void processTypedData(typename ObjectFactory<External>::ObjectProxy* x,
        t_symbol*, int argc, t_atom* argv)
    {
        AtomList l(argc, argv);
        if (l.size() == 1 && l[0].isData()) {
            DataPtr ptr = Data<CustomData>::fromAtom(l[0]);
            if (ptr) {
                x->impl->onData(*ptr->data());
            } else {
                DataDesc d = l[0].getData();
                LIB_ERR << "can't get data with type=" << d.type << " and id=" << d.id;
            }
        } else {
            x->impl->onList(l);
        }
    }

    static void processAnyData(typename ObjectFactory<External>::ObjectProxy* x,
        t_symbol*, int argc, t_atom* argv)
    {
        AtomList l(argc, argv);
        if (l.size() == 1 && l[0].isData()) {
            DataDesc desc = l[0].getData();
            BaseData* ptr = DataFactory::instance().rawData(desc);
            if (ptr) {
                x->impl->onData(*ptr);
            } else {
                LIB_ERR << "can't get data with type=" << desc.type << " and id=" << desc.id;
            }
        } else {
            x->impl->onList(l);
        }
    }
};

#endif // DATA_STRING_FACTORY_H
