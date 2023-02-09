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
#include "list_correlate.h"
#include "ceammc_factory.h"
#include "datatype_mlist.h"

ListCorrelate::ListCorrelate(const PdArgs& args)
    : ListConvolveBase(args)
{
    createInlet();
    createOutlet();

    using namespace ceammc::details;

    mode_ = new SymbolEnumProperty("@mode", { STR_VALID, STR_FULL, STR_SAME });
    addProperty(mode_);

    addProperty(new SymbolEnumAlias("@valid", mode_, gensym(STR_VALID)));
    addProperty(new SymbolEnumAlias("@same", mode_, gensym(STR_SAME)));
    addProperty(new SymbolEnumAlias("@full", mode_, gensym(STR_FULL)));
}

bool ListCorrelate::calc()
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

    const auto N0 = l0_.size();
    const auto N1 = l1_.size();
    for (size_t i = 0; i < N0; i++) {
        for (size_t j = 0; j < N1; j++)
            lout_[i + j] += l0_[i] * l1_[N1 - (j + 1)];
    }

    return true;
}

void setup_list_correlate()
{
    ObjectFactory<ListCorrelate> obj("list.correlate");
    obj.processData<DataTypeMList>();

    obj.setXletsInfo(
        { "list: set first cross-correlation arg then calculate",
            "list: set second cross-correlation arg" },
        { "list: cross-correlation result" });

    obj.setDescription("list cross-correlation");
    obj.setCategory("list");
    obj.setKeywords({"list", "correlation", "cross"});
}
