#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_real_distribution.hpp>
#include <ctime>
#include <m_pd.h>

#include "ceammc.h"

t_class* random_uniform_class;
struct t_random_uniform {
    t_object x_obj;
    t_inlet* upper_range_inlet;
    t_float lower_range;
    t_float upper_range;
};

static boost::random::mt19937 random_gen(std::time(0));

static void random_uniform_bang(t_random_uniform* x)
{
    if (x->lower_range == x->upper_range) {
        outlet_float(x->x_obj.te_outlet, x->lower_range);
        return;
    }

    if (x->upper_range < x->lower_range)
        std::swap(x->upper_range, x->lower_range);

    boost::random::uniform_real_distribution<t_float> dist(x->lower_range, x->upper_range);
    outlet_float(x->x_obj.te_outlet, dist(random_gen));
}

static void random_uniform_float(t_random_uniform* x, t_floatarg lower)
{
    x->lower_range = lower;
}

static void* random_uniform_new(t_floatarg lower, t_floatarg upper)
{
    t_random_uniform* x = reinterpret_cast<t_random_uniform*>(pd_new(random_uniform_class));
    outlet_new(&x->x_obj, &s_float);
    x->upper_range_inlet = floatinlet_new(&x->x_obj, &x->upper_range);

    if (upper < lower)
        std::swap(upper, lower);
    x->upper_range = upper;
    x->lower_range = lower;
    return static_cast<void*>(x);
}

static void random_uniform_free(t_random_uniform* x)
{
    inlet_free(x->upper_range_inlet);
}

extern "C" void setup_random0x2efloat()
{
    random_uniform_class = class_new(gensym("random.float"),
        reinterpret_cast<t_newmethod>(random_uniform_new),
        reinterpret_cast<t_method>(random_uniform_free),
        sizeof(t_random_uniform), 0, A_DEFFLOAT, A_DEFFLOAT, 0);
    class_addbang(random_uniform_class, random_uniform_bang);
    class_addfloat(random_uniform_class, random_uniform_float);
}
