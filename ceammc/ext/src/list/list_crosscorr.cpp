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
#include "list_crosscorr.h"
#include "ceammc_factory.h"

ListCrosscorr::ListCrosscorr(const PdArgs& args)
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

void ListCrosscorr::onInlet(size_t n, const AtomListView& lv)
{
    if (n == 1)
        setB(lv);
}

void ListCrosscorr::onList(const AtomList& lst)
{
    setA(lst.view());
    if (!calc())
        return;

    listTo(0, lout_);
}

void ListCrosscorr::onFloat(t_float f)
{
    l0_.resize(1);
    l0_[0] = f;

    if (!calc())
        return;

    listTo(0, lout_);
}

bool ListCrosscorr::calc()
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

void ListCrosscorr::setA(const AtomListView& lv)
{
    l0_.clear();
    l0_.reserve(lv.size());

    for (auto& a : lv)
        l0_.push_back(a.toT<t_float>(0));
}

void ListCrosscorr::setB(const AtomListView& lv)
{
    l1_.clear();
    l1_.reserve(lv.size());

    for (auto& a : lv)
        l1_.push_back(a.toT<t_float>(0));
}

void setup_list_crosscorr()
{
    ObjectFactory<ListCrosscorr> obj("list.crosscorr");
    obj.addAlias("list.xcorr");

    obj.setXletsInfo(
        { "list: set first cross-correlation arg then calculate",
            "list: set second cross-correlation arg" },
        { "list: cross-correlation result" });
}
