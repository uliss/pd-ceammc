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
#ifndef TEST_DATATYPES_H
#define TEST_DATATYPES_H

#include <string>

#include "ceammc_abstractdata.h"
#include "ceammc_data.h"

class IntData : public ceammc::AbstractData {
    int v_;

public:
    IntData(int v);
    ~IntData();

    int value() const;
    void setValue(int v);
    bool isEqual(const AbstractData* d) const;
    bool isLess(const AbstractData* d) const;

    std::string toString() const;
    ceammc::DataType type() const;
    IntData* clone() const;

public:
    static ceammc::DataType dataType;
    static int constructor_called;
    static int destructor_called;
};

class StrData : public ceammc::AbstractData {
    std::string v_;

public:
    StrData(const std::string& v);
    ~StrData();

    const std::string& get() const;
    void setValue(const std::string& v);
    bool isEqual(const AbstractData* d) const;

    std::string toString() const;
    ceammc::DataType type() const;
    StrData* clone() const;

public:
    static ceammc::DataType dataType;
    static int constructor_called;
    static int destructor_called;
};

#endif // TEST_DATATYPES_H
