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
#include "ceammc_crc32.h"
#include "ceammc_factory.h"
#include "datatype_mlist.h"

constexpr const char* STR_VALID = "valid";
constexpr const char* STR_SAME = "same";
constexpr const char* STR_FULL = "full";

constexpr auto HASH_VALID = "valid"_hash;
constexpr auto HASH_SAME = "same"_hash;
constexpr auto HASH_FULL = "full"_hash;

constexpr bool not_eq3(uint32_t a, uint32_t b, uint32_t c)
{
    return a != b && a != c && b != c;
}

static_assert(not_eq3(HASH_VALID, HASH_SAME, HASH_FULL), "");

ListCorrelate::ListCorrelate(const PdArgs& args)
    : BaseObject(args)
    , mode_(nullptr)
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

    mode_ = new SymbolEnumProperty("@mode", { STR_VALID, STR_SAME, STR_FULL });
    addProperty(mode_);

    addProperty(new SymbolEnumAlias("@valid", mode_, gensym(STR_VALID)));
    addProperty(new SymbolEnumAlias("@same", mode_, gensym(STR_SAME)));
    addProperty(new SymbolEnumAlias("@full", mode_, gensym(STR_FULL)));
}

void ListCorrelate::onInlet(size_t n, const AtomListView& lv)
{
    if (n == 1)
        setB(lv);
}

void ListCorrelate::onList(const AtomList& lst)
{
    setA(lst.view());
    if (!calc())
        return;

    output();
}

void ListCorrelate::onFloat(t_float f)
{
    l0_.resize(1);
    l0_[0] = f;

    if (!calc())
        return;

    output();
}

void ListCorrelate::onDataT(const MListAtom& ml)
{
    onList(ml->data());
}

bool ListCorrelate::calc()
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

void ListCorrelate::output()
{
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

void ListCorrelate::setA(const AtomListView& lv)
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

void ListCorrelate::setB(const AtomListView& lv)
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

void setup_list_crosscorr()
{
    ObjectFactory<ListCorrelate> obj("list.correlate");
    obj.processData<DataTypeMList>();

    obj.setXletsInfo(
        { "list: set first cross-correlation arg then calculate",
            "list: set second cross-correlation arg" },
        { "list: cross-correlation result" });
}
