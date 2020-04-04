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
#ifndef TEST_RANDOM_BASE_H
#define TEST_RANDOM_BASE_H

#include "test_base.h"
#include "test_catch2.hpp"
#include "test_external.h"

using namespace ceammc;

template <typename T>
class OutputRangeBase : public Catch::MatcherBase<T> {
protected:
    t_float min_, max_;
    size_t outl_;

public:
    OutputRangeBase(t_float min, t_float max, size_t outlet = 0)
        : min_(min)
        , max_(max)
        , outl_(outlet)
    {
    }

    std::string describe() const final
    {
        std::ostringstream ss;
        ss << "output float in range "
           << "[" << min_ << ", " << max_ << ")"
           << " to outlet[" << outl_ << "]";
        return ss.str();
    }
};

template <class T>
class OutputInRange : public OutputRangeBase<TestExternal<T>> {
public:
    OutputInRange(t_float min, t_float max, size_t outlet = 0)
        : OutputRangeBase<TestExternal<T>>(min, max, outlet)
    {
    }

    bool match(const TestExternal<T>& t) const override
    {
        return t.hasNewMessages(this->outl_)
            && t.lastMessage(this->outl_).isFloat()
            && t.lastMessage(this->outl_).atomValue().asFloat() >= Approx(this->min_)
            && t.lastMessage(this->outl_).atomValue().asFloat() < this->max_;
    }
};

template <class T>
class OutputInRangeExt : public OutputRangeBase<TestPdExternal<T>> {
public:
    OutputInRangeExt(t_float min, t_float max, size_t outlet = 0)
        : OutputRangeBase<TestPdExternal<T>>(min, max, outlet)
    {
    }

    bool match(const TestPdExternal<T>& t) const override
    {
        return t.hasOutputAt(this->outl_)
            && t.isOutputFloatAt(this->outl_)
            && t.outputFloatAt(this->outl_) >= Approx(this->min_)
            && t.outputFloatAt(this->outl_) < this->max_;
    }
};

template <typename T>
OutputInRange<T> outputInRange(TestExternal<T>* t, t_float min, t_float max, size_t outl = 0)
{
    return OutputInRange<T>(min, max, outl);
}

template <typename T>
OutputInRangeExt<T> outputInRange(TestPdExternal<T>* t, t_float min, t_float max, size_t outl = 0)
{
    return OutputInRangeExt<T>(min, max, outl);
}

#endif // TEST_RANDOM_BASE_H
