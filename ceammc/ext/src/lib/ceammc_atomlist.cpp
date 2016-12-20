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

AtomList::AtomList()
{
}

size_t AtomList::size() const
{
    return atom_list_.size();
}

bool AtomList::empty() const
{
    return atom_list_.empty();
}

Atom AtomList::at(size_t pos) const
{
    return atom_list_.at(pos);
}

Atom* AtomList::relAt(int pos)
{
    if (empty())
        return 0;

    int sz = static_cast<int>(atom_list_.size());
    if (abs(pos) > sz)
        return 0;

    if (pos < 0)
        pos += sz;

    assert(0 <= pos);
    assert(pos < sz);

    return &atom_list_[static_cast<size_t>(pos)];
}

const Atom* AtomList::relAt(int pos) const
{
    return const_cast<AtomList*>(this)->relAt(pos);
}

void AtomList::append(const Atom& a)
{
    atom_list_.push_back(a);
}

void AtomList::insert(size_t pos, const Atom& a)
{
    if (pos > atom_list_.size())
        return;

    atom_list_.insert(atom_list_.begin() + pos, a);
}

Atom* AtomList::first()
{
    if (empty())
        return 0;
    return &atom_list_.front();
}

Atom* AtomList::last()
{
    if (empty())
        return 0;
    return &atom_list_.back();
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
    atom_list_.clear();
}

void AtomList::sort()
{
    std::sort(atom_list_.begin(), atom_list_.end());
}

AtomList AtomList::filtered(AtomPredicate pred) const
{
    if (!pred)
        return *this;
    AtomList res;
    res.atom_list_.reserve(size());
    for (size_t i = 0; i < atom_list_.size(); i++) {
        const Atom& a = atom_list_[i];
        if (pred(a))
            res.atom_list_.push_back(a);
    }
    return res;
}

Atom* AtomList::min()
{
    if (empty())
        return 0;

    return &(*std::min_element(atom_list_.begin(), atom_list_.end()));
}

const Atom* AtomList::min() const
{
    return const_cast<AtomList*>(this)->min();
}

Atom* AtomList::max()
{
    if (empty())
        return 0;

    return &(*std::max_element(atom_list_.begin(), atom_list_.end()));
}

const Atom* AtomList::max() const
{
    return const_cast<AtomList*>(this)->max();
}

FloatList AtomList::asFloats() const
{
    FloatList res;
    for (size_t i = 0; i < atom_list_.size(); i++) {
        res.push_back(atom_list_[i].asFloat());
    }
    return res;
}

bool operator==(const AtomList& l1, const AtomList& l2)
{
    if (&l1 == &l2)
        return true;

    if (l1.size() != l2.size())
        return false;

    return std::equal(l1.atom_list_.begin(), l1.atom_list_.end(), l2.atom_list_.begin());
}

bool operator!=(const AtomList& l1, const AtomList& l2)
{
    return !(l1 == l2);
}

} // namespace ceammc
