/*****************************************************************************
 * Copyright 2018 Serge Poltavsky. All rights reserved.
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
#ifndef TEST_APPROX_H
#define TEST_APPROX_H

#include "ceammc_atomlist.h"

#include <initializer_list>
#include <iostream>
#include <string>

using namespace ceammc;

class ListApprox {
    AtomList lst_;
    t_float margin_ = 0;

public:
    ListApprox();
    ListApprox(std::initializer_list<t_float> l);
    std::string toString() const;
    friend ListApprox operator+(const ListApprox& l0, const ListApprox& l1);
    friend bool operator==(const ListApprox& la, const AtomList& al);

    ListApprox& margin(t_float m);

    operator AtomList() const { return lst_; }
};

std::ostream& operator<<(std::ostream& os, const ListApprox& value);
ListApprox operator+(const ListApprox& l0, const ListApprox& l1);
bool operator==(const ListApprox& la, const AtomList& al);
bool operator==(const AtomList& al, const ListApprox& la);

class AtomListApprox : public AtomList {
public:
    explicit AtomListApprox(const AtomList& lst);
};

bool operator==(const AtomListApprox& la, const AtomList& al);
bool operator==(const AtomList& al, const AtomListApprox& la);

#endif // TEST_APPROX_H
