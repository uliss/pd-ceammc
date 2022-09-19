/*****************************************************************************
 * Copyright 2020 Serge Poltavsky. All rights reserved.
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
#ifndef TEST_WRAPPER_BASE_H
#define TEST_WRAPPER_BASE_H

#include "catch.hpp"
#include "ceammc_data.h"
#include "data_iface.h"
#include "datatype_dict.h"
#include "test_external.h"
#include "wrapper_datatype.h"

#include <cstdlib>
#include <iostream>
#include <string>

using namespace wrapper;

class WrapperDataVoid : public DataIFace {
public:
    WrapperDataVoid() {}

    bool operator==(const WrapperDataVoid& d) const
    {
        return true;
    }

    std::string toString() const
    {
        return "void";
    }

    static const char* typeName()
    {
        return "DataVoid";
    }
};

class WrapperIntPair : public DataIFace {
    int v0_, v1_;

public:
    WrapperIntPair(int v0 = 0, int v1 = 0)
        : v0_(v0)
        , v1_(v1)
    {
    }

    bool operator==(const WrapperIntPair& d) const
    {
        return v0_ == d.v0_ && d.v1_ == v1_;
    }

    std::string toString() const
    {
        return std::string("pair: ") + std::to_string(v0_) + " " + std::to_string(v1_);
    }

    static const char* typeName()
    {
        return "IntPair";
    }

    Result setFromList(const AtomListView& lv) override
    {
        if (lv.size() != 2)
            return error("v0 v1 expected");

        if (!(lv[0].isFloat() && lv[1].isFloat()))
            return error("float arguments required");

        v0_ = lv[0].asFloat();
        v1_ = lv[1].asFloat();

        return ok();
    }

    std::pair<int, int> get() const
    {
        return std::make_pair(v0_, v1_);
    }

    int sum() const { return v0_ + v1_; }

public:
    std::vector<int> add(const WrapperIntPair& p) const
    {
        return { v0_ + p.v0_, v1_ + p.v1_ };
    }

    static AbstractData* initFromList(const AtomList& l)
    {
        WrapperIntPair p;
        if (p.setFromList(l))
            return new AbstractDataWrapper<WrapperIntPair>(p);
        else
            return nullptr;
    }
};

class WrapperInt : public DataIFace {
    int v_;

public:
    WrapperInt(int v = 0)
        : v_(v)
    {
    }

    bool operator==(const WrapperInt& d) const
    {
        return v_ == d.v_;
    }

    std::string toString() const
    {
        return std::string("int: ") + std::to_string(v_);
    }

    Result setFromFloat(t_float v) override
    {
        v_ = static_cast<decltype(v_)>(v);
        return ok();
    }

    static const char* typeName()
    {
        return "DataInt";
    }

    int get() const
    {
        return v_;
    }

    void set(int v)
    {
        v_ = v;
    }

public:
    // methods
    bool is_negative() const { return v_ < 0; }
    std::tuple<int, int> dup2() const { return std::tuple<int, int>(v_, v_); }
    WrapperInt id() const { return *this; }
    std::vector<float> dup(int n) const { return std::vector<float>(n, v_); }

    // overload
    int multiply(const WrapperInt& i) const { return v_ * i.v_; }
    int multiply(int i) const { return v_ * i; }

    int muladd(int a, int b) const { return v_ * a + b; }
    float add_float(float f) const { return v_ + f; }
    double add_double(double f) const { return v_ + f; }
    long add_long(long i) const { return v_ + i; }
    unsigned int add_uint(unsigned int i) const { return v_ + i; }
    unsigned long add_ulong(unsigned long i) const { return v_ + i; }

    int sub(float v) const { return v_ - v; }
    int sub(double v) const { return v_ - v; }
    int sub(int v) const { return v_ - v; }
    int sub(long v) const { return v_ - v; }
    int sub(unsigned int v) const { return v_ - v; }
    int sub(unsigned long v) const { return v_ - v; }

    int on_bool(bool v) const { return v; }

    std::vector<int> list_mul(const std::vector<int>& m) const
    {
        std::vector<int> res(m);
        for (auto& e : res)
            e *= v_;

        return res;
    }

    std::vector<float> list_mulf(const std::vector<float>& m) const
    {
        std::vector<float> res(m);
        for (auto& e : res)
            e *= v_;

        return res;
    }

    int addP(const WrapperIntPair& p) const
    {
        return v_ + p.sum();
    }

    int strtoi(const std::string& str) const { return strtol(str.c_str(), nullptr, 10); }
    std::string str() const { return std::to_string(v_); }

public:
    static AbstractData* initFromList(const AtomList& l)
    {
        WrapperInt wint;
        if (wint.setFromList(l))
            return new AbstractDataWrapper<WrapperInt>(wint);
        else
            return nullptr;
    }
};

class WrapperInt2 : public WrapperInt {
public:
    WrapperInt2()
        : WrapperInt(100)
    {
    }
};

#endif // TEST_WRAPPER_BASE_H
