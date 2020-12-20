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
#include "test_approx.h"
#include "catch.hpp"

#include <sstream>

std::ostream& operator<<(std::ostream& os, const ListApprox& value)
{
    os << value.toString();
    return os;
}

ListApprox operator+(const ListApprox& l0, const ListApprox& l1)
{
    ListApprox res;
    res.lst_ = l0.lst_;
    res.lst_.append(l1.lst_);
    return res;
}

bool operator==(const ListApprox& la, const AtomList& al)
{
    if (!al.allOf(isFloat))
        return false;

    if (la.lst_.size() != al.size())
        return false;

    if (la.margin_ != 0) {
        for (size_t i = 0; i < la.lst_.size(); i++) {
            if (la.lst_[i].asFloat() != Approx(al[i].asFloat()).margin(la.margin_))
                return false;
        }
    } else {
        for (size_t i = 0; i < la.lst_.size(); i++) {
            if (la.lst_[i].asFloat() != Approx(al[i].asFloat()))
                return false;
        }
    }

    return true;
}

bool operator==(const AtomList& al, const ListApprox& la)
{
    return operator==(la, al);
}

ListApprox::ListApprox()
{
}

ListApprox::ListApprox(std::initializer_list<t_float> l)
    : lst_(l)
{
}

std::string ListApprox::toString() const
{
    std::ostringstream oss;
    oss << "ListApprox( " << lst_ << " )";
    return oss.str();
}

ListApprox& ListApprox::margin(t_float m)
{
    margin_ = m;
    return *this;
}

AtomListApprox::AtomListApprox(const AtomList& lst)
    : AtomList(lst)
{
}

bool operator==(const AtomListApprox& la, const AtomList& al)
{
    if (la.size() != al.size())
        return false;

    for (size_t i = 0; i < la.size(); i++) {
        auto& a0 = la[i];
        auto& a1 = al[i];

        if (a0.type() != a1.type())
            return false;

        if (a0.isSymbol() && a0 != a1)
            return false;

        if (a0.asFloat() != Approx(a1.asFloat()).epsilon(0.1))
            return false;
    }

    return true;
}

bool operator==(const AtomList& al, const AtomListApprox& la)
{
    return operator==(la, al);
}
