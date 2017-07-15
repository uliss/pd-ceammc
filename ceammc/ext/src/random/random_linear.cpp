#include <boost/random/mersenne_twister.hpp>
#include <boost/random/piecewise_linear_distribution.hpp>
#include <ctime>

#include "ceammc_factory.h"
#include "random_linear.h"

static boost::random::mt19937 random_gen(std::time(0));

RandomLinear::RandomLinear(const PdArgs& a)
    : BaseObject(a)
    , v0_(0)
    , v1_(0)
    , p0_(0)
    , p1_(0)
{
    createOutlet();

    v0_ = new FloatProperty("@v0", 0);
    v1_ = new FloatProperty("@v1", 1);
    p0_ = new FloatProperty("@p0", 1);
    p1_ = new FloatProperty("@p1", 0);

    createProperty(v0_);
    createProperty(v1_);
    createProperty(p0_);
    createProperty(p1_);
}

void RandomLinear::onBang()
{
    const t_float v0 = v0_->value();
    const t_float v1 = v1_->value();

    if (v0 >= v1) {
        OBJ_ERR << "invalid value range: " << v0 << "-" << v1;
        return;
    }

    t_float intervals[] = { v0, v1 };
    t_float weights[] = { p0_->value(), p1_->value() };

    boost::random::piecewise_linear_distribution<t_float> dist(intervals, intervals + 2, weights);
    floatTo(0, dist(random_gen));
}

extern "C" void setup_random0x2elinear()
{
    ObjectFactory<RandomLinear> obj("random.linear");
}
