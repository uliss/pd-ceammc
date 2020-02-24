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
#include "tree_at.h"
#include "ceammc_factory.h"

TreeAt::TreeAt(const PdArgs& args)
    : BaseObject(args)
    , index_(nullptr)
    , ptr_(new DataTypeTree)
{
    index_ = new IntProperty("@index", positionalFloatArgument(0, 0));
    createProperty(index_);

    createOutlet();
    createInlet();
}

void TreeAt::onBang()
{
    onDataT(ptr_);
}

void TreeAt::onDataT(const DataTPtr<DataTypeTree>& dptr)
{
    ptr_ = dptr;

    if (!ptr_->isArray())
        OBJ_ERR << "tree list expected";
    else if (index_->value() >= 0 && index_->value() < ptr_->size())
        dataTo(0, ptr_->atPtr(index_->value()));
    else
        OBJ_ERR << "invalid index: " << index_->value();
}

void TreeAt::onInlet(size_t idx, const AtomList& lst)
{
    index_->setValue(atomlistToValue<int>(lst, 0));
}

void setup_tree_at()
{
    ObjectFactory<TreeAt> obj("tree.at");
    obj.processData<DataTypeTree>();
}
