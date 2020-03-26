#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_real_distribution.hpp>
#include <ctime>

#include "ceammc.h"
#include "ceammc_factory.h"
#include "random_float.h"

static boost::random::mt19937 random_gen(std::time(0));

RandomFloat::RandomFloat(const PdArgs& a)
    : BaseObject(a)
    , min_(nullptr)
    , max_(nullptr)
{
    createOutlet();

    min_ = new FloatProperty("@min", 0);
    max_ = new FloatProperty("@max", 1);

    addProperty(min_);
    addProperty(max_);

    switch (positionalArguments().size()) {
    case 2:
        min_->setArgIndex(0);
        max_->setArgIndex(1);
        break;
    case 1:
        max_->setArgIndex(0);
        break;
    default:
        break;
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
    obj.addAlias("random.f");
}
