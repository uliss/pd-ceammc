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
#ifndef DATA_SET_H
#define DATA_SET_H

#include "ceammc_data.h"
#include "ceammc_dataatom.h"
#include "ceammc_object.h"
#include "datatype_set.h"

using namespace ceammc;

class DataSet : public BaseObject {
    DataT<DataTypeSet> set_;

public:
    DataSet(const PdArgs& a);

    void dump() const;
    void onBang();
    void onFloat(float f);
    void onSymbol(t_symbol* s);
    void onList(const AtomList& l);
    void onDataT(const DataTypeSet& s);

    void m_clear(t_symbol*, const AtomList&);
    void m_add(t_symbol*, const AtomList& l);
    void m_remove(t_symbol*, const AtomList& l);
};

extern "C" void setup_data0x2eset();

#endif // DATA_SET_H
