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
#include "list_xat.h"
#include "ceammc_convert.h"
#include "ceammc_factory.h"
#include "ceammc_property_enum.h"
#include "datatype_mlist.h"

static t_symbol* SYM_REL;
static t_symbol* SYM_CLIP;
static t_symbol* SYM_WRAP;
static t_symbol* SYM_FOLD;

ListXAt::ListXAt(const PdArgs& args)
    : BaseObject(args)
    , def_(nullptr)
    , at_method_(nullptr)
{
    createInlet();
    createOutlet();

    createCbListProperty(
        "@value",
        [this]() -> AtomList { return list_; },
        [this](const AtomList& l) -> bool { list_ = l; return true; })
        ->setArgIndex(0);

    def_ = new AtomProperty("@default", Atom());
    addProperty(def_);

    at_method_ = new SymbolEnumProperty("@method", { SYM_REL, SYM_CLIP, SYM_WRAP, SYM_FOLD });
    addProperty(at_method_);

    addProperty(new SymbolEnumAlias("@rel", at_method_, SYM_REL));
    addProperty(new SymbolEnumAlias("@clip", at_method_, SYM_CLIP));
    addProperty(new SymbolEnumAlias("@wrap", at_method_, SYM_WRAP));
    addProperty(new SymbolEnumAlias("@fold", at_method_, SYM_FOLD));
}

void ListXAt::onFloat(t_float f)
{
    auto atom = at(int(f));

    if (atom)
        atomTo(0, *atom);
    else if (!def_->value().isNone())
        atomTo(0, def_->value());
    else
        OBJ_ERR << "element not found at: " << f;
}

void ListXAt::onList(const AtomList& lst)
{
    AtomList res;
    res.reserve(lst.size());

    for (auto& el : lst) {
        auto atom = at(el.asInt(std::numeric_limits<int>::max()));

        if (atom)
            res.append(*atom);
        else if (!def_->value().isNone())
            res.append(def_->value());
        else
            OBJ_ERR << "element not found at: " << el;
    }

    listTo(0, res);
}

void ListXAt::onInlet(size_t n, const AtomList& lst)
{
    if (lst.isData() && !lst.isA<DataTypeMList>()) {
        OBJ_ERR << "invalid datatype. Only data.mlist is supported!";
        return;
    }

    list_ = lst;
}

const Atom* ListXAt::at(int pos) const
{
    if (list_.isA<DataTypeMList>()) {
        auto mlist = list_.asD<DataTypeMList>();

        const size_t N = mlist->size();
        if (N == 0)
            return nullptr;

        const auto m = at_method_->value();
        if (m == SYM_CLIP)
            return &mlist->at(clip<int>(pos, 0, N - 1));
        else if (m == SYM_WRAP)
            return &mlist->at(wrapInteger<int>(pos, N));
        else if (m == SYM_FOLD)
            return &mlist->at(foldInteger<int>(pos, N));
        else {
            pos = relativeIndex<int>(pos, N);
            if (pos < 0)
                return nullptr;

            return &mlist->at(pos);
        }

    } else {
        const size_t N = list_.size();
        if (N == 0)
            return nullptr;

        const auto m = at_method_->value();
        if (m == SYM_CLIP)
            return &list_[clip<int>(pos, 0, N - 1)];
        else if (m == SYM_WRAP)
            return &list_[wrapInteger<int>(pos, N)];
        else if (m == SYM_FOLD)
            return &list_[foldInteger<int>(pos, N)];
        else {
            pos = relativeIndex<int>(pos, N);
            if (pos < 0)
                return nullptr;

            return &list_[pos];
        }
    }
}

void setup_list_xat()
{
    SYM_REL = gensym("rel");
    SYM_CLIP = gensym("clip");
    SYM_WRAP = gensym("wrap");
    SYM_FOLD = gensym("fold");

    ObjectFactory<ListXAt> obj("list.^at");

    obj.setDescription("on input index(es) outputs list element(s)");
    obj.addAuthor("Serge Poltavsky");
    obj.setKeywords({ "list", "at" });
    obj.setCategory("list");
    obj.setSinceVersion(0, 6);

    ListXAt::setInletsInfo(obj.classPointer(), { "int:  index\n"
                                                 "list: list of indexes",
                                                   "list: set value" });
    ListXAt::setOutletsInfo(obj.classPointer(), { "list" });
}
