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
#include "dict_from_list.h"
#include "ceammc_factory.h"
#include "datatype_dict.h"

static const int MIN_STEP = 2;
static const int DEFAULT_STEP = 2;

DictFromList::DictFromList(const PdArgs& args)
    : BaseObject(args)
    , n_(nullptr)
{
    n_ = new IntPropertyMinEq("@step", positionalFloatArgument(0, DEFAULT_STEP), MIN_STEP);
    createProperty(n_);

    createOutlet();
}

void DictFromList::onList(const AtomList& l)
{
    DataTypeDict dict;
    size_t STEP = n_->value();
    size_t T = (l.size() / STEP) * STEP;

    if (STEP == 2) {
        for (size_t i = 0; i < T; i += STEP)
            dict.insert(l[i], l[i + 1]);
    } else {
        for (size_t i = 0; i < T; i += STEP)
            dict.insert(l[i], l.slice(i + 1, i + STEP - 1));
    }

    dataTo(0, DataTPtr<DataTypeDict>(dict));
}

void setup_dict_from_list()
{
    ObjectFactory<DictFromList> obj("dict.from_list");
    obj.addAlias("list->dict");
}
