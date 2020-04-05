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
#include "dict_get.h"
#include "ceammc_factory.h"

static const size_t MAX_KEYS = 32;

DictGet::DictGet(const PdArgs& args)
    : BaseObject(args)
    , keys_(args.args.view(0, MAX_KEYS))
{
    for (size_t i = 0; i < keys_.size(); i++)
        createOutlet();
}

void DictGet::onDataT(const DictAtom& dict)
{
    long n = keys_.size();
    // back order
    while (n-- > 0) {
        const auto& k = keys_[n];
        auto v = dict->value(k);
        if (DataTypeDict::isNull(v))
            continue;

        to_outlet(outletAt(n), v);
    }
}

void setup_dict_get()
{
    ObjectFactory<DictGet> obj("dict.get");
    obj.parseOnlyPositionalProps(true);
    obj.processData<DataTypeDict>();
}
