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
#include "ceammc_crc32.h"
#include "ceammc_factory.h"
#include "datatype_mlist.h"

constexpr const char* STR_VALID = "valid";
constexpr const char* STR_SAME = "same";
constexpr const char* STR_FULL = "full";

constexpr auto HASH_VALID = "valid"_hash;
constexpr auto HASH_SAME = "same"_hash;
constexpr auto HASH_FULL = "full"_hash;

static_assert(check_crc32_unique(HASH_VALID, HASH_SAME, HASH_FULL), "");

ListConvolve::ListConvolve(const PdArgs& args)
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

    mode_ = new SymbolEnumProperty("@mode", { STR_FULL, STR_VALID, STR_SAME });
    addProperty(mode_);

    addProperty(new SymbolEnumAlias("@valid", mode_, gensym(STR_VALID)));
    addProperty(new SymbolEnumAlias("@same", mode_, gensym(STR_SAME)));
    addProperty(new SymbolEnumAlias("@full", mode_, gensym(STR_FULL)));
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

    output();
}

void ListConvolve::onFloat(t_float f)
{
    l0_.resize(1);
    l0_[0] = f;

    if (!convolve())
        return;

    output();
}

void ListConvolve::output()
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
