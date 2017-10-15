#include <boost/math/special_functions/prime.hpp>
#include <m_pd.h>

#include "ceammc.h"

t_class* math_prime_class;
struct t_math_prime {
    t_object x_obj;
};

static void math_prime_float(t_math_prime* x, t_floatarg idx)
{
    if (idx < 0 || idx >= boost::math::max_prime) {
        pd_error(x, "invalid prime number index: %f", idx);
        return;
    }

    boost::uint32_t p = boost::math::prime(static_cast<unsigned int>(idx));
    outlet_float(x->x_obj.te_outlet, static_cast<t_float>(p));
}

static void* math_prime_new()
{
    t_math_prime* x = reinterpret_cast<t_math_prime*>(pd_new(math_prime_class));
    outlet_new(&x->x_obj, &s_float);
    return static_cast<void*>(x);
}

extern "C" void setup_math0x2eprime()
{
    math_prime_class = class_new(gensym("math.prime"),
        reinterpret_cast<t_newmethod>(math_prime_new),
        reinterpret_cast<t_method>(0),
        sizeof(t_math_prime), 0, A_NULL);
    class_addfloat(math_prime_class, math_prime_float);
}
