/*****************************************************************************
 * Copyright 2017 Serge Poltavsky. All rights reserved.
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
#ifndef CEAMMC_DATAATOMLIST_H
#define CEAMMC_DATAATOMLIST_H

#include "ceammc_atomlist.h"
#include "ceammc_dataatom.h"

#include <iostream>
#include <vector>

namespace ceammc {

class DataAtomList {
    std::vector<DataAtom> list_;

public:
    DataAtomList();
    ~DataAtomList();
    DataAtomList(const Atom& a);
    DataAtomList(const AtomList& lst);

    void set(const AtomList& lst);

    size_t size() const;
    DataAtom& operator[](size_t idx);
    const DataAtom& operator[](size_t idx) const;
    void clear();

    void append(const Atom& a);
    void append(const DataPtr& d);
    AtomList toList() const;

    bool operator==(const DataAtomList& l) const;
};

std::ostream& operator<<(std::ostream& os, const DataAtomList& l);
}

#endif // CEAMMC_DATAATOMLIST_H
