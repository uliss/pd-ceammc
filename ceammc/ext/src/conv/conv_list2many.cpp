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
#include "conv_list2many.h"
#include "ceammc_factory.h"

ConvList2Many::ConvList2Many(const PdArgs& args)
    : BaseObject(args)
{
    selectors_.reserve(args.args.size());

    for (auto& a : args.args) {
        if (!a.isSymbol()) {
            OBJ_ERR << "symbol argument is expected, got: " << a;
            continue;
        }

        selectors_.push_back(a.asT<t_symbol*>());
    }

    createOutlet();
}

void ConvList2Many::onList(const AtomList& lv)
{
    const size_t N = std::min<size_t>(lv.size(), selectors_.size());

    for (size_t i = 0; i < N; i++)
        anyTo(0, selectors_[i], lv[i]);
}

void setup_conv_list2many()
{
    ObjectFactory<ConvList2Many> obj("conv.list2many");
    obj.addAlias("list->many");
    obj.noArgsAndPropsParse();
}
