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
#include "ceammc_containers.h"
#include "ceammc_factory.h"

static const size_t MAX_KEYS = 32;

DictGet::DictGet(const PdArgs& args)
    : DictBase(args)
    , default_(nullptr)
{
    auto p = createCbListProperty(
        "@keys",
        [this]() -> AtomList {
            AtomList res;
            res.reserve(keys_.size());
            for (auto k : keys_)
                res.append(k);

            return res;
        },
        [this](const AtomListView& lv) -> bool {
            for (auto it = atom_filter_it_begin(lv, isSymbol); it != atom_filter_it_end(lv); ++it)
                keys_.push_back(it->asSymbol());

            return true;
        });

    p->setInitOnly();
    p->setArgIndex(0);

    p->setListCheckFn([this](const AtomListView& lv) {
        if (!lv.anyOf(isSymbol)) {
            OBJ_ERR << "only symbols are allowed as key, got: " << lv;
            return false;
        }

        if (lv.size() > MAX_KEYS) {
            OBJ_ERR << "only " << MAX_KEYS << " max keys are allowed, got: " << lv.size();
            return false;
        }

        return true;
    });

    default_ = new AtomProperty("@default", Atom());
    addProperty(default_);
}

void DictGet::initDone()
{
    for (size_t i = 0; i < keys_.size(); i++)
        createOutlet();
}

void DictGet::onDataT(const DictAtom& dict)
{
    long n = keys_.size();

    // back order
    while (n-- > 0) {
        if (dict->contains(keys_[n]))
            listTo(n, dict->at(keys_[n]));
        else if (!default_->value().isNone())
            atomTo(n, default_->value());
        else
            ; // no output
    }
}

void setup_dict_get()
{
    ObjectFactory<DictGet> obj("dict.get");
    obj.processData<DataTypeDict>();

    obj.setDescription("output dict values");
    obj.setCategory("data");
    obj.setKeywords({"get", "dictionary"});
}
