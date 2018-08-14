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
#include "ceammc_data.h"

#include <nonius/nonius.h++>
#include <random>

using namespace ceammc;

class IntData : public AbstractData {
    int v_;

public:
    IntData(int v)
        : v_(v)
    {
    }

    DataType type() const override
    {
        return DataType(1024);
    }

    IntData* clone() const override
    {
        return new IntData(v_);
    }

    bool isLess(const AbstractData* d) const override
    {
        if (d->type() == type())
            return v_ < d->as<IntData>()->v_;

        return false;
    }

    std::string toString() const override
    {
        return std::to_string(v_);
    }

    static DataType dataType;
};

DataType IntData::dataType = DataType(1024);

static void init(std::vector<DataPtr>& vec)
{
    static int cnt = 0;
    if (cnt++)
        return;

    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution;

    for (size_t i = 0; i < vec.size(); i++)
        vec[i] = DataPtr(new IntData(distribution(generator)));

    std::cerr << "init done...\n";
}

std::vector<DataPtr> vec2000(2000, DataPtr(Atom()));

NONIUS_BENCHMARK("DataPtr", [] {
    std::vector<DataPtr> vec;
    vec.assign(1000, DataPtr(Atom()));
    return 0;
})

NONIUS_BENCHMARK("DataPtr::sort", [] {
    init(vec2000);
    std::sort(vec2000.begin(), vec2000.end());
    return 0;
})
