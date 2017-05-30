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
        ObjectFactory<External>::setFloatFn(processFloatData);
    }

    static void processFloatData(typename ObjectFactory<External>::ObjectProxy* x, t_floatarg f)
    {
        DataPtr p = Data<CustomData>::fromAtom(Atom(f));
        if (!p)
            x->impl->onFloat(f);
        else
            x->impl->onData(*p->data());
    }
};

#endif // DATA_STRING_FACTORY_H
