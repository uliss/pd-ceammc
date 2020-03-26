/*****************************************************************************
 * Copyright 2020 Serge Poltavsky. All rights reserved.
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
#include "ceammc_atomlist_view.h"
#include "ceammc_atomlist.h"
#include "ceammc_convert.h"

#include <cmath>
#include <functional>

namespace ceammc {

AtomListView::AtomListView()
    : AtomListView(nullptr, 0)
{
}

AtomListView::AtomListView(const t_atom* a, size_t n)
    : data_(reinterpret_cast<const Atom*>(a))
    , n_(n)
{
}

AtomListView::AtomListView(const AtomList& l)
    : AtomListView(l.toPdData(), l.size())
{
}

void AtomListView::set(t_atom* a, size_t n)
{
    data_ = reinterpret_cast<Atom*>(a);
    n_ = n;
}

void AtomListView::set(const AtomList& l)
{
    set(l.toPdData(), l.size());
}

bool AtomListView::get(t_atom*& a, int& n)
{
    if (!data_)
        return false;

    a = const_cast<t_atom*>(&data_->atom());
    n = static_cast<int>(n_);
    return true;
}

bool AtomListView::operator==(const AtomList& l) const
{
    if (n_ != l.size())
        return false;

    for (size_t i = 0; i < n_; i++) {
        if (data_[i] != l[i])
            return false;
    }

    return true;
}

bool AtomListView::operator==(const AtomListView& v) const
{
    if (v.n_ != n_)
        return false;

    if (v.data_ == data_)
        return true;

    if (data_ == nullptr)
        return true;

    for (size_t i = 0; i < n_; i++) {
        if (data_[i] != v.data_[i])
            return false;
    }

    return true;
}

AtomListView::operator AtomList() const
{
    AtomList res;
    res.reserve(n_);
    for (auto& a : *this)
        res.append(a);

    return res;
}

bool AtomListView::isBool() const
{
    static t_symbol* SYM_TRUE = gensym("true");
    static t_symbol* SYM_FALSE = gensym("false");

    return (isFloat() && (asFloat() == 1 || asFloat() == 0))
        || ((isSymbol() && (asSymbol() == SYM_TRUE || asSymbol() == SYM_FALSE)));
}

bool AtomListView::isInt() const
{
    return isFloat() && std::equal_to<t_float>()(std::ceil(asFloat()), asFloat());
}

const Atom& AtomListView::relativeAt(long pos) const
{
    return at(relativeIndex(pos, n_));
}

bool AtomListView::boolAt(size_t pos, bool def) const
{
    if (pos >= n_)
        return def;
    else
        return at(pos).asBool(def);
}

t_float AtomListView::floatAt(size_t pos, t_float def) const
{
    if (pos >= n_)
        return def;
    else
        return at(pos).asFloat(def);
}

int AtomListView::intAt(size_t pos, int def) const
{
    if (pos >= n_)
        return def;
    else
        return at(pos).asInt(def);
}

t_symbol* AtomListView::symbolAt(size_t pos, t_symbol* def) const
{
    if (pos >= n_)
        return def;
    else if (at(pos).isSymbol())
        return at(pos).asSymbol();
    else
        return def;
}

AtomListView AtomListView::subView(size_t from) const
{
    return subView(from, n_);
}

AtomListView AtomListView::subView(size_t from, size_t len) const
{
    if (!data_)
        return {};

    if (from >= n_)
        return {};

    return AtomListView(&data_[from].atom(), std::min(n_ - from, len));
}

}
