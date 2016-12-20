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
#include <cstdlib>
#include <iterator>

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

    assert(0 <= pos);
    assert(pos < isz);
    *dest = static_cast<size_t>(pos);
    return true;
}

AtomList::AtomList()
{
}

size_t AtomList::size() const
{
    return atoms_.size();
}

bool AtomList::empty() const
{
    return atoms_.empty();
}

Atom AtomList::at(size_t pos) const
{
    return atoms_.at(pos);
}

Atom* AtomList::relAt(int pos)
{
    size_t idx;
    if (!getRelativeIdx(pos, &idx))
        return 0;

    return &atoms_[static_cast<size_t>(idx)];
}

const Atom* AtomList::relAt(int pos) const
{
    return const_cast<AtomList*>(this)->relAt(pos);
}

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

} // namespace ceammc
