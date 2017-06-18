#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_real_distribution.hpp>
#include <ctime>

#include "ceammc.h"
#include "ceammc_factory.h"
#include "random_float.h"

static boost::random::mt19937 random_gen(std::time(0));

RandomFloat::RandomFloat(const PdArgs& a)
    : BaseObject(a)
    , min_(0)
    , max_(0)
{
    createOutlet();

    min_ = new FloatProperty("@min", 0);
    max_ = new FloatProperty("@max", 1);

    createProperty(min_);
    createProperty(max_);

    if (positionalArguments().size() == 2) {
        min_->setValue(positionalArgument(0).asFloat(0));
        max_->setValue(positionalArgument(1).asFloat(1));
    }

    if (positionalArguments().size() == 1) {
        min_->setValue(0);
        max_->setValue(positionalArgument(0).asFloat(1));
    }
}

void RandomFloat::onBang()
{
    if (min_->value() == max_->value())
        return floatTo(0, min_->value());

    if (min_->value() > max_->value()) {
        float v0 = min_->value();
        float v1 = max_->value();
        min_->setValue(std::min(v0, v1));
        max_->setValue(std::max(v0, v1));
    }

    boost::random::uniform_real_distribution<t_float> dist(min_->value(), max_->value());
    floatTo(0, dist(random_gen));
}

extern "C" void setup_random0x2efloat()
{
    ObjectFactory<RandomFloat> obj("random.float");
}
