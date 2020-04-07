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

#include <iostream>
#include <string>

#include "ceammc_abstractdata.h"

class IntData : public ceammc::AbstractData {
    int v_;

public:
    IntData(int v) noexcept;
    IntData(const IntData& i);
    ~IntData() noexcept;

    int value() const noexcept;
    void setValue(int v) noexcept;
    bool isEqual(const AbstractData* d) const noexcept override;
    bool isLess(const AbstractData* d) const noexcept override;

    std::string toString() const override;
    std::string valueToJsonString() const override;
    int type() const noexcept override;
    IntData* clone() const override;

    static void init();

    bool operator==(const IntData& d) const noexcept;

public:
    static int dataType;
    static int constructor_called;
    static int destructor_called;
};

std::ostream& operator<<(std::ostream& os, const IntData& d);

class StrData : public ceammc::AbstractData {
    std::string v_;

public:
    StrData(const std::string& v);
    ~StrData() noexcept;

    const std::string& get() const noexcept;
    void setValue(const std::string& v);
    bool isEqual(const AbstractData* d) const noexcept override;

    std::string toString() const override;
    int type() const noexcept override;
    StrData* clone() const override;

    bool operator==(const StrData& d) const noexcept;

public:
    static int dataType;
    static int constructor_called;
    static int destructor_called;
};

#endif // TEST_DATATYPES_H
