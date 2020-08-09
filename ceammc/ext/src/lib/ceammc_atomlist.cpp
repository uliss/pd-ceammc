/*****************************************************************************
 * Copyright 2016 Serge Poltavsky. All rights reserved.
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
#include "ceammc_atomlist.h"
#include "ceammc_convert.h"
#include "ceammc_format.h"
#include "ceammc_log.h"
#include "ceammc_output.h"

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <iterator>
#include <string>

#include "lex/quoted_atomlist_lexer.h"
#include "lex/quoted_string.parser.hpp"

namespace ceammc {

using ElementAccessFn = const Atom* (AtomList::*)(int)const;

AtomList::AtomList() noexcept
{
    static_assert(noexcept(Atom()), "Atom should be noexcept");
}

AtomList::AtomList(const AtomList& l)
    : atoms_(l.atoms_)
{
}

AtomList::AtomList(AtomList&& l) noexcept
    : atoms_(std::move(l.atoms_))
{
}

AtomList::AtomList(const Atom& a)
    : atoms_({ a })
{
}

AtomList::AtomList(size_t n, t_atom* lst)
{
    fromPdData(n, lst);
}

AtomList::AtomList(const AtomListView& v)
{
    if (v.isNull() || v.empty())
        return;

    for (auto& a : v)
        atoms_.push_back(a);
}

AtomList::AtomList(int n, t_atom* lst)
{
    fromPdData(n, lst);
}

AtomList::AtomList(std::initializer_list<t_float> l)
    : atoms_(l.begin(), l.end())
{
}

AtomList::AtomList(std::initializer_list<Atom> l)
    : atoms_(l.begin(), l.end())
{
}

void AtomList::operator=(const Atom& a)
{
    atoms_.resize(1);
    atoms_.front() = a;
}

void AtomList::operator=(const AtomList& l)
{
    if (this != &l)
        atoms_ = l.atoms_;
}

void AtomList::operator=(const AtomListView& l)
{
    atoms_.clear();
    atoms_.reserve(l.size());

    if(l.isNull() || l.empty())
        return;

    for (auto& a : l)
        atoms_.push_back(a);
}

void AtomList::operator=(AtomList&& l) noexcept
{
    if (this != &l)
        atoms_ = std::move(l.atoms_);
}

void AtomList::reserve(size_t n)
{
    atoms_.reserve(n);
}

Atom* AtomList::relativeAt(int pos)
{
    auto idx = relativeIndex<int>(pos, atoms_.size());
    if (idx < 0)
        return nullptr;

    return &atoms_[static_cast<size_t>(idx)];
}

const Atom* AtomList::relativeAt(int pos) const
{
    return const_cast<AtomList*>(this)->relativeAt(pos);
}

Atom* AtomList::clipAt(int pos)
{
    if (atoms_.empty())
        return nullptr;

    const size_t N = atoms_.size();
    auto idx = static_cast<size_t>(clip<long>(pos, 0, long(N - 1)));
    return &atoms_[idx];
}

const Atom* AtomList::clipAt(int pos) const
{
    return const_cast<AtomList*>(this)->clipAt(pos);
}

Atom* AtomList::wrapAt(int pos)
{
    if (atoms_.empty())
        return nullptr;

    return &atoms_[wrapInteger<long>(pos, atoms_.size())];
}

const Atom* AtomList::wrapAt(int pos) const
{
    return const_cast<AtomList*>(this)->wrapAt(pos);
}

Atom* AtomList::foldAt(int pos)
{
    auto N = atoms_.size();
    if (N == 0)
        return nullptr;

    return &atoms_[foldInteger<long>(pos, N)];
}

const Atom* AtomList::foldAt(int pos) const
{
    return const_cast<AtomList*>(this)->foldAt(pos);
}

bool AtomList::boolAt(size_t pos, bool def) const noexcept
{
    if (pos >= atoms_.size())
        return def;

    return atoms_[pos].asBool(def);
}

int AtomList::intAt(size_t pos, int def) const noexcept
{
    if (pos >= atoms_.size())
        return def;

    return atoms_[pos].asInt(def);
}

t_float AtomList::floatAt(size_t pos, t_float def) const noexcept
{
    if (pos >= atoms_.size())
        return def;

    return atoms_[pos].asFloat(def);
}

t_symbol* AtomList::symbolAt(size_t pos, t_symbol* def) const noexcept
{
    if (pos >= atoms_.size())
        return def;

    t_symbol* res = def;
    atoms_[pos].getSymbol(&res);
    return res;
}

void AtomList::resizePad(size_t n, const Atom& v)
{
    atoms_.resize(n, v);
}

void AtomList::resizeClip(size_t n)
{
    if (empty())
        return;

    resizePad(n, atoms_.back());
}

void AtomList::resizeWrap(size_t n)
{
    if (empty())
        return;

    if (n < size())
        return atoms_.resize(n);

    atoms_.reserve(n);
    const size_t old_size = size();
    for (size_t i = old_size; i < n; i++)
        atoms_.push_back(atoms_[i % old_size]);
}

void AtomList::resizeFold(size_t n)
{
    if (empty())
        return;

    if (n < size())
        return atoms_.resize(n);

    atoms_.reserve(n);
    const size_t old_size = size();
    const size_t fold_size = (old_size - 1) * 2;

    // check for division by zero if single element in list
    if (fold_size == 0)
        return resizeClip(n);

    for (size_t i = old_size; i < n; i++) {
        size_t wrap = i % fold_size;
        atoms_.push_back(atoms_[std::min(wrap, fold_size - wrap)]);
    }
}

bool AtomList::property(t_symbol* name, Atom* dest) const noexcept
{
    if (!dest)
        return false;

    for (size_t i = 0; i < atoms_.size(); i++) {
        if (!atoms_[i].isProperty())
            continue;

        // found
        if (name == atoms_[i].asSymbol()) {
            if (i < (atoms_.size() - 1)) {
                // if next property
                if (atoms_[i + 1].isProperty())
                    return false;

                *dest = atoms_[i + 1];
                return true;
            } else { // last element is list
                return false;
            }
        }
    }

    return false;
}

bool AtomList::property(t_symbol* name, AtomList* dest) const
{
    if (!dest)
        return false;

    bool found = false;
    AtomList res;

    for (size_t i = 0; i < atoms_.size(); i++) {
        if (atoms_[i].isProperty()) {
            // if next property found
            if (found)
                break;

            // prop found
            if (name == atoms_[i].asSymbol())
                found = true;
        } else {
            // value
            if (found)
                res.append(atoms_[i]);
        }
    }

    if (found)
        *dest = res;

    return found;
}

std::deque<AtomList> AtomList::properties() const
{
    std::deque<AtomList> res;
    for (size_t i = 0; i < atoms_.size(); i++) {
        if (atoms_[i].isProperty()) {
            res.push_back(AtomList());
        }

        if (res.empty())
            continue;

        res.back().append(atoms_[i]);
    }

    return res;
}

bool AtomList::hasProperty(t_symbol* name) const noexcept
{
    for (auto& a : atoms_) {
        if (name == a.asSymbol())
            return true;
    }

    return false;
}

AtomList AtomList::mapFloat(const FloatMapFunction& fn, AtomListMapType t) const
{
    if (t == AtomListMapType::KEEP) {
        AtomList res(*this);

        for (auto& a : res.atoms_)
            a.applyFloat(fn);

        return res;
    } else {
        AtomList res;
        res.reserve(size());

        for (auto& a : atoms_) {
            if (a.isFloat())
                res.atoms_.emplace_back(fn(a.asFloat()));
        }

        return res;
    }
}

AtomList AtomList::mapSymbol(const SymbolMapFunction& fn, AtomListMapType t) const
{
    if (t == AtomListMapType::KEEP) {
        AtomList res(*this);

        for (auto& a : res.atoms_)
            a.applySymbol(fn);

        return res;
    } else {
        AtomList res;
        res.reserve(size());

        for (auto& a : atoms_) {
            if (a.isSymbol())
                res.atoms_.emplace_back(fn(a.asSymbol()));
        }

        return res;
    }
}

AtomList AtomList::map(const AtomMapFunction& fn) const
{
    AtomList res(*this);

    for (auto& a : res.atoms_)
        a = fn(a);

    return res;
}

static size_t normalizeIdx(int idx, size_t N, bool clip)
{
    assert(N > 0);

    const auto last_idx = N - 1;
    const bool is_negative = idx < 0;

    size_t abs_idx = is_negative ? size_t((-idx) - 1) : size_t(idx);

    if (clip)
        abs_idx = std::min<size_t>(abs_idx, last_idx);

    return is_negative ? (last_idx - abs_idx) : abs_idx;
}

AtomList AtomList::slice(int start, int end, size_t step) const
{
    AtomList res;

    if (step < 1 || atoms_.empty())
        return res;

    const size_t N = atoms_.size();

    if (start >= static_cast<int>(N))
        return res;

    const size_t nfirst = normalizeIdx(start, N, false);
    if (nfirst >= N)
        return AtomList();

    const size_t last = normalizeIdx(end, N, true);

    if (nfirst <= last) {
        for (size_t i = nfirst; i <= last; i += step)
            res.append(atoms_[i]);
    } else {
        for (long i = static_cast<long>(nfirst); i >= static_cast<long>(last); i -= step)
            res.append(atoms_[static_cast<size_t>(i)]);
    }

    return res;
}

void AtomList::fromPdData(size_t n, t_atom* lst)
{
    atoms_.clear();
    atoms_.reserve(n);
    for (size_t i = 0; i < n; i++) {
        atoms_.push_back(lst[i]);
    }
}

void AtomList::fromPdData(int n, t_atom* lst)
{
    fromPdData(static_cast<size_t>(n), lst);
}

void AtomList::append(const AtomList& l)
{
    atoms_.insert(atoms_.end(), l.atoms_.begin(), l.atoms_.end());
}

bool AtomList::insert(size_t pos, const Atom& a)
{
    if (pos > atoms_.size())
        return false;

    atoms_.insert(atoms_.begin() + static_cast<long>(pos), a);
    return true;
}

bool AtomList::insert(size_t pos, const AtomList& l)
{
    if (pos > atoms_.size())
        return false;
    atoms_.insert(atoms_.begin() + static_cast<long>(pos), l.atoms_.begin(), l.atoms_.end());
    return true;
}

bool AtomList::remove(size_t pos)
{
    if (pos >= size())
        return false;

    atoms_.erase(atoms_.begin() + static_cast<long>(pos));
    return true;
}

void AtomList::removeAll(const Atom& a)
{
    auto nend = std::remove(atoms_.begin(), atoms_.end(), a);
    atoms_.erase(nend, atoms_.end());
}

void AtomList::removeAll(AtomPredicate pred)
{
    auto nend = std::remove_if(atoms_.begin(), atoms_.end(), pred);
    atoms_.erase(nend, atoms_.end());
}

void AtomList::replaceAll(const Atom& old_value, const Atom& new_value)
{
    std::replace(atoms_.begin(), atoms_.end(), old_value, new_value);
}

void AtomList::replaceAll(AtomPredicate pred, const Atom& new_value)
{
    std::replace_if(atoms_.begin(), atoms_.end(), pred, new_value);
}

Atom* AtomList::first()
{
    if (empty())
        return nullptr;

    return &atoms_.front();
}

Atom* AtomList::last()
{
    if (empty())
        return nullptr;

    return &atoms_.back();
}

const Atom* AtomList::first() const
{
    return const_cast<AtomList*>(this)->first();
}

const Atom* AtomList::last() const
{
    return const_cast<AtomList*>(this)->last();
}

void AtomList::fill(const Atom& a)
{
    std::fill(atoms_.begin(), atoms_.end(), a);
}

void AtomList::fill(const Atom& a, size_t sz)
{
    atoms_ = Container(sz, a);
}

void AtomList::sort()
{
    std::sort(atoms_.begin(), atoms_.end());
}

void AtomList::shuffle()
{
    std::random_shuffle(atoms_.begin(), atoms_.end());
}

void AtomList::reverse()
{
    std::reverse(atoms_.begin(), atoms_.end());
}

Atom* AtomList::min()
{
    auto it = std::min_element(atoms_.begin(), atoms_.end());
    if (it == atoms_.end())
        return nullptr;

    return &(*it);
}

const Atom* AtomList::min() const
{
    return const_cast<AtomList*>(this)->min();
}

Atom* AtomList::max()
{
    if (empty())
        return nullptr;

    return &(*std::max_element(atoms_.begin(), atoms_.end()));
}

MaybeFloat AtomList::sum() const noexcept
{
    return reduceFloat(0, [](t_float a, t_float b) { return a + b; });
}

MaybeFloat AtomList::product() const noexcept
{
    return reduceFloat(1, [](t_float a, t_float b) { return a * b; });
}

bool AtomList::contains(const Atom& a) const noexcept
{
    return std::find(atoms_.begin(), atoms_.end(), a) != atoms_.end();
}

bool AtomList::contains(const AtomList& sublist) const noexcept
{
    auto it = std::search(atoms_.begin(), atoms_.end(), sublist.begin(), sublist.end());
    return it != atoms_.end();
}

long AtomList::findPos(const Atom& a) const noexcept
{
    auto it = std::find(atoms_.begin(), atoms_.end(), a);
    if (it == atoms_.end())
        return -1;

    return std::distance(atoms_.begin(), it);
}

long AtomList::findPos(AtomPredicate pred) const noexcept
{
    auto it = std::find_if(atoms_.begin(), atoms_.end(), pred);
    if (it == atoms_.end())
        return -1;

    return std::distance(atoms_.begin(), it);
}

size_t AtomList::count(const Atom& a) const noexcept
{
    return static_cast<size_t>(std::count(atoms_.begin(), atoms_.end(), a));
}

size_t AtomList::count(AtomPredicate pred) const noexcept
{
    return static_cast<size_t>(std::count_if(atoms_.begin(), atoms_.end(), pred));
}

bool AtomList::allOf(AtomPredicate pred) const noexcept
{
    if (empty())
        return false;

    return std::all_of(atoms_.begin(), atoms_.end(), pred);
}

bool AtomList::anyOf(AtomPredicate pred) const noexcept
{
    return std::any_of(atoms_.begin(), atoms_.end(), pred);
}

bool AtomList::noneOf(AtomPredicate pred) const noexcept
{
    return std::none_of(atoms_.begin(), atoms_.end(), pred);
}

const Atom* AtomList::max() const
{
    return const_cast<AtomList*>(this)->max();
}

bool AtomList::range(Atom& min, Atom& max) const noexcept
{
    if (empty())
        return false;

    auto res = std::minmax_element(atoms_.begin(), atoms_.end());
    min = *res.first;
    max = *res.second;
    return true;
}

AtomListView AtomList::view(size_t from) const
{
    if (from >= size())
        return {};
    else
        return AtomListView(toPdData() + from, atoms_.size() - from);
}

AtomListView AtomList::view(size_t from, size_t length) const
{
    if (from >= size())
        return {};
    else {
        auto len = std::min(length, atoms_.size() - from);
        return AtomListView(toPdData() + from, len);
    }
}

static AtomList listAdd(const AtomList& a, const AtomList& b, ElementAccessFn fn)
{
    AtomList res;
    const size_t N = std::min(a.size(), b.size());
    // protect agains empty list
    if (N == 0)
        return res;

    const size_t sz = std::max(a.size(), b.size());

    for (size_t i = 0; i < sz; i++) {
        auto x = (a.*fn)(static_cast<int>(i))->asFloat();
        auto y = (b.*fn)(static_cast<int>(i))->asFloat();
        res.append(Atom(x + y));
    }

    return res;
}

AtomList AtomList::add(const AtomList& a, const AtomList& b, NonEqualLengthBehaivor lb)
{
    switch (lb) {
    case MINSIZE: {
        const size_t N = std::min(a.size(), b.size());

        AtomList res;
        res.atoms_.reserve(N);

        for (size_t i = 0; i < N; i++)
            res.append(Atom(a[i].asFloat() + b[i].asFloat()));

        return res;
    }
    case PADZERO: {
        const size_t N = std::max(a.size(), b.size());
        AtomList l1(a);
        AtomList l2(b);
        l1.resizePad(N, Atom(0.));
        l2.resizePad(N, Atom(0.));
        return add(l1, l2, MINSIZE);
    }
    case CLIP:
        return listAdd(a, b, &AtomList::clipAt);
    case WRAP:
        return listAdd(a, b, &AtomList::wrapAt);
    case FOLD:
        return listAdd(a, b, &AtomList::foldAt);
    default:
        return a;
    }
}

AtomList AtomList::zeroes(size_t n)
{
    return filled(Atom(t_float(0)), n);
}

AtomList AtomList::ones(size_t n)
{
    return filled(Atom(1), n);
}

AtomList AtomList::filled(const Atom& a, size_t n)
{
    AtomList res;
    res.fill(a, n);
    return res;
}

static AtomList listSubstract(const AtomList& a, const AtomList& b, ElementAccessFn fn)
{
    AtomList res;
    const size_t N = std::min(a.size(), b.size());
    // protect agains empty list
    if (N == 0)
        return res;

    const size_t sz = std::max(a.size(), b.size());

    for (size_t i = 0; i < sz; i++) {
        auto f = (a.*fn)(static_cast<int>(i))->asFloat() - (b.*fn)(static_cast<int>(i))->asFloat();
        res.append(Atom(f));
    }

    return res;
}

AtomList AtomList::sub(const AtomList& a, const AtomList& b, AtomList::NonEqualLengthBehaivor lb)
{
    switch (lb) {
    case MINSIZE: {
        AtomList res;
        const size_t N = std::min(a.size(), b.size());
        res.atoms_.reserve(N);

        for (size_t i = 0; i < N; i++)
            res.append(Atom(a[i].asFloat() - b[i].asFloat()));

        return res;
    }
    case PADZERO: {
        const size_t N = std::max(a.size(), b.size());
        AtomList l1(a);
        AtomList l2(b);
        l1.resizePad(N, Atom(0.));
        l2.resizePad(N, Atom(0.));
        return sub(l1, l2, MINSIZE);
    }
    case CLIP:
        return listSubstract(a, b, &AtomList::clipAt);
    case WRAP:
        return listSubstract(a, b, &AtomList::wrapAt);
    case FOLD:
        return listSubstract(a, b, &AtomList::foldAt);
    default:
        return a;
    }
}

AtomList& AtomList::operator+=(t_float v) noexcept
{
    for (auto& a : atoms_)
        a += v;

    return *this;
}

AtomList& AtomList::operator-=(t_float v) noexcept
{
    for (auto& a : atoms_)
        a -= v;

    return *this;
}

AtomList& AtomList::operator*=(t_float v) noexcept
{
    for (auto& a : atoms_)
        a *= v;

    return *this;
}

AtomList& AtomList::operator/=(t_float v) noexcept
{
    if (std::equal_to<t_float>()(v, 0)) {
        LIB_ERR << "division by zero";
        return *this;
    }

    for (auto& a : atoms_)
        a /= v;

    return *this;
}

AtomList AtomList::operator+(t_float v) const
{
    AtomList res(*this);
    res += v;
    return res;
}

AtomList AtomList::operator-(t_float v) const
{
    AtomList res(*this);
    res -= v;
    return res;
}

AtomList AtomList::operator*(t_float v) const
{
    AtomList res(*this);
    res *= v;
    return res;
}

AtomList AtomList::operator/(t_float v) const
{
    AtomList res(*this);
    res /= v;
    return res;
}

bool AtomList::operator==(const Atom& x) const noexcept
{
    return atoms_.size() == 1 && atoms_.front() == x;
}

bool AtomList::operator==(const AtomList& x) const noexcept
{
    return atoms_ == x.atoms_;
}

bool AtomList::operator==(const AtomListView& x) const noexcept
{
    if (size() != x.size())
        return false;

    if (atoms_.data() == x.data_)
        return true;

    for (size_t i = 0; i < atoms_.size(); i++) {
        if (atoms_[i] != x.data_[i])
            return false;
    }

    return true;
}

AtomList AtomList::parseQuoted(bool quote_properties) const
{
    return view().parseQuoted(quote_properties);
}

AtomList AtomList::parseString(const char* str)
{
    t_binbuf* b = binbuf_new();
    binbuf_text(b, str, strlen(str));
    auto n = binbuf_getnatom(b);
    auto a = binbuf_getvec(b);
    AtomList res(n, a);
    binbuf_free(b);
    return res;
}

std::ostream& operator<<(std::ostream& os, const AtomList& l)
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

AtomList listFrom(const std::vector<std::string>& v)
{
    AtomList res;
    res.reserve(v.size());

    for (auto& s : v)
        res.append(atomFrom(s));

    return res;
}

AtomList listFrom(const std::string& v)
{
    AtomList res;
    res.append(atomFrom(v));
    return res;
}

AtomList listFrom(const AtomList& v)
{
    return v;
}

AtomList listFrom(bool v)
{
    constexpr t_float FALSE = 0;
    constexpr t_float TRUE = 1;

    return Atom(v ? TRUE : FALSE);
}

AtomList operator+(const AtomList& l1, const AtomList& l2)
{
    AtomList res(l1);
    res.append(l2);
    return res;
}

AtomList operator+(const AtomList& l, const Atom& a)
{
    AtomList res(l);
    res.append(a);
    return res;
}

AtomList operator+(const Atom& a, const AtomList& l)
{
    AtomList res;
    res.reserve(l.size() + 1);
    res.append(a);
    res.append(l);
    return res;
}

AtomList AtomList::filtered(const AtomPredicate& pred) const
{
    if (!pred)
        return *this;

    AtomList res;
    res.atoms_.reserve(atoms_.size());

    for (auto& el : atoms_) {
        if (pred(el))
            res.atoms_.push_back(el);
    }

    return res;
}

AtomList AtomList::filteredFloat(const FloatPredicate& pred) const
{
    if (!pred)
        return *this;

    AtomList res;
    res.atoms_.reserve(atoms_.size());

    for (auto& a : atoms_) {
        if (a.isFloat() && pred(a.asFloat()))
            res.atoms_.push_back(a);
    }

    return res;
}

AtomList AtomList::filteredSymbol(const SymbolPredicate& pred) const
{
    if (!pred)
        return *this;

    AtomList res;
    res.atoms_.reserve(atoms_.size());

    for (auto& a : atoms_) {
        if (a.isSymbol() && pred(a.asSymbol()))
            res.atoms_.push_back(a);
    }

    return res;
}

MaybeFloat AtomList::reduceFloat(t_float init, std::function<t_float(t_float, t_float)> fn) const
{
    t_float accum = init;
    size_t n = 0;

    for (auto& el : atoms_) {
        if (el.isFloat()) {
            accum = fn(accum, el.asFloat());
            n++;
        }
    }

    return (n > 0) ? MaybeFloat(accum) : boost::none;
}

} // namespace ceammc
