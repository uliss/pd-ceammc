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
#include "ceammc_log.h"
#include <algorithm>
#include <cassert>
#include <cstdarg>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <string>

namespace ceammc {

typedef const Atom* (AtomList::*ElementAccessFn)(int)const;

bool AtomList::getRelativeIdx(int pos, size_t* idx) const
{
    return calc_rel_idx(pos, idx, atoms_.size());
}

bool AtomList::calc_rel_idx(int pos, size_t* dest, size_t sz)
{
    if (sz == 0)
        return false;

    if (dest == 0)
        return false;

    const int isz = static_cast<int>(sz);
    if (abs(pos) > isz)
        return false;

    if (pos < 0)
        pos += isz;

    if (pos < 0)
        return false;

    if (pos >= isz)
        return false;

    *dest = static_cast<size_t>(pos);
    return true;
}

AtomList::AtomList()
{
}

AtomList::AtomList(const Atom& a)
{
    append(a);
}

AtomList::AtomList(const Atom& a, const Atom& b)
{
    append(a);
    append(b);
}

AtomList::AtomList(size_t n, t_atom* lst)
{
    fromPdData(n, lst);
}

AtomList::AtomList(int n, t_atom* lst)
{
    fromPdData(n, lst);
}

size_t AtomList::size() const
{
    return atoms_.size();
}

void AtomList::reserve(size_t n)
{
    atoms_.reserve(n);
}

bool AtomList::empty() const
{
    return atoms_.empty();
}

Atom& AtomList::at(size_t pos)
{
    return atoms_.at(pos);
}

const Atom& AtomList::at(size_t pos) const
{
    return atoms_.at(pos);
}

Atom& AtomList::operator[](size_t pos)
{
    return atoms_.at(pos);
}

const Atom& AtomList::operator[](size_t pos) const
{
    return atoms_.at(pos);
}

Atom* AtomList::relativeAt(int pos)
{
    size_t idx;
    if (!getRelativeIdx(pos, &idx))
        return 0;

    return &atoms_[static_cast<size_t>(idx)];
}

const Atom* AtomList::relativeAt(int pos) const
{
    return const_cast<AtomList*>(this)->relativeAt(pos);
}

Atom* AtomList::clipAt(int pos)
{
    if (empty())
        return 0;

    const int sz = static_cast<int>(size()) - 1;
    int idx = std::min(std::max(pos, 0), sz);

    return &at(idx);
}

const Atom* AtomList::clipAt(int pos) const
{
    return const_cast<AtomList*>(this)->clipAt(pos);
}

Atom* AtomList::wrapAt(int pos)
{
    if (empty())
        return 0;

    const int sz = static_cast<int>(size());
    int idx = pos % sz;
    if (idx < 0)
        idx += sz;

    return &at(idx);
}

const Atom* AtomList::wrapAt(int pos) const
{
    return const_cast<AtomList*>(this)->wrapAt(pos);
}

Atom* AtomList::foldAt(int pos)
{
    if (empty())
        return 0;

    if (size() == 1)
        return first();

    const size_t a = size() - 1;
    const size_t b = static_cast<size_t>(abs(pos)) % (a * 2);
    return &at(std::min(b, a * 2 - b));
}

const Atom* AtomList::foldAt(int pos) const
{
    return const_cast<AtomList*>(this)->foldAt(pos);
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

bool AtomList::property(const std::string& name, Atom* dest) const
{
    if (!dest)
        return false;

    for (size_t i = 0; i < atoms_.size(); i++) {
        if (!atoms_[i].isProperty())
            continue;

        // found
        if (name == atoms_[i].asSymbol()->s_name) {
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

bool AtomList::property(const std::string& name, AtomList* dest) const
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
            if (name == atoms_[i].asSymbol()->s_name)
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

bool AtomList::hasProperty(const std::string& name) const
{
    for (size_t i = 0; i < atoms_.size(); i++) {
        if (!atoms_[i].isProperty())
            continue;

        if (name == atoms_[i].asSymbol()->s_name)
            return true;
    }
    return false;
}

AtomList AtomList::map(AtomFloatMapFunction f) const
{
    AtomList res(*this);

    for (size_t i = 0; i < res.size(); i++)
        res[i].apply(f);

    return res;
}

AtomList AtomList::map(AtomSymbolMapFunction f) const
{
    AtomList res(*this);

    for (size_t i = 0; i < res.size(); i++)
        res[i].apply(f);

    return res;
}

AtomList AtomList::map(AtomMapFunction f) const
{
    AtomList res(*this);

    for (size_t i = 0; i < res.size(); i++)
        res[i] = f(res[i]);

    return res;
}

AtomList AtomList::slice(int start) const
{
    if (start >= static_cast<int>(size()))
        return AtomList();

    // lower bound
    start = std::max(start, -static_cast<int>(size()));
    size_t pos = 0;
    getRelativeIdx(start, &pos);

    AtomList res;
    res.atoms_.reserve(atoms_.size() - pos);
    std::copy(atoms_.begin() + pos, atoms_.end(), std::back_inserter(res.atoms_));
    return res;
}

AtomList AtomList::slice(int start, int end, size_t step) const
{
    if (step < 1)
        return AtomList();

    if (empty())
        return AtomList();

    if (start >= static_cast<int>(size()))
        return AtomList();

    // lower bound
    start = std::max(start, -static_cast<int>(size()));
    end = std::max(end, -static_cast<int>(size()));
    end = std::min(end, static_cast<int>(size()));

    size_t start_pos = 0;
    getRelativeIdx(start, &start_pos);

    if (end < 0)
        end += size();

    if (start_pos >= end)
        return AtomList();

    AtomList res;
    for (size_t i = start_pos; i < end; i += step)
        res.atoms_.push_back(atoms_[i]);

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

t_atom* AtomList::toPdData() const
{
    return reinterpret_cast<t_atom*>(const_cast<Atom*>(atoms_.data()));
}

void AtomList::append(const Atom& a)
{
    atoms_.push_back(a);
}

void AtomList::append(const AtomList& l)
{
    atoms_.insert(atoms_.end(), l.atoms_.begin(), l.atoms_.end());
}

bool AtomList::insert(size_t pos, const Atom& a)
{
    if (pos > atoms_.size())
        return false;

    atoms_.insert(atoms_.begin() + pos, a);
    return true;
}

bool AtomList::insert(size_t pos, const AtomList& l)
{
    if (pos > atoms_.size())
        return false;
    atoms_.insert(atoms_.begin() + pos, l.atoms_.begin(), l.atoms_.end());
    return true;
}

bool AtomList::remove(size_t pos)
{
    if (pos >= size())
        return false;

    atoms_.erase(atoms_.begin() + pos);
    return true;
}

void AtomList::removeAll(const Atom& a)
{
    atom_iterator nend = std::remove(atoms_.begin(), atoms_.end(), a);
    atoms_.erase(nend, atoms_.end());
}

void AtomList::removeAll(AtomPredicate pred)
{
    atom_iterator nend = std::remove_if(atoms_.begin(), atoms_.end(), pred);
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
        return 0;
    return &atoms_.front();
}

Atom* AtomList::last()
{
    if (empty())
        return 0;
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

bool AtomList::isBang() const
{
    return empty();
}

bool AtomList::isFloat() const
{
    return size() == 1 && atoms_.front().isFloat();
}

bool AtomList::isSymbol() const
{
    return size() == 1 && atoms_.front().isSymbol();
}

bool AtomList::isProperty() const
{
    return size() == 1 && atoms_.front().isProperty();
}

bool AtomList::isList() const
{
    return size() > 1;
}

void AtomList::clear()
{
    atoms_.clear();
}

void AtomList::fill(const Atom& a)
{
    std::fill(atoms_.begin(), atoms_.end(), a);
}

void AtomList::fill(const Atom& a, size_t sz)
{
    atoms_ = atom_list(sz, a);
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

AtomList AtomList::filtered(AtomPredicate pred) const
{
    if (!pred)
        return *this;
    AtomList res;
    res.atoms_.reserve(size());
    for (size_t i = 0; i < atoms_.size(); i++) {
        const Atom& a = atoms_[i];
        if (pred(a))
            res.atoms_.push_back(a);
    }
    return res;
}

Atom* AtomList::min()
{
    if (empty())
        return 0;

    return &(*std::min_element(atoms_.begin(), atoms_.end()));
}

const Atom* AtomList::min() const
{
    return const_cast<AtomList*>(this)->min();
}

Atom* AtomList::max()
{
    if (empty())
        return 0;

    return &(*std::max_element(atoms_.begin(), atoms_.end()));
}

Atom* AtomList::find(const Atom& a)
{
    if (empty())
        return 0;

    atom_iterator it = std::find(atoms_.begin(), atoms_.end(), a);
    return it == atoms_.end() ? 0 : &(*it);
}

Atom* AtomList::findLast(const Atom& a)
{
    if (empty())
        return 0;

    atom_riterator it = std::find(atoms_.rbegin(), atoms_.rend(), a);
    return it == atoms_.rend() ? 0 : &(*it);
}

Atom* AtomList::findLast(AtomPredicate pred)
{
    if (empty())
        return 0;

    atom_riterator it = std::find_if(atoms_.rbegin(), atoms_.rend(), pred);
    return it == atoms_.rend() ? 0 : &(*it);
}

static t_float atom_sum(t_float a, t_float b)
{
    return a + b;
}

float AtomList::sum() const
{
    return reduceFloat(0.f, 0.f, &atom_sum);
}

static t_float atom_mul(t_float a, t_float b)
{
    return a * b;
}

float AtomList::product() const
{
    if (empty())
        return 0;

    return reduceFloat(1.f, 1.f, &atom_mul);
}

bool AtomList::contains(const Atom& a) const
{
    return find(a) != 0;
}

int AtomList::findPos(const Atom& a) const
{
    const_atom_iterator it = std::find(atoms_.begin(), atoms_.end(), a);
    if (it == atoms_.end())
        return -1;

    return std::distance(atoms_.begin(), it);
}

int AtomList::findPos(AtomPredicate pred) const
{
    const_atom_iterator it = std::find_if(atoms_.begin(), atoms_.end(), pred);
    if (it == atoms_.end())
        return -1;

    return std::distance(atoms_.begin(), it);
}

size_t AtomList::count(const Atom& a) const
{
    return std::count(atoms_.begin(), atoms_.end(), a);
}

size_t AtomList::count(AtomPredicate pred) const
{
    return std::count_if(atoms_.begin(), atoms_.end(), pred);
}

bool AtomList::allOf(AtomPredicate pred) const
{
    if (empty())
        return false;

    const_atom_iterator first = atoms_.begin();
    const_atom_iterator last = atoms_.end();
    while (first != last) {
        if (!pred(*first))
            return false;
        ++first;
    }
    return true;
}

bool AtomList::anyOf(AtomPredicate pred) const
{
    const_atom_iterator first = atoms_.begin();
    const_atom_iterator last = atoms_.end();

    while (first != last) {
        if (pred(*first))
            return true;
        ++first;
    }
    return false;
}

bool AtomList::noneOf(AtomPredicate pred) const
{
    const_atom_iterator first = atoms_.begin();
    const_atom_iterator last = atoms_.end();

    while (first != last) {
        if (pred(*first))
            return false;
        ++first;
    }
    return true;
}

Atom* AtomList::find(AtomPredicate pred)
{
    if (empty())
        return 0;

    atom_iterator it = std::find_if(atoms_.begin(), atoms_.end(), pred);
    return it == atoms_.end() ? 0 : &(*it);
}

const Atom* AtomList::max() const
{
    return const_cast<AtomList*>(this)->max();
}

template <class ForwardIterator>
static std::pair<ForwardIterator, ForwardIterator>
minmax_element(ForwardIterator first, ForwardIterator last)
{
    if (first == last)
        return std::make_pair(last, last);

    ForwardIterator smallest = first;
    ForwardIterator largest = first;

    while (++first != last) {
        if (*first < *smallest)
            smallest = first;

        if (*largest < *first)
            largest = first;
    }

    return std::make_pair(smallest, largest);
}

bool AtomList::range(Atom& min, Atom& max) const
{
    if (empty())
        return false;

    std::pair<const_atom_iterator, const_atom_iterator> res = ceammc::minmax_element(atoms_.begin(), atoms_.end());
    min = *res.first;
    max = *res.second;
    return true;
}

const Atom* AtomList::find(const Atom& a) const
{
    return const_cast<AtomList*>(this)->find(a);
}

const Atom* AtomList::findLast(const Atom& a) const
{
    return const_cast<AtomList*>(this)->findLast(a);
}

const Atom* AtomList::findLast(AtomPredicate pred) const
{
    return const_cast<AtomList*>(this)->findLast(pred);
}

const Atom* AtomList::find(AtomPredicate pred) const
{
    return const_cast<AtomList*>(this)->find(pred);
}

FloatList AtomList::asFloats() const
{
    FloatList res;
    for (size_t i = 0; i < atoms_.size(); i++) {
        res.push_back(atoms_[i].asFloat());
    }
    return res;
}

size_t AtomList::asSizeT(size_t defaultValue) const
{
    if (empty())
        return defaultValue;

    return atoms_.front().asSizeT(defaultValue);
}

void AtomList::outputAtoms(t_outlet* x) const
{
    for (size_t i = 0; i < size(); i++)
        to_outlet(x, at(i));
}

void AtomList::output(t_outlet* x) const
{
    to_outlet(x, *this);
}

void AtomList::outputAsAny(t_outlet* x) const
{
    if (empty())
        return;

    // check for valid selector
    if (!atoms_.front().isSymbol())
        return;

    outlet_anything(x, atoms_[0].asSymbol(), static_cast<int>(size() - 1), toPdData() + 1);
}

void AtomList::outputAsAny(_outlet* x, t_symbol* s) const
{
    outlet_anything(x, s, static_cast<int>(size()), toPdData());
}

t_float AtomList::reduceFloat(t_float init, t_float def, t_float (*fn)(t_float, t_float)) const
{
    t_float accum = init;
    const_atom_iterator it;
    for (it = atoms_.begin(); it != atoms_.end(); ++it) {
        accum = fn(accum, it->asFloat(def));
    }

    return accum;
}

bool AtomList::normalizeFloats()
{
    if (empty())
        return false;

    float s = sum();
    if (s == 0.f)
        return false;

    atom_iterator it;
    for (it = atoms_.begin(); it != atoms_.end(); ++it) {
        t_float f = 0.f;
        if (!it->getFloat(&f))
            continue;

        it->setFloat(f / s);
    }

    return true;
}

static AtomList listAdd(const AtomList& a, const AtomList& b, ElementAccessFn fn)
{
    AtomList res;
    const size_t min_sz = std::min(a.size(), b.size());
    // protect agains empty list
    if (min_sz == 0)
        return res;

    const size_t sz = std::max(a.size(), b.size());

    for (size_t i = 0; i < sz; i++)
        res.append((a.*fn)(static_cast<int>(i))->asFloat() + (b.*fn)(static_cast<int>(i))->asFloat());

    return res;
}

AtomList AtomList::add(const AtomList& a, const AtomList& b, NonEqualLengthBehaivor lb)
{
    switch (lb) {
    case MINSIZE: {
        AtomList res;
        size_t sz = std::min(a.size(), b.size());
        res.atoms_.reserve(sz);

        for (size_t i = 0; i < sz; i++)
            res.append(a[i].asFloat() + b[i].asFloat());

        return res;
    }
    case PADZERO: {
        size_t sz = std::max(a.size(), b.size());
        AtomList l1(a);
        AtomList l2(b);
        l1.resizePad(sz, 0.f);
        l2.resizePad(sz, 0.f);
        return add(l1, l2, MINSIZE);
    }
    case CLIP:
        return listAdd(a, b, &AtomList::clipAt);
    case WRAP:
        return listAdd(a, b, &AtomList::wrapAt);
    case FOLD:
        return listAdd(a, b, &AtomList::foldAt);
    }
}

AtomList AtomList::zeroes(size_t n)
{
    return filled(Atom(0.f), n);
}

AtomList AtomList::ones(size_t n)
{
    return filled(1.f, n);
}

AtomList AtomList::filled(const Atom& a, size_t n)
{
    AtomList res;
    res.fill(a, n);
    return res;
}

AtomList AtomList::values(size_t n, ...)
{
    AtomList res;
    va_list ap;
    va_start(ap, n);

    for (size_t i = 2; i <= n + 1; i++) {
        res.append(Atom(static_cast<float>(va_arg(ap, double))));
    }

    va_end(ap);
    return res;
}

static AtomList listSubstract(const AtomList& a, const AtomList& b, ElementAccessFn fn)
{
    AtomList res;
    const size_t min_sz = std::min(a.size(), b.size());
    // protect agains empty list
    if (min_sz == 0)
        return res;

    const size_t sz = std::max(a.size(), b.size());

    for (size_t i = 0; i < sz; i++)
        res.append((a.*fn)(static_cast<int>(i))->asFloat() - (b.*fn)(static_cast<int>(i))->asFloat());

    return res;
}

AtomList AtomList::sub(const AtomList& a, const AtomList& b, AtomList::NonEqualLengthBehaivor lb)
{
    switch (lb) {
    case MINSIZE: {
        AtomList res;
        size_t sz = std::min(a.size(), b.size());
        res.atoms_.reserve(sz);

        for (size_t i = 0; i < sz; i++)
            res.append(a[i].asFloat() - b[i].asFloat());

        return res;
    }
    case PADZERO: {
        size_t sz = std::max(a.size(), b.size());
        AtomList l1(a);
        AtomList l2(b);
        l1.resizePad(sz, 0.f);
        l2.resizePad(sz, 0.f);
        return sub(l1, l2, MINSIZE);
    }
    case CLIP:
        return listSubstract(a, b, &AtomList::clipAt);
    case WRAP:
        return listSubstract(a, b, &AtomList::wrapAt);
    case FOLD:
        return listSubstract(a, b, &AtomList::foldAt);
    }
}

AtomList& AtomList::operator+=(double v)
{
    for (size_t i = 0; i < size(); i++)
        atoms_[i] += v;

    return *this;
}

AtomList& AtomList::operator-=(double v)
{
    for (size_t i = 0; i < size(); i++)
        atoms_[i] -= v;

    return *this;
}

AtomList& AtomList::operator*=(double v)
{
    for (size_t i = 0; i < size(); i++)
        atoms_[i] *= v;

    return *this;
}

AtomList& AtomList::operator/=(double v)
{
    if (v == 0.0) {
        LIB_ERR << "division by zero";
        return *this;
    }

    for (size_t i = 0; i < size(); i++)
        atoms_[i] /= v;

    return *this;
}

AtomList AtomList::operator+(double v) const
{
    AtomList res(*this);
    res += v;
    return res;
}

AtomList AtomList::operator-(double v) const
{
    AtomList res(*this);
    res -= v;
    return res;
}

AtomList AtomList::operator*(double v) const
{
    AtomList res(*this);
    res *= v;
    return res;
}

AtomList AtomList::operator/(double v) const
{
    AtomList res(*this);
    res /= v;
    return res;
}

bool operator==(const AtomList& l1, const AtomList& l2)
{
    if (&l1 == &l2)
        return true;

    if (l1.size() != l2.size())
        return false;

    return std::equal(l1.atoms_.begin(), l1.atoms_.end(), l2.atoms_.begin());
}

bool operator!=(const AtomList& l1, const AtomList& l2)
{
    return !(l1 == l2);
}

void to_outlet(t_outlet* x, const AtomList& a)
{
    if (x == 0) {
        post("[ceammc] ERROR! NULL outlet pointer: %s", __FUNCTION__);
        return;
    }

    t_symbol* sel = 0;
    if (a.isList())
        sel = &s_list;
    else if (a.isFloat())
        sel = &s_float;
    else if (a.isSymbol())
        sel = &s_symbol;
    else if (a.isBang())
        sel = &s_bang;
    else
        sel = &s_list;

    outlet_anything(x, sel, static_cast<int>(a.size()), a.toPdData());
}

std::ostream& operator<<(std::ostream& os, const AtomList& l)
{
    os << "[ ";
    for (size_t i = 0; i < l.size(); i++) {
        if (i != 0)
            os << ", ";

        os << l.at(i);
    }

    os << " ]";
    return os;
}

AtomList listFrom(const std::vector<std::string>& v)
{
    AtomList res;
    for (size_t i = 0; i < v.size(); i++)
        res.append(atomFrom(v[i]));
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
    AtomList res;
    res.append(Atom(v ? 1.f : 0.f));
    return res;
}

Atom atomFrom(const std::string& v)
{
    return Atom(gensym(v.c_str()));
}

} // namespace ceammc
