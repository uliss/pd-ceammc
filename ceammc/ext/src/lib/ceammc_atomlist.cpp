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

void AtomList::append(const Atom& a)
{
    atom_list_.push_back(a);
}

void AtomList::sort()
{
    std::sort(atom_list_.begin(), atom_list_.end());
}

FloatList AtomList::asFloats() const
{
    FloatList res;
    for (size_t i = 0; i < atom_list_.size(); i++) {
        res.push_back(atom_list_[i].asFloat());
    }
    return res;
}

} // namespace ceammc
