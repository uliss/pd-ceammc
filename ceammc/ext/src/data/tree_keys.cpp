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
#include "tree_keys.h"
#include "ceammc_factory.h"

TreeKeys::TreeKeys(const PdArgs& args)
    : BaseObject(args)
    , ptr_(new DataTypeTree)
{
    createOutlet();
}

void TreeKeys::onBang()
{
    onDataT(ptr_);
}

void TreeKeys::onDataT(const DataTPtr<DataTypeTree>& ptr)
{
    ptr_ = ptr;

    if (!ptr_->isObject()) {
        OBJ_ERR << "not a dict tree";
        return;
    }

    listTo(0, ptr_->keys());
}

void setup_tree_keys()
{
    ObjectFactory<TreeKeys> obj("tree.keys");
    obj.processData<DataTypeTree>();
}
