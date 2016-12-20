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
#ifndef CEAMMC_ATOMLIST_H
#define CEAMMC_ATOMLIST_H

#include "ceammc_atom.h"
#include <vector>

namespace ceammc {

typedef std::vector<t_float> FloatList;
typedef bool (*AtomPredicate)(const Atom& a);

class AtomList {
    typedef std::vector<Atom> atom_list;
    atom_list atom_list_;
    typedef atom_list::const_iterator const_atom_iterator;
    typedef atom_list::iterator atom_iterator;

public:
    AtomList();
    size_t size() const;
    bool empty() const;

    Atom at(size_t pos) const;
    void append(const Atom& a);

    Atom* first();
    Atom* last();
    const Atom* first() const;
    const Atom* last() const;

    void sort();
    AtomList filtered(AtomPredicate pred) const;

    const Atom* min() const;
    const Atom* max() const;
    Atom* min();
    Atom* max();

    FloatList asFloats() const;

public:
    friend bool operator==(const AtomList& l1, const AtomList& l2);
    friend bool operator!=(const AtomList& l1, const AtomList& l2);
};

bool operator==(const AtomList& l1, const AtomList& l2);
bool operator!=(const AtomList& l1, const AtomList& l2);

} // namespace ceammc

#endif // CEAMMC_ATOMLIST_H
