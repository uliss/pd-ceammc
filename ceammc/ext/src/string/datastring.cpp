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
#include "datastring.h"
#include "ceammc_atomlist.h"
#include "ceammc_format.h"
#include "ceammc_log.h"

const DataType DataString::dataType = 1;

DataString::DataString(t_symbol* s)
{
    str_ = s->s_name;
    LIB_DBG << "string created: " << str_;
}

DataString::DataString(const Atom& a)
{
    str_ = to_string(a);
    LIB_DBG << "string created: " << str_;
}

DataString::DataString(const AtomList& l)
{
    str_ = to_string(l, " ");
    LIB_DBG << "string created: " << str_;
}

DataString::DataString(const char* str)
    : str_(str)
{
    LIB_DBG << "string created: " << str_;
}

DataString::DataString(const std::string& str)
    : str_(str)
{
    LIB_DBG << "string created: " << str_;
}

DataString::~DataString()
{
    LIB_DBG << "string destructed: " << str_;
}

DataType DataString::type() const
{
    return dataType;
}

DataString* DataString::clone() const
{
    return new DataString(str_);
}
