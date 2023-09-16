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
#ifndef CEAMMC_RANDOM_H
#define CEAMMC_RANDOM_H

#include <cstdint>
#include <memory>
#include <random>

#include "ceammc_property.h"

namespace ceammc {
namespace random {

    using DefaultRandomGen = std::mt19937;
    using RandomGenPtr = std::shared_ptr<DefaultRandomGen>;

    class RandomGen {
        RandomGenPtr gen_;

    public:
        RandomGen();
        void setSeed(uint_fast32_t v);
        DefaultRandomGen& get() { return *gen_; }

        float gen_uniform_float(float a, float b);
        uint32_t gen_uniform_uint(uint32_t a, uint32_t b);
        int32_t gen_uniform_int(int32_t a, int32_t b);
    };

    class SeedProperty : public IntProperty {
        RandomGen& gen_;

    public:
        SeedProperty(RandomGen& gen);
    };

}
}

#endif // CEAMMC_RANDOM_H
