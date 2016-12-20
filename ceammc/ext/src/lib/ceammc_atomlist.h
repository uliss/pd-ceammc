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

class AtomList {
    std::vector<Atom> atom_list_;

public:
    AtomList();
    size_t size() const;
    bool empty() const;

    Atom at(size_t pos) const;
    void append(const Atom& a);

    void sort();

    FloatList asFloats() const;
};

} // namespace ceammc

#endif // CEAMMC_ATOMLIST_H
