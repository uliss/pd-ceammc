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
#include "list_resample.h"
#include "ceammc_factory.h"
#include "ceammc_fn_list.h"
#include "datatype_mlist.h"

ListResample::ListResample(const PdArgs& a)
    : BaseObject(a)
    , ratio_(nullptr)
{
    ratio_ = new FloatProperty("@ratio", 1);
    ratio_->checkOpenedRange(0, 100);
    ratio_->setArgIndex(0);
    addProperty(ratio_);

    createInlet();
    createOutlet();
}

void ListResample::onList(const AtomListView& lv)
{
    AtomList res;

    if (!list::resample(lv, res, ratio_->value())) {
        OBJ_ERR << "resample error";
        return;
    }

    listTo(0, res);
}

void ListResample::onInlet(size_t n, const AtomListView& lv)
{
    ratio_->set(lv);
}

void ListResample::onDataT(const MListAtom& ml)
{
    AtomList res;

    if (!list::resample(ml->data(), res, ratio_->value())) {
        OBJ_ERR << "resample error";
        return;
    }

    atomTo(0, MListAtom(res));
}

void setup_list_resample()
{
    ObjectFactory<ListResample> obj("list.resample");
    obj.addAlias("list.r");
    obj.processData<DataTypeMList>();
}
