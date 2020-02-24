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
#include "tree_symbol.h"
#include "ceammc_factory.h"

TreeSymbol::TreeSymbol(const PdArgs& args)
    : BaseObject(args)
    , ptr_(nullptr)
{
    createOutlet();
}

void TreeSymbol::onBang()
{
    onDataT(ptr_);
}

void TreeSymbol::onDataT(const DataTPtr<DataTypeTree>& ptr)
{
    ptr_ = ptr;
    t_symbol* s = &s_;
    if (ptr.isValid() && ptr->getSymbol(&s))
        symbolTo(0, s);
    else
        OBJ_ERR << "not a symbol tree value";
}

void setup_tree_symbol()
{
    ObjectFactory<TreeSymbol> obj("tree.symbol");
    obj.addAlias("tree.s");
    obj.processData<DataTypeTree>();
}
