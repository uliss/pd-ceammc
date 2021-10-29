/*****************************************************************************
 * Copyright 2021 Serge Poltavsky. All rights reserved.
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
#include "list_convolve.h"
#include "ceammc_factory.h"
#include "datatype_mlist.h"

ListConvolve::ListConvolve(const PdArgs& args)
    : ListConvolveBase(args)
{
    createInlet();
    createOutlet();

    initModeProperty();
}

bool ListConvolve::calc()
{
    if (l0_.empty()) {
        OBJ_ERR << "first arg is empty";
        return false;
    }

    if (l1_.empty()) {
        OBJ_ERR << "second arg is empty";
        return false;
    }

    const auto NS = l0_.size() + l1_.size() - 1;
    lout_.fill(Atom(0.0), NS);

    for (size_t i = 0; i < l0_.size(); i++) {
        for (size_t j = 0; j < l1_.size(); j++)
            lout_[i + j] += l0_[i] * l1_[j];
    }

    return true;
}

void setup_list_convolve()
{
    ObjectFactory<ListConvolve> obj("list.convolve");
    obj.addAlias("list.conv");
    obj.processData<DataTypeMList>();

    obj.setXletsInfo(
        { "list: set first convolution arg then calculate",
            "list: set second convolution arg" },
        { "list: convolution result" });
}
