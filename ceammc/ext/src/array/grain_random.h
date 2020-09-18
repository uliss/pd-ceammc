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
#ifndef GRAIN_RANDOM_H
#define GRAIN_RANDOM_H

#include <cassert>
#include <limits>
#include <random>

namespace ceammc {

class GrainRandom {
    std::mt19937 gen_;

    GrainRandom();
    GrainRandom(const GrainRandom&) = delete;

public:
    static GrainRandom& instance();
    static void seed(size_t seed) { instance().gen_.seed(seed); }

    template <class T>
    static T urand_closed(T a, T b)
    {
        static_assert(std::is_floating_point<T>::value, "not a float type");
        std::uniform_real_distribution<T> dist(a, std::nextafter(b, std::numeric_limits<T>::max()));
        return dist(instance().gen_);
    }

    static float urandf_closed(float a, float b)
    {
        return urand_closed<float>(a, b);
    }

    static double urandd_closed(double a, double b)
    {
        return urand_closed<double>(a, b);
    }

    static float urandf(float a, float b)
    {
        std::uniform_real_distribution<float> dist(a, b);
        return dist(instance().gen_);
    }

    static double urandd(double a, double b)
    {
        std::uniform_real_distribution<double> dist(a, b);
        return dist(instance().gen_);
    }

    static size_t urandul(size_t a, size_t b)
    {
        assert(a <= b);
        std::uniform_int_distribution<size_t> dist(a, b);
        return dist(instance().gen_);
    }
};

}

#endif // GRAIN_RANDOM_H
