#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <ctime>
#include <m_pd.h>

#include "ceammc.h"

t_class* random_uniform_int_class;
struct t_random_uniform_int {
    t_object x_obj;
    t_inlet* upper_range_inlet;
    t_float lower_range;
    t_float upper_range;
};

static boost::random::mt19937 random_gen(std::time(0));

static void random_uniform_int_bang(t_random_uniform_int* x)
{
    x->lower_range = static_cast<int>(x->lower_range);
    x->upper_range = static_cast<int>(x->upper_range);

    if (x->lower_range == x->upper_range) {
        outlet_float(x->x_obj.te_outlet, x->lower_range);
        return;
    }

    boost::random::uniform_int_distribution<int> dist(x->lower_range, x->upper_range);
    outlet_float(x->x_obj.te_outlet, dist(random_gen));
}

static void random_uniform_int_float(t_random_uniform_int* x, t_floatarg lower)
{
    x->lower_range = lower;
}

static void* random_uniform_int_new(t_floatarg lower, t_floatarg upper)
{
    t_random_uniform_int* x = reinterpret_cast<t_random_uniform_int*>(pd_new(random_uniform_int_class));
    outlet_new(&x->x_obj, &s_float);
    x->upper_range_inlet = floatinlet_new(&x->x_obj, &x->upper_range);

    if (upper < lower)
        std::swap(lower, upper);
    x->upper_range = upper;
    x->lower_range = lower;

    return static_cast<void*>(x);
}

static void random_uniform_int_free(t_random_uniform_int* x)
{
    inlet_free(x->upper_range_inlet);
}

extern "C" void setup_random0x2eint()
{
    random_uniform_int_class = class_new(gensym("random.int"),
        reinterpret_cast<t_newmethod>(random_uniform_int_new),
        reinterpret_cast<t_method>(random_uniform_int_free),
        sizeof(t_random_uniform_int), 0, A_DEFFLOAT, A_DEFFLOAT, 0);
    class_addbang(random_uniform_int_class, random_uniform_int_bang);
    class_addfloat(random_uniform_int_class, random_uniform_int_float);
}
