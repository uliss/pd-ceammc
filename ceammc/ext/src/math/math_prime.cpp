#include "m_pd.h"
#include <boost/math/special_functions/prime.hpp>

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

void setup_math_prime()
{
    math_prime_class = class_new(gensym("math.prime"),
        reinterpret_cast<t_newmethod>(math_prime_new),
        reinterpret_cast<t_method>(0),
        sizeof(t_math_prime), 0, A_NULL);
    class_doaddfloat(math_prime_class, reinterpret_cast<t_method>(math_prime_float));
}
