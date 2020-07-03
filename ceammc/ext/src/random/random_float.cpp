#include <ctime>
#include <random>

#include "ceammc_factory.h"
#include "fmt/format.h"
#include "random_float.h"

static std::mt19937 random_gen(std::time(0));

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

    switch (a.args.size()) {
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
    if (min_->value() > max_->value()) {
        LIB_ERR << fmt::format("@min should be less then @max, got: {} > {}",
            min_->value(), max_->value());
        return;
    }

    if (min_->value() == max_->value())
        return floatTo(0, min_->value());

    std::uniform_real_distribution<t_float> dist(min_->value(), max_->value());
    floatTo(0, dist(random_gen));
}

void setup_random_float()
{
    ObjectFactory<RandomFloat> obj("random.float");
    obj.addAlias("random.f");
}
