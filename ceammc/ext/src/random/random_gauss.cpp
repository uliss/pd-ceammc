#include <boost/random/mersenne_twister.hpp>
#include <boost/random/normal_distribution.hpp>
#include <ctime>
#include <m_pd.h>

#include "ceammc.h"

t_class* random_gauss_class;
struct t_random_gauss {
    t_object x_obj;
    t_inlet* sigma_inlet;
    t_float mean;
    t_float sigma;
};

static boost::random::mt19937 random_gen(std::time(0));

static void random_gauss_bang(t_random_gauss* x)
{
    if(x->sigma < 0) {
        pd_error(x, "sigma value should be equal or greater then zero. (%f)", x->sigma);
        return;
    }

    boost::random::normal_distribution<t_float> dist(x->mean, x->sigma);
    outlet_float(x->x_obj.te_outlet, dist(random_gen));
}

static void random_gauss_float(t_random_gauss* x, t_floatarg mean)
{
    x->mean = mean;
}

static void* random_gauss_new(t_floatarg mean, t_floatarg sigma)
{
    t_random_gauss* x = reinterpret_cast<t_random_gauss*>(pd_new(random_gauss_class));
    outlet_new(&x->x_obj, &s_float);
    x->sigma_inlet = floatinlet_new(&x->x_obj, &x->sigma);

    if(sigma < 0) {
        pd_error(x, "sigma value should be equal or greater then zero (%f). Setting to 0", sigma);
        sigma = 0;
    }

    x->sigma = sigma;
    x->mean = mean;
    return static_cast<void*>(x);
}

static void random_gauss_free(t_random_gauss* x)
{
    inlet_free(x->sigma_inlet);
}

extern "C" void setup_random0x2egauss()
{
    random_gauss_class = class_new(gensym("random.gauss"),
        reinterpret_cast<t_newmethod>(random_gauss_new),
        reinterpret_cast<t_method>(random_gauss_free),
        sizeof(t_random_gauss), 0, A_DEFFLOAT, A_DEFFLOAT, 0);
    class_addbang(random_gauss_class, random_gauss_bang);
    class_addfloat(random_gauss_class, random_gauss_float);
}
