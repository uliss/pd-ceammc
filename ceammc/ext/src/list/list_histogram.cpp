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
#include "list_histogram.h"
#include "ceammc_factory.h"
#include "ceammc_fn_list.h"
#include "datatype_mlist.h"

ListHistogram::ListHistogram(const PdArgs& a)
    : ListBase(a)
    , bins_(0)
{
    createInlet();
    createOutlet();

    bins_ = new IntProperty("@bins", 100);
    bins_->setArgIndex(0);
    addProperty(bins_);
}

void ListHistogram::onList(const AtomList& l)
{
    int bins = bins_->value();
    if (bins < 2) {
        OBJ_ERR << "invalid bins value: " << bins;
        return;
    }

    listTo(0, list::histogram(l, size_t(bins)));
}

void ListHistogram::onInlet(size_t, const AtomListView& l)
{
    bins_->set(l);
}

void setup_list_histogram()
{
    ObjectFactory<ListHistogram> obj("list.histogram");
    obj.processData<DataTypeMList>();
}
