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
#include "list_convolve_base.h"
#include "datatype_mlist.h"

ListConvolveBase::ListConvolveBase(const PdArgs& args)
    : BaseObject(args)
    , mode_(nullptr)
{
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

void ListConvolveBase::setA(const AtomListView& lv)
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

void ListConvolveBase::setB(const AtomListView& lv)
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

void ListConvolveBase::onDataT(const MListAtom& ml)
{
    onList(ml->data());
}

void ListConvolveBase::onInlet(size_t n, const AtomListView& lv)
{
    if (n == 1)
        setB(lv);
}

void ListConvolveBase::onList(const AtomListView& lv)
{
    setA(lv);

    if (!calc())
        return;

    output();
}

void ListConvolveBase::onFloat(t_float f)
{
    l0_.resize(1);
    l0_[0] = f;

    if (!calc())
        return;

    output();
}

void ListConvolveBase::output()
{
    if (!mode_) {
        listTo(0, lout_);
        return;
    }

    using namespace ceammc::details;

    switch (crc32_hash(mode_->value()->s_name)) {
    case HASH_VALID: {

        const auto M = l0_.size();
        const auto N = l1_.size();
        const auto LRES = M + N - 1;
        const auto LOUT = std::max(M, N) - std::min(M, N) + 1;
        const auto OFF = (LRES - LOUT) / 2;

        listTo(0, lout_.view(OFF, LOUT));

    } break;
    case HASH_SAME: {
        const auto NS = l0_.size() + l1_.size() - 1;
        const auto N0 = std::max(l0_.size(), l1_.size());
        const auto off = NS - N0;
        listTo(0, lout_.view(off / 2, N0));
    } break;
    case HASH_FULL:
    default:
        listTo(0, lout_);
        break;
    }
}
