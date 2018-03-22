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
#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "ceammc_atomlist.h"
#include "common_external_test.h"

#include <iostream>
#include <sstream>

extern "C" {
#include "s_stuff.h"
}

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

    for (size_t i = 0; i < la.lst_.size(); i++) {
        if (la.lst_[i].asFloat() != Approx(al[i].asFloat()))
            return false;
    }

    return true;
}

bool operator==(const AtomList& al, const ListApprox& la)
{
    return operator==(la, al);
}

ListApprox::ListApprox() {}

ListApprox::ListApprox(float f) { lst_.append(f); }

ListApprox::ListApprox(float f0, float f1)
{
    lst_.append(f0);
    lst_.append(f1);
}

ListApprox::ListApprox(float f0, float f1, float f2)
{
    lst_.append(f0);
    lst_.append(f1);
    lst_.append(f2);
}

ListApprox::ListApprox(float f0, float f1, float f2, float f3)
{
    lst_.append(f0);
    lst_.append(f1);
    lst_.append(f2);
    lst_.append(f3);
}

ListApprox::ListApprox(float f0, float f1, float f2, float f3, float f4)
{
    lst_.append(f0);
    lst_.append(f1);
    lst_.append(f2);
    lst_.append(f3);
    lst_.append(f4);
}

ListApprox::ListApprox(float f0, float f1, float f2, float f3, float f4, float f5)
{
    lst_.append(f0);
    lst_.append(f1);
    lst_.append(f2);
    lst_.append(f3);
    lst_.append(f4);
    lst_.append(f5);
}

ListApprox::ListApprox(float f0, float f1, float f2, float f3, float f4, float f5, float f6)
{
    lst_.append(f0);
    lst_.append(f1);
    lst_.append(f2);
    lst_.append(f3);
    lst_.append(f4);
    lst_.append(f5);
    lst_.append(f6);
}

ListApprox::ListApprox(float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7)
{
    lst_.append(f0);
    lst_.append(f1);
    lst_.append(f2);
    lst_.append(f3);
    lst_.append(f4);
    lst_.append(f5);
    lst_.append(f6);
    lst_.append(f7);
}

std::string ListApprox::toString() const
{
    std::ostringstream oss;
    oss << "ListApprox( " << lst_ << " )";
    return oss.str();
}

void printPdToStdError(bool value)
{
    sys_printtostderr = value;
}

void setPdPrintFunction(pdPrintFunction fn)
{
    sys_printhook = fn;
}
