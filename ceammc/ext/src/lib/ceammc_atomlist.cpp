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
#include <algorithm>
#include <cassert>
#include <cstdarg>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <string>

namespace ceammc {

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

Atom* AtomList::clipAt(size_t pos)
{
    if (empty())
        return 0;

    return &at(std::min(size() - 1, pos));
}

const Atom* AtomList::clipAt(size_t pos) const
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

Atom* AtomList::foldAt(size_t pos)
{
    if (empty())
        return 0;

    if (size() == 1)
        return first();

    const size_t a = size() - 1;
    const size_t b = pos % (a * 2);
    return &at(std::min(b, a * 2 - b));
}

const Atom* AtomList::foldAt(size_t pos) const
{
    return const_cast<AtomList*>(this)->foldAt(pos);
}

void AtomList::resizePad(size_t n, const Atom& v)
{
    atoms_.resize(n, v);
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

//std::string AtomList::toString()
//{
//    std::string ret = "";
//
//    //iterator??
//    for (int i=0; i<this->size();i++)
//    {
//        ret = ret + this->at(i).asString() + (i!=(this->size()-1)?" ":"");
//    }
//
//    return ret;
//};

void AtomList::append(const Atom& a)
{
    atoms_.push_back(a);
}

bool AtomList::insert(size_t pos, const Atom& a)
{
    if (pos > atoms_.size())
        return false;

    atoms_.insert(atoms_.begin() + pos, a);
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

AtomList AtomList::subList(int begin, int end)
{
    AtomList ret;

    if ((end - begin) > 0) {
        //temporary
        for (size_t i = begin; i < end; i++) {
            const Atom& a = atoms_[i];
            ret.atoms_.push_back(a);
        }
    }

    return ret;
}

//AtomList AtomList::sub(int begin, int end)const
//{
//    AtomList ret;
//
//    if ((end - begin) > 0) {
//        //temporary
//        for (size_t i = begin; i < end; i++) {
//            const Atom& a = atoms_[i];
//            ret.atoms_.push_back(a);
//        }
//    }
//
//    return ret;
//}

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

bool AtomList::allOff(AtomPredicate pred) const
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

bool AtomList::anyOff(AtomPredicate pred) const
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

bool AtomList::noneOff(AtomPredicate pred) const
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

AtomList AtomList::sub(const AtomList& l, AtomList::NonEqualLengthBehaivor b) const
{
    AtomList res;
    switch (b) {
    case MINSIZE: {
        size_t sz = std::min(size(), l.size());
        res.atoms_.reserve(sz);

        for (size_t i = 0; i < sz; i++)
            res.append(at(i).asFloat() - l.at(i).asFloat());

        return res;
    }
    case PADZERO: {
        size_t sz = std::max(size(), l.size());
        AtomList l1(*this);
        AtomList l2(l);
        l1.resizePad(sz, 0.f);
        l2.resizePad(sz, 0.f);
        return l1.sub(l2, MINSIZE);
    }
    case CLIP: {
        const size_t min_sz = std::min(size(), l.size());
        // protect agains empty list. with it we can't take clipped value
        if (min_sz == 0)
            return res;

        const size_t sz = std::max(size(), l.size());
        res.atoms_.reserve(sz);

        for (size_t i = 0; i < sz; i++)
            res.append(clipAt(i)->asFloat() - l.clipAt(i)->asFloat());

        return res;
    }

    case WRAP: {
        const size_t min_sz = std::min(size(), l.size());
        // protect agains empty list. with it we can't take wrapped value
        if (min_sz == 0)
            return res;

        const size_t sz = std::max(size(), l.size());
        res.atoms_.reserve(sz);

        for (size_t i = 0; i < sz; i++)
            res.append(wrapAt(i)->asFloat() - l.wrapAt(i)->asFloat());

        return res;
    }

    case FOLD: {
        const size_t min_sz = std::min(size(), l.size());
        // protect agains empty list. with it we can't take folded value
        if (min_sz == 0)
            return res;

        const size_t sz = std::max(size(), l.size());
        res.atoms_.reserve(sz);

        for (size_t i = 0; i < sz; i++)
            res.append(foldAt(i)->asFloat() - l.foldAt(i)->asFloat());

        return res;
    }
    }
}

AtomList AtomList::subFrom(const AtomList& l, AtomList::NonEqualLengthBehaivor b) const
{
    AtomList res;
    switch (b) {
    case MINSIZE: {
        size_t sz = std::min(size(), l.size());
        res.atoms_.reserve(sz);

        for (size_t i = 0; i < sz; i++)
            res.append(-at(i).asFloat() + l.at(i).asFloat());

        return res;
    }
    case PADZERO: {
        size_t sz = std::max(size(), l.size());
        AtomList l1(*this);
        AtomList l2(l);
        l1.resizePad(sz, 0.f);
        l2.resizePad(sz, 0.f);
        return l1.subFrom(l2, MINSIZE);
    }
    case CLIP: {
        const size_t min_sz = std::min(size(), l.size());
        // protect agains empty list. with it we can't take clipped value
        if (min_sz == 0)
            return res;

        const size_t sz = std::max(size(), l.size());
        res.atoms_.reserve(sz);

        for (size_t i = 0; i < sz; i++)
            res.append(-clipAt(i)->asFloat() + l.clipAt(i)->asFloat());

        return res;
    }

    case WRAP: {
        const size_t min_sz = std::min(size(), l.size());
        // protect agains empty list. with it we can't take wrapped value
        if (min_sz == 0)
            return res;

        const size_t sz = std::max(size(), l.size());
        res.atoms_.reserve(sz);

        for (size_t i = 0; i < sz; i++)
            res.append(-wrapAt(i)->asFloat() + l.wrapAt(i)->asFloat());

        return res;
    }

    case FOLD: {
        const size_t min_sz = std::min(size(), l.size());
        // protect agains empty list. with it we can't take folded value
        if (min_sz == 0)
            return res;

        const size_t sz = std::max(size(), l.size());
        res.atoms_.reserve(sz);

        for (size_t i = 0; i < sz; i++)
            res.append(-foldAt(i)->asFloat() + l.foldAt(i)->asFloat());

        return res;
    }
    }
}

AtomList AtomList::addTo(const AtomList& l, AtomList::NonEqualLengthBehaivor b) const
{
    AtomList res;
    switch (b) {
    case MINSIZE: {
        size_t sz = std::min(size(), l.size());
        res.atoms_.reserve(sz);

        for (size_t i = 0; i < sz; i++)
            res.append(at(i).asFloat() + l.at(i).asFloat());

        return res;
    }
    case PADZERO: {
        size_t sz = std::max(size(), l.size());
        AtomList l1(*this);
        AtomList l2(l);
        l1.resizePad(sz, 0.f);
        l2.resizePad(sz, 0.f);
        return l1.addTo(l2, MINSIZE);
    }
    case CLIP: {
        const size_t min_sz = std::min(size(), l.size());
        // protect agains empty list. with it we can't take clipped value
        if (min_sz == 0)
            return res;

        const size_t sz = std::max(size(), l.size());
        res.atoms_.reserve(sz);

        for (size_t i = 0; i < sz; i++)
            res.append(clipAt(i)->asFloat() + l.clipAt(i)->asFloat());

        return res;
    }

    case WRAP: {
        const size_t min_sz = std::min(size(), l.size());
        // protect agains empty list. with it we can't take wrapped value
        if (min_sz == 0)
            return res;

        const size_t sz = std::max(size(), l.size());
        res.atoms_.reserve(sz);

        for (size_t i = 0; i < sz; i++)
            res.append(wrapAt(i)->asFloat() + l.wrapAt(i)->asFloat());

        return res;
    }

    case FOLD: {
        const size_t min_sz = std::min(size(), l.size());
        // protect agains empty list. with it we can't take folded value
        if (min_sz == 0)
            return res;

        const size_t sz = std::max(size(), l.size());
        res.atoms_.reserve(sz);

        for (size_t i = 0; i < sz; i++)
            res.append(foldAt(i)->asFloat() + l.foldAt(i)->asFloat());

        return res;
    }
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

    outlet_list(x, &s_list, static_cast<int>(a.size()), a.toPdData());
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

} // namespace ceammc
