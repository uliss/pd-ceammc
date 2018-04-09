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
#include "list_foldleft.h"
#include "ceammc_factory.h"

ListFoldLeft::ListFoldLeft(const PdArgs& args)
    : BaseObject(args)
    , init_(0)
    , result_(0)
    , in_loop_(false)
{
    createInlet();
    createOutlet();
    createOutlet();

    init_ = new FloatProperty("@init", positionalFloatArgument(0, 0));
    createProperty(init_);
}

void ListFoldLeft::onList(const AtomList& lst)
{
    if (in_loop_) {
        OBJ_ERR << "endless loop detected...";
        return;
    }

    in_loop_ = true;
    result_ = init_->value();

    AtomList pair(0.f, 0.f);

    for (size_t i = 0; i < lst.size(); i++) {
        t_float v = 0;
        if (!lst[i].getFloat(&v))
            continue;

        pair[0].setFloat(result_);
        pair[1].setFloat(v);
        listTo(1, pair);
    }

    floatTo(0, result_);
    in_loop_ = false;
}

void ListFoldLeft::onInlet(size_t n, const AtomList& lst)
{
    bool ok = lst.size() == 1 && lst[0].getFloat(&result_);

    if (!ok) {
        OBJ_ERR << "invalid foldl input from binary function: " << lst;
        return;
    }
}

void setup_list_foldl()
{
    ObjectFactory<ListFoldLeft> obj("list.foldl");
}
