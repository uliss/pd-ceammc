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
#include "flow_list2many.h"
#include "ceammc_factory.h"
#include "fmt/format.h"

constexpr size_t MAX_ARGS = 64;

FlowList2Many::FlowList2Many(const PdArgs& args)
    : BaseObject(args)
{
    const auto N = std::min<size_t>(MAX_ARGS, args.args.size());
    selectors_.reserve(N);
    tooltips_.reserve(N);

    for (size_t i = 0; i < N; i++) {
        const auto& a = args.args[i];
        if (!a.isSymbol()) {
            OBJ_ERR << "symbol argument is expected, got: " << a;
            continue;
        }

        auto sel = a.asT<t_symbol*>();

        selectors_.push_back(sel);
        tooltips_.push_back(fmt::format("any: {} list\\[{}\\]", sel->s_name, i));

        createOutlet();
    }
}

void FlowList2Many::onList(const AtomListView& lv)
{
    const size_t N = std::min<size_t>(lv.size(), selectors_.size());

    for (size_t i = N; i > 0; i--) {
        auto outi = i - 1;
        anyTo(outi, selectors_[outi], lv[outi]);
    }
}

const char* FlowList2Many::annotateOutlet(size_t n) const
{
    if (n < tooltips_.size())
        return tooltips_[n].c_str();
    else
        return "";
}

void setup_flow_list2many()
{
    ObjectFactory<FlowList2Many> obj("flow.list2many");
    obj.addAlias("list->many");
    obj.noArgsAndPropsParse();
    obj.addInletInfo("list: input list");

    obj.setDescription("routes list element with message selectors");
    obj.setCategory("flow");
    obj.setKeywords({"conv", "map", "list"});
}
