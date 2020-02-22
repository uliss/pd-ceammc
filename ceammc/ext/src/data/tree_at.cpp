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
{
    for (size_t i = 0; i < args.args.size(); i++)
        createOutlet();
}

void TreeAt::onDataT(const DataTPtr<DataTypeTree>& dptr)
{
    auto N = args().size();
    for (size_t i = N; i > 0; i--) {
        auto n = i - 1;
        auto& key = args()[n];

        if (key.isFloat()) {
            auto idx = key.asInt();
            if (idx < 0 || idx >= N) {
                OBJ_ERR << "invalid index: " << key;
                continue;
            }

            dataTo(0, dptr->at(idx).asDataPtr());
        } else if (key.isSymbol()) {
            auto idx = key.asSymbol();
            dataTo(0, dptr->at(idx).asDataPtr());
        } else
            OBJ_ERR << "invalid key: " << key;
    }
}

void setup_tree_at()
{
    ObjectFactory<TreeAt> obj("tree.at");
    obj.processData<DataTypeTree>();
}
