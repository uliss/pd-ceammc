/*****************************************************************************
 * Copyright 2020 Serge Poltavsky. All rights reserved.
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
#ifndef DATA_TREE_H
#define DATA_TREE_H

#include "ceammc_data.h"
#include "data_protocol.h"
#include "datatype_dict.h"
#include "datatype_mlist.h"
#include "datatype_set.h"
#include "datatype_string.h"
#include "datatype_tree.h"

using namespace ceammc;

class DataTree : public CollectionIFace<BaseObject> {
    typedef DataTPtr<DataTypeTree> TreePtr;
    TreePtr tree_;

public:
    DataTree(const PdArgs& args);

public:
    void proto_add(const AtomList& lst) final;
    bool proto_remove(const AtomList& lst) final;
    void proto_set(const AtomList& lst) final;
    void proto_clear() final;

    size_t proto_size() const final;

    void onBang() final;
    void onFloat(t_float f) final;
    void onSymbol(t_symbol* s) final;
    void onList(const AtomList& lst) final;
    void dump() const final;

    void onDataT(const DataTPtr<DataTypeTree>& ptr);
    void onDataT(const DataTPtr<DataTypeString>& ptr);
    void onDataT(const DataTPtr<DataTypeSet>& ptr);
    void onDataT(const DataTPtr<DataTypeDict>& ptr);
    void onDataT(const DataTPtr<DataTypeMList>& ptr);

    void m_find(t_symbol* s, const AtomList& lst);
    void m_at(t_symbol* s, const AtomList& lst);
    void m_key(t_symbol* s, const AtomList& lst);
    void m_insert(t_symbol* s, const AtomList& lst);
    void m_set_list(t_symbol* s, const AtomList& lst);

private:
    void setFromSymbol(t_symbol* s);
    void setFromFloat(t_float f);
};

void setup_data_tree();

#endif // DATA_TREE_H
