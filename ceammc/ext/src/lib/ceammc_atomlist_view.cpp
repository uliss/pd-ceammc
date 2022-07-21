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
#include "ceammc_log.h"
#include "ceammc_numeric.h"

#include <cmath>
#include <functional>

namespace ceammc {

AtomListView::AtomListView() noexcept
    : data_(nullptr)
    , n_(0)
{
}

AtomListView::AtomListView(const t_atom* a, size_t n) noexcept
    : data_(reinterpret_cast<const Atom*>(a))
    , n_(n)
{
}

AtomListView::AtomListView(const Atom* a, size_t n) noexcept
    : data_(a)
    , n_(n)
{
}

AtomListView::AtomListView(const Atom& a) noexcept
    : data_(&a)
    , n_(1)
{
}

AtomListView::AtomListView(const AtomList& l) noexcept
    : AtomListView(l.toPdData(), l.size())
{
}

void AtomListView::set(t_atom* a, size_t n)
{
    data_ = reinterpret_cast<Atom*>(a);
    n_ = n;
}

void AtomListView::set(const Atom& a)
{
    data_ = &a;
    n_ = 1;
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

    if (l.atoms_.data() == data_)
        return true;

    for (size_t i = 0; i < n_; i++) {
        if (data_[i] != l[i])
            return false;
    }

    return true;
}

bool AtomListView::operator==(t_float v) const
{
    return isFloat() && math::float_compare(asFloat(), v);
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

bool AtomListView::isBool() const
{
    static t_symbol* SYM_TRUE = gensym("true");
    static t_symbol* SYM_FALSE = gensym("false");

    return (isFloat() && (asFloat() == 1 || asFloat() == 0))
        || ((isSymbol() && (asSymbol() == SYM_TRUE || asSymbol() == SYM_FALSE)));
}

bool AtomListView::isInteger() const
{
    return isFloat() && math::is_integer(asFloat());
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

t_float AtomListView::floatGreaterThenAt(size_t pos, t_float min, t_float def) const
{
    if (pos >= n_)
        return def;
    else
        return at(pos).asFloatGreaterThen(min, def);
}

t_float AtomListView::floatGreaterEqualAt(size_t pos, t_float min, t_float def) const
{
    if (pos >= n_)
        return def;
    else
        return at(pos).asFloatGreaterEqual(min, def);
}

t_float AtomListView::floatLessThenAt(size_t pos, t_float max, t_float def) const
{
    if (pos >= n_)
        return def;
    else
        return at(pos).asFloatLessThen(max, def);
}

t_float AtomListView::floatLessEqualAt(size_t pos, t_float max, t_float def) const
{
    if (pos >= n_)
        return def;
    else
        return at(pos).asFloatLessEqual(max, def);
}

t_int AtomListView::intGreaterThenAt(size_t pos, t_int min, t_int def) const
{
    if (pos >= n_)
        return def;
    else
        return at(pos).asIntGreaterThen(min, def);
}

t_int AtomListView::intGreaterEqualAt(size_t pos, t_int min, t_int def) const
{
    if (pos >= n_)
        return def;
    else
        return at(pos).asIntGreaterEqual(min, def);
}

t_int AtomListView::intLessThenAt(size_t pos, t_int max, t_int def) const
{
    if (pos >= n_)
        return def;
    else
        return at(pos).asIntLessThen(max, def);
}

t_int AtomListView::intLessEqualAt(size_t pos, t_int max, t_int def) const
{
    if (pos >= n_)
        return def;
    else
        return at(pos).asIntLessEqual(max, def);
}

t_int AtomListView::intInClosedIntervalAt(size_t pos, t_int min, t_int max, t_int def) const
{
    if (pos >= n_)
        return def;
    else
        return at(pos).asIntInClosedInterval(min, max, def);
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

bool AtomListView::contains(const Atom& a) const
{
    if (empty() || isNull())
        return false;

    return std::find(data_, data_ + n_, a) != (data_ + n_);
}

bool AtomListView::allOf(std::function<bool(const Atom&)> pred) const
{
    return std::all_of(begin(), end(), pred);
}

bool AtomListView::anyOf(std::function<bool(const Atom&)> pred) const
{
    return std::any_of(begin(), end(), pred);
}

bool AtomListView::noneOf(std::function<bool(const Atom&)> pred) const
{
    return std::none_of(begin(), end(), pred);
}

MaybeFloat AtomListView::sum() const noexcept
{
    return reduceFloat(0, [](t_float a, t_float b) { return a + b; });
}

MaybeFloat AtomListView::product() const noexcept
{
    return reduceFloat(1, [](t_float a, t_float b) { return a * b; });
}

MaybeFloat AtomListView::reduceFloat(t_float init, std::function<t_float(t_float, t_float)> fn) const
{
    t_float accum = init;
    size_t n = 0;

    for (size_t i = 0; i < n_; i++) {
        auto& el = data_[i];
        if (el.isFloat()) {
            accum = fn(accum, el.asFloat());
            n++;
        }
    }

    return (n > 0) ? MaybeFloat(accum) : boost::none;
}

const Atom* AtomListView::relativeAt(int pos) const
{
    auto idx = relativeIndex<int>(pos, n_);
    if (idx < 0)
        return nullptr;

    return data_ + static_cast<size_t>(idx);
}

const Atom* AtomListView::clipAt(int pos) const
{
    if (n_ == 0)
        return nullptr;

    auto idx = static_cast<size_t>(clip<long>(pos, 0, long(n_ - 1)));
    return data_ + idx;
}

const Atom* AtomListView::wrapAt(int pos) const
{
    if (n_ == 0)
        return nullptr;

    return data_ + wrapInteger<long>(pos, n_);
}

const Atom* AtomListView::foldAt(int pos) const
{
    if (n_ == 0)
        return nullptr;

    return data_ + foldInteger<long>(pos, n_);
}

bool AtomListView::range(Atom& min, Atom& max) const noexcept
{
    if (!n_ || !data_)
        return false;

    auto res = std::minmax_element(begin(), end());
    min = *res.first;
    max = *res.second;
    return true;
}

std::ostream& operator<<(std::ostream& os, const AtomListView& l)
{
    os << "( ";
    for (size_t i = 0; i < l.size(); i++) {
        if (i != 0)
            os << " ";

        os << l.at(i);
    }

    os << " )";
    return os;
}

}
