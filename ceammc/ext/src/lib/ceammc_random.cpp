/*****************************************************************************
 * Copyright 2022 Serge Poltavski. All rights reserved.
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
#include "ceammc_random.h"

#include <ctime>

namespace ceammc {
namespace random {

    static RandomGenPtr default_gen()
    {
        static RandomGenPtr instance_;
        if (!instance_) {
            instance_.reset(new DefaultRandomGen(std::time(nullptr)));
        }

        return instance_;
    }

    RandomGen::RandomGen()
        : gen_(default_gen())
    {
    }

    void RandomGen::setSeed(uint_fast32_t v)
    {
        if (gen_ == default_gen())
            gen_.reset(new DefaultRandomGen(v));
        else
            gen_->seed(v);
    }

    float RandomGen::gen_uniform_float(float a, float b)
    {
        std::uniform_real_distribution<float> dist(a, b);
        return dist(*gen_);
    }

    uint32_t RandomGen::gen_uniform_uint(uint32_t a, uint32_t b)
    {
        std::uniform_int_distribution<uint32_t> dist(a, b);
        return dist(*gen_);
    }

    SeedProperty::SeedProperty(RandomGen& gen)
        : IntProperty("@seed", 0)
        , gen_(gen)
    {
        checkMinEq(0);
        setSuccessFn([this](Property* p) {
            gen_.setSeed(value());
        });
    }

}
}
