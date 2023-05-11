/*****************************************************************************
 * Copyright 2023 Serge Poltavski. All rights reserved.
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
#include "data_bimap.h"
#include "ceammc_containers.h"
#include "ceammc_factory.h"

DataBiMap::DataBiMap(const PdArgs& args)
    : EditorListT<DataListIFace<BaseObject>>(args)
{
    createInlet();
    createOutlet();
    createOutlet();

    setHighlightSyntax(EDITOR_SYNTAX_SELECTOR);

    values_ = new ListProperty("@v");
    values_->setListCheckFn([](const AtomListView& lv) -> bool {
        return lv.size() > 0 && (lv.size() % 2 == 0);
    });
    values_->setArgIndex(0);
    values_->setSuccessFn([this](Property* p) {
        auto& v = values_->value();
        for (size_t i = 0; i < v.size(); i += 2)
            bimap_.insert({ v[i], v[i + 1] });
    });
    addProperty(values_);
}

void DataBiMap::onFloat(t_float v)
{
    auto it = bimap_.left.find(Atom(v));
    if (it != bimap_.left.end())
        atomTo(0, it->second);
}

void DataBiMap::onSymbol(t_symbol* s)
{
    auto it = bimap_.left.find(Atom(s));
    if (it != bimap_.left.end())
        atomTo(0, it->second);
}

void DataBiMap::onList(const AtomListView& lv)
{
    SmallAtomList res;
    res.reserve(lv.size());

    for (auto& a : lv) {
        auto it = bimap_.left.find(a);
        if (it != bimap_.left.end())
            res.push_back(it->second);
    }

    listTo(0, res.view());
}

void DataBiMap::onInlet(size_t idx, const AtomListView& lv)
{
    if (lv.empty())
        return;
    else if (lv.isAtom()) {
        auto it = bimap_.right.find(lv[0]);
        if (it != bimap_.right.end())
            atomTo(1, it->second);
    } else {
        SmallAtomList res;
        res.reserve(lv.size());

        for (auto& a : lv) {
            auto it = bimap_.right.find(a);
            if (it != bimap_.right.end())
                res.push_back(it->second);
        }

        listTo(1, res.view());
    }
}

void setup_data_bimap()
{
    ObjectFactory<DataBiMap> obj("data.bimap");
    obj.addAlias("bimap");

    DataBiMap::registerMethods(obj);
}
