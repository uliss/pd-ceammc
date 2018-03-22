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
#include "test_datatypes.h"

#include <sstream>

ceammc::DataType IntData::dataType = 1001;
int IntData::constructor_called = 0;
int IntData::destructor_called = 0;

ceammc::DataType StrData::dataType = 1002;
int StrData::constructor_called = 0;
int StrData::destructor_called = 0;

IntData::IntData(int v)
    : v_(v)
{
    constructor_called++;
}

IntData::~IntData()
{
    destructor_called++;
}

int IntData::value() const { return v_; }

void IntData::setValue(int v) { v_ = v; }

bool IntData::isEqual(const ceammc::AbstractData* d) const
{
    const IntData* dt = d->as<IntData>();
    if (!dt)
        return false;

    return v_ == dt->v_;
}

bool IntData::isLess(const ceammc::AbstractData* d) const
{
    const IntData* dt = d->as<IntData>();
    if (!dt)
        return false;

    return v_ < dt->v_;
}

std::string IntData::toString() const
{
    std::ostringstream buf;
    buf << v_;
    return buf.str();
}

ceammc::DataType IntData::type() const { return dataType; }

IntData* IntData::clone() const { return new IntData(v_); }

StrData::StrData(const std::string& v)
    : v_(v)
{
    constructor_called++;
}

StrData::~StrData()
{
    destructor_called++;
}

const std::string& StrData::get() const { return v_; }

void StrData::setValue(const std::string& v) { v_ = v; }

bool StrData::isEqual(const ceammc::AbstractData* d) const
{
    const StrData* dt = d->as<StrData>();
    if (!dt)
        return false;

    return v_ == dt->v_;
}

std::string StrData::toString() const
{
    return v_;
}

ceammc::DataType StrData::type() const { return dataType; }

StrData* StrData::clone() const { return new StrData(v_); }
