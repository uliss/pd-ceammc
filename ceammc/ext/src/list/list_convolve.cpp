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
    : BaseObject(args)
{
    createInlet();
    createOutlet();

    createCbListProperty(
        "@a",
        [this]() {
            AtomList res;
            for (auto& f : l0_)
                res.append(f);

            return res; },
        [this](const AtomListView& lv) {
            setA(lv);
            return true;
        });

    createCbListProperty(
        "@b", [this]() {
            AtomList res;
            for (auto& f : l1_)
                res.append(f);

            return res; },
        [this](const AtomListView& lv) {
            setB(lv);
            return true;
        })
        ->setArgIndex(0);
}

void ListConvolve::onInlet(size_t n, const AtomListView& lv)
{
    if (n == 1)
        setB(lv);
}

void ListConvolve::onList(const AtomList& lst)
{
    setA(lst.view());
    if (!convolve())
        return;

    listTo(0, lout_);
}

void ListConvolve::onFloat(t_float f)
{
    l0_.resize(1);
    l0_[0] = f;

    if (!convolve())
        return;

    listTo(0, lout_);
}

void ListConvolve::onDataT(const MListAtom& ml)
{
    onList(ml->data());
}

bool ListConvolve::convolve()
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

void ListConvolve::setA(const AtomListView& lv)
{
    l0_.clear();
    l0_.reserve(lv.size());

    if (lv.isA<DataTypeMList>()) {
        auto data = lv.asD<DataTypeMList>();
        for (auto& a : *data)
            l0_.push_back(a.toT<t_float>(0));
    } else {
        for (auto& a : lv)
            l0_.push_back(a.toT<t_float>(0));
    }
}

void ListConvolve::setB(const AtomListView& lv)
{
    l1_.clear();
    l1_.reserve(lv.size());

    if (lv.isA<DataTypeMList>()) {
        auto data = lv.asD<DataTypeMList>();
        for (auto& a : *data)
            l1_.push_back(a.toT<t_float>(0));
    } else {
        for (auto& a : lv)
            l1_.push_back(a.toT<t_float>(0));
    }
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
